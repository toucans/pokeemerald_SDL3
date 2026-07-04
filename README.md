# pokeemerald_SDL3

An **open-world Pokémon Emerald** in C and SDL3, built to run in the browser
(WASM). The goal is *not* a full recreation of the original game: it's a world
you can freely walk around in and enjoy the art, the music and the pokémon of
the original — exploration first, everything else only as far as it serves that.

Runs natively (Linux/macOS) and on the web via Emscripten from the same C code.

## Quick start

```sh
# once: a Pokémon Emerald ROM (USA/Europe) as p.gba in this directory
# right version: sha1 f3ae088181bf583e55daf962a92bb46f4f1d07b7
make poke && ./poke           # native (needs libsdl3-dev, pkg-config)
make web                      # browser build -> poke.html/.js/.wasm/.data
                              # (needs emscripten; SDL3-for-wasm builds itself
                              #  once via tools/build-sdl3-wasm.sh)
```

Arrows move, Enter toggles the fly map, O switches between the game (m4a)
and original (SC-88Pro) soundtrack, 1–4 set window scale, Esc quits.

## Sources of truth

Every fact lives in exactly one place. When adding anything, check this table
first — never introduce a second copy of something listed here.

| What | Source of truth |
|---|---|
| Game data & behavior (maps, graphics, names, mechanics) | [pret/pokeemerald](https://github.com/pret/pokeemerald), the Emerald decompilation. A reference clone lives at `~/pokeemerald` (not part of this repo). When in doubt about how Emerald does something, read it there. |
| **All music** | `pokeemerald-music/web/music.pak` (game arrangement, 204 tracks) + `music-orig.pak` (the composers' original MIDIs with SC-88Pro samples, 210 tracks) — committed, ready to go, no build step. **Never extract music from pret/pokeemerald or from the ROM**; see `pokeemerald-music/README.md` for provenance and regeneration (the leak 7z and soundfont stay gitignored in the repo root). |
| Music engine behavior | `src/m4a.c` **is** the engine, everywhere: the game links it, and the pokeemerald-music site runs the same file compiled to wasm (`tools/build-m4a-wasm.sh`). After changing it, rebuild the wasm and re-verify (below). |
| Remastered-mp3 comparison material | `music-remaster-mp3/` — the fan "Emerald Remastered" album from [khinsider](https://downloads.khinsider.com/game-soundtracks/album/pokemon-emerald-remastered-complete-original-soundtrack). Kept **only** to A/B against our renders; the game never uses it. |
| Tile/map data the game currently draws | Extracted **from the ROM** (`p.gba`) at runtime plus generated tables in `pokeemerald/` (this repo's dir, *not* pret). This is legacy — see roadmap: it should move to data generated from pret/pokeemerald. |

Regenerable caches (never edit, never commit): `vendor/`, build outputs,
`pokeemerald-music/web/data/` (re-extraction intermediate).

## How the music works

`pokeemerald-music/` is self-contained and documented in
[its own README](pokeemerald-music/README.md): Emerald's songs are MIDI scores +
instrument definitions + 8-bit samples, played by Nintendo's m4a ("Sappy")
engine; the extraction turned the decomp source into JSON data, and the
AudioWorklet engine plays it at full bandwidth — better than the GBA's
13 kHz output, with the original loop points intact.

The game and the site play the identical committed files through the identical
C engine:

```
pokeemerald-music/web/music.pak        the game soundtrack (m4a synthesis)
pokeemerald-music/web/music-orig.pak   the composers' originals (GM/SC-88Pro
        │                              synthesis; O key in game, "original"
        │                              buttons on the site, lazily fetched)
        ├── src/m4a.c + src/audio.c    game: one SDL3 audio stream, native + web
        └── web/m4a.wasm               site: same m4a.c, standalone wasm in an
                                       AudioWorklet (bit-identical output)
```

No SDL_mixer, no mp3s, no decoded audio assets: music is synthesized in the
audio callback from ~4 MB of sequence + sample data, identical on native and
web. Maps name their song by pret id (`.music = "MUS_LITTLEROOT"` in
`pokeemerald/maps.c`); `audio_play_music()` looks it up case-insensitively,
`MUS_NONE` means silence, and crossing between maps that share a song does not
restart it.

**Verifying after touching `src/m4a.c` or the pak format**: build
`tools/m4a_dump.c`, render a few songs to raw f32, and compare against a
known-good render — either a dump from the previous engine build, or the
retired reference JS engine (`web/m4a-worklet.js` before the wasm switch, in
git history) driven headless, which the C port matches to int16-quantization
level (diff RMS ≈ 2e-5, correlation ≈ 1.0). Then rerun
`tools/build-m4a-wasm.sh` — the wasm build must stay bit-identical to native.

## Layout

```
main.c, src/           game: loop, overworld, player, camera, fly map, audio
include/               shared basic types
pokeemerald/           generated map/tileset/layout tables (committed C data)
pokeemerald-music/     soundtrack data + reference JS engine (own README)
music-remaster-mp3/    khinsider mp3s, comparison only
tools/                 pack_music.py, m4a_dump.c, build-sdl3-wasm.sh
graphics_extractor     legacy macOS binary that generated pokeemerald/*.c;
                       its source was never committed (see roadmap)
```

## Roadmap

- **Replace ROM extraction with pret-derived data.** The game still reads
  tiles/sprites out of `p.gba` at runtime (`src/map_loader.c`,
  `src/sprite_loader.c`) and draws from tables whose generator
  (`graphics_extractor`, a macOS binary with lost source) can't be re-run.
  New pipeline: scripts that generate C data / assets from the pret clone,
  like `tools/pack_music.py` does for music. Do this before building on the
  current loaders.
- **Route 118 split music**: pret marks it `0x7FFF` (west half plays
  `MUS_ROUTE110`, east half `MUS_ROUTE119`, per `GetCurrLocationDefaultMusic`);
  we currently play silence there.
- **Open world**: walk seamlessly between all outdoor maps, map connections
  from pret data, interiors later.
- **Pokémon**: wild encounters as things to see and hear (cries!) rather than
  battle mechanics.
- Unused-map songs (`MUS_GSC_PEWTER`) aren't in the game-arrangement
  extraction (the originals pak has them); extend
  `pokeemerald-music/extract.py` only if a map we actually render needs one.
- The originals' GM synth is deliberately minimal (no filters — the sf2 uses
  none; fixed light room reverb; snapshot PSG-style vibrato). Refine only
  against listening complaints, not speculatively.
