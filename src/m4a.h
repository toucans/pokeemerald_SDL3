#pragma once
/* m4a.h — the GBA m4a (MP2K/"Sappy") music engine, in C.
 *
 * THE music engine for both the game and the pokeemerald-music site (which
 * runs it compiled to wasm inside an AudioWorklet). Consumes
 * pokeemerald-music/web/music.pak — see pokeemerald-music/README.md for the
 * engine facts and tools/pack_music.py for the pak format.
 *
 * Pure synthesis, no device I/O: audio.c (game) or the worklet (site) call
 * m4a_render() from their audio callback. play/stop must be serialized
 * against m4a_render() by the caller (the game uses SDL_LockAudioStream).
 */
#include <stdbool.h>
#include <stdint.h>

#ifndef M4A_NO_STDIO
bool m4a_init(const char *pak_path, int sample_rate);
#endif
/* wasm/worklet entry: buf is a malloc'd pak the engine takes ownership of */
bool m4a_init_mem(void *buf, long size, int sample_rate);

bool m4a_play_name(const char *name);   /* case-insensitive, e.g. "MUS_LITTLEROOT" */
bool m4a_play_index(uint32_t i);
void m4a_stop(void);
bool m4a_is_playing(void);
const char *m4a_current(void);          /* current song name, or NULL */
void m4a_render(float *stereo, int frames);  /* interleaved stereo f32 */

/* song-table introspection (site UI + viz; i < m4a_song_count()) */
uint32_t m4a_song_count(void);
const char *m4a_song_name(uint32_t i);
const char *m4a_song_title(uint32_t i);
const char *m4a_song_cat(uint32_t i);
float m4a_song_reverb(uint32_t i);
int m4a_song_key_lo(uint32_t i);        /* sounding-pitch range over all notes */
int m4a_song_key_hi(uint32_t i);
uint32_t m4a_song_nvoices(uint32_t i);
int m4a_voice_type(uint32_t i, uint32_t v);   /* 0 pcm, 1 sq, 2 wave, 3 noise */
int m4a_voice_duty(uint32_t i, uint32_t v);
int m4a_voice_rhythm(uint32_t i, uint32_t v);
int m4a_voice_base(uint32_t i, uint32_t v);
const char *m4a_voice_label(uint32_t i, uint32_t v);  /* pcm sample label or "" */

/* viz snapshots: one per GBA frame while enabled. m4a_viz_seq() increments
 * each new snapshot; m4a_viz_data() is [t, wrapped, n, then per live voice
 * vid, soundingPitch, level, pan]. Read between render calls. */
void m4a_set_viz(bool on);
uint32_t m4a_viz_seq(void);
const float *m4a_viz_data(void);
