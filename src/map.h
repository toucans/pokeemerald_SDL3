#pragma once
#include "game.h"

#define OVERWORLD_WIDTH 800
#define OVERWORLD_HEIGHT 383

extern void overworld_init(GameState *state);
extern void map_update(GameState *state);