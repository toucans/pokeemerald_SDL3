# pokeemerald_SDL3 — rules for AI development

Read `README.md` first: the project goal (open-world Emerald, browser-first,
not a full recreation) and the **Sources of truth** table are the contract.
Cross-project principles live in `~/.claude/CLAUDE.md`; don't restate either
here.

The rules that most often prevent wasted work:

- **Music is done.** Both soundtracks are committed in
  `pokeemerald-music/web/` (`music.pak` game arrangement, `music-orig.pak`
  composers' originals); the game build has no music step. Never extract or
  re-derive music from pret/pokeemerald or the ROM; the leak 7z, midi-orig/
  and the sf2 in the repo root are gitignored regeneration inputs — never
  commit them. `src/m4a.c` is the one engine (game + site wasm); after
  changing it, re-verify with `tools/m4a_dump.c` and rebuild `web/m4a.wasm`
  with `tools/build-m4a-wasm.sh` (see README "How the music works").
- **pret/pokeemerald answers behavior questions.** Reference clone at
  `~/pokeemerald`. Don't guess how Emerald does something — read it.
- **Don't build on the ROM-extraction loaders** (`map_loader.c`,
  `sprite_loader.c`, `graphics_extractor`); they're legacy slated for
  replacement by pret-derived generation (README roadmap).
- Dependencies: SDL3 + libc + code in this repo. Nothing else without a very
  good reason (that's why there's no SDL_mixer).
- Verify native (`make poke`, run headless with
  `SDL_VIDEO_DRIVER=offscreen SDL_AUDIO_DRIVER=dummy`) **and** web
  (`make web`) before considering a change done.
