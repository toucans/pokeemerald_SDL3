#pragma once
#include <SDL3/SDL.h>

/*
 * fly_map_surface_create
 *
 * Returns a non-owning 256×160 SDL_PIXELFORMAT_ABGR1555 surface backed
 * by the fly_map_pixels array compiled into the binary.
 *
 * Because the pixel data is a compile-time constant (const uint16_t[]),
 * the surface does NOT own its pixel buffer.  Do NOT call SDL_DestroySurface
 * on the returned surface after uploading it to a texture — or if you do,
 * pass SDL_FALSE to SDL_CreateSurfaceFrom so SDL doesn't try to free it.
 *
 * Typical usage:
 *   SDL_Surface *surf = fly_map_surface_create();
 *   SDL_Texture *tex  = SDL_CreateTextureFromSurface(renderer, surf);
 *   SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
 *   SDL_DestroySurface(surf);   // safe: SDL_CreateSurfaceFrom, not SDL_CreateSurface
 */
SDL_Surface *fly_map_surface_create(void);