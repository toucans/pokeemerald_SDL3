#!/usr/bin/env python3
"""extract_sc88.py — leaked SC-88 Emerald MIDIs + SC-88Pro soundfont
-> pokeemerald-music/web/music-sc88.pak.

The SC-88 MIDIs in midi-sc88/ are the composers' 480-tpqn sources (GM programs,
unquantized velocities) that mid2agb consumed to build the game arrangement.
This packs them, plus only the sf2 samples/zones they actually use, into a
compact pak the C engine (src/m4a.c GM voice path) plays in the game and on
the site — no 440 MB soundfont shipped, no offline renders.

Matching MIDI names to mus_* uses pret's songs.h original-name comments,
like render_compare.py. Python stdlib only.

Format "M4AO" v1 (little-endian; counts u32):
  header: "M4AO" u32 ver=1, u32 nSamples, u32 nProgs, u32 nSongs
  samples[n]: f32 rate, u32 loopMode(0 none,1 loop), u32 loopStart, u32 loopEnd,
              u32 length, i16 data[length] (pad to 4)
  progs[n]: char label[32], u16 gm (0-127 melodic, 128 drums), u16 nZones,
    zones[nZones] (28 bytes):
      u8 keyLo,keyHi,velLo,velHi; u16 sampleIdx; u8 rootKey, u8 exclusiveClass
      i8 coarse; i8 fine(cents); u16 att_cB (initialAttenuation, EMU 0.4 conv.
      applied by the engine); i16 pan10 (-500..500, 0.1%)
      i16 tc[5]: delay,attack,hold,decay,release (timecents); u16 sustain_cB
  songs[n]: char name[32]; f32 loopStart, loopEnd (<0 = none); u32 nTracks;
    tracks[n]: u32 nEvents, events (12 B, same shape as music.pak v2):
      f32 t; u8 type (0 n,1 v,2 p,5 m,9 bendSemis); u8 key,vel,vid; f32 x
      (n: x=gate seconds, vid=song program slot; v/p/m: x=value 0..127 /
       pan -64..63 / mod 0..127; 9: x=semitones)
    followed by u32 nVids, u16 progIdx[nVids] (song program slots -> progs[])
"""
import argparse
import re
import struct
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
WEB = ROOT / "pokeemerald-music" / "web"

# ---------------------------------------------------------------- SMF parsing

def parse_midi(path):
    """-> (tempo-mapped seconds) dict:
    channels: ch -> list of ('n', t, key, vel, dur) / ('cc', t, cc, val) /
                            ('bend', t, value14) / ('prog', t, p)
    loop: (startSec, endSec) or None"""
    data = path.read_bytes()
    assert data[:4] == b"MThd"
    fmt, ntrk, div = struct.unpack(">HHH", data[8:14])
    pos = 14
    raw = []          # (tick, order, kind, ...)
    order = 0
    for _ in range(ntrk):
        assert data[pos:pos + 4] == b"MTrk"
        ln = struct.unpack(">I", data[pos + 4:pos + 8])[0]
        tr = data[pos + 8:pos + 8 + ln]
        pos += 8 + ln
        i = 0
        run = 0
        tick = 0
        while i < len(tr):
            delta = 0
            while tr[i] & 0x80:
                delta = (delta << 7) | (tr[i] & 0x7F)
                i += 1
            delta = (delta << 7) | tr[i]
            i += 1
            tick += delta
            b = tr[i]
            if b == 0xFF:
                mt, l = tr[i + 1], tr[i + 2]
                body = tr[i + 3:i + 3 + l]
                if mt == 0x51:
                    raw.append((tick, order, "tempo", int.from_bytes(body, "big")))
                elif mt == 0x06:
                    raw.append((tick, order, "marker", body.decode(errors="replace")))
                i += 3 + l
                order += 1
                continue
            if b in (0xF0, 0xF7):
                j = i + 1
                l = 0
                while tr[j] & 0x80:
                    l = (l << 7) | (tr[j] & 0x7F)
                    j += 1
                l = (l << 7) | tr[j]
                i = j + 1 + l
                continue
            if b & 0x80:
                run = b
                i += 1
            st, ch = run & 0xF0, run & 0x0F
            if st == 0x90 and tr[i + 1] > 0:
                raw.append((tick, order, "on", ch, tr[i], tr[i + 1]))
                i += 2
            elif st == 0x80 or (st == 0x90 and tr[i + 1] == 0):
                raw.append((tick, order, "off", ch, tr[i]))
                i += 2
            elif st == 0xB0:
                raw.append((tick, order, "cc", ch, tr[i], tr[i + 1]))
                i += 2
            elif st == 0xC0:
                raw.append((tick, order, "prog", ch, tr[i]))
                i += 1
            elif st == 0xE0:
                raw.append((tick, order, "bend", ch, ((tr[i + 1] << 7) | tr[i]) - 8192))
                i += 2
            elif st in (0xA0,):
                i += 2
            elif st in (0xD0,):
                i += 1
            else:
                i += 1
            order += 1
    raw.sort(key=lambda e: (e[0], e[1]))

    # tempo map -> seconds
    sec_per_tick = 500000 / 1e6 / div
    t = 0.0
    last_tick = 0
    events = []
    loop = [None, None]
    for e in raw:
        t += (e[0] - last_tick) * sec_per_tick
        last_tick = e[0]
        if e[2] == "tempo":
            sec_per_tick = e[3] / 1e6 / div
        elif e[2] == "marker":
            if e[3] == "[":
                loop[0] = t
            elif e[3] == "]":
                loop[1] = t
        else:
            events.append((t,) + e[2:])

    chans = defaultdict(list)
    held = {}
    for ev in events:
        t = ev[0]
        if ev[1] == "on":
            _, _, ch, key, vel = ev
            held.setdefault((ch, key), []).append((t, vel))
        elif ev[1] == "off":
            _, _, ch, key = ev
            if held.get((ch, key)):
                t0, vel = held[(ch, key)].pop(0)
                chans[ch].append(("n", t0, key, vel, max(t - t0, 0.005)))
        else:
            kind, ch = ev[1], ev[2]
            chans[ch].append((kind, t) + ev[3:])
    for (ch, key), rest in held.items():   # notes never released: hold 1s
        for t0, vel in rest:
            chans[ch].append(("n", t0, key, vel, 1.0))
    for ch in chans:
        chans[ch].sort(key=lambda e: e[1])
    lp = (loop[0], loop[1]) if loop[0] is not None and loop[1] is not None \
        and loop[1] > loop[0] else None
    return chans, lp

# ---------------------------------------------------------------- SF2 parsing

GEN_KEYRANGE, GEN_VELRANGE = 43, 44
GEN_ATT, GEN_PAN = 48, 17
GEN_COARSE, GEN_FINE, GEN_SAMPLEMODES = 51, 52, 54
GEN_ROOTKEY, GEN_EXCLUSIVE, GEN_SAMPLEID, GEN_INSTRUMENT = 58, 57, 53, 41
GEN_DELAY, GEN_ATTACK, GEN_HOLD, GEN_DECAY, GEN_SUSTAIN, GEN_RELEASE = 33, 34, 35, 36, 37, 38
GEN_FILTERFC = 8


def parse_sf2(path):
    data = path.read_bytes()
    chunks = {}
    def walk(start, end):
        i = start
        while i < end - 8:
            cid = data[i:i + 4]
            sz = struct.unpack("<I", data[i + 4:i + 8])[0]
            if cid == b"LIST":
                walk(i + 12, i + 8 + sz)
            else:
                chunks[cid] = (i + 8, sz)
            i += 8 + sz + (sz & 1)
    walk(12, len(data))

    def records(cid, size):
        off, sz = chunks[cid]
        return [data[off + k:off + k + size] for k in range(0, sz, size)]

    phdr = []
    for r in records(b"phdr", 38):
        name = r[:20].split(b"\0")[0].decode(errors="replace")
        prog, bank, bagndx = struct.unpack("<HHH", r[20:26])
        phdr.append((name, prog, bank, bagndx))
    pbag = [struct.unpack("<HH", r) for r in records(b"pbag", 4)]
    pgen = [struct.unpack("<Hh", r) for r in records(b"pgen", 4)]
    inst = []
    for r in records(b"inst", 22):
        name = r[:20].split(b"\0")[0].decode(errors="replace")
        inst.append((name, struct.unpack("<H", r[20:22])[0]))
    ibag = [struct.unpack("<HH", r) for r in records(b"ibag", 4)]
    igen = [struct.unpack("<Hh", r) for r in records(b"igen", 4)]
    shdr = []
    for r in records(b"shdr", 46):
        name = r[:20].split(b"\0")[0].decode(errors="replace")
        (start, end, ls, le, rate, pitch, corr, link, typ) = struct.unpack(
            "<IIIIIBbHH", r[20:46])
        shdr.append(dict(name=name, start=start, end=end, loopStart=ls,
                         loopEnd=le, rate=rate, pitch=pitch, corr=corr,
                         typ=typ))
    smpl_off, smpl_sz = chunks[b"smpl"]

    def bag_gens(bags, gens, b0, b1):
        out = []
        for b in range(b0, b1):
            g0 = bags[b][0]
            g1 = bags[b + 1][0] if b + 1 < len(bags) else len(gens)
            out.append(dict(gens[g0:g1]))
        return out

    presets = {}
    for k in range(len(phdr) - 1):
        name, prog, bank, bag0 = phdr[k]
        bag1 = phdr[k + 1][3]
        zones = bag_gens(pbag, pgen, bag0, bag1)
        pglobal = {}
        out = []
        for z in zones:
            if GEN_INSTRUMENT not in z:
                pglobal = z
                continue
            iidx = z[GEN_INSTRUMENT]
            ibag0 = inst[iidx][1]
            ibag1 = inst[iidx + 1][1] if iidx + 1 < len(inst) else len(ibag)
            izones = bag_gens(ibag, igen, ibag0, ibag1)
            iglobal = {}
            for iz in izones:
                if GEN_SAMPLEID not in iz:
                    iglobal = iz
                    continue
                g = dict(iglobal)
                g.update(iz)
                # preset gens are additive to instrument gens
                for gen, val in {**pglobal, **z}.items():
                    if gen in (GEN_INSTRUMENT,):
                        continue
                    if gen in (GEN_KEYRANGE, GEN_VELRANGE):
                        # intersect ranges
                        plo, phi = val & 0xFF, val >> 8
                        ilo, ihi = (g.get(gen, 0x7F00 | 0) & 0xFF,
                                    g.get(gen, 0x7F00) >> 8)
                        g[gen] = max(plo, ilo) | (min(phi, ihi) << 8)
                    else:
                        g[gen] = g.get(gen, 0) + val
                out.append(g)
        presets[(bank, prog)] = (name, out)
    return presets, shdr, (smpl_off, smpl_sz), data

# ------------------------------------------------------------- name matching

def orig_name_map(pret):
    m = {}
    for line in (Path(pret).expanduser() / "include" / "constants" /
                 "songs.h").read_text().splitlines():
        g = re.match(r"#define\s+(MUS_\w+)\s+\S+\s*//\s*(MUS_\w+)", line)
        if g:
            m[g.group(2).lower().removeprefix("mus_")] = g.group(1).lower()
    return m


def pak_song_names():
    pak = (WEB / "music.pak").read_bytes()
    _, _, nsamp, nsongs = struct.unpack_from("<4sIII", pak, 0)
    off = 16
    for _ in range(nsamp):
        length = struct.unpack_from("<I", pak, off + 76)[0]
        off += 80 + (length + 3) // 4 * 4
    names = []
    for _ in range(nsongs):
        names.append(pak[off:off + 32].split(b"\0")[0].decode())
        nv, nt = struct.unpack_from("<II", pak, off + 128)
        off += 136 + nv * 48
        for _ in range(nt):
            ne = struct.unpack_from("<I", pak, off)[0]
            off += 4 + ne * 12
    return names

# --------------------------------------------------------------------- emit

def timecents(g, gen, default=-12000):
    v = g.get(gen, default)
    return max(-12000, min(8000, v))


def thin_zones(zones, bank):
    """Halve dense key multisampling: SC-88 rips carry a sample every 2-4
    semitones; keeping every 2nd split and stretching its key range is
    inaudible in context and halves the pak. Stereo pairs (two zones with the
    same key range) are kept/dropped together.

    Only true melodic multisamples qualify: drum kits (bank 128) and kit-like
    presets (a different sound per key or two — e.g. the SC-88 'Gunshot'
    effects bank that Route 104 uses as percussion) must keep every zone, or
    keys start triggering the neighbouring instrument pitched over."""
    if bank == 128:
        return zones
    by_range = {}
    for g in zones:
        kr = g.get(GEN_KEYRANGE, 0x7F00)
        by_range.setdefault(kr, []).append(g)
    splits = sorted(by_range, key=lambda kr: kr & 0xFF)
    if len(splits) < 4:
        return zones
    narrow = sum(1 for kr in splits if (kr >> 8) - (kr & 0xFF) < 4)
    if narrow > len(splits) // 2:
        return zones                     # mostly per-key sounds (a kit): keep all
    kept = splits[::2]
    if kept[-1] != splits[-1]:
        kept.append(splits[-1])          # keep the top split for the extremes
    out = []
    for i, kr in enumerate(kept):
        lo = kr & 0xFF if i > 0 else splits[0] & 0xFF
        hi = (kept[i + 1] & 0xFF) - 1 if i + 1 < len(kept) else splits[-1] >> 8
        for g in by_range[kr]:
            g = dict(g)
            g[GEN_KEYRANGE] = lo | (hi << 8)
            out.append(g)
    return out


ONESHOT_CAP_S = 3.0
FADE_S = 0.12
RATE_CAP = 32000


def condition_pcm(raw, n, rate, looped):
    """One-shot cap + fade, then downsample to <=32 kHz (SC-88 native rate).
    Returns (bytes, n, rate, scale) — scale = new/old length for loop points."""
    import array
    pcm = array.array("h")
    pcm.frombytes(raw)
    if not looped and len(pcm) > int(ONESHOT_CAP_S * rate):
        cut = int(ONESHOT_CAP_S * rate)
        fade = min(int(FADE_S * rate), cut)
        for i in range(fade):
            pcm[cut - fade + i] = int(pcm[cut - fade + i] * (fade - i) / fade)
        del pcm[cut:]
    scale = 1.0
    if rate > RATE_CAP:
        try:
            from scipy.signal import resample_poly
        except ImportError:
            sys.exit("scipy needed to downsample (or pass --no-resample)")
        import math
        g = math.gcd(int(rate), RATE_CAP)
        res = resample_poly(list(pcm), RATE_CAP // g, int(rate) // g)
        pcm = array.array("h", (max(-32768, min(32767, int(v))) for v in res))
        scale = RATE_CAP / rate
        rate = RATE_CAP
    return pcm.tobytes(), len(pcm), rate, scale


def build(args):
    presets, shdr, (smpl_off, _), sf2data = parse_sf2(args.sf2)
    omap = orig_name_map(args.pret)
    known = set(pak_song_names())

    midis = {}
    for p in sorted(args.midis.glob("*.mid")) + sorted(args.midis.glob("*.MID")):
        stem = p.stem.lower().removeprefix("mus_")
        mus = omap.get(stem) or (f"mus_{stem}" if f"mus_{stem}" in known else None)
        if mus and mus not in midis:
            midis[mus] = p

    used_progs = set()      # (bank, prog)
    songs = []
    for mus, path in sorted(midis.items()):
        chans, loop = parse_midi(path)
        tracks = []
        vids = []           # (bank, prog) per song slot
        vid_of = {}
        for ch in sorted(chans):
            evs = []
            prog = 0
            bend_range = 2.0
            rpn = None
            drum = ch == 9
            for e in chans[ch]:
                if e[0] == "n":
                    _, t, key, vel, dur = e
                    bp = (128, 0) if drum else (0, prog)
                    if bp not in vid_of:
                        vid_of[bp] = len(vids)
                        vids.append(bp)
                    evs.append((t, 0, key, vel, vid_of[bp], dur))
                elif e[0] == "prog":
                    prog = e[2]
                elif e[0] == "bend":
                    evs.append((e[1], 9, 0, 0, 0, e[2] / 8192.0 * bend_range))
                elif e[0] == "cc":
                    _, t, cc, val = e
                    if cc == 7:
                        evs.append((t, 1, 0, 0, 0, float(val)))
                    elif cc == 10:
                        evs.append((t, 2, 0, 0, 0, float(val - 64)))
                    elif cc == 1:
                        evs.append((t, 5, 0, 0, 0, float(val)))
                    elif cc == 101:
                        rpn = ("msb", val)
                    elif cc == 100:
                        rpn = ("lsb", val) if rpn and rpn[1] == 0 else None
                    elif cc == 6 and rpn is not None:
                        bend_range = float(val)
                    # 0/32 bank (GM only), 30/31 GBA echo hints, 50/51,
                    # 91 reverb send: intentionally ignored
            if any(ev[1] == 0 for ev in evs):
                tracks.append(sorted(evs, key=lambda ev: ev[0]))
        used_progs.update(vids)
        songs.append((mus, loop, tracks, vids))

    # flatten used presets -> zones, collect used samples
    prog_entries = []       # (label, gm, zones)
    prog_index = {}
    sample_index = {}
    samples = []            # dicts
    missing = []
    for bp in sorted(used_progs):
        if bp not in presets:
            missing.append(bp)
            continue
        name, zones = presets[bp]
        if not args.no_thin:
            zones = thin_zones(zones, bp[0])
        zrecs = []
        for g in zones:
            sid = g[GEN_SAMPLEID]
            s = shdr[sid]
            mode = g.get(GEN_SAMPLEMODES, 0) & 3
            if sid not in sample_index:
                sample_index[sid] = len(samples)
                samples.append(dict(sh=s, loop=1 if mode in (1, 3) else 0))
            else:
                if mode in (1, 3):
                    samples[sample_index[sid]]["loop"] = 1
            kr = g.get(GEN_KEYRANGE, 0x7F00)
            vr = g.get(GEN_VELRANGE, 0x7F00)
            root = g.get(GEN_ROOTKEY, -1)
            if root < 0:
                root = s["pitch"] if s["pitch"] <= 127 else 60
            fine = g.get(GEN_FINE, 0) + s["corr"]
            zrecs.append(dict(
                keyLo=kr & 0xFF, keyHi=kr >> 8, velLo=vr & 0xFF, velHi=vr >> 8,
                sample=sample_index[sid], root=root,
                excl=g.get(GEN_EXCLUSIVE, 0),
                coarse=g.get(GEN_COARSE, 0), fine=fine,
                att=max(0, min(1440, g.get(GEN_ATT, 0))),
                pan=max(-500, min(500, g.get(GEN_PAN, 0))),
                mode=mode,
                tc=[timecents(g, GEN_DELAY), timecents(g, GEN_ATTACK),
                    timecents(g, GEN_HOLD), timecents(g, GEN_DECAY),
                    timecents(g, GEN_RELEASE, -3600)],
                sus=max(0, min(1440, g.get(GEN_SUSTAIN, 0))),
                fc=g.get(GEN_FILTERFC, 13500),
            ))
        prog_index[bp] = len(prog_entries)
        gm = 128 if bp[0] == 128 else bp[1]
        prog_entries.append((name, gm, zrecs))
    if missing:
        print(f"WARNING: presets not in sf2: {missing}")

    # sample data: trim looped samples at loopEnd+4, cap/fade one-shots,
    # downsample to the SC-88's native 32 kHz
    total = 0
    blobs = []
    for rec in samples:
        s = rec["sh"]
        start, end = s["start"], s["end"]
        ls, le = s["loopStart"] - start, s["loopEnd"] - start
        n = end - start
        if rec["loop"] and 0 < le <= n:
            n = min(n, le + 4)
        raw = sf2data[smpl_off + start * 2: smpl_off + (start + n) * 2]
        rate = s["rate"]
        if not args.no_resample:
            raw, n, rate, k = condition_pcm(raw, n, rate, rec["loop"])
            ls, le = int(ls * k), int(le * k)
        blobs.append((raw, n, ls, le))
        total += n * 2
        rec["n"], rec["ls"], rec["le"], rec["rate"] = n, ls, le, rate

    if args.stats:
        atts = sorted(z["att"] for _, _, zs in prog_entries for z in zs)
        fcs = [z["fc"] for _, _, zs in prog_entries for z in zs if z["fc"] < 13500]
        nz = sum(len(zs) for _, _, zs in prog_entries)
        print(f"songs: {len(songs)}  progs: {len(prog_entries)}  zones: {nz}  "
              f"samples: {len(samples)}  sample bytes: {total/1e6:.1f} MB")
        print(f"attenuation cB: min {atts[0]} median {atts[len(atts)//2]} "
              f"max {atts[-1]}; zones with filter: {len(fcs)}")
        ev = sum(len(t) for _, _, ts, _ in songs for t in ts)
        print(f"events: {ev} (~{ev*12/1e6:.1f} MB)")
        return 0

    out = bytearray()
    out += struct.pack("<4sIIII", b"M4AO", 1, len(samples), len(prog_entries),
                       len(songs))
    for rec, (raw, n, ls, le) in zip(samples, blobs):
        out += struct.pack("<fIIII", float(rec["rate"]), rec["loop"],
                           max(0, ls), max(0, min(le, n)), n)
        out += raw
        out += b"\0" * (-len(raw) % 4)
    for name, gm, zs in prog_entries:
        out += struct.pack("<32sHH", name.encode(errors="replace"), gm, len(zs))
        for z in zs:
            out += struct.pack("<BBBBHBBbbHh5hH",
                               z["keyLo"], z["keyHi"], z["velLo"], z["velHi"],
                               z["sample"], z["root"], z["excl"],
                               max(-128, min(127, z["coarse"])),
                               max(-128, min(127, z["fine"])),
                               z["att"], z["pan"], *z["tc"], z["sus"])
        out += b"\0" * (-(26 * len(zs)) % 4)   # keep song f32 fields 4-aligned
    for mus, loop, tracks, vids in songs:
        out += struct.pack("<32sff", mus.encode(),
                           loop[0] if loop else -1.0, loop[1] if loop else -1.0)
        out += struct.pack("<I", len(tracks))
        for evs in tracks:
            out += struct.pack("<I", len(evs))
            for t, typ, key, vel, vid, x in evs:
                out += struct.pack("<fBBBBf", t, typ, key, vel, vid, x)
        out += struct.pack("<I", len(vids))
        for bp in vids:
            out += struct.pack("<H", prog_index.get(bp, 0xFFFF))
        out += b"\0" * (-(len(vids) * 2) % 4)

    dest = WEB / "music-sc88.pak"
    dest.write_bytes(out)
    print(f"{dest}: {len(songs)} songs, {len(prog_entries)} progs, "
          f"{len(samples)} samples, {len(out)/1e6:.1f} MB")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--midis", type=Path, default=ROOT / "midi-sc88")
    ap.add_argument("--sf2", type=Path, default=ROOT / "GBApokemonTestLite.sf2")
    ap.add_argument("--pret", type=Path, default=Path("~/pokeemerald"))
    ap.add_argument("--stats", action="store_true")
    ap.add_argument("--no-thin", action="store_true")
    ap.add_argument("--no-resample", action="store_true")
    args = ap.parse_args()
    return build(args)


if __name__ == "__main__":
    sys.exit(main())
