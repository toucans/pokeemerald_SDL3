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
    SDL_Texture *fly_map_texture;        /* created on first MODE_FLY_MAP entry */
    SDL_Texture *fly_map_cursor[2];      /* cursor frames 0 and 1               */
    int          fly_cursor_x;           /* tile column, range 1–28             */
    int          fly_cursor_y;           /* tile row, range 2–16                */
    u32          fly_cursor_tick;        /* increments each frame for animation */
    float        fly_cursor_px;          /* current rendered screen x (top-left)*/
    float        fly_cursor_py;          /* current rendered screen y (top-left)*/
    float        fly_cursor_target_px;
    float        fly_cursor_target_py;
    int          fly_cursor_dir_x;       /* -1, 0, +1 — nonzero means moving   */
    int          fly_cursor_dir_y;
} GameState;


extern void game_init(GameState *state);
extern void game_update(GameState *state);
extern void game_render(const GameState *state);

extern u8* get_u8_data(u32 offset, u32 length);
extern u16* get_u16_data(u32 offset, u32 length);
