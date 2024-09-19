#include "ray.h"
/**
 * drawFloorAndCeiling - draws floor and ceiling on the raycaster
 *
 * @renderer: SDL renderer
 * @rcstate: raycaster state
 *
 * Description: All textures are stored in a pixel Buffer to boost performance
 * 		instead of using SDL_RenderCopy
 */
void drawFloorAndCeiling(SDL_Renderer *renderer, RaycasterState *rcstate)
{
	int y, p, x;
	float rayDirX0, rayDirY0, rayDirX1, rayDirY1, posZ, rowDistance;
	float floorStepX, floorStepY, floorX, floorY;
	Uint8 r, g, b;
	Uint32 color;

	posZ = 0.5 * SCREEN_HEIGHT;

	for (y = 0; y < SCREEN_HEIGHT; y++)
	{
		int isCeiling = y < SCREEN_HEIGHT / 2;
		int p = isCeiling ? SCREEN_HEIGHT / 2 - y : y - SCREEN_HEIGHT / 2;

		// Set direction of the rays for floor/ceiling
		rayDirX0 = rcstate->dirX - rcstate->planeX;
		rayDirY0 = rcstate->dirY - rcstate->planeY;
		rayDirX1 = rcstate->dirX + rcstate->planeX;
		rayDirY1 = rcstate->dirY + rcstate->planeY;

		// Calculate distance and steps
		rowDistance = posZ / p;
		floorStepX = rowDistance * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
		floorStepY = rowDistance * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;

		floorX = rcstate->posX + rowDistance * rayDirX0;
		floorY = rcstate->posY + rowDistance * rayDirY0;

		for (x = 0; x < SCREEN_WIDTH; ++x)
		{
			int cellX = (int)(floorX * FLOOR_MAGNIFICATION) & (FLOOR_TEXTURE_SIZE - 1);
			int cellY = (int)(floorY * FLOOR_MAGNIFICATION) & (FLOOR_TEXTURE_SIZE - 1);

			floorX += floorStepX;
			floorY += floorStepY;

			// Choose the texture and apply darkness based on y position (ceiling vs floor)
			if (isCeiling)
			{
				int ceilingPixel = rcstate->ceilingTexture[cellX + cellY * CEILING_TEXTURE_SIZE];
				r = ceilingPixel & 0xFF;
				g = (ceilingPixel >> 8) & 0xFF;
				b = (ceilingPixel >> 16) & 0xFF;

				float ceilingDarkness = 0.5 + ((float)y / (float)(SCREEN_HEIGHT));
				r = (Uint8)(r * ceilingDarkness);
				g = (Uint8)(g * ceilingDarkness);
				b = (Uint8)(b * ceilingDarkness);
			}
			else
			{
				int floorTexture = rcstate->floorTexture[cellY * FLOOR_TEXTURE_SIZE + cellX];
				r = floorTexture & 0xFF;
				g = (floorTexture >> 8) & 0xFF;
				b = (floorTexture >> 16) & 0xFF;

				float floorDarkness = 1.0 - (float)y / SCREEN_HEIGHT;
				r = (Uint8)(r * floorDarkness);
				g = (Uint8)(g * floorDarkness);
				b = (Uint8)(b * floorDarkness);
			}

			// Store the pixel in the pixel buffer
			pixelBuffer[y * SCREEN_WIDTH + x] = (255 << 24) | (r << 16) | (g << 8) | b;
		}
	}
}

