#pragma once
#include <SDL3/SDL.h>
#include <types.h>
#include "game.h"

typedef struct {
    SDL_Surface *bg_surface;
    SDL_Surface *fg_surface;
} MapSurfaces;

extern void write_tile(
	const u8 *tileNumber, 
	u8 h_flip, 
	u8 v_flip, 
	const u16 *palette, 
	u16 *SDLsurfacePixels, 
	u8 quadrant,
	u16 map_x,
	u16 map_y,
	u16 map_width
);
extern MapSurfaces load_map_surfaces(MapLayout *mapLayout);