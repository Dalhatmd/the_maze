#include "ray.h"
#include <math.h>

SDL_Texture* createTextureFromFloorData(SDL_Renderer* renderer, float startX, float startY, float stepX, float stepY, int width, RaycasterState* rcState) {
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, 1);
    Uint32* pixels = NULL;
    int pitch;

    SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);

    float floorX = startX;
    float floorY = startY;

    for (int x = 0; x < width; x++) {
        int cellX = (int)(floorX * FLOOR_MAGNIFICATION) & (FLOOR_TEXTURE_SIZE - 1);
        int cellY = (int)(floorY * FLOOR_MAGNIFICATION) & (FLOOR_TEXTURE_SIZE - 1);

        Uint32 color = rcState->floorTexture[FLOOR_TEXTURE_SIZE * cellY + cellX];
        float darkness = 1.0f - (float)x / width;  // Adjust this for desired lighting effect
        Uint8 r = ((color >> 16) & 0xFF) * darkness;
        Uint8 g = ((color >> 8) & 0xFF) * darkness;
        Uint8 b = (color & 0xFF) * darkness;

        pixels[x] = (r << 24) | (g << 16) | (b << 8) | 0xFF;

        floorX += stepX;
        floorY += stepY;
    }

    SDL_UnlockTexture(texture);
    return texture;
}

SDL_Texture* createTextureFromCeilingData(SDL_Renderer* renderer, float startX, float startY, float stepX, float stepY, int width, RaycasterState* rcState) {
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, 1);
    Uint32* pixels = NULL;
    int pitch;

    SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);

    float ceilingX = startX;
    float ceilingY = startY;

    for (int x = 0; x < width; x++) {
        int cellX = (int)(ceilingX * CEILING_MAGNIFICATION) & (CEILING_TEXTURE_SIZE - 1);
        int cellY = (int)(ceilingY * CEILING_MAGNIFICATION) & (CEILING_TEXTURE_SIZE - 1);

        Uint32 color = rcState->ceilingTexture[CEILING_TEXTURE_SIZE * cellY + cellX];
        float darkness = 0.5f + ((float)x / width) * 0.5f;  // Adjust this for desired lighting effect
        Uint8 r = ((color >> 16) & 0xFF) * darkness;
        Uint8 g = ((color >> 8) & 0xFF) * darkness;
        Uint8 b = (color & 0xFF) * darkness;

        pixels[x] = (r << 24) | (g << 16) | (b << 8) | 0xFF;

        ceilingX += stepX;
        ceilingY += stepY;
    }

    SDL_UnlockTexture(texture);
    return texture;
}

