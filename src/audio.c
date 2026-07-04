/* audio.c — SDL3 audio device plumbing around the m4a engine.
 *
 * One SDL_AudioStream with a callback that asks m4a.c to synthesize; SDL
 * owns resampling and the platform backend (ALSA/Pulse natively, WebAudio
 * under Emscripten), so this same file serves the native and wasm builds.
 * No SDL_mixer, no decoded audio assets — music is synthesized from
 * assets/music.pak. */
#include "audio.h"

#include <SDL3/SDL.h>

#include "m4a.h"

#define RATE 48000
#define CHUNK 512                       /* frames rendered per callback pass */

static SDL_AudioStream *stream;

static void callback(void *userdata, SDL_AudioStream *s, int additional, int total) {
    (void)userdata; (void)total;
    static float buf[CHUNK * 2];
    while (additional > 0) {
        int frames = additional / (int)(2 * sizeof(float));
        if (frames > CHUNK) frames = CHUNK;
        if (frames <= 0) break;
        m4a_render(buf, frames);
        SDL_PutAudioStreamData(s, buf, frames * 2 * (int)sizeof(float));
        additional -= frames * 2 * (int)sizeof(float);
    }
}

static bool sc88Mode;
static bool haveSC88;
static char lastName[64];

void audio_init(void) {
    if (!m4a_init("docs/music.pak", RATE)) {
        SDL_Log("audio: docs/music.pak missing - no music");
        return;
    }
    haveSC88 = m4a_sc88_init("docs/music-sc88.pak");
    if (!haveSC88)
        SDL_Log("audio: music-sc88.pak missing - SC-88 soundtrack disabled");
    SDL_AudioSpec spec = { SDL_AUDIO_F32, 2, RATE };
    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
                                       &spec, callback, NULL);
    if (!stream) {
        SDL_Log("audio: SDL_OpenAudioDeviceStream failed: %s", SDL_GetError());
        return;
    }
    SDL_ResumeAudioStreamDevice(stream);
}

static void play_locked(const char *name) {
    int oi;
    if (sc88Mode && haveSC88 && (oi = m4a_sc88_find(name)) >= 0)
        m4a_play_sc88_index((uint32_t)oi);
    else if (!m4a_play_name(name))
        SDL_Log("audio: unknown song %s", name);
}

void audio_play_music(const char *name) {
    if (!stream || !name) return;
    if (SDL_strcasecmp(name, "MUS_NONE") == 0) { audio_stop_music(); return; }
    SDL_strlcpy(lastName, name, sizeof(lastName));
    SDL_LockAudioStream(stream);
    /* crossing between maps that share a song must not restart it */
    const char *cur = m4a_current();
    if (!(cur && m4a_is_playing() && SDL_strcasecmp(cur, name) == 0))
        play_locked(name);
    SDL_UnlockAudioStream(stream);
}

void audio_toggle_sc88(void) {
    if (!stream || !haveSC88) return;
    sc88Mode = !sc88Mode;
    SDL_Log("audio: %s soundtrack", sc88Mode ? "SC-88" : "GBA (m4a)");
    if (lastName[0]) {
        SDL_LockAudioStream(stream);
        play_locked(lastName);
        SDL_UnlockAudioStream(stream);
    }
}

void audio_stop_music(void) {
    if (!stream) return;
    SDL_LockAudioStream(stream);
    m4a_stop();
    SDL_UnlockAudioStream(stream);
}

void audio_unlock(void) {
    if (stream) SDL_ResumeAudioStreamDevice(stream);
}

void audio_close(void) {
    if (stream) {
        SDL_DestroyAudioStream(stream);
        stream = NULL;
    }
}
