#include "ray.h"

void drawCeiling(SDL_Renderer *renderer, RaycasterState *rcState)
{
	float posZ, rayDirX0, rayDirY0, rayDirX1, rayDirY1, rowDistance;
	float floorStepX, floorStepY, floorX, floorY;
	int y, p, x, cellX, cellY, tx, ty;

	posZ = 0.5 * SCREEN_HEIGHT;
	for (y = 0; y < SCREEN_HEIGHT / 2; y++)
	{
		rayDirX0 = rcState->dirX - rcState->planeX;
		rayDirY0 = rcState->dirY - rcState->planeY;
		rayDirX1 = rcState->dirX + rcState->planeX;
		rayDirY1 = rcState->dirY + rcState->planeY;
		
		p = SCREEN_HEIGHT / 2 - y;
		rowDistance = posZ / p;

		floorStepX = rowDistance * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
		floorStepY = rowDistance * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;

		floorX = rcState->posX + rowDistance * rayDirX0;
		floorY = rcState->posY + rowDistance * rayDirY0;
		
		for (x = 0; x < SCREEN_WIDTH; x++)
		{
			cellX = (int)(floorX * CEILING_MAGNIFICATION) & (CEILING_TEXTURE_SIZE - 1);
			cellY = (int)(floorY * CEILING_MAGNIFICATION) & (CEILING_TEXTURE_SIZE - 1);


			floorX += floorStepX;
			floorY += floorStepY;

		int ceilingPixel = rcState->ceilingTexture[cellX + cellY * CEILING_TEXTURE_SIZE];
		Uint8 r = ceilingPixel & 0xFF;
		Uint8 g = (ceilingPixel >> 8) & 0xFF;
		Uint8 b = (ceilingPixel >> 16) & 0xFF;

		float darkness = 0.5 + ((float)y / (float)(SCREEN_HEIGHT));
		r = (Uint8)(r * darkness);
		g = (Uint8)(g * darkness);
		b = (Uint8)(b * darkness);

		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		SDL_RenderDrawPoint(renderer, x, y);
		

		}
	}
}
