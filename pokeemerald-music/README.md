# pokeemerald-music

Pokémon Emerald's **overworld background music**, decoded straight from the
`pokeemerald` decompilation source and played live with **WebAudio** — no ROM,
no emulator, no MP3s. Standalone; nothing here is wired into the SDL3 port.

| Piece | What |
|---|---|
| `extract.py` | pokeemerald source → `web/data/` (song JSON + sample bank). Python stdlib only. |
| `web/` | The player: `index.html` + `player.js` (vanilla, no build) + committed `data/` — self-contained, 13 town/route themes with proper GBA loop points. |
| `render_previews.py` | Optional offline WAV renders of the same data (needs numpy+scipy). |

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
   the 4-bit wave patterns. One trap: `aif2pcm` stores the playable size as
   `num_frames − 1` — the AIFFs' final frame duplicates the loop-start sample and is
   *not* played. Keep it and every loop cycle stutters on the join sample (an
   audible tick per cycle on sustained notes).
5. Emit `web/data/songs/<name>.json` (per-track event lists in seconds + resolved
   voices) and one shared `web/data/samples.json` (base64 PCM + rate + loop points).
   13 overworld songs come to ~36 samples / 343 KiB of PCM — the entire overworld
   soundtrack's sample memory.

**Verification:** `render_previews.py` renders the same JSON offline (numpy) with the
same envelope math; an FFT of the render was cross-checked against the MIDI — the
pitch classes sounding in each window match the score (8/8 windows for Littleroot).

## Playback (web/player.js)

- PCM voices → `AudioBufferSourceNode` at the sample's native rate with AIFF loop
  points; pitch via `detune` = (key−60)×100 + bend cents. Pulse & wavetable voices →
  `OscillatorNode` with a `PeriodicWave` (Fourier series of the duty cycle / DFT of
  the 32-step table — band-limited for free), at the GB-register frequencies. Noise →
  looped LFSR buffer at the NR43 clock. Per-note linear-pan gain pairs and the
  quantised PSG levels implement the volume model above; loops follow the `[`/`]`
  markers exactly like the engine's track jump.
- The output is deliberately **full-bandwidth** — the approach of the fan
  "Emerald Remastered" album (Kurausukun/ipatix): identical sequence data and mix
  rules, identical instruments, minus the GBA's output degradation (13.4 kHz
  nearest-neighbour resampling). The PCM bus gets the engine's real reverb
  (117 ms mono feedback echo at `-R`/128); PSG stays dry, as on hardware.
- Verified against that remaster: FFT peak-frequency match of 21–24/25 spectral
  peaks per window and closely matching band balance on the tracks compared.

## Regenerating

```bash
./extract.py --src <path to a pokeemerald checkout>   # -> web/data/ (13 songs)
./render_previews.py --seconds 40 mus_littleroot ...  # -> previews/*.wav (needs numpy+scipy)
```

`web/data/` is committed (small, makes the player self-contained); previews are
optional offline renders of the same data (gitignored, regenerable). To listen, serve this directory with any
static file server (e.g. `python3 -m http.server` in `web/`) and open the page —
`fetch()` needs http, not file://.
