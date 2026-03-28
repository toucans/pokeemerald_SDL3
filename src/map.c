#include "game.h"
#include "map.h"
#include <string.h>
#include <stdlib.h>


void overworld_init(GameState *state) {

    SDL_Surface *overworld_bg_surface = SDL_CreateSurface(OVERWORLD_WIDTH * 16, OVERWORLD_HEIGHT * 16, SDL_PIXELFORMAT_ABGR1555);
    SDL_Surface *overworld_fg_surface = SDL_CreateSurface(OVERWORLD_WIDTH * 16, OVERWORLD_HEIGHT * 16, SDL_PIXELFORMAT_ABGR1555);

    for (size_t i = 0; i < TOWNS_AND_ROUTES_COUNT; i++) {
        MapLayout *mapLayout = MapGroups[MAP_GROUP_TOWNS_AND_ROUTES][i]->layout;
        if(mapLayout->overworld_pos_x != -1) {

            // Load individual map surfaces
            MapSurfaces mapSurfaces = load_map_surfaces(mapLayout);

            // Calculate pixel destination coordinates
            int dest_x = mapLayout->overworld_pos_x;
            int dest_y = mapLayout->overworld_pos_y;

            SDL_Rect dst_rect = {
                .x = dest_x * 16,
                .y = dest_y * 16,
                .w = mapLayout->width * 16,
                .h = mapLayout->height * 16
            };

            // Blit background and foreground layers
            SDL_BlitSurface(mapSurfaces.bg_surface, NULL, overworld_bg_surface, &dst_rect);
            SDL_BlitSurface(mapSurfaces.fg_surface, NULL, overworld_fg_surface, &dst_rect);

            // Clean up map surfaces
            SDL_DestroySurface(mapSurfaces.bg_surface);
            SDL_DestroySurface(mapSurfaces.fg_surface);
        }
    }

    state->overworld.bg_texture = SDL_CreateTextureFromSurface(state->renderer, overworld_bg_surface);
    state->overworld.fg_texture = SDL_CreateTextureFromSurface(state->renderer, overworld_fg_surface);

    SDL_SetTextureScaleMode(state->overworld.bg_texture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(state->overworld.fg_texture, SDL_SCALEMODE_NEAREST);

    // Destroy surfaces after texture creation
    SDL_DestroySurface(overworld_bg_surface);
    SDL_DestroySurface(overworld_fg_surface);

    // Build tile → map lookup table (calloc zeroes all entries to NULL)
    state->tile_map = calloc(OVERWORLD_WIDTH * OVERWORLD_HEIGHT, sizeof(const Map *));
    for (size_t i = 0; i < TOWNS_AND_ROUTES_COUNT; i++) {
        const Map *map = MapGroups[MAP_GROUP_TOWNS_AND_ROUTES][i];
        const MapLayout *layout = map->layout;
        if (layout->overworld_pos_x == -1) continue;
        for (int y = layout->overworld_pos_y; y < layout->overworld_pos_y + (int)layout->height; y++) {
            for (int x = layout->overworld_pos_x; x < layout->overworld_pos_x + (int)layout->width; x++) {
                state->tile_map[y * OVERWORLD_WIDTH + x] = map;
            }
        }
    }
}


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