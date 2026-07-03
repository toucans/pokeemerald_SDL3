#!/usr/bin/env python3
"""Render preview WAVs from the extracted song JSON.

Mirrors web/player.js (same envelope math, same GBA-flavoured output) so the
WAVs in previews/ are honest previews of what the WebAudio page plays.
Needs numpy+scipy; everything else is stdlib.

Usage: render_previews.py [--seconds 45] [--rate 44100] [song ...]
"""
import argparse
import base64
import json
import math
import wave
from pathlib import Path

import numpy as np
from scipy.signal import lfilter

FRAME = 1 / 59.7275
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


def sample_chunk(smp, ratio, out_rate, n_out):
    """Resample `n_out` frames of a (possibly looped) sample at pitch ratio."""
    step = smp["rate"] * ratio / out_rate
    pos = np.arange(n_out, dtype=np.float64) * step
    data, n = smp["data"], len(smp["data"])
    if smp["loop"]:
        ls = smp["loopStart"]
        span = n - ls
        pos = np.where(pos < n, pos, ls + np.mod(pos - ls, span))
    else:
        pos = np.minimum(pos, n - 1.001)
    i = pos.astype(np.int64)
    frac = (pos - i).astype(np.float32)
    j = np.minimum(i + 1, n - 1)
    out = data[i] * (1 - frac) + data[j] * frac
    if not smp["loop"]:
        out[np.arange(n_out) * step >= n - 1] = 0.0
    return out


def square_osc(freq_hz, duty, n, rate, phase0=0.0):
    """Naive pulse wave rendered 4x oversampled then averaged (cheap AA)."""
    os = 4
    t = phase0 + np.arange(n * os, dtype=np.float64) * (freq_hz / (rate * os))
    frac = np.mod(t, 1.0)
    w = np.where(frac < duty, 1.0, -1.0).astype(np.float32)
    return w.reshape(n, os).mean(axis=1)


def wave_osc(pattern, freq_hz, n, rate):
    table = (np.array(pattern, dtype=np.float32) - 7.5) / 7.5
    pos = np.mod(np.arange(n, dtype=np.float64) * (freq_hz * 32 / rate), 32)
    i = pos.astype(np.int64)
    frac = (pos - i).astype(np.float32)
    return table[i] * (1 - frac) + table[(i + 1) % 32] * frac


def lfsr_noise(period7):
    bits = 127 if period7 else 32767
    out = np.empty(bits, dtype=np.float32)
    lfsr = 0x7FFF
    for i in range(bits):
        bit = (lfsr ^ (lfsr >> 1)) & 1
        lfsr = (lfsr >> 1) | (bit << (6 if period7 else 14))
        if period7:
            lfsr &= 0x7F
        out[i] = 0.6 if (lfsr & 1) else -0.6
    return out


def ds_env(n_gate, n_total, adsr, peak, rate, rel_scale, echo_v, echo_l):
    """m4a DirectSound envelope as a float array of n_total samples."""
    a, d, s, r = adsr
    env = np.zeros(n_total, dtype=np.float32)
    t = np.arange(n_total, dtype=np.float32) / rate
    # attack
    t_a = 0.0 if a >= 255 else (255 / max(1, a)) * FRAME
    att = np.where(t < t_a, t / max(t_a, 1e-9), 1.0)
    # decay toward sustain
    sus = s / 256.0
    if d == 0:
        dec = np.where(t >= t_a, sus, 1.0)
    else:
        tau = FRAME / math.log(256 / d)
        dec = np.where(t >= t_a, sus + (1 - sus) * np.exp(-(t - t_a) / tau), 1.0)
    env = np.minimum(att, dec)
    # release
    t_off = n_gate / rate
    if r == 0:
        env[n_gate:] = 0.0
    else:
        tau = (FRAME / math.log(256 / r)) * rel_scale
        lvl = env[min(n_gate, n_total - 1)]
        rel = lvl * np.exp(-(t[n_gate:] - t_off) / tau)
        if echo_v > 0 and echo_l > 0:
            n_echo = n_gate + int(echo_l * FRAME * rate)
            rel = np.maximum(rel, echo_v / 256.0)
            cut = n_echo - n_gate
            if cut < len(rel):
                rel[cut:] = 0.0
        env[n_gate:] = rel
    return env * peak


def psg_env(n_gate, n_total, adsr, peak, rate, rel_scale):
    a, d, s, r = adsr
    t = np.arange(n_total, dtype=np.float32) / rate
    t_a = a * 15 * FRAME
    att = np.where(t < t_a, t / max(t_a, 1e-9), 1.0) if a else np.ones(n_total, np.float32)
    sus = s / 15.0
    t_d = d * (15 - s) * FRAME
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
        t_r = r * 15 * FRAME * rel_scale
        lvl = env[min(n_gate, n_total - 1)]
        env[n_gate:] = np.clip(lvl * (1 - (t[n_gate:] - t_off) / max(t_r, 1e-9)), 0, 1)
    return env * peak


# ----------------------------------------------------------- track playback

def expand_events(track, loop_start, loop_end, total):
    """Yield (abs_time, event) with GBA-style looping up to `total` seconds."""
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


def controller_curves(events, total, rate):
    """Track vol/pan/bend/mod state sampled per note; return lookup closures."""
    state = []  # (time, vol, pan, bend, bendr, mod, lfos, echo_v, echo_l)
    cur = dict(v=100, p=0, b=0, br=2, m=0, ls=22, ev=0, el=0)
    state.append((0.0, cur.copy()))
    for at, e in events:
        if e[1] in cur:
            cur[e[1]] = e[2]
            state.append((at, cur.copy()))
    times = [s[0] for s in state]

    def at_time(t):
        import bisect
        i = bisect.bisect_right(times, t) - 1
        return state[max(0, i)][1]
    return at_time


def render_song(song, samples, total, rate):
    rel_scale = 1.0
    n_total = int(total * rate)
    mix = np.zeros((n_total, 2), dtype=np.float32)
    noise_cache = {}

    for track in song["tracks"]:
        events = expand_events(track, song["loopStart"], song["loopEnd"], total)
        lookup = controller_curves(events, total, rate)
        for at, e in events:
            if e[1] != "n":
                continue
            _, _, key, vel, dur, vid = e
            v = song["voices"][vid]
            st = lookup(at + 1e-4)
            vol = st["v"] / 127.0
            pan = (v["pan"] / 128.0) if "pan" in v else st["p"] / 64.0
            bend_st = (st["b"] * st["br"] / 64.0)
            play_key = v["base"] if v.get("rhythm") else key
            peak = vel / 127.0

            n_gate = max(1, int(dur * rate))
            if v["t"] == "pcm":
                tail = (FRAME / math.log(256 / v["adsr"][3])) * rel_scale * 7 \
                    if v["adsr"][3] else 0.01
            else:
                tail = v["adsr"][3] * 15 * FRAME * rel_scale + 0.01
            n_tot = min(n_total - int(at * rate), n_gate + int(tail * rate) + 1)
            if n_tot <= 0:
                continue
            i0 = int(at * rate)

            semis = bend_st
            if st["m"]:  # vibrato: average pitch unchanged; add sine FM
                lfo_hz = st["ls"] * 59.7275 / 256
                depth = st["m"] * 4 / 256.0  # semitones
                tt = np.arange(n_tot) / rate
                vib = depth * np.sin(2 * np.pi * lfo_hz * tt)
            else:
                vib = None

            if v["t"] == "pcm":
                ratio = 1.0 if v.get("fixed") else 2 ** ((play_key - 60 + semis) / 12)
                sig = sample_chunk(samples[v["sample"]], ratio, rate, n_tot)
                env = ds_env(min(n_gate, n_tot), n_tot, v["adsr"], peak, rate,
                             rel_scale, st["ev"], st["el"])
            else:
                freq = 440 * 2 ** ((play_key - 69 + semis) / 12)
                if vib is not None:
                    freq = freq * 2 ** (vib / 12)
                    phase = np.cumsum(freq / rate)
                else:
                    phase = None
                if v["t"] == "sq":
                    duty = [0.125, 0.25, 0.5, 0.75][v["duty"]]
                    if phase is None:
                        sig = square_osc(freq, duty, n_tot, rate)
                    else:
                        frac = np.mod(phase, 1.0)
                        sig = np.where(frac < duty, 1.0, -1.0).astype(np.float32)
                elif v["t"] == "wave":
                    if phase is None:
                        sig = wave_osc(v["pattern"], freq, n_tot, rate)
                    else:
                        table = (np.array(v["pattern"], np.float32) - 7.5) / 7.5
                        sig = table[np.mod((phase * 32).astype(np.int64), 32)]
                else:  # noise
                    p7 = v["period"] == 1
                    if p7 not in noise_cache:
                        noise_cache[p7] = lfsr_noise(p7)
                    nz = noise_cache[p7]
                    step = 48000 / rate * 2 ** ((play_key - 60) / 12)
                    idx = np.mod((np.arange(n_tot) * step).astype(np.int64), len(nz))
                    sig = nz[idx]
                lvl = 0.5 if v["t"] != "noise" else 0.4
                env = psg_env(min(n_gate, n_tot), n_tot, v["adsr"], peak * lvl,
                              rate, rel_scale)
            sig = sig[:n_tot] * env * vol
            l = sig * min(1.0, 1 - pan)
            r = sig * min(1.0, 1 + pan)
            mix[i0:i0 + n_tot, 0] += l
            mix[i0:i0 + n_tot, 1] += r
    return mix


# --------------------------------------------------------------- mastering

def biquad_lowpass(x, fc, rate, q=0.55):
    w0 = 2 * np.pi * fc / rate
    alpha = np.sin(w0) / (2 * q)
    b0 = (1 - np.cos(w0)) / 2
    b1 = 1 - np.cos(w0)
    b2 = b0
    a0 = 1 + alpha
    a1 = -2 * np.cos(w0)
    a2 = 1 - alpha
    b = np.array([b0, b1, b2]) / a0
    a = np.array([1.0, a1 / a0, a2 / a0])
    return lfilter(b, a, x, axis=0).astype(np.float32)


def master_original(mix, rate, reverb):
    out = biquad_lowpass(mix, 6600, rate)
    # m4a-ish feedback echo
    d = int(5 * FRAME * rate)
    g = reverb / 128 * 0.75
    wetg = reverb / 128 * 0.9
    acc = np.zeros_like(out)
    src = out.copy()
    for _ in range(6):
        delayed = np.zeros_like(src)
        delayed[d:] = src[:-d]
        acc += delayed
        src = delayed * g
    out = out + acc * wetg
    return out * 0.5



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
        mix = render_song(song, samples, args.seconds, args.rate)
        out = master_original(mix, args.rate, song.get("reverb", 0))
        # fade out the last 2.5 s
        n = len(out)
        f = int(2.5 * args.rate)
        out[n - f:] *= np.linspace(1, 0, f)[:, None]
        p = outdir / f"{entry['name']}.wav"
        peak = write_wav(p, out, args.rate)
        rms = float(np.sqrt((out ** 2).mean()))
        print(f"{p.name:38} peak={peak:5.2f} rms={rms:.3f}")


if __name__ == "__main__":
    main()
