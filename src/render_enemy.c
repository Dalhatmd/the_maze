#include "ray.h"

void renderEnemy(SDL_Renderer *renderer, RaycasterState *state) {
    if (!state->boss.isAlive) return;

    // Calculate enemy position relative to player
    double spriteX = state->boss.posX - state->posX;
    double spriteY = state->boss.posY - state->posY;

    // Transform sprite with the inverse camera matrix
    double invDet = 1.0 / (state->planeX * state->dirY - state->dirX * state->planeY);
    double transformX = invDet * (state->dirY * spriteX - state->dirX * spriteY);
    double transformY = invDet * (-state->planeY * spriteX + state->planeX * spriteY);

    int spriteScreenX = (int)((SCREEN_WIDTH / 2) * (1 + transformX / transformY));

    // Calculate sprite dimensions on screen
    int spriteHeight = abs((int)(SCREEN_HEIGHT / transformY));
    int drawStartY = -spriteHeight / 2 + SCREEN_HEIGHT / 2;
    if (drawStartY < 0) drawStartY = 0;
    int drawEndY = spriteHeight / 2 + SCREEN_HEIGHT / 2;
    if (drawEndY >= SCREEN_HEIGHT) drawEndY = SCREEN_HEIGHT - 1;

    int spriteWidth = abs((int)(SCREEN_HEIGHT / transformY));
    int drawStartX = -spriteWidth / 2 + spriteScreenX;
    if (drawStartX < 0) drawStartX = 0;
    int drawEndX = spriteWidth / 2 + spriteScreenX;
    if (drawEndX >= SCREEN_WIDTH) drawEndX = SCREEN_WIDTH - 1;

    // Loop through every vertical stripe of the sprite on screen
    for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
        int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * TEXWIDTH / spriteWidth) / 256;
        if (transformY > 0 && stripe > 0 && stripe < SCREEN_WIDTH && transformY < state->zBuffer[stripe]) {
            for (int y = drawStartY; y < drawEndY; y++) {
                int d = (y) * 256 - SCREEN_HEIGHT * 128 + spriteHeight * 128;
                int texY = ((d * TEXHEIGHT) / spriteHeight) / 256;
                Uint32 color = state->textures[state->boss.textureId][TEXWIDTH * texY + texX];
                // Here we're assuming the color 0 is transparent
                if ((color & 0x00FFFFFF) != 0) {
                    SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 255);
                    SDL_RenderDrawPoint(renderer, stripe, y);
                }
            }
        }
    }
}
