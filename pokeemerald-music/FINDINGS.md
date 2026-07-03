# How Pokémon Emerald's overworld music works — and how it was extracted

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

Engine behaviours that matter for faithful playback:

- **Envelopes tick per frame.** DirectSound ADSR: attack *adds* `a` until 255, decay
  *multiplies* by `d/256` down to sustain `s/256`, release multiplies by `r/256` —
  i.e. exponential decays with τ = frame/ln(256/d). PSG envelopes are 4-bit,
  one linear step per `n` frames.
- **Pan is linear**, −64…+63; drum voices can carry their own fixed pan.
- **Vibrato** is a triangle LFO at `speed × 59.73/256` Hz; depth ≈ mod × 4/256 semitones.
- **"Reverb" (`-R50`)** is not reverb: the mixer feeds its own DMA buffer from a few
  frames back into the mix at 50/128 — a short mushy feedback echo.
- **Pseudo-echo** (XCMD IECV/IECL): after release, hold a low envelope level for n
  frames — several overworld tracks use it for their percussion tails.

## What extract.py does

No ROM and no emulation needed — everything is in the source tree:

1. Parse `midi.cfg` for the song's voicegroup, master volume and reverb.
2. Parse the MIDI directly (stdlib struct): tempo map → seconds, notes with exact
   gate times, the CCs above, and the `[`/`]` loop markers.
3. Parse the voicegroup `.inc` (plus keysplit tables / drumset sub-groups) and resolve
   every `(program, key)` a note actually uses into a concrete voice; drums are
   flattened (per-key sub-voice, own pan, plays at its own base key).
4. Decode the referenced `.aif` files (COMM rate, MARK/INST loop, SSND 8-bit PCM) and
   the 4-bit wave patterns.
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
  the 32-step table — band-limited for free). Noise → looped LFSR buffer.
- ADSR scheduled analytically on a per-note `GainNode` (gate length is known up
  front), m4a frame-based math as above; track volume/pan/bend/vibrato are scheduled
  automation; loops follow the `[`/`]` markers exactly like the engine's track jump.
- Output aims to sound like the GBA: lowpass ≈ 6.6 kHz (the 13 379 Hz output's
  band) + the m4a-style feedback echo at the song's `-R` level.

## Regenerating

```bash
./extract.py --src <path to a pokeemerald checkout>   # -> web/data/ (13 songs)
./render_previews.py --seconds 40 mus_littleroot ...  # -> previews/*.wav (needs numpy+scipy)
```

`web/data/` is committed (small, makes the player self-contained); previews are
optional offline renders of the same data. To listen, serve this directory with any
static file server (e.g. `python3 -m http.server` in `web/`) and open the page —
`fetch()` needs http, not file://.
