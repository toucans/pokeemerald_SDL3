# pokeemerald_SDL3 — rules for AI development

Read `README.md` first: the project goal (open-world Emerald, browser-first,
not a full recreation) and the **Sources of truth** table are the contract.
Cross-project principles live in `~/.claude/CLAUDE.md`; don't restate either
here.

The rules that most often prevent wasted work:

- **Music is done.** All 204 tracks come from `pokeemerald-music/` via
  `tools/pack_music.py` → `src/m4a.c`. Never extract or re-derive music from
  pret/pokeemerald or the ROM. Engine changes: worklet first
  (`pokeemerald-music/web/m4a-worklet.js`), then port to `src/m4a.c`, then
  re-verify with `tools/m4a_dump.c` (see README "Verifying the port").
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
