#pragma once
#include "game.h"

typedef struct Camera {
    SDL_FRect rect; // current camera position
    int dir_x, dir_y;
    double move_timer;
    bool move_ended;
} Camera;

struct GameState;
extern void camera_update(struct GameState *state);