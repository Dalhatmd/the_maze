#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>

#define MAX_GUNS 3
#define GUN_TEXTURE_HEIGHT 256
#define GUN_TEXTURE_WIDTH 256
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


/**
 * Enemy - A struct that handles the enemy in the game
 *
 * @posX: x coordinate of the enemy
 * @posY: y coordinate of enemy
 * @textureId: number that represents the texture of the enemy
 * @dirX: X direction of enemy
 * @dirY: y direction of enemy
 * @isAlive: boolean value that holds information about the enemy's life
 */
typedef struct
{
	double posX, posY;
	int textureId;
	double dirX, dirY;
	bool isAlive;
} Enemy;

/**
 * struct Gun - struct that holds weapon functionality
 * @damage: amount of damage a the gun does
 * @fireRate: how fast the gun fires
 * @lastFireTime: time from last shot
 */
typedef struct
{
	int textureId;
	int damage;
	double fireRate;
	double lastFireTime;
} Gun;

typedef struct {
	int map[MAP_WIDTH][MAP_HEIGHT];
	double posX, posY;
	double dirX, dirY;
	double planeX, planeY;
	bool moveForward, moveBackward;
	bool strafeLeft, strafeRight;
	bool rotateLeft, rotateRight;
	bool toggleMap;
	bool isShooting;
	int textures[9][TEXWIDTH * TEXHEIGHT];
	int gunTextures[MAX_GUNS][GUN_TEXTURE_HEIGHT * GUN_TEXTURE_WIDTH];
	int floorTexture[FLOOR_TEXTURE_SIZE * FLOOR_TEXTURE_SIZE];
	int ceilingTexture[CEILING_TEXTURE_SIZE * CEILING_TEXTURE_SIZE];
	Enemy boss;
	double zBuffer[SCREEN_WIDTH];
	Gun guns[MAX_GUNS];
	int currentGun;
} RaycasterState;

/**
 * SDLState - Structs that handles the SDL instance
 *
 * @window: SDL window
 * @renderer; SDL renderer
 */
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
void initGuns(RaycasterState *state);
void renderGun(SDL_Renderer *renderer, RaycasterState *state);
void handleShooting(RaycasterState *state);
#endif // RAYCASTER_H

