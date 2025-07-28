#include "game.h"
#include <string.h>

//bool direction_exists(MapLayout mapLayout, )

void map_update(GameState *state) {
    if(state->player.x < 0) {
        //printf("new map");
    }
    if(state->player.x >= state->currentMap->layout->width) {
        //printf("new map");
    }
    if(state->player.y < 0) {
        
    }
}