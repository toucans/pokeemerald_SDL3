# Handoff: rebuild the player as an AudioWorklet (plain JS)

Everything a fresh session needs to replace the current WebAudio-native-node
player with a single-loop **AudioWorklet** engine in **plain JavaScript**.
Read this top to bottom, then verify the load-bearing facts against the files
it cites before writing code.

---

## 0. The goal, in one paragraph

Replace `web/player.js` (which synthesizes GBA music by wiring up dozens of
WebAudio nodes per note and leaning on the browser's built-in
oscillators/resamplers/gain automation) with an **AudioWorkletProcessor** that
generates every output sample itself, in one per-sample mixing loop, following
the GBA m4a ("MP2K"/"Sappy") engine's exact rules. Same data, same synthesis
model, but **we own every sample** — no browser DSP black boxes between us and
the speaker. The data format, the extractor, and the samples do **not** change.

Why: purity + *provable* faithfulness. The current player sounds right but its
envelopes/mixing are approximations the browser interpolates its own way. A
worklet lets us match the decomp bit-for-bit and diff against a reference. It's
also lighter at runtime (one node + one buffer vs. ~5 nodes per live note).

## 1. Decisions already made (do not relitigate)

- **Plain JS, not C→WASM.** Workload is ~11M ops/s (16 voices × 48kHz × ~15
  ops) ≈ <1% of a core; WASM's ~2× edge is imperceptible here, and its binary
  + glue + build step are bigger than the whole JS engine. WASM only if you
  ever want a literal line-for-line `m4a.c` transcription for its own sake.
- **No build step, no bundler, no npm deps in the shipped artifact.** One
  worklet `.js` + a small main-thread shim (can live in `player.js`). Vanilla,
  served static. This is johan's "monk-like" rule: own the code, smallest
  long-lived dependency surface (see `~/.claude/CLAUDE.md`).
- **agbplay/mGBA are offline test oracles only** — never shipped or depended on.
- **Data format frozen.** `extract.py` and `web/data/**` stay as-is.

## 2. Where everything lives

- Repo: `~/pokeemerald_SDL3/pokeemerald-music/` (this dir), pushed to
  `github.com/toucans/pokeemerald_SDL3` under `pokeemerald-music/`.
- Served live at `http://10.7.0.1/pokeemerald-music/` by a thin stdlib server
  at `~/pokeemerald-music/server.py` (port 8769, behind the dashboard nginx).
  That server just serves `web/`; deploy of the site = `git pull` here.
- Files:
  - `web/player.js` — the current player to REPLACE (good reference for the
    engine math; it's correct, just built on native nodes).
  - `web/index.html` — tiny UI (song list + play/stop). Keep or restyle.
  - `web/data/manifest.json` — `[{file,name,title}, …]`, 13 songs.
  - `web/data/songs/<name>.json` — one per song (format in §3).
  - `web/data/samples.json` — shared sample bank (format in §3).
  - `extract.py` — decomp source → `web/data/**`. Don't change it; read it to
    understand the format and the engine facts it encodes.
  - `render_previews.py` — offline WAV renderer mirroring the engine (needs
    numpy). Useful as a second reference AND to A/B new vs old output.
  - `README.md` — full write-up of the m4a engine + extraction + the gotchas.
- Decomp source (authoritative for engine details): `/mnt/lexar/pokeemerald.zip`.
  Extract it and read `src/m4a.c`, `src/m4a_1.s`, `src/m4a_tables.c`,
  `include/gba/m4a_internal.h`, `sound/`. (The scratchpad copy from prior
  sessions is gone — re-extract.)

## 3. Data format (frozen — build the worklet to consume this)

**`samples.json`**: `{ "<label>": {rate, loop, loopStart, b64} }`
- `rate` Hz (float), `loop` bool, `loopStart` sample index into the PCM.
- `b64` = base64 of **8-bit signed** PCM (one byte per sample, −128..127).
- Loop region is `loopStart .. end` (end = full length; the array is already
  trimmed so its last sample is the true loop end — see §5 gotcha 3/4).

**`songs/<name>.json`**: `{name, title, reverb, loopStart, loopEnd, tracks, voices}`
- `reverb` 0..127 (song's `-R` value; 50 for most overworld). 0 = no reverb.
- `loopStart`/`loopEnd` in **seconds** (from the MIDI `[`/`]` markers), or null.
- `tracks`: array of tracks; each track is a time-ordered array of events.
- `voices`: `{ "v0": {…}, … }` referenced by note events.

**Event format** — `[time, op, …args]`, time in seconds:
| op | args | meaning |
|----|------|---------|
| `n` | key, vel, dur, vid | note: MIDI key, velocity (0..127, pre-quantized), gate length (s), voice id |
| `v` | val | track volume 0..127 (already scaled by the song's master `-V`) |
| `p` | val | pan −64..63 (linear) |
| `b` | val | pitch bend −64..63 (cents = `bend*bendRange*100/64`) |
| `br`| val | bend range in semitones |
| `m` | val | mod depth (vibrato) |
| `ls`| val | LFO speed |
| `ev`| val | pseudo-echo volume |
| `el`| val | pseudo-echo length (frames) |

**Voice types** (`t`): each has `base` (MIDI key of native pitch, always 60)
and `adsr` = `[attack, decay, sustain, release]`.
- `pcm`: `{t:'pcm', sample, base, fixed, adsr, rhythm?, pan?}`
  - `sample` = key into `samples.json`. `fixed:true` → ignore pitch, play at
    native rate (drums). `rhythm:true` → note plays at the voice's own `base`,
    not the event key; `pan` (−64..63) is the drum's own pan.
- `sq`: `{t:'sq', duty:0..3, base, sweep, adsr}` — pulse. duty→width
  [.125,.25,.5,.75].
- `wave`: `{t:'wave', pattern:[32 nibbles 0..15], base, adsr}` — GB wavetable.
- `noise`: `{t:'noise', period:0|1, base, adsr}` — LFSR (period 1 = 7-bit).

## 4. The m4a engine rules (implement these exactly)

All verified against the decomp; `render_previews.py` and `web/player.js`
already implement them — copy the math, restructure into a per-sample loop.

**Timing.** Engine ticks once per GBA frame: `FRAME = 1/59.7275 s`. Envelopes
and the sequencer advance per frame; audio is generated per output sample. At
48 kHz a frame is ~804 samples.

**DirectSound (PCM) pitch.** `playKey = rhythm ? voice.base : eventKey`.
Non-fixed: playback rate = `sampleRate * 2^((playKey - 60 + bendSemis)/12)`.
Fixed: native rate. Resample with a phase accumulator + linear interp; loop
`loopStart..len` when `loop`.

**DirectSound envelope** (per frame, on a 0..255 volume): attack ADDS `a` until
255; decay MULTIPLIES by `d/256` each frame down to sustain `s/256`; release
MULTIPLIES by `r/256` each frame. Continuous form used in the current player:
exponential with `tau = FRAME/ln(256/x)`. Peak = velocity/127.

**PSG frequency** (from GB registers, `gCgbFreqTable`/`gCgbScaleTable`):
```
CGB_FREQ = [-2004,-1891,-1785,-1685,-1591,-1501,-1417,-1337,-1262,-1192,-1125,-1062]
k   = clamp(key,36,166) - 36
reg = 2048 + (CGB_FREQ[k%12] >> (k//12))      // 11-bit
fSquare = 131072 / (2048 - reg)
fWave   = fSquare / 2      // 32-step table holds TWO cycles (see gotcha 1)
```
Band-limit the squares (Fourier series of the duty pulse) and the wave (DFT of
the 32-step table) — the current player builds `PeriodicWave`s this way; in a
worklet you either precompute band-limited wavetables or accept mild aliasing
(prefer band-limited; it's part of "better than hardware").

**Noise** (`gNoiseTable`, NR43 per key): `NOISE_TABLE = [(s<<4)|r for s in
13..0 for r in (7,6,5,4)] + [3,2,1,0]`; `nr43 = NOISE_TABLE[clamp(key-21,0,59)]`;
`clock = 524288 / (r||0.5) / 2^(s+1)` where `s=nr43>>4, r=nr43&7`. Play the GB
LFSR sequence (15-bit, or 7-bit if `period==1`) advanced at `clock` Hz.

**PSG envelope** = 4-bit levels, one level step per `a`/`d`/`r` frames. Volume
**goal** = `clamp(0,15,(sideL+sideR)>>4)` (quantized!). Sustain level =
`(goal*s+15)>>4`. Wave channel volume is coarser still — only 25/50/75/100%
(`gCgb3Vol`): level≤1→0, ≤5→.25, ≤9→.5, ≤13→.75, else 1.

**Note volume + pan** (`ChnVolSetAsm`, linear pan, per side 0..255):
```
y     = clamp(-128,127, 2*pan)
volMR = ((y+128) * 2*trackVol) >> 8
volML = ((127-y) * 2*trackVol) >> 8
rp    = drum's own pan (0 for melodic voices)
sideR = min(255, (vel*(rp+128)*volMR) >> 14)
sideL = min(255, (vel*(127-rp)*volML) >> 14)
```
A rhythm voice's `pan` MULTIPLIES the track pan (that's the `rp` term).

**PSG pan** (`CgbPan`) is a hard 3-way switch, not continuous: if
`r>=l && r>=2l` → left off; if `l>r && l>=2r` → right off; else both on.

**PSG level vs PCM.** One full PSG channel ≈ `15*8/1024 ≈ 0.117` of a
full-scale PCM channel (`PSG_FULL`). Scale PSG output by `goal/15 * PSG_FULL`.

**Pseudo-echo** (XCMD IECV/IECL; `ev`/`el`). DirectSound: the release decays
DOWN INTO `echo = peak*echoVol/256`, HOLDS there for `echoLen` frames, then the
channel is cut — one continuous curve (jumping up to the echo level = a tick
per note; see gotcha 2). CGB: release steps down to `ceil(goal*echoVol/256)`
levels, holds `echoLen` frames, cuts.

**Vibrato.** Triangle LFO, freq = `lfoSpeed * 59.7275/256` Hz, peak depth =
`mod/16` semitones (engine does `pitM += 16*modM`, 256 units/semitone).

**Reverb** (PCM sub-mix ONLY; PSG is dry, mixed post-DAC on hardware). The
mixer re-feeds its own DMA buffer: a **mono** two-tap feedback echo at ~7 and
~6 frames old. At Emerald's 13379 Hz DirectSound rate, 224 samples/frame →
taps at `7*224/13379 ≈ 117 ms` and `6*224/13379 ≈ 100 ms`, each at gain
`reverb/256` (sum `reverb/128`), with feedback. Implement as a ring buffer on
the PCM accumulator.

**Master gain** currently `0.45` (calibrated so the loudest song, Fortree,
peaks < 1.0). Keep headroom; re-check after the rewrite.

## 5. Gotchas already found & fixed — DO NOT REGRESS

1. **Wave channel is an octave too high if you play the 32-step table at the
   square rate.** Emerald's patterns hold TWO cycles, so `fWave = fSquare/2`.
2. **Pseudo-echo tick.** Must decay INTO the echo level then hold+cut, never
   jump to it at a fixed time (that pops once per note — the "fast tapping"
   johan heard first).
3. **aif2pcm playable size = num_frames − 1.** The AIFFs' final frame
   duplicates the loop-start sample and is NOT played; keep it and every loop
   cycle stutters. (`extract.py` already trims; samples.json is correct.)
4. **Fabricated loops = machine-gun drums.** Every AIFF has a boilerplate INST
   sustain-loop chunk; a sample is only truly looped if the MARK chunk with the
   loop markers ALSO exists (that's what aif2pcm keys off). `extract.py` already
   gets this right — `samples.json` has drums as `loop:false`. Don't
   reintroduce a "default to whole-sample loop" anywhere.
5. **AudioBuffer rate clamp [8000,96000] no longer applies.** The current
   player needs it because it hands raw sample rates to WebAudio; a worklet does
   its OWN resampling, so it can play any rate directly. `extract.py` currently
   upsamples the few sub-8kHz samples for the old player — harmless for the
   worklet (already-upsampled data still plays correct), but note it if you ever
   re-extract for the worklet specifically.

## 6. Recommended worklet architecture

- **Main thread (in `player.js`)**: fetch `manifest.json`, `samples.json`, and
  the selected song JSON; base64-decode each sample to an `Int8Array` (or
  Float32); `audioContext.audioWorklet.addModule('m4a-worklet.js')`; create one
  `AudioWorkletNode`; `postMessage` the sample bank once and the song on play
  (transfer the ArrayBuffers). No SharedArrayBuffer (it needs cross-origin
  isolation headers — avoid, keep it simple).
- **`m4a-worklet.js`** — `class M4AProcessor extends AudioWorkletProcessor`:
  - State: sample bank; current song; per-track sequencer cursor + controller
    state (vol/pan/bend/bendRange/mod/lfoSpeed/echoV/echoL); list of active
    voices (each: source ref, phase accumulator, envelope phase+value, side
    gains, frames-remaining).
  - `process(_, outputs)`: for each 128-sample quantum, walk a sample counter;
    cross frame boundaries at `FRAME` intervals to tick the sequencer (spawn
    notes, apply controller events, advance envelopes per frame); per output
    sample, sum each active voice (resample PCM via phase+lerp, or synth PSG),
    multiply by envelope × side gain, accumulate into a PCM bus and a PSG bus;
    run the reverb ring buffer on the PCM bus; write `pcmBus+psgBus` × master to
    `outputs[0][0/1]`. Handle the `[`/`]` loop by resetting cursors at loopEnd.
  - Keep it integer-honest where the engine is (`>>`, `| 0`, `Math.imul`,
    `& 0xFF`) so it can be made bit-exact.
- Envelopes can stay in the continuous exp form the current player uses, OR go
  fully per-frame-multiplicative for exactness — prefer per-frame; it's the
  whole point of the rewrite.

## 7. How to prove it's faithful

1. **Self-consistency first**: A/B the worklet against the current player and
   against `render_previews.py` output on the same songs (spectrum + envelope,
   like the scripts used in this project's history). They should match closely.
2. **Oracle**: build/obtain **agbplay** (or render with **mGBA**) for a few
   songs and diff waveforms — this is the actual proof of faithfulness the
   spectral checks only approximate. Oracle stays offline; never shipped.
3. **Reference remaster** (sanity, not ground truth): the fan album on
   khinsider ("Pokémon Emerald Remastered", Kurausukun/ipatix) is the target
   *aesthetic*; its per-track MP3s live on the `*.vgmtreasurechest.com` CDN
   (find the URL by fetching the khinsider track page with a browser UA). Our
   samples are 8-bit so we won't match their sub-bass — that's expected and
   documented; the fix there is swapping in real SC-88 Pro samples, not engine
   changes.
4. Tools used in prior sessions (recreate a venv, they don't persist):
   `numpy`+`scipy`+`soundfile` for analysis, `esprima` (pip) to syntax-check JS
   headlessly since there's no node on the box. There is **no `node`** here — JS
   runs only in the browser, so test playback in a browser over the dashboard.

## 8. Definition of done

- `web/m4a-worklet.js` + reworked `web/player.js` main-thread shim; old
  node-graph synthesis removed. `index.html` still lists songs and plays/stops.
- Plays all 13 songs, loops on the `[`/`]` markers, no ticks/machine-gunning,
  levels calibrated with headroom.
- No build step, no deps, still ~a couple of small JS files + the unchanged
  `web/data/**`.
- Verified: matches the current player / render_previews A/B, and (ideally)
  diffed against agbplay/mGBA. Update `README.md`'s "Playback" section.
- Serve `web/` (`cd web && python3 -m http.server`) or hit
  `http://10.7.0.1/pokeemerald-music/` (already serves this dir) and confirm in
  a browser. Commit + push to `github.com/toucans/pokeemerald_SDL3`.
