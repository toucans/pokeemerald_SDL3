#!/usr/bin/env python3
"""render_compare.py — render the composers' original MIDIs for A/B on the site.

Renders each original MIDI (from the leaked Emerald sound source; local only,
never committed) through the SC-88Pro soundfont with fluidsynth, peak-
normalizes, encodes to opus, and writes docs/compare/
(gitignored) plus its index.json. The site shows an extra "original" button
for every song that has a render, next to the engine's live playback.

Matching MIDI files to game songs uses pret's include/constants/songs.h,
whose comments carry the original internal names:
    #define MUS_ROUTE101 359 // MUS_DOORO1
so a MIDI named dooro1.mid (any case, optional leading mus_) maps to
mus_route101.

Needs: fluidsynth, opusenc (apt: fluidsynth opus-tools). Stdlib only.

Usage:
  tools/render_compare.py --midis <dir with original .mid> \
      --sf2 "/mnt/lexar/Davinci Media Storage/GBApokemonTestLite.sf2" \
      [--pret ~/pokeemerald] [--gain 0.7] [--only mus_littleroot ...]
"""
import argparse
import array
import json
import re
import subprocess
import sys
import tempfile
import wave
from pathlib import Path

WEB = Path(__file__).resolve().parent.parent / "docs"
OUT = WEB / "compare"


def orig_name_map(pret):
    """{original internal name (lower, no mus_): mus_* name} from songs.h."""
    m = {}
    songs_h = Path(pret).expanduser() / "include" / "constants" / "songs.h"
    for line in songs_h.read_text().splitlines():
        g = re.match(r"#define\s+(MUS_\w+)\s+\S+\s*//\s*(MUS_\w+)", line)
        if g:
            m[g.group(2).lower().removeprefix("mus_")] = g.group(1).lower()
    return m


def song_titles():
    """{mus_* name: display title} from the committed pak (v2 layout)."""
    import struct
    pak = (WEB / "music.pak").read_bytes()
    magic, ver, nsamp, nsongs = struct.unpack_from("<4sIII", pak, 0)
    off = 16
    for _ in range(nsamp):
        length = struct.unpack_from("<I", pak, off + 76)[0]
        off += 80 + (length + 3) // 4 * 4
    titles = {}
    for _ in range(nsongs):
        name = pak[off:off + 32].split(b"\0")[0].decode()
        title = pak[off + 32:off + 80].split(b"\0")[0].decode()
        titles[name] = title
        nvoices, ntracks = struct.unpack_from("<II", pak, off + 128)
        off += 136 + nvoices * 48
        for _ in range(ntracks):
            nev = struct.unpack_from("<I", pak, off)[0]
            off += 4 + nev * 12
    return titles


def render(mid, sf2, out_opus, gain):
    with tempfile.NamedTemporaryFile(suffix=".wav") as tmp:
        subprocess.run(
            ["fluidsynth", "-ni", "-r", "48000", "-g", str(gain),
             "-F", tmp.name, str(sf2), str(mid)],
            check=True, capture_output=True)
        with wave.open(tmp.name, "rb") as w:
            params = w.getparams()
            pcm = array.array("h")
            pcm.frombytes(w.readframes(w.getnframes()))
        peak = max(abs(s) for s in pcm) if pcm else 0
        if peak > 0:
            k = min(32000 / peak, 8.0)
            pcm = array.array("h", (int(s * k) for s in pcm))
        pcm = pcm.tobytes()
        with tempfile.NamedTemporaryFile(suffix=".wav") as norm:
            with wave.open(norm.name, "wb") as w:
                w.setparams(params)
                w.writeframes(pcm)
            subprocess.run(["opusenc", "--quiet", "--bitrate", "128",
                            norm.name, str(out_opus)], check=True)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--midis", required=True, type=Path)
    ap.add_argument("--sf2", required=True, type=Path)
    ap.add_argument("--pret", type=Path, default=Path("~/pokeemerald"))
    ap.add_argument("--gain", type=float, default=0.7)
    ap.add_argument("--only", nargs="*", default=None)
    args = ap.parse_args()

    omap = orig_name_map(args.pret)
    titles = song_titles()
    OUT.mkdir(exist_ok=True)

    index, misses = {}, []
    for mid in sorted(args.midis.rglob("*.mid")) + sorted(args.midis.rglob("*.MID")):
        stem = mid.stem.lower().removeprefix("mus_")
        mus = omap.get(stem) or (f"mus_{stem}" if f"mus_{stem}" in titles else None)
        if not mus:
            misses.append(mid.name)
            continue
        if args.only and mus not in args.only:
            continue
        out = OUT / f"{mus}.opus"
        print(f"{mid.name} -> {mus} ({titles.get(mus, '?')})")
        render(mid, args.sf2, out, args.gain)
        index[mus] = {"file": f"compare/{mus}.opus", "src": mid.name}

    (OUT / "index.json").write_text(json.dumps(index, indent=1))
    print(f"\n{len(index)} rendered -> {OUT}")
    if misses:
        print(f"unmatched midis ({len(misses)}): {' '.join(misses[:20])}"
              + (" …" if len(misses) > 20 else ""))


if __name__ == "__main__":
    sys.exit(main())
