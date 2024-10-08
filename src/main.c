#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "ray.h"
#include "enemy.h"
/**
 * main - Entry point
 *
 * @argc: number of command line arguments
 * @argv: array of command line arguments
 * Return: Always 0
 */
int main(int argc, char *argv[])
{
	(void)argc;
	SDLState *sdlState = initSDL();
	if (!sdlState)
	{
		fprintf(stderr, "Failed to initialize SDL\n");
		return 1;
	}

	int menuChoice;
	bool gameRunning = true;

	while (gameRunning)
	{
		menuChoice = showMenu(sdlState->renderer);

		switch (menuChoice)
		{
			case 0: // Start Game
			{
				RaycasterState *rcState = parseMapFile(argv[1], sdlState);
				if (!rcState)
				{
					fprintf(stderr, "Failed to parse map file\n");
					cleanupSDL(sdlState);
					return 1;
				}

				initEnemy(rcState);
				textures_init(rcState);

				int running = 1;

				Uint32 frameStart, frameTime;
				frameTime = 0;
				SDL_Event event;

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
					updateEnemy(rcState); 
					render(sdlState, rcState);
					renderEnemy(sdlState->renderer, rcState);

					frameTime = SDL_GetTicks() - frameStart;
					if (frameTime < FRAME_TARGET_TIME)
					{
						SDL_Delay((int)(FRAME_TARGET_TIME - frameTime));
					}
//					double deltaTime = (SDL_GetTicks() - frameStart) / 1000.0;
				}

				cleanupRaycaster(rcState);
				break;
			}
			case 1:
				/*TODO:*Implement options menu here*/
				break;
			case 2:
			case -1:
				gameRunning = false;
				break;
		}
	}

	cleanupSDL(sdlState);
	return 0;
}
