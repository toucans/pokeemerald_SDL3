#include "audio.h"
#include <SDL3/SDL.h>

static MIX_Mixer *mixer = NULL;
static MIX_Track *track  = NULL;
static MIX_Audio *current_audio = NULL;

void audio_init(void) {
    if (!MIX_Init()) {
        SDL_Log("MIX_Init failed: %s", SDL_GetError());
        return;
    }
    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (!mixer) {
        SDL_Log("MIX_CreateMixerDevice failed: %s", SDL_GetError());
        return;
    }
    track = MIX_CreateTrack(mixer);
}

void audio_play_music(const char *path) {
    if (!mixer || !track) return;

    MIX_StopTrack(track, 0);
    MIX_DestroyAudio(current_audio);

    char full_path[256];
    SDL_snprintf(full_path, sizeof(full_path), "music/%s.mp3", path);

    current_audio = MIX_LoadAudio(mixer, full_path, false); // false = stream, don't predecode
    if (!current_audio) {
        SDL_Log("MIX_LoadAudio failed for %s: %s", full_path, SDL_GetError());
        return;
    }

    MIX_SetTrackAudio(track, current_audio);

    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1); // -1 = loop forever
    MIX_PlayTrack(track, props);
    SDL_DestroyProperties(props);
}

void audio_stop_music(void) {
    if (track) MIX_StopTrack(track, 0);
}

void audio_close(void) {
    if (track) MIX_StopTrack(track, 0);
    MIX_DestroyAudio(current_audio);
    current_audio = NULL;
    MIX_DestroyMixer(mixer);
    mixer = NULL;
    track = NULL;
    MIX_Quit();
}
