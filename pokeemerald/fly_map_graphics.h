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
