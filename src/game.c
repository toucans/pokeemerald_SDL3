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

#define FLY_CURSOR_SPEED  120.0f   /* px/s — matches GBA: 2px/frame × 60fps */
#define FLY_CURSOR_X_MIN  0
#define FLY_CURSOR_X_MAX  27
#define FLY_CURSOR_Y_MIN  1
#define FLY_CURSOR_Y_MAX  15

/* pixel top-left of cursor on screen given a tile position */
#define CURSOR_PX(tx)  ((float)(8 * (tx) + 4))
#define CURSOR_PY(ty)  ((float)(8 * (ty) + 4))

static void fly_map_update(GameState *state) {
    bool moving = (state->fly_cursor_dir_x != 0 || state->fly_cursor_dir_y != 0);

    if (!moving) {
        int dx = 0, dy = 0;
        if      (state->input & INPUT_RIGHT && state->fly_cursor_x < FLY_CURSOR_X_MAX) dx = +1;
        else if (state->input & INPUT_LEFT  && state->fly_cursor_x > FLY_CURSOR_X_MIN) dx = -1;
        else if (state->input & INPUT_DOWN  && state->fly_cursor_y < FLY_CURSOR_Y_MAX) dy = +1;
        else if (state->input & INPUT_UP    && state->fly_cursor_y > FLY_CURSOR_Y_MIN) dy = -1;

        if (dx || dy) {
            state->fly_cursor_x      += dx;
            state->fly_cursor_y      += dy;
            state->fly_cursor_dir_x   = dx;
            state->fly_cursor_dir_y   = dy;
            state->fly_cursor_target_px = CURSOR_PX(state->fly_cursor_x);
            state->fly_cursor_target_py = CURSOR_PY(state->fly_cursor_y);
        }
    } else {
        float step = FLY_CURSOR_SPEED * (float)state->timestep;
        state->fly_cursor_px += (float)state->fly_cursor_dir_x * step;
        state->fly_cursor_py += (float)state->fly_cursor_dir_y * step;

        bool done = false;
        if (state->fly_cursor_dir_x > 0 && state->fly_cursor_px >= state->fly_cursor_target_px)
            { state->fly_cursor_px = state->fly_cursor_target_px; done = true; }
        if (state->fly_cursor_dir_x < 0 && state->fly_cursor_px <= state->fly_cursor_target_px)
            { state->fly_cursor_px = state->fly_cursor_target_px; done = true; }
        if (state->fly_cursor_dir_y > 0 && state->fly_cursor_py >= state->fly_cursor_target_py)
            { state->fly_cursor_py = state->fly_cursor_target_py; done = true; }
        if (state->fly_cursor_dir_y < 0 && state->fly_cursor_py <= state->fly_cursor_target_py)
            { state->fly_cursor_py = state->fly_cursor_target_py; done = true; }

        if (done) {
            state->fly_cursor_dir_x = 0;
            state->fly_cursor_dir_y = 0;
        }
    }

    state->fly_cursor_tick++;
}

static void fly_map_mode_render(const GameState *state) {
    SDL_Renderer *renderer = state->renderer;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_FRect src = { 0, 0, 240, 160 }; /* show left 240px of the 256-wide map */
    SDL_RenderTexture(renderer, state->fly_map_texture, &src, NULL);

    /* cursor: frame alternates every 20 ticks (~333 ms at 60 fps) */
    int frame = (state->fly_cursor_tick / 40) & 1;

    SDL_FRect cdst = { state->fly_cursor_px, state->fly_cursor_py, 16.0f, 16.0f };
    SDL_RenderTexture(renderer, state->fly_map_cursor[frame], NULL, &cdst);

    SDL_RenderPresent(renderer);
}


/* ── Public API ──────────────────────────────────────────────────────────── */

void game_init(GameState *state) {
    state->mode = MODE_OVERWORLD;
    state->fly_map_texture    = NULL;
    state->fly_map_cursor[0]  = NULL;
    state->fly_map_cursor[1]  = NULL;
    state->fly_cursor_x          = 13;  /* start near centre of 0-27 range */
    state->fly_cursor_y          = 8;   /* start near centre of 1-15 range */
    state->fly_cursor_tick       = 0;
    state->fly_cursor_px         = (float)(8 * 13 + 4);
    state->fly_cursor_py         = (float)(8 * 8  + 4);
    state->fly_cursor_target_px  = state->fly_cursor_px;
    state->fly_cursor_target_py  = state->fly_cursor_py;
    state->fly_cursor_dir_x      = 0;
    state->fly_cursor_dir_y      = 0;

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
                fly_map_cursor_textures_create(state->renderer,
                    &state->fly_map_cursor[0], &state->fly_map_cursor[1]);
                state->fly_cursor_tick = 0;
            }
        } else if (state->mode == MODE_FLY_MAP) {
            state->mode = MODE_OVERWORLD;
        }
    }

    switch (state->mode) {
        case MODE_OVERWORLD: overworld_update(state); break;
        case MODE_FLY_MAP:   fly_map_update(state); break;
    }
}

void game_render(const GameState *state) {
    switch (state->mode) {
        case MODE_OVERWORLD: overworld_render(state);   break;
        case MODE_FLY_MAP:   fly_map_mode_render(state); break;
    }
}
