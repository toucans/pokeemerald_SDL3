SRCFILES := $(shell cat srcfiles.txt)

SRCFILES := $(shell cat srcfiles.txt)

SDKROOT := $(shell xcrun --show-sdk-path)

CC := clang
CFLAGS := -std=c17 -Iinclude -isysroot $(SDKROOT)
LDFLAGS := $(shell pkg-config --cflags --libs sdl3)


poke: $(SRCFILES)
	$(CC) $(SRCFILES) $(CFLAGS) $(LDFLAGS) -o poke


pre: main.c
	clang -E main.c -Iinclude -std=c23 `pkg-config --cflags --libs sdl3` > main.pre.c

clean:
	rm -f poke

