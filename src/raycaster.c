#include "ray.h"

static void calculateRayPosition(int x, RaycasterState* state, double* rayDirX, double* rayDirY) {
    double cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
    *rayDirX = state->dirX + state->planeX * cameraX;
    *rayDirY = state->dirY + state->planeY * cameraX;
}

static void performDDA(RaycasterState* state, double rayDirX, double rayDirY, int* mapX, int* mapY, double* perpWallDist, int* side) {
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

static void drawWall(SDL_Renderer* renderer, int x, double perpWallDist, int side) {
    int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
    int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
    if (drawStart < 0) drawStart = 0;
    int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
    if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    if (side == 1) SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
}

void render(SDLState* sdlState, RaycasterState* rcState) {
    SDL_SetRenderDrawColor(sdlState->renderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlState->renderer);

    for (int x = 0; x < SCREEN_WIDTH; x++) {
        double rayDirX, rayDirY;
        calculateRayPosition(x, rcState, &rayDirX, &rayDirY);

        int mapX, mapY, side;
        double perpWallDist;
        performDDA(rcState, rayDirX, rayDirY, &mapX, &mapY, &perpWallDist, &side);

        drawWall(sdlState->renderer, x, perpWallDist, side);
    }

    SDL_RenderPresent(sdlState->renderer);
}
