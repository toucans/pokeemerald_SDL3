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

ROM      := p.gba
GENDIR   := generated
EXTRACTOR := graphics_extractor

# ── Generated C source files (compiled into the binary) ───────────────────
GENERATED := \
    $(GENDIR)/fly_map.c

# ── Main build ────────────────────────────────────────────────────────────
poke: $(GENERATED) $(SRCFILES)
	$(CC) $(SRCFILES) $(GENERATED) $(CFLAGS) $(LDFLAGS) -o poke

# ── Web build ─────────────────────────────────────────────────────────────
web: $(GENERATED) $(EMSRCFILES)
	$(EMCC) $(EMSRCFILES) $(GENERATED) $(EMCFLAGS) -o poke.html

# ── Build the extractor (plain C, no SDL) ────────────────────────────────
$(EXTRACTOR): src/graphics_extractor.c
	$(CC) -std=c17 -isysroot $(SDKROOT) src/graphics_extractor.c -o $(EXTRACTOR)

# ── Generate C sources from ROM ──────────────────────────────────────────
#    Re-runs only when the ROM or extractor source changes.
$(GENERATED): $(EXTRACTOR) $(ROM)
	@mkdir -p $(GENDIR)
	./$(EXTRACTOR) $(ROM) $(GENDIR)

# ── Misc ──────────────────────────────────────────────────────────────────
pre: main.c
	clang -E main.c -Iinclude -std=c23 `pkg-config --cflags --libs sdl3` > main.pre.c

clean:
	rm -f poke $(EXTRACTOR)

clean-generated:
	rm -rf $(GENDIR)