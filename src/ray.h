#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>

#define CEILING_TEXTURE_SIZE 64
#define CEILING_MAGNIFICATION 8
#define FLOOR_TEXTURE_SIZE 64
#define FLOOR_MAGNIFICATION 8
#define NUM_TEXTURES 9
#define TEXWIDTH 64
#define CEILING_TEXTURE_SIZE 64
#define CEILING_MAGNIFICATION 8
#define TEXHEIGHT 64
#define MINIMAP_SIZE (MAP_WIDTH * MINIMAP_SCALE)
#define MINIMAP_SCALE 5
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAP_WIDTH 21
#define MAP_HEIGHT 21
#define FPS 60
#define FRAME_TARGET_TIME 1000 / FPS
#define MOVE_SPEED  0.5
#define ROTATE_SPEED  0.5

typedef struct
{
	double posX, posY;
	int textureId;
	double dirX, dirY;
	bool isAlive;
} Enemy;


typedef struct {
    int map[MAP_WIDTH][MAP_HEIGHT];
    double posX, posY;
    double dirX, dirY;
    double planeX, planeY;
	bool moveForward, moveBackward;
	bool strafeLeft, strafeRight;
	bool rotateLeft, rotateRight;
	bool toggleMap;
	 int textures[9][TEXWIDTH * TEXHEIGHT];
	 int floorTexture[FLOOR_TEXTURE_SIZE * FLOOR_TEXTURE_SIZE];
	 int ceilingTexture[CEILING_TEXTURE_SIZE * CEILING_TEXTURE_SIZE];
	 Enemy boss;
	 double zBuffer[SCREEN_WIDTH];
} RaycasterState;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
   
} SDLState;



void initEnemy(RaycasterState *state);
void renderEnemy(SDL_Renderer *renderer, RaycasterState *state);

SDLState* initSDL(void);
void cleanupSDL(SDLState* state);
RaycasterState* initRaycaster(void);
void cleanupRaycaster(RaycasterState* state);
void render(SDLState* sdlState, RaycasterState* rcState);
void handleInput(SDL_Event *event, RaycasterState *state);
void updatePosition(RaycasterState *state, double deltaTime);
void drawMiniMap(SDL_Renderer *renderer, RaycasterState *state);
void textures_init(RaycasterState *rcState);
RaycasterState* parseMapFile(const char *filename);
void drawFloor(SDL_Renderer *renderer, RaycasterState *state);
void drawCeiling(SDL_Renderer *renderer, RaycasterState *state);
#endif // RAYCASTER_H

