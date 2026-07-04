/* m4a_dump.c — render a song offline through src/m4a.c to raw f32 stereo.
 * Verification harness: its output is diffed against the reference JS engine
 * (pokeemerald-music/web/m4a-worklet.js run headless). Not part of the game.
 *
 *   cc -O2 tools/m4a_dump.c src/m4a.c -lm -o m4a_dump
 *   ./m4a_dump mus_littleroot 20 out.f32     # 48 kHz interleaved stereo f32
 */
#include <stdio.h>
#include <stdlib.h>

#include "../src/m4a.h"

int main(int argc, char **argv) {
    if (argc < 4) { fprintf(stderr, "usage: %s song seconds out.f32\n", argv[0]); return 2; }
    if (!m4a_init("pokeemerald-music/web/music.pak", 48000)) return 1;
    if (!m4a_play_name(argv[1])) { fprintf(stderr, "no song %s\n", argv[1]); return 1; }
    FILE *f = fopen(argv[3], "wb");
    long frames = (long)(atof(argv[2]) * 48000);
    float buf[128 * 2];
    for (long i = 0; i < frames; i += 128) {
        int n = frames - i < 128 ? (int)(frames - i) : 128;
        m4a_render(buf, n);
        fwrite(buf, sizeof(float) * 2, (size_t)n, f);
    }
    fclose(f);
    return 0;
}
