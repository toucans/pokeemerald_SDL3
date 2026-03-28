/*
 * fly_map_renderer.c
 *
 * The fly map pixels are baked into the binary at compile time by
 * graphics_extractor; this file just wraps them in an SDL surface.
 */

#include <SDL3/SDL.h>
#include "../generated/fly_map.h"   /* fly_map_pixels, fly_map_W, fly_map_H */

SDL_Surface *fly_map_surface_create(void)
{
    return SDL_CreateSurfaceFrom(
        fly_map_W,
        fly_map_H,
        SDL_PIXELFORMAT_ABGR1555,
        (void *)fly_map_pixels,
        fly_map_W * 2);  /* pitch in bytes */
}