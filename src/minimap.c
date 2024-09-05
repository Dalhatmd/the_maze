#include "ray.h"
/**
 * drawMiniMap - draws the minimap
 * @renderer: the renderer
 * @state: the raycaster state
 */
void drawMiniMap(SDL_Renderer *renderer, RaycasterState *state)
{
	int minimapX = SCREEN_WIDTH - MINIMAP_SIZE - 10;
	int minimapY = 10;

	// Draw the minimap
	for (int y = 0; y < MAP_WIDTH; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			SDL_Rect rect = {
				minimapX + x * MINIMAP_SCALE,
				minimapY + y * MINIMAP_SCALE,
				MINIMAP_SCALE,
				MINIMAP_SCALE
			};
			if (state->map[x][y] != 0)
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			}
			SDL_RenderFillRect(renderer, &rect);
		}
	}

	// Draw the player
	int playerX = minimapX + (int)(state->posX * MINIMAP_SCALE);
	int playerY = minimapY + (int)(state->posY * MINIMAP_SCALE);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_Rect playerRect = {
		playerX,
		playerY ,
		4,
		4
	};
	SDL_RenderFillRect(renderer, &playerRect);
	int dirX = playerX + (int)(state->dirX * MINIMAP_SCALE);
	int dirY = playerY + (int)(state->dirY * MINIMAP_SCALE);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderDrawLine(renderer, playerX, playerY, dirX, dirY);
}
