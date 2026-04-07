#pragma once
#include <SDL3/SDL.h>

/*
 * fly_map_surface_create
 *
 * Reads the fly-destination map from the ROM at runtime, renders it to a
 * 256×160 SDL_PIXELFORMAT_ABGR1555 surface, and returns it.
 *
 * The caller owns the surface and must call SDL_DestroySurface() when done.
 */
SDL_Surface *fly_map_surface_create(void);

/*
 * fly_map_cursor_textures_create
 *
 * Decodes the two-frame cursor_small sprite from the ROM into two 16×16
 * SDL_PIXELFORMAT_ABGR1555 textures with blend mode BLEND (transparent bg).
 *
 * Frame 0 and frame 1 alternate every 20 ticks (~333 ms at 60 fps).
 * The caller owns both textures and must SDL_DestroyTexture them.
 */
void fly_map_cursor_textures_create(SDL_Renderer *renderer,
                                    SDL_Texture **out_frame0,
                                    SDL_Texture **out_frame1);
