#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "game.h"

//hero palette offset = 4818936
//hero animation data offset = 4814328
//0x20*8

SDL_Surface *load_hero_surface(void) {

	
	u8 *actorAnimationData = get_u8_data(4814328, 0x20*8 * 9); //get all 9

	u16 *palette = get_u16_data(4818936, 16);	


	u16 heroWidth = 16;
    u16 heroHeight = 32;
	SDL_Surface * hero_surface = SDL_CreateSurface(heroWidth, heroHeight * 12, SDL_PIXELFORMAT_ABGR1555);


	u16 heroTileWidth = heroWidth / 8;
	u16 heroTileHeight = heroHeight / 8 * 9; //get all 9, DOWN_STOPPED, UP_STOPPED, LEFT_STOPPED, DOWN_WALKING1, DOWN_WALKING2, UP_WALKING1, UP_WALKING2, LEFT_WALKING1, LEFT_WALKING2, 
											// RIGHT_STOPPED, RIGHT_WALKING1, RIGHT_WALKING2 + flipped versions of LEFT_STOPPED, LEFT_WALKING1 and LEFT_WALKING2
	for (u8 i = 0; i < heroTileHeight; i++) {
		for (u8 j = 0; j < heroTileWidth; j++) {
			int tileIndex = (i * heroTileWidth + j) * 32;
			const u8* tileData = &actorAnimationData[tileIndex];

			// Write normal tile
			write_tile(tileData, 0, 0, palette, hero_surface->pixels, 0, j * 8, i * 8, heroWidth);

			int flipDstRow = -1;
			if (i >= 8 && i < 12) flipDstRow = 7 * 4 + i; // move up 7 spots
			else if (i >= 28 && i < 32) flipDstRow = 3 * 4 + i; // move up 3 spots
			else if (i >= 32 && i < 36) flipDstRow = 3 * 4 + i; // move up 3 spots

			if (flipDstRow != -1) {
				// Reverse tile order on this row
				int flipped_j = heroTileWidth - 1 - j;
				write_tile(tileData, 1, 0, palette, hero_surface->pixels, 0, flipped_j * 8, flipDstRow * 8, heroWidth);
			}
		}
	}
	free(actorAnimationData);
	free(palette);

	return hero_surface;
}