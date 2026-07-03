#!/usr/bin/env python3
"""Render preview WAVs from the extracted song JSON.

Mirrors web/player.js (same m4a engine model taken from the decomp source) so
the WAVs in previews/ are honest previews of what the WebAudio page plays.
Needs numpy; everything else is stdlib.

Usage: render_previews.py [--seconds 45] [--rate 44100] [song ...]
"""
import argparse
import base64
import json
import math
import wave
from pathlib import Path

import numpy as np

FRAME = 1 / 59.7275
PSG_FULL = 15 * 8 / 1024     # one full PSG channel vs full-scale PCM channel
MASTER = 0.45
ROOT = Path(__file__).parent
DATA = ROOT / "web" / "data"


# ------------------------------------------------------------------ helpers

def load_samples():
    raw = json.loads((DATA / "samples.json").read_text())
    out = {}
    for k, v in raw.items():
        pcm = np.frombuffer(base64.b64decode(v["b64"]), dtype=np.int8)
        out[k] = {
            "data": pcm.astype(np.float32) / 128.0,
            "rate": v["rate"], "loop": v["loop"], "loopStart": v["loopStart"],
        }
    return out


def sample_chunk(smp, pos):
    """Linear-interpolated read of a (possibly looped) sample at positions."""
    data, n = smp["data"], len(smp["data"])
    if smp["loop"]:
        ls = smp["loopStart"]
        pos = np.where(pos < n, pos, ls + np.mod(pos - ls, n - ls))
    else:
        dead = pos >= n - 1
        pos = np.minimum(pos, n - 1.001)
    i = pos.astype(np.int64)
    frac = (pos - i).astype(np.float32)
    j = np.minimum(i + 1, n - 1)
    out = data[i] * (1 - frac) + data[j] * frac
    if not smp["loop"]:
        out[dead] = 0.0
    return out


# --------------------------------------------------- GB PSG frequency math

CGB_FREQ = [-2004, -1891, -1785, -1685, -1591, -1501, -1417, -1337,
            -1262, -1192, -1125, -1062]


def sq_freq(key):
    k = max(36, min(166, key)) - 36
    reg = 2048 + (CGB_FREQ[k % 12] >> (k // 12))
    return 131072 / (2048 - reg)


NOISE_TABLE = [(s << 4) | r for s in range(13, -1, -1)
               for r in (7, 6, 5, 4)] + [3, 2, 1, 0]


def noise_clock(key):
    nr43 = NOISE_TABLE[max(0, min(59, key - 21))]
    s, r = nr43 >> 4, nr43 & 7
    return 524288 / (r or 0.5) / 2 ** (s + 1)


def square_osc(freq_hz, duty, n, rate):
    """Naive +-1 pulse wave rendered 4x oversampled then averaged (cheap AA).
    freq_hz is scalar or per-sample array (vibrato/bends)."""
    os = 4
    if np.isscalar(freq_hz):
        t = np.arange(n * os, dtype=np.float64) * (freq_hz / (rate * os))
    else:
        t = np.cumsum(np.repeat(freq_hz, os) / (rate * os))
    frac = np.mod(t, 1.0)
    w = np.where(frac < duty, 1.0, -1.0).astype(np.float32)
    return w.reshape(n, os).mean(axis=1)


def wave_osc(pattern, freq_hz, n, rate):
    table = (np.array(pattern, dtype=np.float32) - 7.5) / 7.5
    if np.isscalar(freq_hz):
        pos = np.mod(np.arange(n, dtype=np.float64) * (freq_hz * 32 / rate), 32)
    else:
        pos = np.mod(np.cumsum(freq_hz * 32 / rate), 32)
    i = pos.astype(np.int64)
    frac = (pos - i).astype(np.float32)
    return table[i] * (1 - frac) + table[(i + 1) % 32] * frac


def lfsr_noise(period7):
    bits = 127 if period7 else 32767
    out = np.empty(bits, dtype=np.float32)
    lfsr = 0x7F if period7 else 0x7FFF
    for i in range(bits):
        bit = (lfsr ^ (lfsr >> 1)) & 1
        lfsr = (lfsr >> 1) | (bit << (6 if period7 else 14))
        out[i] = 1.0 if (lfsr & 1) else -1.0
    return out


def wave_vol_code(level):
    if level <= 1:
        return 0.0
    if level <= 5:
        return 0.25
    if level <= 9:
        return 0.5
    if level <= 13:
        return 0.75
    return 1.0


# ------------------------------------------------------------- envelopes

def ds_env(n_gate, n_total, adsr, peak, rate, echo_v, echo_l):
    a, d, s, r = adsr
    t = np.arange(n_total, dtype=np.float32) / rate
    t_a = 0.0 if a >= 255 else (255 / max(1, a)) * FRAME
    att = np.where(t < t_a, t / max(t_a, 1e-9), 1.0)
    sus = s / 256.0
    if d == 0:
        dec = np.where(t >= t_a, sus, 1.0)
    else:
        tau = FRAME / math.log(256 / d)
        dec = np.where(t >= t_a, sus + (1 - sus) * np.exp(-(t - t_a) / tau), 1.0)
    env = np.minimum(att, dec)
    t_off = n_gate / rate
    if r == 0:
        env[n_gate:] = 0.0
    else:
        tau = FRAME / math.log(256 / r)
        lvl = env[min(n_gate, n_total - 1)]
        rel = lvl * np.exp(-(t[n_gate:] - t_off) / tau)
        if echo_v > 0 and echo_l > 0:
            # pseudo-echo: release decays DOWN INTO echoVol, holds echoLen
            # frames, then the channel is cut (continuous - no jump).
            echo = echo_v / 256.0
            floor = np.maximum(rel, min(echo, lvl))
            t_reach = tau * math.log(lvl / echo) if echo < lvl else 0.0
            cut = int((t_reach + echo_l * FRAME) * rate)
            if cut < len(floor):
                fade = min(len(floor) - cut, max(1, int(0.003 * rate)))
                floor[cut:cut + fade] *= np.linspace(1, 0, fade)
                floor[cut + fade:] = 0.0
            rel = floor
        env[n_gate:] = rel
    return env * peak


def psg_env(n_gate, n_total, adsr, goal, rate, echo_v=0, echo_l=0):
    """GB envelope as fraction 0..1 of the volume goal (level steps)."""
    a, d, s, r = adsr
    sus_level = ((goal * s + 15) >> 4) if goal > 0 else 0
    sus = sus_level / goal if goal > 0 else 0.0
    t = np.arange(n_total, dtype=np.float32) / rate
    t_a = a * goal * FRAME
    att = np.where(t < t_a, t / max(t_a, 1e-9), 1.0) if a else np.ones(n_total, np.float32)
    t_d = d * max(0, goal - sus_level) * FRAME
    if d == 0:
        dec = np.where(t >= t_a, sus, 1.0)
    else:
        dec = np.clip(1 - (t - t_a) / max(t_d, 1e-9) * (1 - sus), sus, 1.0)
        dec[t < t_a] = 1.0
    env = np.minimum(att, dec)
    t_off = n_gate / rate
    if r == 0:
        env[n_gate:] = 0.0
    else:
        lvl = env[min(n_gate, n_total - 1)]
        echo_lvl = ((goal * echo_v + 0xFF) >> 8) if (echo_v > 0 and echo_l > 0
                                                     and goal > 0) else 0
        t_r = r * max(1, lvl * goal - echo_lvl) * FRAME
        rel = np.clip(lvl * (1 - (t[n_gate:] - t_off) / max(t_r, 1e-9)), 0, 1)
        if echo_lvl > 0 and echo_lvl / goal < lvl:
            # CGB pseudo-echo: release steps down to echo_lvl levels, holds
            # echoLen frames, then cuts.
            rel = np.maximum(rel, echo_lvl / goal)
            cut = int((t_r + echo_l * FRAME) * rate)
            if cut < len(rel):
                fade = min(len(rel) - cut, max(1, int(0.003 * rate)))
                rel[cut:cut + fade] *= np.linspace(1, 0, fade)
                rel[cut + fade:] = 0.0
        env[n_gate:] = rel
    return env


# ------------------------------------------------------- volume/pan maths

def side_vols(vel, vol, pan, rp):
    """ChnVolSetAsm: (velocity * panFactor * trackVolM) >> 14, 0..255/side."""
    y = max(-128, min(127, 2 * pan))
    vol_mr = ((y + 128) * 2 * vol) >> 8
    vol_ml = ((127 - y) * 2 * vol) >> 8
    return (min(255, (vel * (127 - rp) * vol_ml) >> 14),
            min(255, (vel * (rp + 128) * vol_mr) >> 14))


def psg_gains(vel, vol, pan, rp):
    """CgbModVol/CgbPan: quantised goal 0..15, hard 3-way pan."""
    l, r = side_vols(vel, vol, pan, rp)
    goal = min(15, (l + r) >> 4)
    amp = goal / 15 * PSG_FULL
    gl = gr = amp
    if r >= l and (r >> 1) >= l:
        gl = 0.0
    elif l > r and (l >> 1) >= r:
        gr = 0.0
    return goal, gl, gr


# ----------------------------------------------------------- track playback

def expand_events(track, loop_start, loop_end, total):
    looping = loop_start is not None and loop_end is not None and loop_end > loop_start
    out = []
    offset, pass_no = 0.0, 0
    while True:
        emitted = False
        for e in track:
            s = e[0]
            if looping:
                if pass_no > 0 and s < loop_start:
                    continue
                if s >= loop_end:
                    continue
            at = offset + (s if pass_no == 0 else s - loop_start)
            if at >= total:
                continue
            out.append((at, e))
            emitted = True
        if not looping or not emitted:
            break
        offset += loop_end if pass_no == 0 else loop_end - loop_start
        if offset >= total:
            break
        pass_no += 1
    out.sort(key=lambda x: x[0])
    return out


def controller_curves(events):
    import bisect
    state = []
    cur = dict(v=100, p=0, b=0, br=2, m=0, ls=22, ev=0, el=0)
    state.append((0.0, cur.copy()))
    for at, e in events:
        if e[1] in cur:
            cur[e[1]] = e[2]
            state.append((at, cur.copy()))
    times = [s[0] for s in state]

    def at_time(t):
        i = bisect.bisect_right(times, t) - 1
        return state[max(0, i)][1]
    return at_time


def triangle_lfo(n, rate, hz, depth_semis):
    """m4a LFO: triangle wave, peak +-depth (semitones)."""
    ph = np.mod(np.arange(n) * (hz / rate), 1.0)
    tri = np.where(ph < 0.25, ph * 4,
                   np.where(ph < 0.75, 2 - ph * 4, ph * 4 - 4))
    return depth_semis * tri.astype(np.float32)


def render_song(song, samples, total, rate):
    n_total = int(total * rate)
    pcm_mix = np.zeros((n_total, 2), dtype=np.float32)
    psg_mix = np.zeros((n_total, 2), dtype=np.float32)
    noise_cache = {}

    for track in song["tracks"]:
        events = expand_events(track, song["loopStart"], song["loopEnd"], total)
        lookup = controller_curves(events)
        for at, e in events:
            if e[1] != "n":
                continue
            _, _, key, vel, dur, vid = e
            v = song["voices"][vid]
            st = lookup(at + 1e-4)
            rp = v.get("pan", 0)
            play_key = v["base"] if v.get("rhythm") else key
            bend_semis = st["b"] * st["br"] / 64.0

            n_gate = max(1, int(dur * rate))
            i0 = int(at * rate)

            if v["t"] == "pcm":
                r_ = v["adsr"][3]
                tail = (FRAME / math.log(256 / r_)) * 7 if r_ else 0.01
                if st["ev"] > 0 and st["el"] > 0:
                    tail = max(tail, st["el"] * FRAME + 0.02)
            else:
                goal_pre = psg_gains(vel, st["v"], st["p"], rp)[0]
                tail = v["adsr"][3] * max(1, goal_pre) * FRAME + 0.01
                if st["ev"] > 0 and st["el"] > 0:
                    tail += st["el"] * FRAME
            n_tot = min(n_total - i0, n_gate + int(tail * rate) + 1)
            if n_tot <= 0:
                continue

            vib = None
            if st["m"] and v["t"] != "noise":
                # pitM += 16*modM (256/semitone) -> peak depth mod/16 semis
                vib = triangle_lfo(n_tot, rate, st["ls"] * 59.7275 / 256,
                                   st["m"] * 16 / 256.0)

            if v["t"] == "pcm":
                smp = samples[v["sample"]]
                ratio = 1.0 if v.get("fixed") else 2 ** ((play_key - 60 + bend_semis) / 12)
                if vib is not None and not v.get("fixed"):
                    step = smp["rate"] * ratio * 2 ** (vib / 12) / rate
                    pos = np.cumsum(step.astype(np.float64))
                else:
                    pos = np.arange(n_tot, dtype=np.float64) * (smp["rate"] * ratio / rate)
                sig = sample_chunk(smp, pos)
                env = ds_env(min(n_gate, n_tot), n_tot, v["adsr"], vel / 127,
                             rate, st["ev"], st["el"])
                y = max(-128, min(127, 2 * st["p"]))
                gl = ((127 - y) / 128) * ((127 - rp) / 128) * (st["v"] / 127)
                gr = ((y + 128) / 128) * ((rp + 128) / 128) * (st["v"] / 127)
                sig = sig[:n_tot] * env
                pcm_mix[i0:i0 + n_tot, 0] += sig * gl
                pcm_mix[i0:i0 + n_tot, 1] += sig * gr
            else:
                goal, gl, gr = psg_gains(vel, st["v"], st["p"], rp)
                if goal == 0:
                    continue
                if v["t"] == "sq":
                    f0 = sq_freq(play_key) * 2 ** (bend_semis / 12)
                    f = f0 * 2 ** (vib / 12) if vib is not None else f0
                    sig = square_osc(f, [0.125, 0.25, 0.5, 0.75][v["duty"]],
                                     n_tot, rate)
                elif v["t"] == "wave":
                    # 32-step wave runs at half the square rate (patterns hold
                    # two cycles, so written pitch matches)
                    f0 = sq_freq(play_key) / 2 * 2 ** (bend_semis / 12)
                    f = f0 * 2 ** (vib / 12) if vib is not None else f0
                    sig = wave_osc(v["pattern"], f, n_tot, rate)
                    sc = wave_vol_code(goal) * 15 / max(goal, 1)
                    gl, gr = gl * sc, gr * sc
                else:  # noise
                    p7 = v["period"] == 1
                    if p7 not in noise_cache:
                        noise_cache[p7] = lfsr_noise(p7)
                    nz = noise_cache[p7]
                    step = noise_clock(play_key) / rate
                    idx = np.mod((np.arange(n_tot) * step).astype(np.int64), len(nz))
                    sig = nz[idx]
                env = psg_env(min(n_gate, n_tot), n_tot, v["adsr"], goal, rate,
                              st["ev"], st["el"])
                sig = sig[:n_tot] * env
                psg_mix[i0:i0 + n_tot, 0] += sig * gl
                psg_mix[i0:i0 + n_tot, 1] += sig * gr
    return pcm_mix, psg_mix


# --------------------------------------------------------------- mastering

def apply_reverb(pcm, rate, reverb):
    """m4a reverb (PCM only): mono 2-tap (~117/100 ms) feedback echo at rev/128."""
    if reverb <= 0:
        return pcm
    d_a = int(7 * 224 / 13379 * rate)
    d_b = int(6 * 224 / 13379 * rate)
    g = reverb / 256.0
    n = len(pcm)
    mono = pcm.mean(axis=1).astype(np.float32)
    echo = np.zeros(n, dtype=np.float32)
    src = mono
    for _ in range(8):                       # unroll the feedback loop
        e = np.zeros(n, dtype=np.float32)
        e[d_a:] += g * src[:-d_a]
        e[d_b:] += g * src[:-d_b]
        if np.abs(e - echo).max() < 1e-5:
            echo = e
            break
        echo = e
        src = mono + echo
    return pcm + echo[:, None]


def write_wav(path, mix, rate):
    peak = np.abs(mix).max()
    if peak > 0.98:
        mix = mix * (0.98 / peak)
    pcm = (np.clip(mix, -1, 1) * 32767).astype("<i2")
    with wave.open(str(path), "wb") as w:
        w.setnchannels(2)
        w.setsampwidth(2)
        w.setframerate(rate)
        w.writeframes(pcm.tobytes())
    return peak


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--seconds", type=float, default=45)
    ap.add_argument("--rate", type=int, default=44100)
    ap.add_argument("songs", nargs="*")
    args = ap.parse_args()

    manifest = json.loads((DATA / "manifest.json").read_text())
    samples = load_samples()
    outdir = ROOT / "previews"
    outdir.mkdir(exist_ok=True)

    for entry in manifest:
        if args.songs and entry["name"] not in args.songs:
            continue
        song = json.loads((DATA / entry["file"]).read_text())
        pcm, psg = render_song(song, samples, args.seconds, args.rate)
        out = (apply_reverb(pcm, args.rate, song.get("reverb", 0)) + psg) * MASTER
        n = len(out)
        f = int(2.5 * args.rate)
        out[n - f:] *= np.linspace(1, 0, f)[:, None]
        p = outdir / f"{entry['name']}.wav"
        peak = write_wav(p, out, args.rate)
        rms = float(np.sqrt((out ** 2).mean()))
        print(f"{p.name:38} peak={peak:5.2f} rms={rms:.3f}")


if __name__ == "__main__":
    main()
