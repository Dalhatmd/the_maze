#include "ray.h"

void drawFloor(SDL_Renderer *renderer, RaycasterState *rcstate)
{
	int y, p, x;
	float rayDirX0, rayDirY0, rayDirX1, rayDirY1, posZ, rowDistance;
	float floorStepX, floorStepY, floorX, floorY;

	posZ = 0.5 * SCREEN_HEIGHT;

	for (y = SCREEN_HEIGHT / 2; y < SCREEN_HEIGHT; y++)
	{
		rayDirX0 = rcstate->dirX - rcstate->planeX;
		rayDirY0 = rcstate->dirY - rcstate->planeY;
		rayDirX1 = rcstate->dirX + rcstate->planeX;
		rayDirY1 = rcstate->dirY + rcstate->planeY;

		p = y - SCREEN_HEIGHT / 2;
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

			int floorTexture = rcstate->floorTexture[FLOOR_TEXTURE_SIZE * cellY + cellX];
			Uint8 r = floorTexture & 0XFF;
			Uint8 g = (floorTexture >> 8) & 0XFF;
			Uint8 b = (floorTexture >> 16) & 0XFF;
			
			float darkness = 1.0 - (float)y / SCREEN_HEIGHT;
			r = (Uint8)(r * darkness);
			g = (Uint8)(g * darkness);
			b = (Uint8)(b * darkness);

			SDL_SetRenderDrawColor(renderer, r, g, b, 255);
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}
}
