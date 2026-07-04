SRCFILES := $(shell cat srcfiles.txt)

# ── Native build (Linux/macOS: needs sdl3 via pkg-config) ────────────────────
CC     ?= cc
CFLAGS := -std=c17 -O2 -Wall -Iinclude $(shell pkg-config --cflags sdl3)
LIBS   := $(shell pkg-config --libs sdl3) -lm

poke: $(SRCFILES) src/*.h
	$(CC) $(SRCFILES) $(CFLAGS) $(LIBS) -o poke

# ── Web build (Emscripten; the real target — see README) ─────────────────────
# SDL3 for wasm is built once by tools/build-sdl3-wasm.sh (emscripten has no
# SDL3 port of its own).
EMCC      := emcc
SDL3_WASM := vendor/sdl3-wasm
# MINIFY_HTML=0: Debian's emscripten lacks the html minifier it wants at -O2
EMCFLAGS  := -std=c17 -O2 -Iinclude -I$(SDL3_WASM)/include \
             -sALLOW_MEMORY_GROWTH=1 -sINITIAL_MEMORY=67108864 -sMINIFY_HTML=0 \
             --preload-file p.gba --preload-file docs/music.pak \
             --preload-file docs/music-sc88.pak

web: $(SRCFILES) src/*.h $(SDL3_WASM)/lib/libSDL3.a
	$(EMCC) $(SRCFILES) $(SDL3_WASM)/lib/libSDL3.a $(EMCFLAGS) -o poke.html

$(SDL3_WASM)/lib/libSDL3.a:
	tools/build-sdl3-wasm.sh

# ── Music engine for the pokeemerald-music site (same src/m4a.c, as wasm) ────
wasm-engine:
	tools/build-m4a-wasm.sh

clean:
	rm -f poke poke.html poke.js poke.wasm poke.data
