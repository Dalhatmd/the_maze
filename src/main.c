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
                Enemy enemy;
                RaycasterState *rcState = parseMapFile(argv[1], sdlState, &enemy);
                if (!rcState)
                {
                    fprintf(stderr, "Failed to parse map file\n");
                    cleanupSDL(sdlState);
                    return 1;
                }

                initEnemy(rcState, &enemy);
                textures_init(rcState);
                initGuns(rcState);

                int running = 1;
                double deltaTime = 0.0;
                Uint32 frameStart, frameTime;
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
                    updateEnemy(rcState, &enemy); 
                    render(sdlState, rcState);
                    renderEnemy(sdlState->renderer, rcState, &enemy);

                    frameTime = SDL_GetTicks() - frameStart;
                    if (frameTime < FRAME_TARGET_TIME)
                    {
                        SDL_Delay((int)(FRAME_TARGET_TIME - frameTime));
                    }
                    deltaTime = (SDL_GetTicks() - frameStart) / 1000.0;
                }

                cleanupRaycaster(rcState);
                break;
            }
            case 1: // Options
                // Implement options menu here
                break;
            case 2: // Quit
            case -1: // SDL_QUIT event
                gameRunning = false;
                break;
        }
    }

    cleanupSDL(sdlState);
    return 0;
}
