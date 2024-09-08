#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "ray.h"
/**
 * main - Entry point
 *
 * Return: Always 0
 */
int main(int argc, char *argv[])
{
	SDLState* sdlState = initSDL();
	if (!sdlState)
	{
		fprintf(stderr, "Failed to initialize SDL\n");
		return 1;
	}

	RaycasterState* rcState = parseMapFile(argv[1]);
	if (!rcState)
	{
		fprintf(stderr, "Failed to parse map file\n");
		cleanupSDL(sdlState);
		return 1;
	}
	textures_init(rcState);

	int running = 1;
	SDL_Event event;
	Uint32 frameStart, frameTime;
	double deltaTime = 0.0;

	while (running)
	{
		frameStart = SDL_GetTicks();

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				running = 0;
		handleInput(&event, rcState);
		}
		updatePosition(rcState, frameTime);
		render(sdlState, rcState);
		frameTime = SDL_GetTicks() - frameStart;

		if (frameTime < FRAME_TARGET_TIME)
		{
			SDL_Delay((int)(FRAME_TARGET_TIME - frameTime));
		}
		deltaTime = (SDL_GetTicks() - frameStart) / 1000.0;
//		printf("FPS: %f\n", 1.0 / deltaTime);
	}

	cleanupRaycaster(rcState);
	cleanupSDL(sdlState);
	return (0);
}
