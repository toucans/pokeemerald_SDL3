#!/usr/bin/env python3
"""pack_music.py — extracted music JSON -> pokeemerald-music/web/music.pak.

music.pak is THE music format: the game (src/m4a.c) loads it directly and the
pokeemerald-music site plays it through the same engine compiled to wasm. The
pak is committed; this script only runs when regenerating the soundtrack from
a pret/pokeemerald checkout (extract.py first — see pokeemerald-music/README).

Python stdlib only. Little-endian throughout.

Format v2 (all counts u32, scalars little-endian):
  "M4AP" u32 version=2
  u32 nSamples, u32 nSongs
  nSamples x sample:
    char label[64] (NUL-padded)
    f32 rate, u32 loop(0/1), u32 loopStart, u32 length,
    i8 data[length], pad to 4
  nSongs x song:
    char name[32], char title[48], char cat[32]
    f32 reverb, f32 loopStart, f32 loopEnd   (loopStart/End < 0 = non-looping)
    u8 keyLo, u8 keyHi (sounding-pitch range, for the viz), u16 pad
    u32 nVoices, u32 nTracks
    nVoices x voice (48 bytes):
      u8 type (0 pcm, 1 square, 2 wave, 3 noise)
      u8 base, u8 fixed, u8 rhythm, i8 pan, u8 duty, u8 period, u8 pad
      u16 sampleIdx (0xFFFF = none), u16 pad
      u8 adsr[4], u8 pattern[32]
    nTracks x track: u32 nEvents, then nEvents x event (12 bytes):
      f32 t, u8 type (0 n,1 v,2 p,3 b,4 br,5 m,6 ls,7 ev,8 el)
      u8 key, u8 vel, u8 vid, f32 x (note: x = gate seconds; else: x = value)
Samples are referenced by index in samples.json key order.
"""
import base64
import json
import struct
import sys
from pathlib import Path

DATA = Path(__file__).resolve().parent.parent / "pokeemerald-music" / "web" / "data"
OUT = Path(__file__).resolve().parent.parent / "pokeemerald-music" / "web" / "music.pak"

VOICE_T = {"pcm": 0, "sq": 1, "wave": 2, "noise": 3}
EVENT_T = {"n": 0, "v": 1, "p": 2, "b": 3, "br": 4, "m": 5, "ls": 6, "ev": 7, "el": 8}


def key_range(song):
    """Sounding-pitch range across all notes (rhythm voices play their base key)."""
    lo, hi = 127, 0
    voices = list(song["voices"].values())
    vids = {k: i for i, k in enumerate(song["voices"])}
    for track in song["tracks"]:
        for e in track:
            if e[1] != "n":
                continue
            v = voices[vids[e[5]]]
            k = v["base"] if v.get("rhythm") else e[2]
            lo, hi = min(lo, k), max(hi, k)
    return (lo, hi) if lo <= hi else (60, 60)


def main():
    samples = json.loads((DATA / "samples.json").read_text())
    manifest = json.loads((DATA / "manifest.json").read_text())
    sample_idx = {label: i for i, label in enumerate(samples)}

    out = bytearray()
    out += struct.pack("<4sIII", b"M4AP", 2, len(samples), len(manifest))

    for label, s in samples.items():
        data = base64.b64decode(s["b64"])
        out += struct.pack("<64s", label.encode())
        out += struct.pack("<fIII", float(s["rate"]), int(bool(s["loop"])),
                           int(s.get("loopStart") or 0), len(data))
        out += data
        out += b"\0" * (-len(data) % 4)  # keep later f32/u32 fields 4-aligned

    for entry in manifest:
        song = json.loads((DATA / entry["file"]).read_text())
        voices = song["voices"]  # dict "v0".. ; vid in events indexes this
        vids = {k: i for i, k in enumerate(voices)}
        loop_s = song.get("loopStart")
        loop_e = song.get("loopEnd")
        looping = loop_s is not None and loop_e is not None and loop_e > loop_s
        lo, hi = key_range(song)
        out += struct.pack("<32s48s32s", song["name"].encode(),
                           entry["title"].encode(), (entry.get("cat") or "").encode())
        out += struct.pack("<fff", float(song.get("reverb") or 0),
                           loop_s if looping else -1.0, loop_e if looping else -1.0)
        out += struct.pack("<BBH", lo, hi, 0)
        out += struct.pack("<II", len(voices), len(song["tracks"]))

        for v in voices.values():
            pattern = bytes(v.get("pattern") or [0] * 32)
            out += struct.pack(
                "<BBBBbBBBHH4s32s",
                VOICE_T[v["t"]], int(v.get("base") or 0), int(bool(v.get("fixed"))),
                int(bool(v.get("rhythm"))), int(v.get("pan") or 0),
                int(v.get("duty") or 0), int(v.get("period") or 0), 0,
                sample_idx.get(v.get("sample"), 0xFFFF), 0,
                bytes(v["adsr"]), pattern)

        for track in song["tracks"]:
            out += struct.pack("<I", len(track))
            for e in track:
                if e[1] == "n":
                    t, _, key, vel, dur, vid = e
                    out += struct.pack("<fBBBBf", t, 0, key, vel, vids[vid], dur)
                else:
                    out += struct.pack("<fBBBBf", e[0], EVENT_T[e[1]], 0, 0, 0, float(e[2]))

    OUT.write_bytes(out)
    print(f"{OUT}: {len(samples)} samples, {len(manifest)} songs, {len(out)/1e6:.1f} MB")


if __name__ == "__main__":
    sys.exit(main())
