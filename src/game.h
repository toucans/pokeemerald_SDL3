#pragma once
#include <SDL3/SDL.h>
#include "map_loader.h"
#include "player.h"
#include "camera.h"
#include "../pokeemerald/maps.h"
#include <types.h>

#define filepath "p.gba"

#define INPUT_UP    (1 << 0)
#define INPUT_DOWN  (1 << 1)
#define INPUT_LEFT  (1 << 2)
#define INPUT_RIGHT (1 << 3)
#define INPUT_ENTER (1 << 4)

typedef enum {
    MODE_OVERWORLD,
    MODE_FLY_MAP,
} GameMode;

typedef struct GameState {
    double timestep; //time elapsed since the last frame in seconds
    u16 input; //keyboard keys currently pressed
    GameMode mode;
    Player player;
    Camera camera;
    const Map *currentMap;
    const Map **tile_map; // flat [y * OVERWORLD_WIDTH + x] lookup, built at init
    SDL_Renderer *renderer;
    MapTextures overworld;
    MapTextures mapTextures;
    MapConnectionsTextures mapConnectionTextures;
    SDL_Texture *fly_map_texture; // created on first MODE_FLY_MAP entry
} GameState;


extern void game_init(GameState *state);
extern void game_update(GameState *state);
extern void game_render(const GameState *state);

extern u8* get_u8_data(u32 offset, u32 length);
extern u16* get_u16_data(u32 offset, u32 length);
