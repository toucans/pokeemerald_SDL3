#include <SDL3/SDL.h>
#include <stdlib.h>
#include <string.h>
#include "fly_map_renderer.h"
#include "game.h"
#include "map_loader.h"
#include "../pokeemerald/fly_map_graphics.h"

/*
 * blit_tile_packed
 * Blit one 8×8 tile using the fly-map packed encoding:
 *   each byte = (gba_slot << 4) | color_index   (one pixel per byte)
 * slot_offset is subtracted from the stored GBA slot to get the palette row.
 */
static void blit_tile_packed(const u8 *tile_bytes, const u16 *palette,
                              int slot_offset, u16 *dst, int x, int y, int dst_w)
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

SDL_Surface *fly_map_surface_create(void)
{
    const FlyMapGraphics *g = &gFlyMapGraphics;
    int out_w = g->visible_cols * 8;
    int out_h = g->visible_rows * 8;

    /* palette: read raw BGR555, convert to ABGR1555 */
    u8  *pal_raw = get_u8_data(g->pal_offset, (u32)(g->pal_slots * 16 * 2));
    u16 *pal     = malloc((size_t)(g->pal_slots * 16) * sizeof(u16));
    for (int i = 0; i < g->pal_slots * 16; i++) {
        u16 bgr;
        memcpy(&bgr, pal_raw + i * 2, 2);
        pal[i] = (i % 16 == 0) ? 0x0000u : (bgr | 0x8000u);
    }
    free(pal_raw);

    /* tileset: read compressed data, decompress */
    u8 *tiles_cmp = get_u8_data(g->tiles_offset, g->tiles_data_size);
    u32 tiles_size;
    u8 *tiles = lz77_decompress(tiles_cmp, &tiles_size);
    free(tiles_cmp);

    /* tilemap: read compressed data, decompress */
    u8 *map_cmp = get_u8_data(g->tilemap_offset, g->tilemap_data_size);
    u32 map_size;
    u8 *tilemap = lz77_decompress(map_cmp, &map_size);
    free(map_cmp);

    /* render */
    SDL_Surface *surf = SDL_CreateSurface(out_w, out_h, SDL_PIXELFORMAT_ABGR1555);
    u16 *pixels = surf->pixels;

    size_t num_tiles = tiles_size / 64;
    for (int ty = 0; ty < g->visible_rows; ty++) {
        for (int tx = 0; tx < g->visible_cols; tx++) {
            int tile_idx = tilemap[ty * g->tilemap_stride + tx];
            if ((size_t)tile_idx >= num_tiles) continue;
            blit_tile_packed(tiles + (size_t)tile_idx * 64, pal,
                             g->slot_offset, pixels, tx * 8, ty * 8, out_w);
        }
    }

    free(tilemap);
    free(tiles);
    free(pal);

    return surf;
}

void fly_map_cursor_textures_create(SDL_Renderer *renderer,
                                    SDL_Texture **out_frame0,
                                    SDL_Texture **out_frame1)
{
    const FlyMapCursorGraphics *g = &gFlyMapCursorGraphics;

    /* palette: raw BGR555, color 0 = transparent */
    u8 *pal_raw = get_u8_data(g->pal_offset, 32);
    u16 pal[16];
    for (int i = 0; i < 16; i++) {
        u16 bgr;
        memcpy(&bgr, pal_raw + i * 2, 2);
        pal[i] = (i == 0) ? 0x0000u : (bgr | 0x8000u);
    }
    free(pal_raw);

    /* tiles: LZ77 → 256 bytes (8 tiles × 32 bytes, 4 bytes × 8 rows per tile) */
    u8 *tiles_cmp = get_u8_data(g->tiles_offset, g->tiles_data_size);
    u32 tiles_size;
    u8 *tiles = lz77_decompress(tiles_cmp, &tiles_size);
    free(tiles_cmp);

    for (int f = 0; f < 2; f++) {
        SDL_Surface *surf = SDL_CreateSurface(16, 16, SDL_PIXELFORMAT_ABGR1555);
        SDL_SetSurfaceBlendMode(surf, SDL_BLENDMODE_BLEND);
        u16 *px = surf->pixels;
        memset(px, 0, 16 * 16 * 2);  /* fully transparent */

        /* 4 tiles in 2×2 layout:
             tile 0 = top-left,  tile 1 = top-right
             tile 2 = bot-left,  tile 3 = bot-right */
        int tile_base = f * 4;
        for (int t = 0; t < 4; t++) {
            int tx = (t & 1) * 8;
            int ty = (t >> 1) * 8;
            const u8 *tile = tiles + ((size_t)(tile_base + t)) * 32;
            for (int row = 0; row < 8; row++) {
                for (int col = 0; col < 4; col++) {  /* 4 bytes = 8 pixels */
                    u8 b  = tile[row * 4 + col];
                    int c0 = b & 0xF;
                    int c1 = (b >> 4) & 0xF;
                    if (c0) px[(ty + row) * 16 + tx + col * 2]     = pal[c0];
                    if (c1) px[(ty + row) * 16 + tx + col * 2 + 1] = pal[c1];
                }
            }
        }

        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
        SDL_DestroySurface(surf);

        if (f == 0) *out_frame0 = tex;
        else        *out_frame1 = tex;
    }

    free(tiles);
}
