#include "game.h"
#include "player.h"

#define CAMERA_SPEED 360


void camera_update(GameState *state) {
    Camera *camera = &state->camera;

    if (state->player.state != PLAYER_MOVING) return;

    camera->rect.x += camera->dir_x * CAMERA_SPEED * (float)state->timestep;
    camera->rect.y += camera->dir_y * CAMERA_SPEED * (float)state->timestep;

    bool done = false;
    if (camera->dir_x > 0 && camera->rect.x >= camera->target_x) { camera->rect.x = camera->target_x; done = true; }
    if (camera->dir_x < 0 && camera->rect.x <= camera->target_x) { camera->rect.x = camera->target_x; done = true; }
    if (camera->dir_y > 0 && camera->rect.y >= camera->target_y) { camera->rect.y = camera->target_y; done = true; }
    if (camera->dir_y < 0 && camera->rect.y <= camera->target_y) { camera->rect.y = camera->target_y; done = true; }

    if (done) {
        state->player.state = PLAYER_IDLE;
        camera->dir_x = 0;
        camera->dir_y = 0;
        camera_move_ended(state);
    }
}
