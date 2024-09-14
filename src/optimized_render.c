#include "ray.h"
#include <math.h>

void renderFloorAndCeiling(SDL_Renderer* renderer, RaycasterState* rcState)
{
	int w = SCREEN_WIDTH / RENDER_SCALE;
    int h = SCREEN_HEIGHT / RENDER_SCALE;

    SDL_SetRenderTarget(renderer, rcState->textures.renderTexture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    float posZ = 0.5f * SCREEN_HEIGHT;

    for (int y = 0; y < h; y++) {
        float rayDirX0 = rcState->dirX - rcState->planeX;
        float rayDirY0 = rcState->dirY - rcState->planeY;
        float rayDirX1 = rcState->dirX + rcState->planeX;
        float rayDirY1 = rcState->dirY + rcState->planeY;

        int p = y - h / 2;
        float rowDistance = posZ / p;

        float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / w;
        float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / w;

        float floorX = rcState->posX + rowDistance * rayDirX0;
        float floorY = rcState->posY + rowDistance * rayDirY0;

        SDL_Texture* floorTexture = createTextureFromFloorData(renderer, floorX, floorY, floorStepX, floorStepY, w, rcState);
        SDL_Texture* ceilingTexture = createTextureFromCeilingData(renderer, floorX, floorY, floorStepX, floorStepY, w, rcState);

        SDL_Rect dstRect = {0, y, w, 1};
        SDL_RenderCopy(renderer, floorTexture, NULL, &dstRect);

        dstRect.y = h - y - 1;
        SDL_RenderCopy(renderer, ceilingTexture, NULL, &dstRect);

        SDL_DestroyTexture(floorTexture);
        SDL_DestroyTexture(ceilingTexture);
    }

    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, rcState->textures.renderTexture, NULL, NULL);
}

