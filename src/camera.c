#include "game.h"
#include "player.h"
#include <math.h>

#define CAMERA_SPEED 48

void camera_update(GameState *state) {
    state->camera.move_ended = false;

    if (state->player.state == PLAYER_MOVING) {
        Camera *camera = &state->camera;
        camera->move_timer += state->timestep;

        camera->rect.x += camera->dir_x * CAMERA_SPEED * state->timestep;
        camera->rect.y += camera->dir_y * CAMERA_SPEED * state->timestep;

        if (camera->move_timer >= 1.0 * 16 / CAMERA_SPEED) {
            camera->rect.x = roundf((camera->rect.x / 16.0f) * 2.0f) / 2.0f * 16.0f; //multiply by 2 before rounding, divide by 2 after rounding to round to nearest .5
            camera->rect.y = roundf((camera->rect.y / 16.0f) * 2.0f) / 2.0f * 16.0f;
            state->player.state = PLAYER_IDLE;
            camera->dir_x = 0;
            camera->dir_y = 0;
            camera->move_timer = 0;
            camera->move_ended = true;
        }
    }
}