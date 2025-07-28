#pragma once
#include <SDL3/SDL.h>
#include "game.h"
#include <types.h>


typedef enum {
    PLAYER_IDLE,
    PLAYER_TURNING,
    PLAYER_MOVING
} PlayerState;
typedef enum {
    FACING_UP,
    FACING_DOWN,
    FACING_LEFT,
    FACING_RIGHT
} PlayerFacing;

#define DOWN_STOPPED 0
#define UP_STOPPED 32
#define LEFT_STOPPED 64
#define DOWN_WALKING1 96
#define DOWN_WALKING2 128
#define UP_WALKING1 160
#define UP_WALKING2 192
#define LEFT_WALKING1 224
#define LEFT_WALKING2 256
#define RIGHT_STOPPED 288       // LEFT_STOPPED horizontally flipped
#define RIGHT_WALKING1 320       // LEFT_WALKING1 horizontally flipped
#define RIGHT_WALKING2 352       // LEFT_WALKING2 horizontally flipped



typedef struct Player {
    int x, y;
    PlayerFacing facing;
    u8 stepLoop;
    PlayerState state;
    double anim_timer;
    u8 anim_frame;
    SDL_Texture *texture;
    SDL_FRect srcRect;
    SDL_FRect dstRect;
} Player;

struct GameState;
extern void player_update(struct GameState *state);

