#include "ray.h"
/**
 * calculateRayPosition - calculates the position of the ray
 *
 * @state: raycaster state
 * @raydirX: X coordinate of ray
 * @raydirY: Y coordinate of ray
 */
static void calculateRayPosition(int x, RaycasterState* state, double* rayDirX, double* rayDirY)
{
	double cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
	*rayDirX = state->dirX + state->planeX * cameraX;
	*rayDirY = state->dirY + state->planeY * cameraX;
}
/**
 * performDDA - performs raycasting using the DDA algorithm
 *
 * @state: raycaster state
 * @rayDirX: x coordinate of the ray
 * @raydirY: y coordinate of the ray
 * @mapX: x coordinate of map
 * @mapY: y coordinte of map
 * @perpawallDist: distance to wall
 * @side: side of wall (North or sourth)
 *
 */
static void performDDA(RaycasterState* state, double rayDirX, double rayDirY, int* mapX, int* mapY, double *perpWallDist, int* side)
{
	int stepX, stepY, hit = 0;
	double sideDistX, sideDistY;
	double deltaDistX = fabs(1 / rayDirX);
	double deltaDistY = fabs(1 / rayDirY);

	*mapX = (int)state->posX;
	*mapY = (int)state->posY;

	stepX = rayDirX < 0 ? -1 : 1;
	sideDistX = (rayDirX < 0) ? (state->posX - *mapX) * deltaDistX : (*mapX + 1.0 - state->posX) * deltaDistX;

	stepY = rayDirY < 0 ? -1 : 1;
	sideDistY = (rayDirY < 0) ? (state->posY - *mapY) * deltaDistY : (*mapY + 1.0 - state->posY) * deltaDistY;

	while (hit == 0) {
		if (sideDistX < sideDistY) {
			sideDistX += deltaDistX;
			*mapX += stepX;
			*side = 0;
		} else {
			sideDistY += deltaDistY;
			*mapY += stepY;
			*side = 1;
	}
		if (state->map[*mapX][*mapY] > 0) hit = 1;
	}

	*perpWallDist = (*side == 0)
		? (*mapX - state->posX + (1 - stepX) / 2) / rayDirX
		: (*mapY - state->posY + (1 - stepY) / 2) / rayDirY;
}

/**
 * Draws the walls by filling the pixel buffer instead of using SDL_RenderDrawPoint.
 * @param rcState: The raycaster state
 * @param x: The column of the screen being rendered
 * @param perpWallDist: The perpendicular distance from the player to the wall
 * @param side: The side of the wall hit (0 for x, 1 for y)
 */
void drawWallToBuffer(int x, double perpWallDist, int side, RaycasterState* rcState, double rayDirX, double rayDirY, int mapX, int mapY)
{
	int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
	int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
	if (drawStart < 0) drawStart = 0;
	int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
	if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

	int texNum = rcState->map[mapX][mapY] - 1;
	if (texNum < 0 || texNum >= 8) texNum = 0;

	double wallX = (side == 0) ? rcState->posY + perpWallDist * rayDirY : rcState->posX + perpWallDist * rayDirX;
	wallX -= floor(wallX);

	int texX = (int)(wallX * (double)TEXWIDTH);
	if (side == 0 && rayDirX > 0) texX = TEXWIDTH - texX - 1;
	if (side == 1 && rayDirY < 0) texX = TEXWIDTH - texX - 1;

	double step = 1.0 * TEXHEIGHT / lineHeight;
	double texPos = (drawStart - SCREEN_HEIGHT / 2 + lineHeight / 2) * step;

	for (int y = drawStart; y < drawEnd; y++) {
		int texY = (int)texPos & (TEXHEIGHT - 1);
		texPos += step;

		Uint32 color = rcState->textures[texNum][texY * TEXWIDTH + texX];
		Uint8 r = color & 0xFF;
		Uint8 g = (color >> 8) & 0xFF;
		Uint8 b = (color >> 16) & 0xFF;

		if (side == 1) {
			r = (Uint8)(r * 0.7);
			g = (Uint8)(g * 0.7);
			b = (Uint8)(b * 0.7);
		}

		// Store the pixel in the buffer (ARGB format)
		rcState->pixelBuffer[y * SCREEN_WIDTH + x] = (255 << 24) | (r << 16) | (g << 8) | b;
	}
}

/**
 * Renders the scene using a pixel buffer and SDL_Texture for faster rendering.
 * @param sdlState: The SDL state
 * @param rcState: The raycaster state
 */
void render(SDLState* sdlState, RaycasterState* rcState)
{

	memset(rcState->pixelBuffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

	drawFloorAndCeiling(rcState);
	for (int x = 0; x < SCREEN_WIDTH; x++) {
		double rayDirX, rayDirY;
		calculateRayPosition(x, rcState, &rayDirX, &rayDirY);

		int mapX, mapY, side;
		double perpWallDist;
		performDDA(rcState, rayDirX, rayDirY, &mapX, &mapY, &perpWallDist, &side);

		drawWallToBuffer(x, perpWallDist, side, rcState, rayDirX, rayDirY, mapX, mapY);

		rcState->zBuffer[x] = perpWallDist;
	}

	// Create a texture for the pixel buffer
	SDL_Texture* texture = SDL_CreateTexture(sdlState->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_UpdateTexture(texture, NULL, rcState->pixelBuffer, SCREEN_WIDTH * sizeof(Uint32));

	// Render the texture
	SDL_RenderClear(sdlState->renderer);
	SDL_RenderCopy(sdlState->renderer, texture, NULL, NULL);
	SDL_DestroyTexture(texture);
	// Render the enemy and gun after walls
	renderEnemy(sdlState->renderer, rcState);
//	handleShooting(rcState);
	renderGun(sdlState->renderer, rcState);

	if (rcState->toggleMap)
		drawMiniMap(sdlState->renderer, rcState);

	SDL_RenderPresent(sdlState->renderer);
}
