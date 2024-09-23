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
		"textures/colorstone.png",
		"textures/barrel.png"
	};

	for (i = 0; i < NUM_TEXTURES; i++)
	{
		SDL_Surface *surface = IMG_Load(texture_files[i]);
		if (surface == NULL)
		{
			printf("Unable to load texture image %s! SDL_image Error: %s\n", texture_files[i], IMG_GetError());
			exit(1);
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
	const char *floor_texture = "textures/greystone.png";
	SDL_Surface *floor_surface = IMG_Load(floor_texture);
	if (floor_surface == NULL)
	{
		printf("Unable to load floor texture image %s! SDL_image Error: %s\n", floor_texture, IMG_GetError());
		exit(1);
	}
	SDL_Surface *formatted_floor_surface = SDL_ConvertSurfaceFormat(floor_surface, SDL_PIXELFORMAT_ABGR8888, 0);
	SDL_FreeSurface(floor_surface);
	if (formatted_floor_surface == NULL)
	{
		printf("Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_LockSurface(formatted_floor_surface);
	Uint32 *floorPixels = (Uint32 *)formatted_floor_surface->pixels;
	for (y = 0; y < FLOOR_TEXTURE_SIZE; y++)
	{
		for (x = 0; x < TEXWIDTH; x++)
		{
			pixel = floorPixels[y * FLOOR_TEXTURE_SIZE + x];
			rcState->floorTexture[y * FLOOR_TEXTURE_SIZE + x] = pixel;
		}
	}
	SDL_UnlockSurface(formatted_floor_surface);
	SDL_FreeSurface(formatted_floor_surface);

	const char *ceiling_texture = "textures/wood.png";
	SDL_Surface *ceiling_surface = IMG_Load(ceiling_texture);
	if (ceiling_surface == NULL)
	{
		printf("Unable to load ceiling texture image %s! SDL_image Error: %s\n", ceiling_texture, IMG_GetError());
		exit(1);
	}

	if (ceiling_surface->w != CEILING_TEXTURE_SIZE || ceiling_surface->h != CEILING_TEXTURE_SIZE)
	{
		printf("Ceiling texture must be %dx%d\n", CEILING_TEXTURE_SIZE, CEILING_TEXTURE_SIZE);
		exit(1);
	}
	SDL_Surface *formatted_ceiling_surface = SDL_ConvertSurfaceFormat(ceiling_surface, SDL_PIXELFORMAT_ABGR8888, 0);
	SDL_FreeSurface(ceiling_surface);

	if (formatted_ceiling_surface == NULL)
	{
		printf("Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_LockSurface(formatted_ceiling_surface);
	Uint32 *ceilingPixels = (Uint32 *)formatted_ceiling_surface->pixels;
	for (y = 0; y < CEILING_TEXTURE_SIZE; y++)
	{
		for (x = 0; x < CEILING_TEXTURE_SIZE; x++)
		{
			pixel = ceilingPixels[y * CEILING_TEXTURE_SIZE + x];
			rcState->ceilingTexture[y * CEILING_TEXTURE_SIZE + x] = pixel;
		}
	}
	SDL_UnlockSurface(formatted_ceiling_surface);
	SDL_FreeSurface(formatted_ceiling_surface);

	IMG_Quit();
}
