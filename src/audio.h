#pragma once
/* audio.h — game-facing music API. One implementation (src/audio.c) for both
 * native and web: the m4a engine (src/m4a.c) renders into an SDL3 audio
 * stream. Song names are the map music ids ("MUS_LITTLEROOT"). */

void audio_init(void);
void audio_play_music(const char *name);  /* no-op if that song is already playing */
void audio_stop_music(void);
void audio_toggle_sc88(void);         /* GBA (m4a) <-> SC-88 soundtrack */
void audio_unlock(void);                  /* resume device after first user input
                                             (browser autoplay policy) */
void audio_close(void);
