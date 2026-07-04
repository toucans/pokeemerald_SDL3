# pokeemerald-music

Pokémon Emerald's **complete soundtrack** (204 tracks — every `mus_*` in the
game, overworld to battles to fanfares), decoded straight from the
`pokeemerald` decompilation source into **`web/music.pak`** — the single
committed music format. The game (`src/m4a.c`) loads it directly, and this
site plays it through the **same C engine compiled to wasm** inside an
AudioWorklet — no ROM, no emulator, no MP3s, one engine everywhere.

| Piece | What |
|---|---|
| `web/music.pak` | **The game soundtrack.** All 204 tracks: sequences, voices, 8-bit samples, loop points, titles/categories. Committed; format documented in `tools/pack_music.py`. |
| `web/music-orig.pak` | **The original soundtrack**: the composers' 480-tpqn MIDIs + the SC-88Pro samples they were written for (trimmed to what's used). Committed; format in `tools/extract_orig.py`. |
| `web/m4a.wasm` | The engine: `src/m4a.c` built standalone by `tools/build-m4a-wasm.sh` (committed; ~32 KB). Plays both paks. |
| `web/m4a-worklet.js` | Thin `AudioWorkletProcessor` that hosts the wasm: feeds it the paks, pulls rendered samples + viz snapshots. No synthesis logic in JS. |
| `web/player.js` + `index.html` | Main-thread shim + tiny UI: every song has `play` (game version) and `original` (engine GM synth) buttons, plus `rendered` (fluidsynth opus from `tools/render_compare.py`, local-only/gitignored) when present — for A/B'ing the engine against a second synth. The originals pak is fetched lazily on first use. |
| `web/viz.js` | Live 16:9 canvas visualization of what the engine is playing (see below). |
| `extract.py` | pokeemerald source → `web/data/` JSON (regeneration-time intermediate, gitignored). Python stdlib only. |
| `../tools/pack_music.py` | `web/data/` JSON → `web/music.pak`. |
| `../tools/extract_orig.py` | `../midi-orig/` + the sf2 → `web/music-orig.pak`. |
| `render_previews.py` | Offline WAV renders from the JSON intermediate, for A/B checks (needs numpy). |

Listen: serve `web/` with any static server (`cd web && python3 -m http.server`)
and open the page.

## Where the notes come from (provenance)

Two arrangements of the same music, from two sources:

- **Game arrangement** (`music.pak`): extracted from pret/pokeemerald. pret's
  MIDIs are machine-derived from the game data (24 ticks/beat — the m4a
  clock — velocities quantized to the game's steps-of-4 LUT), rendered at
  full bandwidth instead of the GBA's 13 kHz output.
- **Originals** (`music-orig.pak`): the composers' own MIDI sources from the
  leaked Emerald sound source (`pm_eme_ose`, kept as a gitignored 7z in the
  repo root; the working MIDIs live in gitignored `../midi-orig/`). Same
  arrangements, but 480 ticks/beat, unquantized velocities, GM programs —
  what Game Freak's composers played on their SC-88 Pro before mid2agb
  squeezed it into the GBA. The samples come from the community
  `GBApokemonTestLite.sf2` (SC-88Pro rips arranged in GM layout, also
  gitignored), thinned to every 2nd key split, one-shots capped at 3 s, and
  downsampled to the SC-88's native 32 kHz.

The engine's GM voice path applies the gain model this soundfont was authored
against: `initialAttenuation` at the EMU 0.4 convention plus GM square-law
velocity/CC7 curves. (Rendering the bank with fluidsynth, which applies the
full spec attenuation, puts instruments 10+ dB apart — that experiment is
why the offline-render approach was dropped.)

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

## Playback (src/m4a.c as web/m4a.wasm, hosted by web/m4a-worklet.js)

The engine is **`src/m4a.c` in the game repo** — one per-sample mixing loop
that follows the decomp's rules exactly, with no browser DSP between it and
the speaker. The sequencer and envelopes tick once per GBA frame (59.7275 Hz);
audio is synthesized per output sample: PCM voices by phase-accumulator
resampling of the 8-bit samples (looping between the AIFF points), pulses by a
4× box-averaged pulse, the wavetable by 32-step interpolation at *half* the
square rate, noise from the GB LFSR at the NR43 clock. Envelopes, the linear
pan/volume law, the quantised PSG levels and 3-way PSG pan, vibrato,
pseudo-echo and the `[`/`]` loop all follow §"engine behaviours" above. The
PCM sub-mix gets the engine's real reverb (mono two-tap feedback echo at
`-R`/128); PSG stays dry.

- **`web/m4a.wasm`** — that C file compiled standalone (no emscripten JS
  runtime); `tools/build-m4a-wasm.sh` rebuilds it after engine changes.
- **`web/m4a-worklet.js`** — hosts the wasm in an `AudioWorkletProcessor`:
  passes music.pak into wasm memory, copies rendered blocks out, forwards the
  engine's per-frame viz snapshots. **`web/player.js`** is UI + fetch only.
- **Secure context required.** `AudioWorklet` is only exposed on
  https/localhost — on this box's dashboard that's the local mkcert cert on
  the nginx front door (`https://10.7.0.1/`). Over plain http play surfaces
  an "audio error".
- The output is deliberately **full-bandwidth** — the approach of the fan
  "Emerald Remastered" album (Kurausukun/ipatix): identical sequence data, mix
  rules and instruments, minus the GBA's output degradation (13.4 kHz
  nearest-neighbour resampling).
- **Verified**: the engine lineage is (1) the original JS worklet, A/B'd
  against `render_previews.py` and the official GBA recordings (79% FFT
  peak-frequency match, zero drum re-triggers, seamless loops); (2) the C
  port, sample-exact against that worklet (corr 1.000000, diff at int16
  quantization level, all 204 songs NaN-free); (3) this wasm build,
  bit-identical to the native C build. The retired JS engine lives in git
  history (`web/m4a-worklet.js` before the wasm switch).
- **Real-time robustness**: the render path allocates nothing; the reverb
  feedback tail flushes subnormal floats (10–100× slower arithmetic on x86 =
  a CPU spike mid-silence); a PSG note whose quantised volume goal is 0 does
  not compute a 0/0 = NaN attack; and the `AudioContext` uses
  `latencyHint: "playback"` — a music player wants a big buffer, not low
  latency.

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

Only needed when re-extracting from a pret checkout or after changing the
engine — day to day, `web/music.pak`, `web/music-orig.pak` and `web/m4a.wasm`
are committed and ready to go (the game build has **no music build step**).

```bash
./extract.py --src ~/pokeemerald          # -> web/data/ JSON (gitignored intermediate)
../tools/pack_music.py                    # -> web/music.pak
../tools/extract_orig.py                  # -> web/music-orig.pak (needs ../midi-orig/
                                          #    + ../GBApokemonTestLite.sf2; scipy to downsample)
../tools/build-m4a-wasm.sh                # -> web/m4a.wasm (after src/m4a.c changes)
./render_previews.py --seconds 40 mus_littleroot ...  # optional WAV checks (numpy)
```

To listen locally, serve `web/` with any static file server
(`python3 -m http.server` in `web/`) and open the page — `fetch()` needs
http, not file://.
