#include "ray.h"

void textures_init(RaycasterState *rcState, SDL_Renderer *renderer)
{
    int i;
    const char *texture_files[NUM_TEXTURES] = {
        "textures/eagle.png",
        "textures/redbrick.png",
        "textures/purplestone.png",
        "textures/greystone.png",
        "textures/bluestone.png",
        "textures/mossy.png",
        "textures/wood.png",
        "textures/colorstone.png"
    };

    for (i = 0; i < NUM_TEXTURES; i++)
    {
        SDL_Surface *surface = IMG_Load(texture_files[i]);
        if (surface == NULL)
        {
            printf("Unable to load texture image %s! SDL_image Error: %s\n", texture_files[i], IMG_GetError());
            continue;
        }

        rcState->wallTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        if (rcState->wallTextures[i] == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", texture_files[i], SDL_GetError());
        }

        SDL_FreeSurface(surface);
    }

    // Load floor texture
    SDL_Surface *floorSurface = IMG_Load("textures/greystone.png");
    if (floorSurface == NULL)
    {
        printf("Unable to load floor texture image! SDL_image Error: %s\n", IMG_GetError());
    }
    else
    {
        rcState->floorTexture = SDL_CreateTextureFromSurface(renderer, floorSurface);
        SDL_FreeSurface(floorSurface);
    }

    // Load ceiling texture
    SDL_Surface *ceilingSurface = IMG_Load("textures/wood.png");
    if (ceilingSurface == NULL)
    {
        printf("Unable to load ceiling texture image! SDL_image Error: %s\n", IMG_GetError());
    }
    else
    {
        rcState->ceilingTexture = SDL_CreateTextureFromSurface(renderer, ceilingSurface);
        SDL_FreeSurface(ceilingSurface);
    }
}

