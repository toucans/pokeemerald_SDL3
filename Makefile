SRCFILES := $(shell cat srcfiles.txt)

# ── Native build ──────────────────────────────────────────────────────────────
SDKROOT  := $(shell xcrun --show-sdk-path)
CC       := clang
CFLAGS   := -std=c17 -Iinclude -isysroot $(SDKROOT)
LDFLAGS  := $(shell pkg-config --cflags --libs sdl3 sdl3-mixer)

# ── Web build (Emscripten) ────────────────────────────────────────────────────
EMCC     := emcc
EMCFLAGS := -std=c17 -Iinclude -sUSE_SDL=3 -sALLOW_MEMORY_GROWTH=1 \
            -sINITIAL_MEMORY=67108864 \
            --preload-file p.gba \
            -O2
# audio.c excluded: SDL_mixer has no Emscripten port yet; audio_web.c used instead
EMSRCFILES := $(filter-out src/audio.c, $(SRCFILES)) src/audio_web.c

# ── Main build ────────────────────────────────────────────────────────────────
poke: $(SRCFILES)
	$(CC) $(SRCFILES) $(CFLAGS) $(LDFLAGS) -o poke

# ── Web build ─────────────────────────────────────────────────────────────────
web: $(EMSRCFILES)
	$(EMCC) $(EMSRCFILES) $(EMCFLAGS) -o poke.html

# ── Misc ──────────────────────────────────────────────────────────────────────
pre: main.c
	clang -E main.c -Iinclude -std=c23 `pkg-config --cflags --libs sdl3` > main.pre.c

clean:
	rm -f poke
