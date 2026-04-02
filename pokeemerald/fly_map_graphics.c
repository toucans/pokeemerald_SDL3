#include "fly_map_graphics.h"

/*
 * ROM assets (verified against pokeemerald / p.gba):
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
 */
const FlyMapGraphics gFlyMapGraphics = {
    .pal_offset       = 5895996U,   /* 0x0059F73C */
    .pal_slots        = 2,
    .tiles_offset     = 5896060U,   /* 0x0059F77C */
    .tiles_data_size  = 3428U,      /* 0x005A04E0 - 0x0059F77C */
    .tilemap_offset   = 5899488U,   /* 0x005A04E0 */
    .tilemap_data_size = 4096U,     /* safe upper bound; lz77 stops when done */
    .tilemap_stride   = 64,
    .visible_cols     = 32,
    .visible_rows     = 20,
    .slot_offset      = 7,
};
