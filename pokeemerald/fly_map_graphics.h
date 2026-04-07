#pragma once
#include <types.h>

/*
 * FlyMapGraphics
 *
 * ROM asset locations and layout parameters for the fly-destination map
 * (the 256×160 overview shown when selecting a fly target).
 *
 *   Palette   2 × 16-colour GBA BGR555 slots
 *               slot 7: water / land colours
 *               slot 8: route / edge colours
 *   Tileset   LZ77-compressed; packed encoding: byte = (gba_slot<<4)|color_idx
 *   Tilemap   LZ77-compressed; 64×64 affine map, 1 byte/cell = tile index
 *             Top-left visible_cols × visible_rows cells form the screen.
 */
typedef struct {
    u32 pal_offset;          /* ROM byte offset of palette data           */
    int pal_slots;           /* number of 16-colour palette slots         */
    u32 tiles_offset;        /* ROM byte offset of LZ77-compressed tiles  */
    u32 tiles_data_size;     /* compressed tile data size in bytes        */
    u32 tilemap_offset;      /* ROM byte offset of LZ77-compressed tilemap*/
    u32 tilemap_data_size;   /* upper-bound size to read before decompressing */
    int tilemap_stride;      /* tilemap row width in cells                */
    int visible_cols;        /* columns of tiles to render                */
    int visible_rows;        /* rows of tiles to render                   */
    int slot_offset;         /* GBA VRAM slot base (subtracted when blitting) */
} FlyMapGraphics;

extern const FlyMapGraphics gFlyMapGraphics;

/*
 * FlyMapCursorGraphics
 *
 * ROM asset locations for the small cursor sprite drawn on the fly map.
 *
 *   Palette   Raw 16-colour GBA BGR555 (32 bytes). Color 0 = transparent.
 *   Tiles     LZ77-compressed 4bpp GBA tile data → 256 bytes:
 *               8 tiles × 32 bytes, 2 animation frames of 4 tiles each.
 *               Each tile is 8×8 pixels; each byte = 2 pixels
 *               (low nibble = left pixel, high nibble = right pixel).
 *               Frame 0: tiles 0-3  (bytes   0-127)
 *               Frame 1: tiles 4-7  (bytes 128-255)
 *               4 tiles per frame in a 2×2 layout → 16×16 px sprite.
 */
typedef struct {
    u32 pal_offset;        /* ROM byte offset of raw 32-byte BGR555 palette */
    u32 tiles_offset;      /* ROM byte offset of LZ77-compressed tile data  */
    u32 tiles_data_size;   /* compressed size upper bound                   */
} FlyMapCursorGraphics;

extern const FlyMapCursorGraphics gFlyMapCursorGraphics;
