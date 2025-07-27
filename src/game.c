#include <SDL3/SDL.h>
#include "map_loader.h"
#include "game.h"
#include "sprite_loader.h"
#include <stdlib.h>



void game_init(GameState *state) {

    state->currentMap = MapGroups[MAP_GROUP_TOWNS_AND_ROUTES][MAP_LITTLEROOT_TOWN];
    MapLayout *mapLayout = state->currentMap->layout;
    u16 mapWidth = mapLayout->width * 16;
    u16 mapHeight = mapLayout->height * 16;
    MapSurfaces mapSurfaces = load_map_surfaces(mapLayout);
    state->camera.rect = (SDL_FRect){ 
        (state->player.x + 7) * 16, 
        (state->player.y + 4.5) * 16, 
        mapWidth, 
        mapHeight 
    };
    
    state->bg_texture = SDL_CreateTextureFromSurface(state->renderer, mapSurfaces.bg_surface);
    SDL_SetTextureScaleMode(state->bg_texture, SDL_SCALEMODE_NEAREST);
    state->fg_texture = SDL_CreateTextureFromSurface(state->renderer, mapSurfaces.fg_surface);
    SDL_SetTextureScaleMode(state->fg_texture, SDL_SCALEMODE_NEAREST);
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

void game_render(const GameState *state) {

    SDL_Renderer *renderer = state->renderer;

    // Clear screen to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    
    
    //SDL_RenderTexture(renderer, circle_texture, NULL, &dst);
    SDL_RenderTexture(renderer, state->bg_texture, NULL, &state->camera.rect);
    //SDL_RenderTexture(renderer, state->player.texture, &state->player.srcRect, &state->player.dstRect);
    SDL_RenderTexture(renderer, state->fg_texture, NULL, &state->camera.rect);



    

    // TODO: Draw your game here (tilemap, sprite, etc.)

    SDL_RenderPresent(renderer);

    // Optional delay if not using vsync
    //SDL_Delay(FRAME_TIME_MS);
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

    // player_update(state);
    // camera_update(state);

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