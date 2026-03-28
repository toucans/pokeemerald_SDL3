/*
 * graphics_extractor.c
 *
 * Build-time tool.  Reads p.gba and writes C source files into generated/
 * that are compiled directly into the game binary.  Each graphic becomes a
 * const array of SDL_PIXELFORMAT_ABGR1555 pixels — no runtime file I/O.
 *
 * Compile & run (handled by Makefile):
 *   cc graphics_extractor.c -o graphics_extractor
 *   ./graphics_extractor p.gba generated/
 *
 * ── Output format ────────────────────────────────────────────────────────
 *
 * Each output file is a .c file declaring one array:
 *
 *   // generated/fly_map.c
 *   #include <stdint.h>
 *   const uint16_t fly_map_pixels[256 * 160] = { 0x..., 0x..., ... };
 *
 * A matching header is written alongside it:
 *
 *   // generated/fly_map.h
 *   #pragma once
 *   #include <stdint.h>
 *   #define FLY_MAP_W 256
 *   #define FLY_MAP_H 160
 *   extern const uint16_t fly_map_pixels[256 * 160];
 *
 * Pixels are SDL_PIXELFORMAT_ABGR1555 (little-endian u16):
 *   bit 15     = alpha  (1 = opaque, 0 = transparent)
 *   bits 14-10 = blue
 *   bits  9-5  = green
 *   bits  4-0  = red
 * At runtime, pass the array straight to SDL_CreateSurfaceFrom().
 *
 * ── Adding new graphics ──────────────────────────────────────────────────
 *
 * All the work is done by the library functions below.  To add a new
 * graphic, write one extract_*() function and call it from main().
 * The library covers every common GBA graphics format:
 *
 *   rom_lz77_decompress(rom, offset, &size)
 *       Decompress a GBA LZ77 (type 0x10) block.
 *
 *   palette_bgr555_to_abgr1555(raw, n_slots)
 *       Convert n_slots × 16 GBA BGR555 colours to ABGR1555.
 *       Colour index 0 of every slot is forced transparent (0x0000).
 *
 *   surface_create(w, h)      allocate zeroed ABGR1555 pixel buffer
 *   surface_fill(px, w, h, c) flood-fill background colour
 *
 *   blit_tile_packed(tile, pal, slot_offset, dst, x, y, dst_w)
 *       Fly-map packed encoding: each byte = (gba_slot<<4)|color_idx.
 *       One pixel per byte, slot_offset maps GBA slot → palette row.
 *
 *   blit_tile_4bpp(tile, pal, pal_row, dst, x, y, dst_w)
 *       Standard GBA 4bpp: each byte = lo_pixel|(hi_pixel<<4).
 *       Two pixels per byte, 32 bytes per 8×8 tile.
 *       Use for overworld tilesets, sprites, and most other GBA graphics.
 *
 *   write_c_pixels(out_dir, name, pixels, w, h)
 *       Write generated/<name>.c and generated/<name>.h.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

/* ═══════════════════════════════════════════════════════════════════════════
 * ROM ACCESS
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * rom_lz77_decompress
 * Decompress the GBA LZ77 (type 0x10) block at rom[offset].
 * Writes uncompressed byte count to *out_size (may be NULL).
 * Returns heap-allocated buffer; caller must free().
 */
static u8 *rom_lz77_decompress(const u8 *rom, u32 offset, u32 *out_size)
{
    const u8 *src = rom + offset;
    if (src[0] != 0x10) {
        fprintf(stderr, "lz77: expected 0x10 at offset %u, got 0x%02X\n",
                offset, src[0]);
        return NULL;
    }
    u32 size = (u32)src[1] | ((u32)src[2] << 8) | ((u32)src[3] << 16);
    if (out_size) *out_size = size;

    u8 *dst = malloc(size);
    if (!dst) { perror("malloc"); return NULL; }

    const u8 *rd  = src + 4;
    u8       *wr  = dst;
    u8       *end = dst + size;

    while (wr < end) {
        u8 flags = *rd++;
        for (int bit = 7; bit >= 0 && wr < end; bit--) {
            if (flags & (1 << bit)) {
                u8 b0 = rd[0], b1 = rd[1]; rd += 2;
                int len  = ((b0 >> 4) & 0xF) + 3;
                int disp = (((int)(b0 & 0xF) << 8) | b1) + 1;
                for (int k = 0; k < len && wr < end; k++, wr++)
                    *wr = *(wr - disp);
            } else {
                *wr++ = *rd++;
            }
        }
    }
    return dst;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * PALETTE
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * palette_bgr555_to_abgr1555
 * Convert n_slots × 16 raw GBA BGR555 u16 values to SDL ABGR1555.
 * GBA BGR555 and SDL ABGR1555 have identical channel layout; bit 15 is
 * unused in GBA and the alpha flag in SDL.  We set it for opaque colours
 * and force colour-index-0 of every slot to 0x0000 (transparent).
 * Returns heap-allocated array of n_slots * 16 u16; caller must free().
 */
static u16 *palette_bgr555_to_abgr1555(const u8 *raw_bgr555, int n_slots)
{
    int  n   = n_slots * 16;
    u16 *pal = malloc((size_t)n * sizeof(u16));
    if (!pal) { perror("malloc"); return NULL; }
    for (int i = 0; i < n; i++) {
        u16 bgr;
        memcpy(&bgr, raw_bgr555 + i * 2, 2);
        pal[i] = (i % 16 == 0) ? 0x0000u : (bgr | 0x8000u);
    }
    return pal;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * PIXEL SURFACE
 * ═══════════════════════════════════════════════════════════════════════════ */

/* surface_create: allocate and zero a width × height ABGR1555 pixel buffer. */
static u16 *surface_create(int width, int height)
{
    u16 *px = calloc((size_t)(width * height), sizeof(u16));
    if (!px) perror("calloc");
    return px;
}

/* surface_fill: flood-fill every pixel in a width × height buffer. */
static void surface_fill(u16 *pixels, int width, int height, u16 colour)
{
    int n = width * height;
    for (int i = 0; i < n; i++)
        pixels[i] = colour;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * TILE BLITTERS
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * blit_tile_packed
 * Blit one 8×8 tile using the fly-map packed encoding:
 *   each byte = (gba_slot << 4) | color_index   (one pixel per byte)
 *
 * tile_bytes:  64 bytes of packed tile data.
 * palette:     ABGR1555 palette (n_slots * 16 entries).
 * slot_offset: subtracted from the stored GBA slot number to get the
 *              palette row.  Fly map uses slots 7 and 8; slot_offset=7
 *              maps slot 7 → row 0, slot 8 → row 1.
 * dst:         destination pixel buffer.
 * x, y:        top-left destination pixel.
 * dst_w:       destination buffer width in pixels (stride).
 */
static void blit_tile_packed(const u8 *tile_bytes, const u16 *palette,
                              int slot_offset,
                              u16 *dst, int x, int y, int dst_w)
{
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            u8  b    = tile_bytes[row * 8 + col];
            int slot = (b >> 4) & 0xF;
            int cidx =  b       & 0xF;
            if (cidx == 0) continue;
            dst[(y + row) * dst_w + (x + col)] =
                palette[(slot - slot_offset) * 16 + cidx];
        }
    }
}

/*
 * blit_tile_4bpp
 * Blit one 8×8 tile using standard GBA 4bpp encoding:
 *   each byte = lo_pixel | (hi_pixel << 4)   (two pixels per byte, 32 bytes/tile)
 *
 * tile_bytes:  32 bytes of 4bpp tile data.
 * palette:     ABGR1555 palette.
 * pal_row:     which 16-colour row of the palette to use (0-based).
 * dst, x, y, dst_w: same as blit_tile_packed.
 */
static void blit_tile_4bpp(const u8 *tile_bytes, const u16 *palette,
                            int pal_row,
                            u16 *dst, int x, int y, int dst_w)
{
    const u16 *slot = palette + pal_row * 16;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            u8  byte = tile_bytes[row * 4 + col / 2];
            int cidx = (col & 1) ? (byte >> 4) & 0xF : byte & 0xF;
            if (cidx == 0) continue;
            dst[(y + row) * dst_w + (x + col)] = slot[cidx];
        }
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * C SOURCE OUTPUT
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * write_c_pixels
 *
 * Write generated/<name>.c and generated/<name>.h.
 *
 * The .c file contains a const uint16_t array of w*h ABGR1555 pixels.
 * The .h file declares the array and exposes width/height as macros so
 * call sites never need to hard-code dimensions.
 *
 * The array uses the name "<name>_pixels" (e.g. "fly_map_pixels").
 *
 * Returns 0 on success, -1 on error.
 */
static int write_c_pixels(const char *out_dir, const char *name,
                           const u16 *pixels, int w, int h)
{
    char path_c[512], path_h[512];
    snprintf(path_c, sizeof(path_c), "%s/%s.c", out_dir, name);
    snprintf(path_h, sizeof(path_h), "%s/%s.h", out_dir, name);

    /* ── Write .h ── */
    FILE *fh = fopen(path_h, "w");
    if (!fh) { perror(path_h); return -1; }
    fprintf(fh,
        "#pragma once\n"
        "#include <stdint.h>\n"
        "\n"
        "#define %s_W %d\n"
        "#define %s_H %d\n"
        "\n"
        "/* %d × %d SDL_PIXELFORMAT_ABGR1555 pixels.\n"
        " * Load with:\n"
        " *   SDL_CreateSurfaceFrom(%s_pixels, %s_W, %s_H,\n"
        " *                         %s_W * 2, SDL_PIXELFORMAT_ABGR1555); */\n"
        "extern const uint16_t %s_pixels[%d * %d];\n",
        name, w,
        name, h,
        w, h,
        name, name, name,
        name,
        name, w, h);
    fclose(fh);

    /* ── Write .c ── */
    FILE *fc = fopen(path_c, "w");
    if (!fc) { perror(path_c); return -1; }
    fprintf(fc, "#include \"%s.h\"\n\n", name);
    fprintf(fc, "const uint16_t %s_pixels[%d * %d] = {\n", name, w, h);

    int total = w * h;
    for (int i = 0; i < total; i++) {
        if (i % 16 == 0) fprintf(fc, "    ");
        fprintf(fc, "0x%04X", pixels[i]);
        if (i + 1 < total) fprintf(fc, ",");
        if (i % 16 == 15 || i + 1 == total) fprintf(fc, "\n");
        else fprintf(fc, " ");
    }
    fprintf(fc, "};\n");
    fclose(fc);

    printf("  wrote  %-38s  (%d × %d = %d pixels)\n",
           path_c, w, h, total);
    return 0;
}

static void mkdir_p(const char *dir)
{
#ifdef _WIN32
    _mkdir(dir);
#else
    mkdir(dir, 0755);
#endif
}

/* ═══════════════════════════════════════════════════════════════════════════
 * GRAPHICS: FLY MAP
 *
 * ROM assets (verified against p.gba):
 *
 *   Palette  0x0059F73C  64 raw bytes    2 × 16-colour GBA BGR555
 *                          [0..15]  = GBA VRAM slot 7 (water, land)
 *                          [16..31] = GBA VRAM slot 8 (routes, edges)
 *   Tileset  0x0059F77C  LZ77 → 14912 bytes
 *                          233 tiles × 64 bytes/tile
 *                          Packed: byte = (gba_slot << 4) | color_idx
 *   Tilemap  0x005A04E0  LZ77 → 4096 bytes
 *                          64×64 affine map, 1 byte/cell = tile index
 *                          Top-left 32×20 cells = 256×160 visible screen
 *
 * Output: generated/fly_map.c  +  generated/fly_map.h
 * ═══════════════════════════════════════════════════════════════════════════ */

#define FLY_PAL_OFFSET    5895996U   /* 0x0059F73C */
#define FLY_PAL_SLOTS     2
#define FLY_TILES_OFFSET  5896060U   /* 0x0059F77C */
#define FLY_MAP_OFFSET    5899488U   /* 0x005A04E0 */
#define FLY_TILEMAP_W     64
#define FLY_VISIBLE_COLS  32
#define FLY_VISIBLE_ROWS  20
#define FLY_OUT_W         (FLY_VISIBLE_COLS * 8)   /* 256 */
#define FLY_OUT_H         (FLY_VISIBLE_ROWS * 8)   /* 160 */

static int extract_fly_map(const u8 *rom, const char *out_dir)
{
    u16 *pal = palette_bgr555_to_abgr1555(rom + FLY_PAL_OFFSET, FLY_PAL_SLOTS);
    if (!pal) return -1;

    u32 tiles_size;
    u8 *tiles = rom_lz77_decompress(rom, FLY_TILES_OFFSET, &tiles_size);
    if (!tiles) { free(pal); return -1; }

    u32 map_size;
    u8 *tilemap = rom_lz77_decompress(rom, FLY_MAP_OFFSET, &map_size);
    if (!tilemap) { free(tiles); free(pal); return -1; }

    u16 *pixels = surface_create(FLY_OUT_W, FLY_OUT_H);
    if (!pixels) { free(tilemap); free(tiles); free(pal); return -1; }

    surface_fill(pixels, FLY_OUT_W, FLY_OUT_H, pal[1]); /* water background */

    size_t num_tiles = tiles_size / 64;
    for (int ty = 0; ty < FLY_VISIBLE_ROWS; ty++) {
        for (int tx = 0; tx < FLY_VISIBLE_COLS; tx++) {
            int tile_idx = tilemap[ty * FLY_TILEMAP_W + tx];
            if (tile_idx == 0 || (size_t)tile_idx >= num_tiles) continue;
            blit_tile_packed(tiles + (size_t)tile_idx * 64, pal,
                             7, pixels, tx * 8, ty * 8, FLY_OUT_W);
        }
    }

    free(tilemap);
    free(tiles);
    free(pal);

    int rc = write_c_pixels(out_dir, "fly_map", pixels, FLY_OUT_W, FLY_OUT_H);
    free(pixels);
    return rc;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * MAIN
 * ═══════════════════════════════════════════════════════════════════════════ */

int main(int argc, char *argv[])
{
    const char *rom_path = (argc >= 2) ? argv[1] : "p.gba";
    const char *out_dir  = (argc >= 3) ? argv[2] : "generated";

    FILE *f = fopen(rom_path, "rb");
    if (!f) { perror(rom_path); return 1; }
    fseek(f, 0, SEEK_END);
    long rom_size = ftell(f);
    rewind(f);
    u8 *rom = malloc((size_t)rom_size);
    if (!rom) { perror("malloc ROM"); fclose(f); return 1; }
    if (fread(rom, 1, (size_t)rom_size, f) != (size_t)rom_size) {
        perror("fread"); free(rom); fclose(f); return 1;
    }
    fclose(f);
    printf("Loaded %s (%ld bytes)\n", rom_path, rom_size);

    mkdir_p(out_dir);

    int rc = 0;
    rc |= extract_fly_map(rom, out_dir);
    /* rc |= extract_next_thing(rom, out_dir); */

    free(rom);
    puts(rc == 0 ? "Done." : "Errors occurred.");
    return rc;
}