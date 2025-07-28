#include "game.h"
#include "player.h"
#include <math.h>

#define CAMERA_SPEED 48

void set_connection_rect(SDL_FRect *dstRect, SDL_FRect *cameraRect, MapConnection *mapConnection) {
    if (strcmp(mapConnection->direction, "up") == 0) {
        dstRect->x = cameraRect->x + (mapConnection->offset * 16);
        dstRect->y = cameraRect->y - (mapConnection->map->layout->height * 16);
    }
    else if (strcmp(mapConnection->direction, "down") == 0) {
        dstRect->x = cameraRect->x + (mapConnection->offset * 16);
        dstRect->y = cameraRect->y + (cameraRect->h);
    }
    else if (strcmp(mapConnection->direction, "left") == 0) {
        dstRect->x = cameraRect->x - (mapConnection->map->layout->width * 16);
        dstRect->y = cameraRect->y + (mapConnection->offset * 16);
    }
    else if (strcmp(mapConnection->direction, "right") == 0) {
        dstRect->x = cameraRect->x + (cameraRect->w);
        dstRect->y = cameraRect->y + (mapConnection->offset * 16);
    }
}

void camera_update(GameState *state) {
    state->camera.move_ended = false;
    Camera *camera = &state->camera;

    if (state->player.state == PLAYER_MOVING) {
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

    const Map *currentMap = state->currentMap;
	MapConnection *mapConnections = currentMap->connections;
    u8 count = 0;
	for (u8 i = 0; i < currentMap->connections_count; i++)
	{
		if (strcmp(mapConnections[i].direction, "dive") != 0 && strcmp(mapConnections[i].direction, "emerge") != 0) {
			switch (count)
			{
			case 0:
                set_connection_rect(&state->mapConnectionTextures.rect1, &camera->rect, &mapConnections[i]);
                // printf("con w%f\n", state->mapConnectionTextures.rect1.w);
                // printf("main w%f\n", camera->rect.w);
                // printf("con x%f\n", state->mapConnectionTextures.rect1.x);
                // printf("con y%f\n", state->mapConnectionTextures.rect1.y);
                // printf("main x%f\n", camera->rect.x);
                // printf("main y%f\n", camera->rect.y);
				break;
			case 1:

				break;
			case 2:
				break;
			case 3:
				break;
			default:
				break;
			}
			count++;
		}
	}

}