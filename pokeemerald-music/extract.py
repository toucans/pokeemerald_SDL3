#!/usr/bin/env python3
"""Extract pokeemerald overworld BGM into JSON + sample bank for the WebAudio player.

Reads a pokeemerald source tree (unzip /mnt/lexar/pokeemerald.zip) and, for each
requested song, resolves the full chain the GBA m4a ("MP2K/Sappy") engine uses:

    MIDI (sound/songs/midi/*.mid, options in midi.cfg)
      -> voicegroup (sound/voicegroups/*.inc, program# -> voice)
          -> keysplits (sound/keysplit_tables.inc + voicegroups/keysplits/*)
          -> drumsets  (voice_keysplit_all, indexed by MIDI key)
      -> DirectSound samples (sound/direct_sound_samples/*.aif, 8-bit signed PCM
         with loop markers) and GB PSG voices (square 1/2, wave, noise)

Output (web/data/):
    songs/<name>.json  - flat per-track event lists in seconds + resolved voices
    samples.json       - base64 8-bit signed PCM, rate + loop points
    manifest.json      - song list

Only stdlib. Usage: ./extract.py --src <pokeemerald dir> [song ...]
"""
import argparse
import base64
import json
import math
import re
import struct
import sys
from pathlib import Path

DEFAULT_SONGS = [
    "mus_littleroot", "mus_oldale", "mus_petalburg", "mus_rustboro",
    "mus_dewford", "mus_slateport", "mus_verdanturf", "mus_fortree",
    "mus_route101", "mus_route104", "mus_route110", "mus_route119",
    "mus_surf",
]

TITLES = {
    "mus_littleroot": "Littleroot Town",
    "mus_oldale": "Oldale Town",
    "mus_petalburg": "Petalburg City",
    "mus_rustboro": "Rustboro City",
    "mus_dewford": "Dewford Town",
    "mus_slateport": "Slateport City",
    "mus_verdanturf": "Verdanturf Town",
    "mus_fortree": "Fortree City",
    "mus_route101": "Route 101",
    "mus_route104": "Route 104",
    "mus_route110": "Route 110",
    "mus_route119": "Route 119",
    "mus_surf": "Surf",
}


# ---------------------------------------------------------------- AIFF samples

def parse_aif(path):
    """Return dict(rate, data(signed bytes), loop, loopStart, loopEnd)."""
    d = path.read_bytes()
    assert d[:4] == b"FORM" and d[8:12] == b"AIFF", path
    i = 12
    rate = None
    num_frames = None
    marks = {}
    sustain = None
    data = None
    while i + 8 <= len(d):
        cid = d[i:i + 4]
        sz = struct.unpack(">I", d[i + 4:i + 8])[0]
        body = d[i + 8:i + 8 + sz]
        if cid == b"COMM":
            num_frames = struct.unpack(">I", body[2:6])[0]
            exp = struct.unpack(">h", body[8:10])[0]
            mant = struct.unpack(">Q", body[10:18])[0]
            rate = mant * 2.0 ** (exp - 16383 - 63)
        elif cid == b"MARK":
            n = struct.unpack(">H", body[:2])[0]
            j = 2
            for _ in range(n):
                mid, pos = struct.unpack(">HI", body[j:j + 6])
                ln = body[j + 6]
                marks[mid] = pos
                j += 7 + ln + ((ln + 1) % 2)
        elif cid == b"INST":
            play_mode, begin_id, end_id = struct.unpack(">hHH", body[8:14])
            if play_mode == 1:
                sustain = (begin_id, end_id)
        elif cid == b"SSND":
            off = struct.unpack(">I", body[:4])[0]
            data = body[8 + off:]
        i += 8 + sz + (sz % 2)
    assert rate and data is not None, path
    loop = sustain is not None
    loop_start = marks.get(sustain[0], 0) if loop else 0
    # aif2pcm stores size = num_frames - 1: the final frame (a duplicate of
    # the loop-start sample in these AIFFs) is NOT part of the playable/loop
    # region. Keeping it makes every loop cycle play the join sample twice -
    # an audible tick per cycle on sustained notes.
    end = min(num_frames - 1, len(data))
    smp = {
        "rate": rate,
        "data": bytearray(data[:end]),
        "loop": loop,
        "loopStart": loop_start,
    }
    # WebAudio AudioBuffer sample rates must be in [8000, 96000]; resample the
    # few outliers here (linear interp) so the player can stay dumb.
    while smp["rate"] < 8000:
        up = bytearray()
        s = smp["data"]
        for k in range(len(s)):
            a = s[k] - 256 if s[k] > 127 else s[k]
            nb = s[(k + 1) % len(s)] if smp["loop"] else s[min(k + 1, len(s) - 1)]
            b = nb - 256 if nb > 127 else nb
            up.append(a & 0xFF)
            up.append(((a + b) // 2) & 0xFF)
        smp["data"] = up
        smp["rate"] *= 2
        smp["loopStart"] *= 2
    assert smp["rate"] <= 96000, (path, smp["rate"])
    return smp


# ------------------------------------------------------------- asm .inc files

def parse_incbin_labels(path, subdir_key):
    """direct_sound_data.inc / programmable_wave_data.inc: label -> file path."""
    out = {}
    label = None
    for line in path.read_text().splitlines():
        m = re.match(r"(\w+)::", line.strip())
        if m:
            label = m.group(1)
            continue
        m = re.search(r'\.incbin\s+"([^"]+)"', line)
        if m and label:
            out[label] = m.group(1)
            label = None
    return out


def parse_keysplit_tables(path):
    """keysplit_tables.inc -> {name: {midiKey: voiceIndex}}"""
    tables = {}
    cur = None
    last = 0
    for line in path.read_text().splitlines():
        line = line.split("@")[0].strip()
        m = re.match(r"keysplit\s+(\w+)(?:\s*,\s*(\d+))?", line)
        if m:
            cur = {}
            tables[m.group(1)] = cur
            last = int(m.group(2) or 0)
            continue
        m = re.match(r"split\s+(\d+)\s*,\s*(\d+)", line)
        if m and cur is not None:
            idx, end = int(m.group(1)), int(m.group(2))
            for k in range(last, end + 1):
                cur[k] = idx
            last = end + 1
    return tables


VOICE_RE = re.compile(r"(voice_\w+|cry\w*)\s+(.*)")


def parse_voicegroup_file(path):
    """One .inc file -> (name, offset, [raw voice tuples])."""
    name = None
    offset = 0
    entries = []
    for line in path.read_text().splitlines():
        line = line.split("@")[0].strip()
        if not line:
            continue
        m = re.match(r"voice_group\s+(\w+)(?:\s*,\s*(\d+))?", line)
        if m:
            name = m.group(1)
            offset = int(m.group(2) or 0)
            continue
        m = VOICE_RE.match(line)
        if m:
            args = [a.strip() for a in m.group(2).split(",")]
            entries.append((m.group(1), args))
    return name, offset, entries


class Bank:
    """All voicegroups + keysplit tables + sample label maps for one source tree."""

    def __init__(self, src):
        self.src = src
        self.groups = {}   # name -> (offset, entries)
        for p in (src / "sound" / "voicegroups").rglob("*.inc"):
            name, offset, entries = parse_voicegroup_file(p)
            if name:
                self.groups[name] = (offset, entries)
        self.keysplits = parse_keysplit_tables(src / "sound" / "keysplit_tables.inc")
        self.ds_files = parse_incbin_labels(src / "sound" / "direct_sound_data.inc", "direct_sound_samples")
        self.wave_files = parse_incbin_labels(src / "sound" / "programmable_wave_data.inc", "programmable_wave_samples")
        self.samples = {}      # label -> parsed aif (lazy)
        self.used_samples = set()

    def sample(self, label):
        if label not in self.samples:
            rel = self.ds_files[label]
            aif = self.src / rel.replace(".bin", ".aif")
            self.samples[label] = parse_aif(aif)
        self.used_samples.add(label)
        return self.samples[label]

    def wave_pattern(self, label):
        raw = (self.src / self.wave_files[label]).read_bytes()
        vals = []
        for b in raw:  # 32 x 4-bit samples, high nibble first
            vals.append(b >> 4)
            vals.append(b & 0xF)
        return vals

    def entry(self, group_name, index):
        offset, entries = self.groups[group_name]
        i = index - offset
        if 0 <= i < len(entries):
            return entries[i]
        return None

    # ---- resolve a (voicegroup, program, midiKey) to a concrete voice dict
    def resolve(self, group_name, prog, key):
        raw = self.entry(group_name, prog)
        if raw is None:
            return None
        kind, args = raw
        if kind == "voice_keysplit":
            sub_group = args[0].replace("voicegroup_", "")
            table = self.keysplits[args[1].replace("keysplit_", "")]
            idx = table.get(key)
            if idx is None:
                return None
            raw = self.entry(sub_group, idx)
            return self._concrete(raw, rhythm=False)
        if kind == "voice_keysplit_all":
            sub_group = args[0].replace("voicegroup_", "")
            raw = self.entry(sub_group, key)
            return self._concrete(raw, rhythm=True)
        return self._concrete(raw, rhythm=False)

    def _concrete(self, raw, rhythm):
        if raw is None:
            return None
        kind, args = raw
        if kind.startswith("voice_keysplit"):
            return None  # engine forbids nesting
        v = None
        if kind.startswith("voice_directsound"):
            base, pan, label = int(args[0]), int(args[1]), args[2]
            adsr = [int(a) for a in args[3:7]]
            self.sample(label)
            v = {"t": "pcm", "sample": label, "base": base,
                 "fixed": "no_resample" in kind, "adsr": adsr}
        elif kind.startswith("voice_square_1"):
            base, pan, sweep, duty = (int(a) for a in args[0:4])
            adsr = [int(a) for a in args[4:8]]
            v = {"t": "sq", "duty": duty & 3, "base": base, "sweep": sweep,
                 "adsr": adsr}
        elif kind.startswith("voice_square_2"):
            base, pan, duty = (int(a) for a in args[0:3])
            adsr = [int(a) for a in args[3:7]]
            v = {"t": "sq", "duty": duty & 3, "base": base, "sweep": 0,
                 "adsr": adsr}
        elif kind.startswith("voice_programmable_wave"):
            base, pan, label = int(args[0]), int(args[1]), args[2]
            adsr = [int(a) for a in args[3:7]]
            v = {"t": "wave", "pattern": self.wave_pattern(label), "base": base,
                 "adsr": adsr}
        elif kind.startswith("voice_noise"):
            base, pan, period = int(args[0]), int(args[1]), int(args[2])
            adsr = [int(a) for a in args[3:7]]
            v = {"t": "noise", "period": period & 1, "base": base, "adsr": adsr}
        else:
            return None
        pan = int(args[1])
        if rhythm:
            v["rhythm"] = True   # note plays at the voice's own base key
            if pan != 0:         # engine: pan byte 0x80|pan -> ((0x80|pan)-0xC0)*2
                v["pan"] = ((0x80 | pan) - 0xC0) * 2  # -128..126, 0 = centre
        return v


# ----------------------------------------------------------------------- MIDI

def read_varlen(d, j):
    v = 0
    while True:
        v = (v << 7) | (d[j] & 0x7F)
        ok = not (d[j] & 0x80)
        j += 1
        if ok:
            return v, j


def parse_midi(path):
    """-> (division, tempos[[tick,usPerBeat]], loop(start,end), tracks[events])"""
    d = path.read_bytes()
    assert d[:4] == b"MThd"
    fmt, ntr, div = struct.unpack(">HHH", d[8:14])
    i = 14
    tempos = []
    loop_start = loop_end = None
    tracks = []
    for _ in range(ntr):
        assert d[i:i + 4] == b"MTrk"
        ln = struct.unpack(">I", d[i + 4:i + 8])[0]
        j = i + 8
        end = j + ln
        tick = 0
        run = 0
        evs = []
        open_notes = {}
        while j < end:
            delta, j = read_varlen(d, j)
            tick += delta
            st = d[j]
            if st & 0x80:
                j += 1
                run = st
            else:
                st = run
            if st == 0xFF:
                mt = d[j]
                l = d[j + 1]
                body = d[j + 2:j + 2 + l]
                j += 2 + l
                if mt == 0x51:
                    tempos.append([tick, (body[0] << 16) | (body[1] << 8) | body[2]])
                elif mt == 0x06:
                    if body == b"[":
                        loop_start = tick
                    elif body == b"]":
                        loop_end = tick
            elif st in (0xF0, 0xF7):
                l, j = read_varlen(d, j)
                j += l
            else:
                hi = st & 0xF0
                if hi in (0xC0, 0xD0):
                    p1 = d[j]
                    j += 1
                    if hi == 0xC0:
                        evs.append([tick, "prog", p1])
                else:
                    p1, p2 = d[j], d[j + 1]
                    j += 2
                    if hi == 0x90 and p2 > 0:
                        open_notes.setdefault(p1, []).append((tick, p2, len(evs)))
                        evs.append(None)  # placeholder, filled at note off
                    elif hi == 0x80 or (hi == 0x90 and p2 == 0):
                        if open_notes.get(p1):
                            t0, vel, slot = open_notes[p1].pop(0)
                            evs[slot] = [t0, "note", p1, vel, tick - t0]
                    elif hi == 0xB0:
                        evs.append([tick, "cc", p1, p2])
                    elif hi == 0xE0:
                        evs.append([tick, "bend", ((p2 << 7) | p1) - 8192])
        evs = [e for e in evs if e is not None]
        tracks.append(evs)
        i = end
    if not tempos:
        tempos = [[0, 500000]]
    tempos.sort()
    return div, tempos, (loop_start, loop_end), tracks


def tick_to_sec(tempos, div):
    """Return function ticks->seconds."""
    spans = []  # (tick, sec, usPerBeat)
    sec = 0.0
    last_tick, last_us = 0, tempos[0][1]
    for tick, us in tempos:
        sec += (tick - last_tick) * last_us / 1e6 / div
        spans.append((tick, sec, us))
        last_tick, last_us = tick, us

    def f(t):
        s = spans[0]
        for sp in spans:
            if sp[0] <= t:
                s = sp
            else:
                break
        return s[1] + (t - s[0]) * s[2] / 1e6 / div
    return f


# ------------------------------------------------------------------ per song

def vel_lut(v):
    """mid2agb g_noteVelocityLUT quantisation."""
    return min(127, 4 * math.ceil(v / 4)) if v else 0


def extract_song(bank, src, name, cfg):
    div, tempos, loop, mtracks = parse_midi(src / "sound" / "songs" / "midi" / f"{name}.mid")
    t2s = tick_to_sec(tempos, div)
    mvl = cfg["mvl"]
    voices = {}
    voice_ids = {}

    def voice_id(prog, key):
        v = bank.resolve(cfg["group"], prog, key)
        if v is None:
            return None
        sig = json.dumps(v, sort_keys=True)
        if sig not in voice_ids:
            voice_ids[sig] = f"v{len(voice_ids)}"
            voices[voice_ids[sig]] = v
        return voice_ids[sig]

    out_tracks = []
    for evs in mtracks:
        prog = 0
        xcmd = None
        out = []
        for e in evs:
            tick = e[0]
            s = round(t2s(tick), 5)
            if e[1] == "prog":
                prog = e[2]
            elif e[1] == "note":
                vid = voice_id(prog, e[2])
                if vid is None:
                    continue
                dur = round(t2s(tick + e[4]) - t2s(tick), 5)
                out.append([s, "n", e[2], vel_lut(e[3]), dur, vid])
            elif e[1] == "bend":
                out.append([s, "b", round(e[2] / 128)])  # -> -64..63 like BEND
            elif e[1] == "cc":
                cc, val = e[2], e[3]
                if cc == 7:
                    out.append([s, "v", val * mvl // 127])
                elif cc == 10:
                    out.append([s, "p", val - 64])
                elif cc == 1:
                    out.append([s, "m", val])
                elif cc == 20:
                    out.append([s, "br", val])
                elif cc == 21:
                    out.append([s, "ls", val])
                elif cc == 30:
                    xcmd = val
                elif cc == 29 and xcmd == 8:
                    out.append([s, "ev", val])   # pseudo-echo volume
                elif cc == 29 and xcmd == 9:
                    out.append([s, "el", val])   # pseudo-echo length
        if any(e[1] == "n" for e in out):
            out_tracks.append(out)

    song = {
        "name": name,
        "title": TITLES.get(name, name),
        "reverb": cfg["reverb"],
        "loopStart": round(t2s(loop[0]), 5) if loop[0] is not None else None,
        "loopEnd": round(t2s(loop[1]), 5) if loop[1] is not None else None,
        "tracks": out_tracks,
        "voices": voices,
    }
    return song


def parse_midi_cfg(src):
    cfg = {}
    for line in (src / "sound" / "songs" / "midi" / "midi.cfg").read_text().splitlines():
        m = re.match(r"(\w+)\.mid:\s+(.*)", line)
        if not m:
            continue
        name, opts = m.group(1), m.group(2)
        g = re.search(r"-G(\w+)", opts)
        v = re.search(r"-V(\d+)", opts)
        r = re.search(r"-R(\d+)", opts)
        cfg[name] = {
            "group": g.group(1).lstrip("_") if g else None,
            "mvl": int(v.group(1)) if v else 127,
            "reverb": int(r.group(1)) if r else 0,
        }
    return cfg


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--src", required=True, type=Path, help="pokeemerald source root")
    ap.add_argument("--out", type=Path, default=Path(__file__).parent / "web" / "data")
    ap.add_argument("songs", nargs="*", default=None)
    args = ap.parse_args()
    songs = args.songs or DEFAULT_SONGS

    bank = Bank(args.src)
    cfgs = parse_midi_cfg(args.src)
    (args.out / "songs").mkdir(parents=True, exist_ok=True)

    manifest = []
    for name in songs:
        song = extract_song(bank, args.src, name, cfgs[name])
        p = args.out / "songs" / f"{name}.json"
        p.write_text(json.dumps(song, separators=(",", ":")))
        n_notes = sum(1 for t in song["tracks"] for e in t if e[1] == "n")
        print(f"{name:18} tracks={len(song['tracks'])} notes={n_notes} "
              f"loop={song['loopStart']}..{song['loopEnd']}s "
              f"voices={len(song['voices'])} -> {p.name}")
        manifest.append({"file": f"songs/{name}.json", "name": name,
                         "title": song["title"]})

    smp_out = {}
    for label in sorted(bank.used_samples):
        s = bank.samples[label]
        smp_out[label] = {
            "rate": round(s["rate"], 2),
            "loop": s["loop"],
            "loopStart": s["loopStart"],
            "b64": base64.b64encode(bytes(s["data"])).decode(),
        }
    (args.out / "samples.json").write_text(json.dumps(smp_out, separators=(",", ":")))
    (args.out / "manifest.json").write_text(json.dumps(manifest, indent=1))
    total = sum(len(bank.samples[l]["data"]) for l in bank.used_samples)
    print(f"\n{len(smp_out)} samples, {total/1024:.0f} KiB raw PCM -> samples.json")


if __name__ == "__main__":
    main()
