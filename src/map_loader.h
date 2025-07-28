#pragma once
#include <SDL3/SDL.h>
#include <types.h>
#include "game.h"
#include "../pokeemerald/map_layouts.h"

typedef struct {
    SDL_Texture *bg_texture;
    SDL_Texture *fg_texture;
} MapTextures;

typedef struct {
    MapTextures mapTextures1;
	MapTextures mapTextures2;
	MapTextures mapTextures3;
	MapTextures mapTextures4;
	SDL_FRect rect1;
	SDL_FRect rect2;
	SDL_FRect rect3;
	SDL_FRect rect4;
} MapConnectionsTextures;

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
struct GameState;
extern MapTextures load_map_textures(struct GameState *state, MapLayout *mapLayout, SDL_FRect *dstRect);
extern MapConnectionsTextures load_connections_textures(struct GameState *state);