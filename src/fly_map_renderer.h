#pragma once
#include <SDL3/SDL.h>

/*
 * fly_map_surface_create
 *
 * Reads the fly-destination map from the ROM at runtime, renders it to a
 * 256×160 SDL_PIXELFORMAT_ABGR1555 surface, and returns it.
 *
 * The caller owns the surface and must call SDL_DestroySurface() when done.
 *
 * Typical usage:
 *   SDL_Surface *surf = fly_map_surface_create();
 *   SDL_Texture *tex  = SDL_CreateTextureFromSurface(renderer, surf);
 *   SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
 *   SDL_DestroySurface(surf);
 */
SDL_Surface *fly_map_surface_create(void);
