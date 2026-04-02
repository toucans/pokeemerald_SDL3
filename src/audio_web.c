#ifdef __EMSCRIPTEN__
#include "audio.h"
#include <emscripten.h>

EM_JS(void, _js_play_music, (const char *id), {
    var path = "music/" + UTF8ToString(id) + ".mp3";
    if (window._gameMusic) {
        window._gameMusic.pause();
        window._gameMusic.src = "";
    }
    window._gameMusic = new Audio(path);
    window._gameMusic.loop = true;
    window._gameMusic.play().catch(function() {});
});

EM_JS(void, _js_stop_music, (), {
    if (window._gameMusic) {
        window._gameMusic.pause();
        window._gameMusic.src = "";
        window._gameMusic = null;
    }
});

void audio_init(void) {}
void audio_play_music(const char *id) { _js_play_music(id); }
void audio_stop_music(void) { _js_stop_music(); }
void audio_close(void) { _js_stop_music(); }

#endif
