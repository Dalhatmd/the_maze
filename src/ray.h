#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAP_WIDTH 21
#define MAP_HEIGHT 21
#define FPS 60
#define FRAME_TARGET_TIME 1000 / FPS
#define MOVE_SPEED  0.5
#define ROTATE_SPEED  0.5

typedef struct {
    int map[MAP_WIDTH][MAP_HEIGHT];
    double posX, posY;
    double dirX, dirY;
    double planeX, planeY;
	bool moveForward, moveBackward;
	bool rotateLeft, rotateRight;
} RaycasterState;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
} SDLState;

SDLState* initSDL(void);
void cleanupSDL(SDLState* state);
RaycasterState* initRaycaster(void);
void cleanupRaycaster(RaycasterState* state);
void render(SDLState* sdlState, RaycasterState* rcState);
void handleInput(SDL_Event *event, RaycasterState *state);
void updatePosition(RaycasterState *state, double deltaTime);

#endif // RAYCASTER_H

