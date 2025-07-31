#include <SDL3/SDL.h>
#include <stdlib.h>
#include "game.h"
#include "map.h"
//#include "sprite_loader.h"



void game_init(GameState *state) {

    state->currentMap = MapGroups[MAP_GROUP_TOWNS_AND_ROUTES][MAP_MAUVILLE_CITY];
    MapLayout *mapLayout = state->currentMap->layout;
    u16 mapWidth = mapLayout->width * 16;
    u16 mapHeight = mapLayout->height * 16;
    state->mapTextures = load_map_textures(state, state->currentMap->layout, &state->camera.rect);

    overworld_init(state);
    state->mapTextures = state->overworld;
    state->camera.rect = (SDL_FRect){
		(state->player.x + 7) * 16,
		(state->player.y + 4.5) * 16,
		OVERWORLD_WIDTH * 16.0f,
		OVERWORLD_HEIGHT * 16.0f
	};
    
    
/*
    SDL_Surface *hero_surface = load_hero_surface();
    state->player.texture = SDL_CreateTextureFromSurface(state->renderer, hero_surface);
    SDL_SetTextureScaleMode(state->player.texture, SDL_SCALEMODE_NEAREST);
    state->player.srcRect = (SDL_FRect){ 
        0,
        0, 
        16,
        32
    };
    state->player.dstRect = (SDL_FRect){ 
        7 * 16,
        3.5 * 16, 
        16,
        32
    };
    state->player.facing = FACING_DOWN;
    */
}

void game_update(GameState *state) {
    SDL_PumpEvents();
    const bool *keys = SDL_GetKeyboardState(NULL);
    u16 *input = &state->input;
    state->input = 0; //clear previous input

    state->input |= ((u16)keys[SDL_SCANCODE_UP] << 0);
    state->input |= ((u16)keys[SDL_SCANCODE_DOWN] << 1);
    state->input |= ((u16)keys[SDL_SCANCODE_LEFT] << 2);
    state->input |= ((u16)keys[SDL_SCANCODE_RIGHT] << 3);

    player_update(state);
    camera_update(state);
    map_update(state);

}

void game_render(const GameState *state) {

    SDL_Renderer *renderer = state->renderer;

    // Clear screen to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    
    SDL_RenderTexture(renderer, state->mapTextures.bg_texture, NULL, &state->camera.rect);
    //SDL_RenderTexture(renderer, state->player.texture, &state->player.srcRect, &state->player.dstRect);
    SDL_RenderTexture(renderer, state->mapTextures.fg_texture, NULL, &state->camera.rect);    


    SDL_RenderPresent(renderer);

}
    

u8* get_u8_data(u32 offset, u32 length) {
    FILE* file = fopen(filepath, "rb");
    fseek(file, offset, SEEK_SET);
    u8* data = malloc(length * sizeof(u8));
    fread(data, sizeof(u8), length, file);
    fclose(file);
    return data;
}

u16* get_u16_data(u32 offset, u32 length) {
    FILE* file = fopen(filepath, "rb");
    fseek(file, offset, SEEK_SET);
    u16* data = malloc(length * sizeof(u16));
    fread(data, sizeof(u16), length, file);
    fclose(file);
    return data;
}