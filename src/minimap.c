#include "ray.h"
/**
 * drawMiniMap - draws the minimap
 * @renderer: the renderer
 * @state: the raycaster state
 */
#include <math.h>

void drawMiniMap(SDL_Renderer *renderer, RaycasterState *state)
{
	int minimapX = SCREEN_WIDTH - MINIMAP_SIZE - 10;
	int minimapY = 10;
	int y, x;

	/* Draw the minimap */
	for (y = 0; y < MAP_WIDTH; y++)
	{
		for (x = 0; x < MAP_WIDTH; x++)
		{
			SDL_Rect rect = {
				minimapX + x * MINIMAP_SCALE,
				minimapY + y * MINIMAP_SCALE,
				MINIMAP_SCALE,
				MINIMAP_SCALE
			};
			if (state->map[x][y] != 0)
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // walls
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // empty space
			}
			SDL_RenderFillRect(renderer, &rect);
		}
	}

	/* Draw the player */
	int playerX = minimapX + (int)(state->posX * MINIMAP_SCALE);
	int playerY = minimapY + (int)(state->posY * MINIMAP_SCALE);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_Rect playerRect = {
		playerX - 2,
		playerY - 2,
		4,
		4
	};
	SDL_RenderFillRect(renderer, &playerRect);

	/* Draw the field of view as a cone */
	double fovHalfAngle = M_PI / 6;  // 30 degrees for half FOV
	double coneLength = 15;  // Length of the cone in minimap scale

	// Calculate direction vectors for the left and right sides of the FOV
	double leftDirX = cos(atan2(state->dirY, state->dirX) - fovHalfAngle);
	double leftDirY = sin(atan2(state->dirY, state->dirX) - fovHalfAngle);

	double rightDirX = cos(atan2(state->dirY, state->dirX) + fovHalfAngle);
	double rightDirY = sin(atan2(state->dirY, state->dirX) + fovHalfAngle);

	// Calculate the end points of the cone
	int leftEndX = playerX + (int)(leftDirX * coneLength);
	int leftEndY = playerY + (int)(leftDirY * coneLength);
	int rightEndX = playerX + (int)(rightDirX * coneLength);
	int rightEndY = playerY + (int)(rightDirY * coneLength);

	// Set color to semi-transparent red for the cone
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 128);

	// Draw the filled triangle representing the FOV cone
	SDL_RenderDrawLine(renderer, playerX, playerY, leftEndX, leftEndY);
	SDL_RenderDrawLine(renderer, playerX, playerY, rightEndX, rightEndY);
	SDL_RenderDrawLine(renderer, leftEndX, leftEndY, rightEndX, rightEndY);

	// (Optional) For debugging, you can also draw lines for the cone edges
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawLine(renderer, playerX, playerY, leftEndX, leftEndY);
	SDL_RenderDrawLine(renderer, playerX, playerY, rightEndX, rightEndY);
}

