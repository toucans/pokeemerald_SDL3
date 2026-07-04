# pokeemerald-music

Pokémon Emerald's **complete soundtrack** (204 tracks — every `mus_*` in the
game, overworld to battles to fanfares), decoded straight from the
`pokeemerald` decompilation source and played live in the browser by an
**AudioWorklet that reimplements the GBA m4a sound engine** — no ROM, no
emulator, no MP3s. Standalone; nothing here is wired into the SDL3 port.

| Piece | What |
|---|---|
| `extract.py` | pokeemerald source → `web/data/` (song JSON + sample bank + categorized manifest). Python stdlib only. |
| `web/m4a-worklet.js` | The engine: an `AudioWorkletProcessor` that synthesizes every sample per the m4a rules (vanilla, no build). Needs a secure context (https). |
| `web/player.js` + `index.html` | Main-thread shim (loads data, drives the worklet) + tiny UI: the soundtrack grouped into categories (towns, routes, battles, fanfares, …) with a filter box. |
| `web/viz.js` | Live 16:9 canvas visualization of what the engine is playing (see below). |
| `web/data/` | Committed song JSON + sample bank — self-contained, all 204 tracks with proper GBA loop points. |
| `render_previews.py` | Offline WAV renders of the same engine, used to A/B the worklet (needs numpy). |

Listen: serve `web/` with any static server (`cd web && python3 -m http.server`)
and open the page.

---

# How the music works & how it was extracted

Findings from reading the `pokeemerald` decompilation source tree.
`extract.py` turns that source into the JSON/sample bank that `web/` plays with WebAudio.

## The sound engine (m4a / "MP2K" / "Sappy")

The GBA has no music format of its own — every Pokémon game ships Nintendo's **m4a**
engine (`src/m4a.c`, `src/m4a_1.s`), a software sequencer + mixer that runs on the CPU.
Each frame (59.7275 Hz VBlank) it steps every song track, updates envelopes, and mixes:

- **DirectSound channels** (up to 12): 8-bit signed PCM samples, software-mixed into a
  DMA buffer at 13 379 Hz (Emerald's config) and played through the two DAC FIFOs.
- **PSG channels**: the four Game-Boy-compat hardware voices — 2 pulse (4 duty cycles),
  1 programmable 32×4-bit wavetable, 1 LFSR noise. Emerald's overworld tracks use the
  pulses and wavetable constantly (they're the "chiptune" layer under the samples).

## Where a song lives in the repo

A song is **three plain-text/source pieces**, combined at build time:

1. **The score — a real MIDI file.** `sound/songs/midi/mus_littleroot.mid` etc.
   The build runs Nintendo-compatible `tools/mid2agb` on it (options per song in
   `sound/songs/midi/midi.cfg`, e.g. `-E -R50 -G_littleroot -V100`):
   - `-G` names the **voicegroup**, `-V` a master volume scaler applied to every CC7,
     `-R` the reverb level, `-E` keeps exact gate times.
   - **Loop points are MIDI marker events `[` and `]`** — that's the whole mechanism
     behind seamless overworld looping.
   - Ordinary MIDI CCs map to engine commands: CC1 vibrato depth, CC7 volume, CC10 pan,
     CC20 bend range, CC21 LFO speed, CC30+CC29 "extended" commands (pseudo-echo
     volume/length). Note velocities are quantized to steps of 4 by a LUT.

2. **The orchestra — a voicegroup.** `sound/voicegroups/littleroot.inc` maps MIDI
   program numbers to 12-byte voice definitions (`asm/macros/music_voice.inc`):
   - `voice_directsound key pan SAMPLE a d s r` — a PCM sample voice,
   - `voice_square_1/2`, `voice_programmable_wave`, `voice_noise` — PSG voices,
   - `voice_keysplit group, table` — melodic key splits (e.g. piano sampled at 4
     pitches; `sound/keysplit_tables.inc` maps key ranges to sub-voices),
   - `voice_keysplit_all group` — a **drumset**: the MIDI key picks the sub-voice
     (`voice_group rs_drumset, 36` = entry 0 sits at key 36), and the note then plays
     at the *sub-voice's own* base key — that's how one tom sample becomes six toms.

3. **The instruments — AIFF samples.** `sound/direct_sound_samples/*.aif`, 8-bit
   signed mono (mostly ripped from a Roland SC-88 Pro, per the filenames). Sample rate
   is in the COMM chunk; **sustain-loop points are AIFF MARK/INST chunks**. `aif2pcm`
   converts them to the ROM format whose `freq` field = rate × 1024 — meaning **MIDI
   key 60 plays a sample at its native rate**, everything else is 2^((key−60)/12).
   The GB wavetable patterns are 16-byte files (32 × 4-bit) in
   `sound/programmable_wave_samples/`.

Engine behaviours that matter for faithful playback (all from `m4a.c`/`m4a_1.s`):

- **Envelopes tick per frame.** DirectSound ADSR: attack *adds* `a` until 255, decay
  *multiplies* by `d/256` down to sustain `s/256`, release multiplies by `r/256` —
  i.e. exponential decays with τ = frame/ln(256/d). PSG envelopes step one 4-bit
  level per `n` frames.
- **Note volume** (ChnVolSetAsm): per side = `velocity × panFactor × trackVol >> 14`.
  Pan is **linear**, −64…+63, and a drum voice's own pan **multiplies** the track pan.
- **PSG is quantised and hardware-mixed.** Volume goal = `clamp15((sideL+sideR)/16)`;
  a full PSG channel is 15×8/1024 ≈ 0.117 of a full-scale PCM channel (SOUNDCNT_H
  all-full). The wave channel only has 25/50/75/100 % volume steps (gCgb3Vol), and
  PSG pan is a hard 3-way switch (CgbPan flips a side off at a 2:1 ratio). PSG is
  mixed after the DAC: full bandwidth, **no reverb**.
- **PSG pitch comes from GB registers**: `f = 131072/(2048−reg)` (gCgbFreqTable) for
  squares; the 32-step wave runs at *half* that — Emerald's patterns hold two cycles,
  so written pitch matches. Noise is an LFSR clocked at `524288/r/2^(s+1)` via
  `gNoiseTable[key−21]`.
- **Vibrato** is a triangle LFO at `speed × 59.73/256` Hz; peak depth = mod/16
  semitones (`pitM += 16·modM`, 256 units per semitone).
- **"Reverb" (`-R50`)** is not reverb: the mixer re-feeds its own DMA buffer, ~7
  frames (117 ms) old, mono-summed at 50/128 with feedback — a short echo, and it
  only touches the PCM mix.
- **Pseudo-echo** (XCMD IECV/IECL): the release decays *down into* the echo level,
  holds there for n frames, then the channel is cut — several overworld tracks use
  it for note tails. (Getting this wrong — e.g. jumping to the echo level at a fixed
  time — produces an audible tick after every note.) CGB channels do it too, at
  ceil(goal×echoVol/256) of their quantised volume.

## What extract.py does

No ROM and no emulation needed — everything is in the source tree:

1. Parse `midi.cfg` for the song's voicegroup, master volume and reverb.
2. Parse the MIDI directly (stdlib struct): tempo map → seconds, notes with exact
   gate times, the CCs above, and the `[`/`]` loop markers.
3. Parse the voicegroup `.inc` (plus keysplit tables / drumset sub-groups) and resolve
   every `(program, key)` a note actually uses into a concrete voice; drums are
   flattened (per-key sub-voice, own pan, plays at its own base key).
4. Decode the referenced `.aif` files (COMM rate, MARK/INST loop, SSND 8-bit PCM) and
   the 4-bit wave patterns. Two traps here: (a) `aif2pcm` stores the playable size
   as `num_frames − 1` — the AIFFs' final frame duplicates the loop-start sample and
   is *not* played (keep it and every loop cycle stutters); (b) **every** AIFF has a
   boilerplate INST chunk claiming a sustain loop — a sample is only really looped
   if the MARK chunk with the loop markers exists (that's what `aif2pcm` keys off).
   Trust the INST alone and every one-shot drum machine-guns at ~10 Hz.
5. Emit `web/data/songs/<name>.json` (per-track event lists in seconds + resolved
   voices) and one shared `web/data/samples.json` (base64 PCM + rate + loop points).
   13 overworld songs come to ~36 samples / 343 KiB of PCM — the entire overworld
   soundtrack's sample memory.

**Verification:** `render_previews.py` renders the same JSON offline (numpy) with the
same envelope math; an FFT of the render was cross-checked against the MIDI — the
pitch classes sounding in each window match the score (8/8 windows for Littleroot).

## Playback (web/m4a-worklet.js + web/player.js)

The player is a single **AudioWorklet** that *is* the m4a engine — it generates
every output sample itself in one per-sample mixing loop, rather than wiring up
browser oscillators/resamplers per note. This is the faithful hardware model and
keeps the whole signal path in code we own (no browser DSP black boxes).

- **`web/m4a-worklet.js`** — the `AudioWorkletProcessor`. The sequencer and
  envelopes tick once per GBA frame (59.7275 Hz); audio is synthesized per output
  sample: PCM voices by phase-accumulator resampling of the 8-bit samples
  (looping between the AIFF points), pulses by a 4× box-averaged pulse, the
  wavetable by 32-step interpolation at *half* the square rate, noise from the GB
  LFSR at the NR43 clock. Envelopes, the linear pan/volume law, the quantised PSG
  levels and 3-way PSG pan, vibrato, pseudo-echo and the `[`/`]` loop all follow
  §"engine behaviours" above. The PCM sub-mix gets the engine's real reverb (mono
  two-tap feedback echo at `-R`/128); PSG stays dry.
- **`web/player.js`** — a thin main-thread shim: loads the JSON + sample bank,
  base64-decodes the samples once, drives one `AudioWorkletNode`. UI only.
- **Secure context required.** `AudioWorklet` is only exposed on https/localhost,
  so the page must be served over TLS — on this box's dashboard that's the local
  mkcert cert on the nginx front door (`https://10.7.0.1/`). Over plain http
  `ctx.audioWorklet` is undefined and play surfaces an "audio error"; serve it
  over https.
- The output is deliberately **full-bandwidth** — the approach of the fan
  "Emerald Remastered" album (Kurausukun/ipatix): identical sequence data, mix
  rules and instruments, minus the GBA's output degradation (13.4 kHz
  nearest-neighbour resampling).
- **Verified**: the worklet was driven headlessly in V8 and A/B'd against
  `render_previews.py` (band balance matches to ≈0.02) and against the official
  GBA recording (79% FFT peak-frequency match — same as the offline renderer,
  confirming correct notes/octaves/tuning), with zero drum re-triggers, zero
  high-band ticks, and seamless loops. No build step, no deps: two small vanilla
  JS files. Plain JS was chosen over C→WASM — the workload is <1% of a core, so
  WASM's edge is imperceptible while its toolchain/binary are pure overhead.
- **Real-time robustness** (the "tiny glitches sometimes" class of bug): the
  per-sample render path allocates nothing (GC pauses on the audio thread are
  audible dropouts — this is also the only real advantage a WASM build would
  have had); the reverb feedback tail flushes subnormal floats (10–100×
  slower arithmetic on x86 = a CPU spike mid-silence); a PSG note whose
  quantised volume goal is 0 no longer computes a 0/0 = NaN attack (NaN
  poisoned ~67 ms of output per occurrence); and the `AudioContext` uses
  `latencyHint: "playback"` — a music player wants a big buffer, not low
  latency, and the default interactive-size buffer underruns when the
  listening machine is busy.

## The visualization (`web/viz.js`)

While a song plays, a 16:9 canvas above the list draws the engine's state
live — the worklet posts one snapshot per GBA frame (59.7275 Hz) with every
voice's vid, *sounding* pitch, envelope level and pan, and `viz.js` renders a
scrolling waterfall (click ⛶ for fullscreen — sized for a YouTube frame):

- one hue per PCM instrument; PSG gets fixed neons (squares cyan, wavetable
  violet, noise a jittered gray speckle) — legend chips name what's playing
- stroke thickness/brightness = envelope, so attacks bloom, decays taper,
  and **pseudo-echo** reads as a long dim streak that cuts off dead
- **vibrato and pitch bends draw as real wobbles** (the snapshot carries the
  sounding pitch, not the note key)
- each stroke's upper half is the right channel, lower half the left — pan
  and the PSG's hard 3-way switch are visible directly
- PCM strokes trail a faint ghost at the engine's real **reverb** delay
  (~117 ms, mono two-tap echo; PSG correctly has none)
- a dashed gold line marks each `[`/`]` **loop wrap**; octave grid + clock +
  title round it out. Drawing is output-latency-compensated so it lines up
  with what you hear.

## Regenerating

```bash
./extract.py --src <path to a pokeemerald checkout>   # -> web/data/ (all 204 tracks)
./render_previews.py --seconds 40 mus_littleroot ...  # -> previews/*.wav (needs numpy+scipy)
```

`web/data/` is committed (small, makes the player self-contained); previews are
optional offline renders of the same data (gitignored, regenerable). To listen, serve this directory with any
static file server (e.g. `python3 -m http.server` in `web/`) and open the page —
`fetch()` needs http, not file://.
