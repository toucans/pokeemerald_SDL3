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

Arrows move, Enter toggles the fly map, 1–4 set window scale, Esc quits.

## Sources of truth

Every fact lives in exactly one place. When adding anything, check this table
first — never introduce a second copy of something listed here.

| What | Source of truth |
|---|---|
| Game data & behavior (maps, graphics, names, mechanics) | [pret/pokeemerald](https://github.com/pret/pokeemerald), the Emerald decompilation. A reference clone lives at `~/pokeemerald` (not part of this repo). When in doubt about how Emerald does something, read it there. |
| **All music** | `pokeemerald-music/` in this repo — the complete 204-track soundtrack as extracted sequence/instrument data plus the m4a engine that plays it (see below). **Never extract music from pret/pokeemerald or from the ROM**; this data is already the better, verified version. |
| Music engine behavior | `pokeemerald-music/web/m4a-worklet.js` is the verified reference implementation. `src/m4a.c` is its C port (A/B'd sample-exact). Engine changes go into the worklet first, then get ported. |
| Remastered-mp3 comparison material | `music-remaster-mp3/` — the fan "Emerald Remastered" album from [khinsider](https://downloads.khinsider.com/game-soundtracks/album/pokemon-emerald-remastered-complete-original-soundtrack). Kept **only** to A/B against our renders; the game never uses it. |
| Tile/map data the game currently draws | Extracted **from the ROM** (`p.gba`) at runtime plus generated tables in `pokeemerald/` (this repo's dir, *not* pret). This is legacy — see roadmap: it should move to data generated from pret/pokeemerald. |

Regenerable caches (never edit, never commit): `assets/music.pak`, `vendor/`,
build outputs. `make` rebuilds them.

## How the music works

`pokeemerald-music/` is self-contained and documented in
[its own README](pokeemerald-music/README.md): Emerald's songs are MIDI scores +
instrument definitions + 8-bit samples, played by Nintendo's m4a ("Sappy")
engine; the extraction turned the decomp source into JSON data, and the
AudioWorklet engine plays it at full bandwidth — better than the GBA's
13 kHz output, with the original loop points intact.

The game plays the same data through the same engine, in C:

```
pokeemerald-music/web/data/*.json      committed source of truth (204 songs)
        │  tools/pack_music.py         (make assets/music.pak)
        ▼
assets/music.pak                       compact binary, gitignored
        │  src/m4a.c                   the m4a engine in C (port of the worklet)
        ▼
src/audio.c                            one SDL3 audio stream, native + web
```

No SDL_mixer, no mp3s, no decoded audio assets: music is synthesized in the
audio callback from ~4 MB of sequence + sample data, identical on native and
web. Maps name their song by pret id (`.music = "MUS_LITTLEROOT"` in
`pokeemerald/maps.c`); `audio_play_music()` looks it up case-insensitively,
`MUS_NONE` means silence, and crossing between maps that share a song does not
restart it.

**Verifying the port** (after touching `src/m4a.c` or the pak format): build
`tools/m4a_dump.c`, render a few songs to raw f32, and compare against the
worklet rendered headless — they must match to int16-quantization level
(diff RMS ≈ 2e-5, correlation ≈ 1.0). The worklet harness from the original
verification is `run_worklet.py` (py_mini_racer; add `scale: 1/128` to the
bank entries, as player.js does).

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
- Unused-map songs (`MUS_GSC_PEWTER`) aren't in the extraction; extend
  `pokeemerald-music/extract.py` only if a map we actually render needs one.
