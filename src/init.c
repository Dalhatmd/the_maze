#include "ray.h"
/**
 * initSDL - initializes SDL and creates a window and renderer
 * Return: SDLState* - a pointer to the SDLState struct
 */
SDLState* initSDL(void)
{
	SDLState* state = malloc(sizeof(SDLState));

	if (!state) return NULL;
	SDL_Init(SDL_INIT_VIDEO);
	state->window = SDL_CreateWindow("Simple Raycaster",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	state->renderer = SDL_CreateRenderer(state->window, -1,
			SDL_RENDERER_ACCELERATED);

    return state;
}
/**
 * cleanupSDL - frees the SDLState struct
 * @state: SDLState* - a pointer to the SDLState struct
 * Return: RaycasterState* - a pointer to the RaycasterState struct
 */
void cleanupSDL(SDLState* state) {
    if (state) {
        SDL_DestroyRenderer(state->renderer);
        SDL_DestroyWindow(state->window);
        SDL_Quit();
        free(state);
    }
}


/**
 * cleanupRaycaster - frees the RaycasterState struct
 * @state: RaycasterState* - a pointer to the RaycasterState struct
 */
void cleanupRaycaster(RaycasterState* state)
{
    free(state);
}
