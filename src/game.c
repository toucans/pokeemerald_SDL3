#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "game.h"

u8 *get_u8_data(u32 offset, u32 length) {
    FILE *file = fopen(filepath, "rb");
    fseek(file, offset, SEEK_SET);
    u8 *data = malloc(length * sizeof(u8));
    fread(data, sizeof(u8), length, file);
    fclose(file);
    return data;
}

u16 *get_u16_data(u32 offset, u32 length) {
    FILE *file = fopen(filepath, "rb");
    fseek(file, offset, SEEK_SET);
    u16 *data = malloc(length * sizeof(u16));
    fread(data, sizeof(u16), length, file);
    fclose(file);
    return data;
}
#include "map.h"
#include "sprite_loader.h"
#include "audio.h"
#include "fly_map_renderer.h"


/* ── Mode: OVERWORLD ─────────────────────────────────────────────────────── */

static void overworld_update(GameState *state) {
    player_update(state);
    camera_update(state);
    map_update(state);
}

static void overworld_render(const GameState *state) {
    SDL_Renderer *renderer = state->renderer;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, state->mapTextures.bg_texture, NULL, &state->camera.rect);
    SDL_RenderTexture(renderer, state->player.texture, &state->player.srcRect, &state->player.dstRect);
    SDL_RenderTexture(renderer, state->mapTextures.fg_texture, NULL, &state->camera.rect);
    SDL_RenderPresent(renderer);
}


/* ── Mode: FLY_MAP ───────────────────────────────────────────────────────── */

static void fly_map_mode_render(const GameState *state) {
    SDL_Renderer *renderer = state->renderer;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, state->fly_map_texture, NULL, NULL); /* NULL dst = fill logical screen */
    SDL_RenderPresent(renderer);
}


/* ── Public API ──────────────────────────────────────────────────────────── */

void game_init(GameState *state) {
    state->mode = MODE_OVERWORLD;
    state->fly_map_texture = NULL;

    state->currentMap = MapGroups[MAP_GROUP_TOWNS_AND_ROUTES][MAP_ROUTE115];

    state->player.x = 35;
    state->player.y = 55;
    state->camera.rect = (SDL_FRect){
        (-state->player.x + 7) * 16,
        (-state->player.y + 4.5) * 16,
        OVERWORLD_WIDTH * 16.0f,
        OVERWORLD_HEIGHT * 16.0f
    };
    overworld_init(state);
    state->mapTextures = state->overworld;

    SDL_Surface *hero_surface = load_hero_surface();
    state->player.texture = SDL_CreateTextureFromSurface(state->renderer, hero_surface);
    SDL_SetTextureScaleMode(state->player.texture, SDL_SCALEMODE_NEAREST);
    state->player.srcRect = (SDL_FRect){ 0,  0,  16, 32 };
    state->player.dstRect = (SDL_FRect){ 7 * 16, 3.5 * 16, 16, 32 };
    state->player.facing  = FACING_DOWN;

    audio_init();
    audio_play_music(state->currentMap->music);
}

void game_update(GameState *state) {
    /* held arrow keys (keyboard state supplements event-based input) */
    const bool *keys = SDL_GetKeyboardState(NULL);
    state->input |= ((u16)keys[SDL_SCANCODE_UP]    << 0);
    state->input |= ((u16)keys[SDL_SCANCODE_DOWN]  << 1);
    state->input |= ((u16)keys[SDL_SCANCODE_LEFT]  << 2);
    state->input |= ((u16)keys[SDL_SCANCODE_RIGHT] << 3);

    /* unlock audio on first input (web autoplay policy) */
    static bool audio_unlocked = false;
    if (!audio_unlocked && state->input) {
        audio_play_music(state->currentMap->music);
        audio_unlocked = true;
    }

    /* Enter toggles fly map */
    if (state->input & INPUT_ENTER) {
        if (state->mode == MODE_OVERWORLD) {
            state->mode = MODE_FLY_MAP;
            if (!state->fly_map_texture) {
                SDL_Surface *surf = fly_map_surface_create();
                state->fly_map_texture = SDL_CreateTextureFromSurface(state->renderer, surf);
                SDL_SetTextureScaleMode(state->fly_map_texture, SDL_SCALEMODE_NEAREST);
                SDL_DestroySurface(surf);
            }
        } else if (state->mode == MODE_FLY_MAP) {
            state->mode = MODE_OVERWORLD;
        }
    }

    switch (state->mode) {
        case MODE_OVERWORLD: overworld_update(state); break;
        case MODE_FLY_MAP:   /* no per-frame updates needed */  break;
    }
}

void game_render(const GameState *state) {
    switch (state->mode) {
        case MODE_OVERWORLD: overworld_render(state);   break;
        case MODE_FLY_MAP:   fly_map_mode_render(state); break;
    }
}
