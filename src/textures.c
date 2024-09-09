#include "ray.h"

void textures_init(RaycasterState *rcState)
{
	int i, x, y;
	Uint32 pixel;
	int imgFlags = IMG_INIT_PNG;

	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		exit(1);
	}

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
	
	SDL_Surface *formattedSureface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, 0);
	SDL_FreeSurface(surface);

	if (formattedSureface == NULL)
	{
		printf("Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_LockSurface(formattedSureface);
	Uint32 *pixels = (Uint32 *)formattedSureface->pixels;
	for (y = 0; y < TEXHEIGHT; y++)
	{
		for (x = 0; x < TEXWIDTH; x++)
		{
			pixel = pixels[y * TEXWIDTH + x];
			rcState->textures[i][y * TEXHEIGHT + x] = pixel;
		}
	}
	SDL_UnlockSurface(formattedSureface);
	SDL_FreeSurface(formattedSureface);
	}
	IMG_Quit();
}
