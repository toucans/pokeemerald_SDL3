#!/usr/bin/env python3
"""Build the optional soundfont sample bank for the web player.

Reads a GM-layout SoundFont (GBApokemonTestLite.sf2 — SC-88-quality Roland
instruments + the Charm drum kit, made by its author specifically for the
pokeemerald MIDIs) and, for every *PCM* voice the extracted songs use
(melodic instruments + drums), resolves the matching sf2 preset zone and
emits:

    web/data/sf2/samples.json   - base64 16-bit LE PCM, ONLY the samples the
                                  13 songs touch, looped ones trimmed at the
                                  loop end (a few MB, not the sf2's 440)
    web/data/sf2/overlays.json  - per song: replacement voices + a
                                  {vid: [[keyLo,keyHi,newVid],...]} remap the
                                  player applies to note events

Only PCM voices are substituted: the PSG squares/wave/noise are already
synthesized exactly, while the sf2's win is sample quality. The m4a
envelopes, velocity/pan law and sequence timing are kept as-is, so
original-vs-sf2 is a pure samples A/B.

Needs `prog` on each voice (extract.py writes it). Only stdlib.
Usage: ./extract_sf2.py [--sf2 <file>] [--data web/data]
"""
import argparse
import base64
import json
import math
import struct
from pathlib import Path

FRAME = 1 / 59.7275   # GBA frame; m4a release constants are per-frame


def levels(data, full, loop, loop_start, rate):
    """(rms, peak) normalized to full scale. RMS over the loop region for
    looped samples (the sustained level), else the first 0.5 s (attack);
    peak over the whole sample."""
    seg = data[loop_start:] if loop else data[:max(1, min(len(data), int(rate / 2)))]
    if not seg:
        return 0.0, 0.0
    r = math.sqrt(sum(x * x for x in seg) / len(seg)) / full
    return r, max(abs(x) for x in data) / full


def ring_time(adsr):
    """Seconds a note stays audible after its gate ends (m4a release:
    multiply by r/256 each frame until < 1/512), capped at 30 s."""
    r = adsr[3]
    if r == 0:
        return 0.0
    if r >= 255:
        return 30.0
    tau = FRAME / math.log(256 / r)
    return min(30.0, tau * math.log(512))

# SF2 generator opers we honour (the .sf2's mod chunks are empty).
G_START, G_END, G_SLOOP, G_ELOOP, G_STARTC = 0, 1, 2, 3, 4
G_ENDC, G_PAN, G_ATT, G_COARSE, G_FINE = 12, 17, 48, 51, 52
G_INSTRUMENT, G_SAMPLEID, G_MODES, G_ROOT = 41, 53, 54, 58
G_KEYRANGE, G_VELRANGE = 43, 44
RANGE_GENS = (G_KEYRANGE, G_VELRANGE)


class Sf2:
    """Minimal SF2 reader: presets -> instrument zones -> sample headers."""

    def __init__(self, path):
        self.f = open(path, "rb")
        riff, _, sfbk = struct.unpack("<4sI4s", self.f.read(12))
        assert riff == b"RIFF" and sfbk == b"sfbk", path
        chunks = {}
        self.f.seek(0, 2)
        end = self.f.tell()
        self.f.seek(12)
        while self.f.tell() < end:
            cid, csz = struct.unpack("<4sI", self.f.read(8))
            if cid == b"LIST":
                sub_end = self.f.tell() + csz
                self.f.read(4)
                while self.f.tell() < sub_end:
                    scid, ssz = struct.unpack("<4sI", self.f.read(8))
                    chunks[scid.decode()] = (self.f.tell(), ssz)
                    self.f.seek(ssz + (ssz & 1), 1)
            else:
                self.f.seek(csz + (csz & 1), 1)
        self.smpl_off = chunks["smpl"][0]

        def table(name, fmt, size):
            off, sz = chunks[name]
            self.f.seek(off)
            raw = self.f.read(sz)
            return [struct.unpack_from(fmt, raw, i) for i in range(0, sz, size)]

        self.phdr = table("phdr", "<20sHHHIII", 38)
        self.pbag = table("pbag", "<HH", 4)
        self.pgen = table("pgen", "<Hh", 4)
        self.inst = table("inst", "<20sH", 22)
        self.ibag = table("ibag", "<HH", 4)
        self.igen = table("igen", "<Hh", 4)
        self.shdr = table("shdr", "<20sIIIIIBbHH", 46)
        self.presets = {}
        for i in range(len(self.phdr) - 1):
            name, prog, bank, bag, *_ = self.phdr[i]
            self.presets[(bank, prog)] = (self.phdr[i][3], self.phdr[i + 1][3])

    @staticmethod
    def _zones(bags, gens, lo, hi, terminal_gen):
        """bag range -> (global_gens, [zone_gens...]); terminal = inst/sampleID."""
        glob, zones = {}, []
        for b in range(lo, hi):
            g0, g1 = bags[b][0], bags[b + 1][0]
            z = {}
            for gi in range(g0, g1):
                oper, amount = gens[gi]
                if oper in RANGE_GENS:
                    z[oper] = (amount & 0xFF, (amount >> 8) & 0xFF)
                else:
                    z[oper] = amount
            if terminal_gen in z:
                zones.append(z)
            elif not zones and b == lo:
                glob = z
        return glob, zones

    @staticmethod
    def _in_range(z, glob, key, vel):
        for oper, val in ((G_KEYRANGE, key), (G_VELRANGE, vel)):
            r = z.get(oper, glob.get(oper))
            if r is not None and not (r[0] <= val <= r[1]):
                return False
        return True

    def find(self, bank, prog, key, vel):
        """-> list of matched (izone_combined, pzone_combined, shdr_index)."""
        if (bank, prog) not in self.presets:
            return []
        plo, phi = self.presets[(bank, prog)]
        pglob, pzones = self._zones(self.pbag, self.pgen, plo, phi, G_INSTRUMENT)
        out = []
        for pz in pzones:
            if not self._in_range(pz, pglob, key, vel):
                continue
            padd = {**pglob, **pz}
            ilo = self.inst[pz[G_INSTRUMENT]][1]
            ihi = self.inst[pz[G_INSTRUMENT] + 1][1]
            iglob, izones = self._zones(self.ibag, self.igen, ilo, ihi, G_SAMPLEID)
            for iz in izones:
                if not self._in_range(iz, iglob, key, vel):
                    continue
                out.append(({**iglob, **iz}, padd, iz[G_SAMPLEID]))
        return out

    def pcm(self, start, end):
        """16-bit LE frames [start,end) from the smpl chunk."""
        self.f.seek(self.smpl_off + 2 * start)
        raw = self.f.read(2 * (end - start))
        return list(struct.unpack("<%dh" % (len(raw) // 2), raw))


def resolve_note(sf2, prog, key, rhythm):
    """One concrete (sample-data, rate, loop, tune, gain, pan) for a note.

    Stereo pairs (two matched zones whose samples are the L/R halves) are
    mono-mixed — the m4a engine is a mono-source engine and the drum's own
    m4a/sf2 pan still applies.
    """
    matches = sf2.find(128 if rhythm else 0, prog, key, 100)
    if not matches and rhythm:
        matches = sf2.find(128, 0, key, 100)   # any drumset -> the one GM kit
    if not matches:
        return None
    left = right = None
    for m in matches:
        stype = sf2.shdr[m[2]][9]
        if stype == 4 and left is None:
            left = m
        elif stype == 2 and right is None:
            right = m
    pair = None
    if left and right and sf2.shdr[left[2]][8] == right[2]:
        pair = (left, right)
    use = [left or matches[0]] if not pair else [pair[0], pair[1]]

    datas, meta = [], None
    for iz, pz, sid in use:
        name, start, end, sloop, eloop, rate, pitch, corr, link, stype = sf2.shdr[sid]
        start += iz.get(G_START, 0) + 32768 * iz.get(G_STARTC, 0)
        end += iz.get(G_END, 0) + 32768 * iz.get(G_ENDC, 0)
        sloop += iz.get(G_SLOOP, 0)
        eloop += iz.get(G_ELOOP, 0)
        loop = (iz.get(G_MODES, 0) & 3) in (1, 3)
        stop = eloop if loop and eloop > start else end   # trim: past-loop never plays
        datas.append(sf2.pcm(start, stop))
        if meta is None:
            root = iz.get(G_ROOT, -1)
            root = root if 0 <= root <= 127 else pitch
            cents = corr + iz.get(G_FINE, 0) + pz.get(G_FINE, 0) \
                + 100 * (iz.get(G_COARSE, 0) + pz.get(G_COARSE, 0))
            pan = iz.get(G_PAN, 0) + pz.get(G_PAN, 0)
            meta = {
                "sid": sid, "rate": rate, "loop": loop,
                "loopStart": (sloop - start) if loop else 0,
                "tune": 2 ** ((60 - root) / 12 + cents / 1200),
                "pan": 0 if pair else max(-128, min(126, round(pan * 127 / 500))),
                "name": name.rstrip(b"\0 ").decode(errors="replace"),
            }
    if pair:
        n = min(len(d) for d in datas)
        meta["data"] = [(datas[0][i] + datas[1][i]) // 2 for i in range(n)]
        meta["sid"] = (use[0][2], use[1][2])
    else:
        meta["data"] = datas[0]
    return meta


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--sf2", type=Path, default=Path("/mnt/lexar/GBApokemonTestLite.sf2"))
    ap.add_argument("--data", type=Path, default=Path(__file__).parent / "web" / "data")
    args = ap.parse_args()

    sf2 = Sf2(args.sf2)
    manifest = json.loads((args.data / "manifest.json").read_text())
    out_dir = args.data / "sf2"
    out_dir.mkdir(exist_ok=True)

    # loudness references: the GBA sample a voice originally plays. The m4a
    # mix law treats sample amplitude as the reference (its 8-bit samples are
    # normalized hot); GM fonts carry headroom + attenuation instead, so each
    # replacement is gain-matched to the sample it replaces — otherwise the
    # untouched PSG layer sits too loud over the sf2 instruments.
    gba = json.loads((args.data / "samples.json").read_text())
    gba_lvl = {}
    for label, s in gba.items():
        raw = base64.b64decode(s["b64"])
        data = [b - 256 if b > 127 else b for b in raw]
        gba_lvl[label] = levels(data, 128, s["loop"], s["loopStart"], s["rate"])
    sf2_lvl = {}   # meta sid -> (rms, peak) cache

    bank = {}        # label -> sample dict (shared across songs)
    sid_label = {}   # meta["sid"] -> label
    overlays = {}

    for entry in manifest:
        song = json.loads((args.data / entry["file"]).read_text())
        used = {}    # vid -> {key: longest gate (s)}
        for tr in song["tracks"]:
            for e in tr:
                if e[1] == "n":
                    km = used.setdefault(e[5], {})
                    km[e[2]] = max(km.get(e[2], 0.0), e[4])

        ov_voices, ov_ids, vmap = {}, {}, {}
        for vid, keys in sorted(used.items()):
            v = song["voices"][vid]
            if v["t"] != "pcm" or "prog" not in v:
                continue
            rhythm = bool(v.get("rhythm"))
            entries = []                       # (key, overlay voice id)
            for key in sorted(keys):
                m = resolve_note(sf2, v["prog"], key, rhythm)
                if m is None:
                    continue
                if m["sid"] not in sid_label:
                    label = "sf2_%s" % (m["sid"] if isinstance(m["sid"], int)
                                        else "%d_%d" % m["sid"])
                    sid_label[m["sid"]] = label
                    bank[label] = {
                        "rate": m["rate"], "loop": m["loop"],
                        "loopStart": m["loopStart"], "name": m["name"],
                        "data": m["data"], "need": 0,
                    }
                # A note consumes (gate + ring) * rate * pitch-ratio source
                # frames; track the max so unlooped 8 s decay tails can be
                # trimmed to what the songs can actually make audible.
                b = bank[sid_label[m["sid"]]]
                if not b["loop"]:
                    ratio = m["tune"] * 2 ** ((key - 60) / 12)
                    sec = keys[key] + ring_time(v["adsr"]) + 0.5
                    b["need"] = max(b["need"], int(sec * m["rate"] * ratio) + 1)
                if m["sid"] not in sf2_lvl:
                    sf2_lvl[m["sid"]] = levels(m["data"], 32768, m["loop"],
                                               m["loopStart"], m["rate"])
                ref, got = gba_lvl.get(v["sample"], (0, 0)), sf2_lvl[m["sid"]]
                # match sustained loudness, but never hotter than the original
                # sample's peak (transient-heavy drums overshoot on RMS alone)
                gain = 1.0
                if ref[0] and got[0] and ref[1] and got[1]:
                    gain = max(0.05, min(8.0, min(ref[0] / got[0], ref[1] / got[1])))
                nv = {"t": "pcm", "sample": sid_label[m["sid"]], "base": 60,
                      "fixed": False, "adsr": v["adsr"],
                      "tune": round(m["tune"], 6), "gain": round(gain, 4)}
                if rhythm:
                    nv["rhythm"] = True
                    nv["base"] = key           # rhythm plays at base; base=key
                    # prefer the sf2 kit's own pan, else keep the m4a drum pan
                    nv["pan"] = m["pan"] if m["pan"] else v.get("pan", 0)
                elif v.get("pan"):
                    nv["pan"] = v["pan"]
                sig = json.dumps(nv, sort_keys=True)
                if sig not in ov_ids:
                    ov_ids[sig] = "s%d" % len(ov_ids)
                    ov_voices[ov_ids[sig]] = nv
                entries.append((key, ov_ids[sig]))
            if not entries:
                continue
            # merge consecutive keys mapping to the same overlay voice
            ranges = []
            for key, sid in entries:
                if ranges and ranges[-1][2] == sid and key <= ranges[-1][1] + 12:
                    ranges[-1][1] = key        # melodic zones cover gaps too
                else:
                    ranges.append([key, key, sid])
            vmap[vid] = ranges
        overlays[song["name"]] = {"voices": ov_voices, "map": vmap}
        n_sub = len(vmap)
        n_pcm = sum(1 for v in song["voices"].values() if v["t"] == "pcm")
        print(f"{song['name']:18} pcm voices {n_pcm:2d} -> substituted {n_sub:2d}, "
              f"overlay voices {len(ov_voices)}")

    (out_dir / "overlays.json").write_text(json.dumps(overlays, separators=(",", ":")))
    out_bank, total = {}, 0
    for label, b in bank.items():
        data = b["data"]
        if not b["loop"]:
            end = len(data)
            if 0 < b["need"] < end:
                end = b["need"]
            # also drop the inaudible tail: past the last sample above -54 dB
            # the natural decay is effectively silence
            floor = int(32768 * 10 ** (-54 / 20))
            while end > 1 and abs(data[end - 1]) < floor:
                end -= 1
            if end < len(data):
                data = data[:end]
                for i in range(max(0, end - 1024), end):          # click guard
                    data[i] = data[i] * (end - i) // 1024
        total += len(data) * 2
        out_bank[label] = {
            "rate": b["rate"], "loop": b["loop"], "loopStart": b["loopStart"],
            "name": b["name"],
            "b64": base64.b64encode(struct.pack("<%dh" % len(data), *data)).decode(),
        }
    (out_dir / "samples.json").write_text(json.dumps(out_bank, separators=(",", ":")))
    print(f"\n{len(out_bank)} sf2 samples, {total / 1024:.0f} KiB 16-bit PCM -> {out_dir}/")


if __name__ == "__main__":
    main()
