#include "ray.h"

static void calculateRayPosition(int x, RaycasterState* state, double* rayDirX, double* rayDirY) {
    double cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
    *rayDirX = state->dirX + state->planeX * cameraX;
    *rayDirY = state->dirY + state->planeY * cameraX;
}

/*static void performDDA(RaycasterState* state, double rayDirX, double rayDirY, int* mapX, int* mapY, double *perpWallDist, int* side) {
    int stepX, stepY, hit = 0;
    double sideDistX, sideDistY;
    double deltaDistX = fabs(1 / rayDirX);
    double deltaDistY = fabs(1 / rayDirY);

    *mapX = (int)state->posX;
    *mapY = (int)state->posY;

    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (state->posX - *mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (*mapX + 1.0 - state->posX) * deltaDistX;
    }
    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (state->posY - *mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (*mapY + 1.0 - state->posY) * deltaDistY;
    }

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
    if (*side == 0) *perpWallDist = (*mapX - state->posX + (1 - stepX) / 2) / rayDirX;
    else *perpWallDist = (*mapY - state->posY + (1 - stepY) / 2) / rayDirY;

}

static void drawWall(SDL_Renderer* renderer, int x, double perpWallDist, int side, RaycasterState* rcState, double rayDirX, double rayDirY, int mapX, int mapY) {
	int drawEnd, texNum, texX, texY, y; //texOffset;
	double wallX, step, texPos;
	Uint32 color;
	Uint8 r, g, b;
    int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
    int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;

    if (drawStart < 0) drawStart = 0;
    drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
    if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

    texNum = rcState->map[mapX][mapY] - 1;
    if (texNum < 0 || texNum >= 8) texNum = 0;
    if (side == 0) wallX = rcState->posY + perpWallDist * rayDirY;
    else           wallX = rcState->posX + perpWallDist * rayDirX;
    wallX -= floor(wallX);

    texX = (int)(wallX * (double)TEXWIDTH);
    if (side == 0 && rayDirX > 0) texX = TEXWIDTH - texX - 1;
    if (side == 1 && rayDirY < 0) texX = TEXWIDTH - texX - 1;

    step = 1.0 * TEXHEIGHT / lineHeight;
    texPos = (drawStart - SCREEN_HEIGHT / 2 + lineHeight / 2) * step;
//   texOffset = texX * TEXHEIGHT;
    for (y = drawStart; y < drawEnd; y++) {
	texY = (int)texPos & (TEXHEIGHT - 1);
	texPos += step;
	
	color = rcState->texture[texNum][texY * TEXWIDTH + texX];
	r = color & 0xFF;
	g = (color >> 8) & 0xFF;
	b = (color >> 16) & 0xFF;
	if (side == 1)
	{
		r = (Uint8)(r * 0.7);
		g = (Uint8)(g * 0.7);
		b = (Uint8)(b * 0.7);
	}
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderDrawPoint(renderer, x, y);
    }
}*/

void render(SDLState* sdlState, RaycasterState* rcState) {
SDL_SetRenderDrawColor(sdlState->renderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlState->renderer);

    renderFloorAndCeiling(sdlState->renderer, rcState);

    // Wall rendering
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        double cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
        double rayDirX = rcState->dirX + rcState->planeX * cameraX;
        double rayDirY = rcState->dirY + rcState->planeY * cameraX;

        int mapX = (int)rcState->posX;
        int mapY = (int)rcState->posY;

        double sideDistX, sideDistY;
        double deltaDistX = fabs(1 / rayDirX);
        double deltaDistY = fabs(1 / rayDirY);
        double perpWallDist;

        int stepX, stepY;
        int hit = 0;
        int side;

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (rcState->posX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - rcState->posX) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (rcState->posY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - rcState->posY) * deltaDistY;
        }

        while (hit == 0) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (rcState->map[mapX][mapY] > 0) hit = 1;
        }

        if (side == 0) perpWallDist = (mapX - rcState->posX + (1 - stepX) / 2) / rayDirX;
        else           perpWallDist = (mapY - rcState->posY + (1 - stepY) / 2) / rayDirY;

        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

        int texNum = rcState->map[mapX][mapY] - 1;
        double wallX;
        if (side == 0) wallX = rcState->posY + perpWallDist * rayDirY;
        else           wallX = rcState->posX + perpWallDist * rayDirX;
        wallX -= floor(wallX);

        int texX = (int)(wallX * (double)TEXWIDTH);
        if (side == 0 && rayDirX > 0) texX = TEXWIDTH - texX - 1;
        if (side == 1 && rayDirY < 0) texX = TEXWIDTH - texX - 1;

        double step = 1.0 * TEXHEIGHT / lineHeight;
        double texPos = (drawStart - SCREEN_HEIGHT / 2 + lineHeight / 2) * step;

        for (int y = drawStart; y < drawEnd; y++) {
            int texY = (int)texPos & (TEXHEIGHT - 1);
            texPos += step;
            Uint32 color = rcState->texture[texNum][TEXHEIGHT * texY + texX];
            if (side == 1) color = (color >> 1) & 8355711; // Make side walls darker
            SDL_SetRenderDrawColor(sdlState->renderer, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 255);
            SDL_RenderDrawPoint(sdlState->renderer, x, y);
        }
    }

    if (rcState->toggleMap)
        drawMiniMap(sdlState->renderer, rcState);

    SDL_RenderPresent(sdlState->renderer);
}
