#include "game.h"
#include "player.h"
#include "camera.h"

int get_walk1_frame(PlayerFacing facing) {
    switch (facing) {
        case FACING_DOWN: return DOWN_WALKING1;
        case FACING_UP: return UP_WALKING1;
        case FACING_LEFT: return LEFT_WALKING1;
        case FACING_RIGHT: return RIGHT_WALKING1;
        default: return 0;
    }
}

int get_walk2_frame(PlayerFacing facing) {
    switch (facing) {
        case FACING_DOWN: return DOWN_WALKING2;
        case FACING_UP: return UP_WALKING2;
        case FACING_LEFT: return LEFT_WALKING2;
        case FACING_RIGHT: return RIGHT_WALKING2;
        default: return 0;
    }
}

int get_stopped_frame(PlayerFacing facing) {
    switch (facing) {
        case FACING_DOWN: return DOWN_STOPPED;
        case FACING_UP: return UP_STOPPED;
        case FACING_LEFT: return LEFT_STOPPED;
        case FACING_RIGHT: return RIGHT_STOPPED;
        default: return 0;
    }
}

void stepTaken(GameState *state) {
    Player *p = &state->player;
    const Map *m = state->currentMap;
    MapLayout *l = m->layout;
    if(p->x >= l->overworld_pos_x + l->width 
    || p->y >= l->overworld_pos_y + l->height
    || p->x < l->overworld_pos_x
    || p->y < l->overworld_pos_y) 
    {
        for (size_t i = 0; i < m->connections_count; i++)
        {
            s16 pos_x = m->connections[i].map->layout->overworld_pos_x;
            s16 pos_y = m->connections[i].map->layout->overworld_pos_y;
            u16 width = m->connections[i].map->layout->width;
            u16 height = m->connections[i].map->layout->height;
            if(p->x >= pos_x 
            && p->x < pos_x + width
            && p->y >= pos_y
            && p->y < pos_y + height) {
                state->currentMap = m->connections[i].map;
                printf("new map is %s\n", m->connections[i].map->id);
            }
        }
        

    }
}

void camera_move_ended(GameState *state) {
    Player *p = &state->player;
    p->srcRect.y = get_stopped_frame(p->facing);
    p->state = PLAYER_IDLE;
    stepTaken(state);
}

void player_update(GameState *state) {
    Player *p = &state->player;

    if(p->state == PLAYER_IDLE) {
        u16 input = state->input;
        if(input & 0b1111) {
            if (input & INPUT_UP) {
                if(p->facing != FACING_UP) {
                    p->facing = FACING_UP;
                    p->state = PLAYER_TURNING;
                    p->anim_timer = 0.0f;
                    p->anim_frame = 0;
                } else {
                    p->y -= 1;
                    p->state = PLAYER_MOVING;
                    state->camera.dir_y = 1;
                }
            }
            else if (input & INPUT_DOWN) {
                if(p->facing != FACING_DOWN) {
                    p->facing = FACING_DOWN;
                    p->state = PLAYER_TURNING;
                    p->anim_timer = 0.0f;
                    p->anim_frame = 0;
                } else {
                    p->y += 1;
                    p->state = PLAYER_MOVING;
                    state->camera.dir_y = -1;
                }
            }
            else if (input & INPUT_LEFT) {
                if(p->facing != FACING_LEFT) {
                    p->facing = FACING_LEFT;
                    p->state = PLAYER_TURNING;
                    p->anim_timer = 0.0f;
                    p->anim_frame = 0;
                } else {
                    p->x -= 1;
                    p->state = PLAYER_MOVING;
                    state->camera.dir_x = 1;
                }
            }
            else if (input & INPUT_RIGHT) {
                if(p->facing != FACING_RIGHT) {
                    p->facing = FACING_RIGHT;
                    p->state = PLAYER_TURNING;
                    p->anim_timer = 0.0f;
                    p->anim_frame = 0;
                } else {
                    p->x += 1;
                    p->state = PLAYER_MOVING;
                    state->camera.dir_x = -1;
                }
            }
        }
    }

    #define TURN_ANIMATION_DURATION (8.0 / 60.0)
    #define TURN_ANIMATION_FRAMES 8
    if(p->state == PLAYER_TURNING) {

        p->anim_timer += state->timestep;
        
        // Calculate fractional progress
        float progress = p->anim_timer / TURN_ANIMATION_DURATION;
        int frame = (int)(progress * TURN_ANIMATION_FRAMES);

        // Clamp to last frame
        if (frame >= TURN_ANIMATION_FRAMES) {
            frame = TURN_ANIMATION_FRAMES - 1;
        }

        p->anim_frame = frame;

        if (p->anim_timer >= TURN_ANIMATION_DURATION) {
            p->state = PLAYER_IDLE;
            p->anim_timer = 0.0;
            p->anim_frame = 0;

            // Loop step state
            p->stepLoop = (p->stepLoop + 1) % 2;
        }
        else if (frame < TURN_ANIMATION_FRAMES / 2) {
            if (p->stepLoop == 0) {
                p->srcRect.y = get_walk1_frame(p->facing);
            } else {
                p->srcRect.y = get_walk2_frame(p->facing);
            }
        } else {
            p->srcRect.y = get_stopped_frame(p->facing);
        }
    }

}