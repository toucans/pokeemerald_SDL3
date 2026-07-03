# pokeemerald-music

Pokémon Emerald's **overworld background music**, decoded straight from the
`pokeemerald` decompilation source and played live with **WebAudio** — no ROM,
no emulator, no MP3s. Standalone; nothing here is wired into the SDL3 port.

**[FINDINGS.md](FINDINGS.md)** documents how the GBA's m4a sound engine works and
how the music was extracted.

| Piece | What |
|---|---|
| `extract.py` | pokeemerald source → `web/data/` (song JSON + sample bank). Python stdlib only. |
| `web/` | The player: `index.html` + `player.js` (vanilla, no build) + committed `data/` — self-contained, 13 town/route themes with proper GBA loop points. |
| `render_previews.py` | Optional offline WAV renders of the same data (needs numpy+scipy). |

Listen: serve `web/` with any static server (`cd web && python3 -m http.server`)
and open the page.
