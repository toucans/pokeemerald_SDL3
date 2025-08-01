#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <SDL3/SDL.h>

#include "game.h"
#include "map_loader.h"
#include "../pokeemerald/map_layouts.h"


u8* lz77_decompress(const u8 *src, u32 *out_size) {
    u32 uncompressed_size = src[1] | (src[2] << 8) | (src[3] << 16); // Extract uncompressed size from header
    if (out_size) *out_size = uncompressed_size;

    u8 *dst = malloc(uncompressed_size);
    if (!dst) return NULL;

    const u8 *input = src + 4;  // Skip header
    u8 *write_ptr = dst;

    while ((write_ptr - dst) < uncompressed_size) {
        u8 flags = *input++;

        for (int bit = 0; bit < 8 && (write_ptr - dst) < uncompressed_size; bit++) {
            if (flags & (0x80 >> bit)) {
                u8 byte1 = *input++;
                u8 byte2 = *input++;
                int offset = ((byte1 & 0xF) << 8) | byte2;
                int length = (byte1 >> 4) + 3;
                for (int j = 0; j < length; j++) {
                    *write_ptr = *(write_ptr - offset - 1);
                    write_ptr++;
                }
            } else {
                *write_ptr++ = *input++;
            }
        }
    }
    return dst;
}

void write_tile(
	const u8 *tileNumber, 
	u8 h_flip, 
	u8 v_flip, 
	const u16 *palette, 
	u16 *SDLsurfacePixels, 
	u8 quadrant,
	u16 map_x,
	u16 map_y,
	u16 map_width
) {
	int start_x = (quadrant % 2) * 8;
	int start_y = (quadrant / 2) * 8;
	int pitch = map_width;

	for (size_t i = 0; i < 32; i++) {
		u8 byte = tileNumber[i];

		// Extract two pixels from the byte and OR with 0x8000 to set highest bit to 1 for the AGBR1555 format if the color isn't 0 (transparent)
		u16 color0 = 0x0000;
		u16 color1 = 0x0000;
		if(byte & 0x0F) color0 = palette[byte & 0x0F] | 0x8000;   // lower nibble
		if(byte >> 4)   color1 = palette[byte >> 4] | 0x8000;     // upper nibble
		

		// Compute pixel position in 8x8 tile space. 2 pixels per byte

		int tile_x0 = i * 2 % 8; 
		int tile_x1 = (i * 2 + 1) % 8;
		int tile_y  = i * 2 / 8;

		if (h_flip) {
		tile_x0 = 7 - tile_x0;
		tile_x1 = 7 - tile_x1;
		}
		if (v_flip) tile_y = 7 - tile_y;

		int surface_x0 = map_x + start_x + tile_x0;
		int surface_x1 = map_x + start_x + tile_x1;
		int surface_y  = map_y + start_y + tile_y;

		
		SDLsurfacePixels[surface_y * pitch + surface_x0] = color0;
		SDLsurfacePixels[surface_y * pitch + surface_x1] = color1;
	}
}

void write_cell(
	u16 cell, 
	MapLayout *mapLayout,
	u8 *primary_tileset_tiles, 
	u8 *secondary_tileset_tiles,
	u16 *SDLsurfacePixels, 
	u16 map_x, 
	u16 map_y,
	bool layer //false for BG layer, true for FG layer
) {

	u16 cellNumber = cell & 0b0000001111111111;
	const u16 *cellData;
	const u8 *charData;
	const u16 *palettes;
	u16 startIndex;
	if(cellNumber < 512) {
		cellData = mapLayout->primary_tileset->metatiles;
		charData = primary_tileset_tiles;
		palettes = mapLayout->primary_tileset->palettes;
		startIndex = 0;
    } else {
		cellData = mapLayout->secondary_tileset->metatiles;
		charData = secondary_tileset_tiles;
		palettes = mapLayout->secondary_tileset->palettes;
		startIndex = 512;
    }
	const u16 *tiles = &cellData[(cellNumber-startIndex)*8];

	u16 map_width = mapLayout->width * 16;
	for (u8 i = 0; i < 4; i++)
	{
		
		int index = (layer) ? i + 4 : i;
		int tileIndex = (tiles[index] & 0b0000001111111111) - startIndex;


		const u8 *tile = &charData[tileIndex*32];
		u8 h_flip = (tiles[index] & 0b0000010000000000) >> 10;
		u8 v_flip = (tiles[index] & 0b0000100000000000) >> 11;
		const u16 *palette = &palettes[(tiles[index] >> 12)*16];
		write_tile(tile, h_flip, v_flip, palette, SDLsurfacePixels, i, map_x, map_y, map_width);
	}
}



MapSurfaces load_map_surfaces(MapLayout *mapLayout) {
	u16 mapWidth = mapLayout->width * 16;
	u16 mapHeight = mapLayout->height * 16;
	SDL_Surface *bg_surface = SDL_CreateSurface(mapWidth, mapHeight, SDL_PIXELFORMAT_ABGR1555);
	SDL_Surface *fg_surface = SDL_CreateSurface(mapWidth, mapHeight, SDL_PIXELFORMAT_ABGR1555);

	u8 *primary_tileset = get_u8_data(mapLayout->primary_tileset->tiles, mapLayout->primary_tileset->dataSize);
	u8 *secondary_tileset = get_u8_data(mapLayout->secondary_tileset->tiles, mapLayout->secondary_tileset->dataSize);

	u32 sizeMain, sizeSub;
	u8 *primary_tileset_tiles = lz77_decompress(primary_tileset, &sizeMain);
	u8 *secondary_tileset_tiles = lz77_decompress(secondary_tileset, &sizeSub);
	free(primary_tileset);
	free(secondary_tileset);

	u8 *combined_tileset = malloc(sizeMain + sizeSub); // Need to combine them for when write_cell accesses secondary_tileset_tiles with negative indexes
	memcpy(combined_tileset, primary_tileset_tiles, sizeMain);
	memcpy(combined_tileset + sizeMain, secondary_tileset_tiles, sizeSub);
	free(primary_tileset_tiles);
	free(secondary_tileset_tiles);
	primary_tileset_tiles = combined_tileset;
	secondary_tileset_tiles = combined_tileset + sizeMain;



	mapLayout->blockdata = get_u16_data(mapLayout->blockdata_offset, mapLayout->width * mapLayout->height);

	mapLayout->primary_tileset->metatiles = get_u16_data(mapLayout->primary_tileset->metatiles_offset, 512 * 8);
	mapLayout->secondary_tileset->metatiles = get_u16_data(mapLayout->secondary_tileset->metatiles_offset, 512 * 8);
	mapLayout->primary_tileset->palettes = get_u16_data(mapLayout->primary_tileset->palettes_offset, 256);
	mapLayout->secondary_tileset->palettes = get_u16_data(mapLayout->secondary_tileset->palettes_offset, 256);

	for (u8 layer = 0; layer < 2; layer++) {
		SDL_Surface *surface = (layer == 0) ? bg_surface : fg_surface;
		for (int map_y = 0; map_y < mapLayout->height; map_y++) {
			for (int map_x = 0; map_x < mapLayout->width; map_x++) {
				write_cell(
					mapLayout->blockdata[map_y * mapLayout->width + map_x],
					mapLayout,
					primary_tileset_tiles,
					secondary_tileset_tiles,
					surface->pixels,
					map_x * 16,
					map_y * 16,
					layer
				);
			}
		}
	}

	free(mapLayout->blockdata);
	free(mapLayout->primary_tileset->metatiles);
	free(mapLayout->secondary_tileset->metatiles);
	free(mapLayout->primary_tileset->palettes);
	free(mapLayout->secondary_tileset->palettes);

	return (MapSurfaces){
		.bg_surface = bg_surface,
		.fg_surface = fg_surface
	};
}

MapTextures load_map_textures(GameState *state, MapLayout *mapLayout, SDL_FRect *dstRect) {
	MapSurfaces surfaces = load_map_surfaces(mapLayout);

	SDL_Texture *bg_texture = SDL_CreateTextureFromSurface(state->renderer, surfaces.bg_surface);
	SDL_SetTextureScaleMode(bg_texture, SDL_SCALEMODE_NEAREST);
	SDL_Texture *fg_texture = SDL_CreateTextureFromSurface(state->renderer, surfaces.fg_surface);
	SDL_SetTextureScaleMode(fg_texture, SDL_SCALEMODE_NEAREST);

	SDL_DestroySurface(surfaces.bg_surface);
	SDL_DestroySurface(surfaces.fg_surface);

	*dstRect = (SDL_FRect){
		(state->player.x + 7) * 16,
		(state->player.y + 4.5) * 16,
		mapLayout->width * 16.0f,
		mapLayout->height * 16.0f
	};

	return (MapTextures){
		.bg_texture = bg_texture,
		.fg_texture = fg_texture
	};
}

