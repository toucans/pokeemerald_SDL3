#pragma once
#include <SDL3_mixer/SDL_mixer.h>

void audio_init(void);
void audio_play_music(const char *path);
void audio_stop_music(void);
void audio_close(void);
