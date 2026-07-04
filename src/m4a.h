#pragma once
/* m4a.h — the GBA m4a (MP2K/"Sappy") music engine, in C.
 *
 * C port of pokeemerald-music/web/m4a-worklet.js (same math, same data —
 * see pokeemerald-music/README.md for the engine facts). Consumes
 * assets/music.pak, generated from pokeemerald-music/web/data/ by
 * tools/pack_music.py.
 *
 * Pure synthesis, no I/O: audio.c owns the SDL audio device and calls
 * m4a_render() from the audio callback. m4a_play_name()/m4a_stop() must be
 * serialized against m4a_render() by the caller (SDL_LockAudioStream).
 */
#include <stdbool.h>

bool m4a_init(const char *pak_path, int sample_rate);
bool m4a_play_name(const char *name);   /* case-insensitive, e.g. "MUS_LITTLEROOT" */
void m4a_stop(void);
bool m4a_is_playing(void);
const char *m4a_current(void);          /* current song name, or NULL */
void m4a_render(float *stereo, int frames);  /* interleaved stereo f32 */
