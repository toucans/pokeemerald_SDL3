#pragma once
#include <stdint.h>

#define fly_map_W 256
#define fly_map_H 160

/* 256 × 160 SDL_PIXELFORMAT_ABGR1555 pixels.
 * Load with:
 *   SDL_CreateSurfaceFrom(fly_map_pixels, fly_map_W, fly_map_H,
 *                         fly_map_W * 2, SDL_PIXELFORMAT_ABGR1555); */
extern const uint16_t fly_map_pixels[256 * 160];
