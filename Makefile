SRCFILES := $(shell cat srcfiles.txt)

SDKROOT  := $(shell xcrun --show-sdk-path)
CC       := clang
CFLAGS   := -std=c17 -Iinclude -isysroot $(SDKROOT)
LDFLAGS  := $(shell pkg-config --cflags --libs sdl3 sdl3-mixer)

ROM      := p.gba
GENDIR   := generated
EXTRACTOR := graphics_extractor

# ── Generated C source files (compiled into the binary) ───────────────────
GENERATED := \
    $(GENDIR)/fly_map.c

# ── Main build ────────────────────────────────────────────────────────────
poke: $(GENERATED) $(SRCFILES)
	$(CC) $(SRCFILES) $(GENERATED) $(CFLAGS) $(LDFLAGS) -o poke

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