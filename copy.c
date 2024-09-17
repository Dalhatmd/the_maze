#include "ray.h"

void drawCeiling(SDL_Renderer *renderer, RaycasterState *rcState)
{
	float posZ, rayDirX0, rayDirY0, rayDirX1, rayDirY1, rowDistance;
	float floorStepX, floorStepY, floorX, floorY;
	int y, p, x, cellX, cellY, tx, ty;

	posZ = 0.5 * SCREEN_HEIGHT;
	for (y = 0; y < SCREEN_HEIGHT / 2; y++)
	{
		rayDirX0 = rcState->dirX - rcState->planeX;
		rayDirY0 = rcState->dirY - rcState->planeY;
		rayDirX1 = rcState->dirX + rcState->planeX;
		rayDirY1 = rcState->dirY + rcState->planeY;
		
		p = SCREEN_HEIGHT / 2 - y;
		rowDistance = posZ / p;

		floorStepX = rowDistance * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
		floorStepY = rowDistance * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;

		floorX = rcState->posX + rowDistance * rayDirX0;
		floorY = rcState->posY + rowDistance * rayDirY0;
		
		for (x = 0; x < SCREEN_WIDTH; x++)
		{
			cellX = (int)(floorX * CEILING_MAGNIFICATION) & (CEILING_TEXTURE_SIZE - 1);
			cellY = (int)(floorY * CEILING_MAGNIFICATION) & (CEILING_TEXTURE_SIZE - 1);


			floorX += floorStepX;
			floorY += floorStepY;

		int ceilingPixel = rcState->ceilingTexture[cellX + cellY * CEILING_TEXTURE_SIZE];
		Uint8 r = ceilingPixel & 0xFF;
		Uint8 g = (ceilingPixel >> 8) & 0xFF;
		Uint8 b = (ceilingPixel >> 16) & 0xFF;

		float darkness = 0.5 + ((float)y / (float)(SCREEN_HEIGHT));
		r = (Uint8)(r * darkness);
		g = (Uint8)(g * darkness);
		b = (Uint8)(b * darkness);

		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		SDL_RenderDrawPoint(renderer, x, y);
		

		}
	}
}
#include "ray.h"

void drawFloor(SDL_Renderer *renderer, RaycasterState *rcstate)
{
	int y, p, x;
	float rayDirX0, rayDirY0, rayDirX1, rayDirY1, posZ, rowDistance;
	float floorStepX, floorStepY, floorX, floorY;

	posZ = 0.5 * SCREEN_HEIGHT;

	for (y = SCREEN_HEIGHT / 2; y < SCREEN_HEIGHT; y++)
	{
		rayDirX0 = rcstate->dirX - rcstate->planeX;
		rayDirY0 = rcstate->dirY - rcstate->planeY;
		rayDirX1 = rcstate->dirX + rcstate->planeX;
		rayDirY1 = rcstate->dirY + rcstate->planeY;

		p = y - SCREEN_HEIGHT / 2;
		rowDistance = posZ / p;

		floorStepX = rowDistance * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
		floorStepY = rowDistance * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;

		floorX = rcstate->posX + rowDistance * rayDirX0;
		floorY = rcstate->posY + rowDistance * rayDirY0;

		for (x = 0; x < SCREEN_WIDTH; ++x)
		{
			int cellX = (int)(floorX * FLOOR_MAGNIFICATION) & (FLOOR_TEXTURE_SIZE - 1);
			int cellY = (int)(floorY * FLOOR_MAGNIFICATION) & (FLOOR_TEXTURE_SIZE - 1);


			floorX += floorStepX;
			floorY += floorStepY;

			int floorTexture = rcstate->floorTexture[FLOOR_TEXTURE_SIZE * cellY + cellX];
			Uint8 r = floorTexture & 0XFF;
			Uint8 g = (floorTexture >> 8) & 0XFF;
			Uint8 b = (floorTexture >> 16) & 0XFF;
			
			float darkness = 1.0 - (float)y / SCREEN_HEIGHT;
			r = (Uint8)(r * darkness);
			g = (Uint8)(g * darkness);
			b = (Uint8)(b * darkness);

			SDL_SetRenderDrawColor(renderer, r, g, b, 255);
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}
}
#include "ray.h"
/**
 * initSDL - initializes SDL and creates a window and renderer
 * Return: SDLState* - a pointer to the SDLState struct
 */
SDLState *initSDL(void)
{
	SDLState *state = malloc(sizeof(SDLState));

	if (!state)
		return (NULL);
	SDL_Init(SDL_INIT_VIDEO);
	state->window = SDL_CreateWindow("Simple Raycaster",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	state->renderer = SDL_CreateRenderer(state->window, -1,
			SDL_RENDERER_ACCELERATED);

	return (state);
}
/**
 * cleanupSDL - frees the SDLState struct
 * @state: SDLState* - a pointer to the SDLState struct
 * Return: RaycasterState* - a pointer to the RaycasterState struct
 */
void cleanupSDL(SDLState *state)
{
	if (state)
	{
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
void cleanupRaycaster(RaycasterState *state)
{
	free(state);
}
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "ray.h"
/**
 * main - Entry point
 *
 * @argc: number of command line arguments
 * @argv: array of command line arguments
 * Return: Always 0
 */
int main(int argc, char *argv[])
{
	int running;
	double deltaTime;
	Uint32 frameStart, frameTime;
	SDL_Event event;
	(void)argc;

	SDLState *sdlState = initSDL();

	if (!sdlState)
	{
		fprintf(stderr, "Failed to initialize SDL\n");
		return (1);
	}

	RaycasterState *rcState = parseMapFile(argv[1]);

	if (!rcState)
	{
		fprintf(stderr, "Failed to parse map file\n");
		cleanupSDL(sdlState);
		return (1);
	}
	textures_init(rcState);

	running = 1;
	deltaTime = 0.0;

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
		printf("FPS: %f\n", 1.0 / deltaTime);
	}

	cleanupRaycaster(rcState);
	cleanupSDL(sdlState);
	return (0);
}
#include "ray.h"
/**
 * drawMiniMap - draws the minimap
 * @renderer: the renderer
 * @state: the raycaster state
 */
void drawMiniMap(SDL_Renderer *renderer, RaycasterState *state)
{
	int minimapX = SCREEN_WIDTH - MINIMAP_SIZE - 10;
	int minimapY = 10;
	int y, x, dirX, dirY;

	// Draw the minimap
	for (y = 0; y < MAP_WIDTH; y++)
	{
		for (x = 0; x < MAP_WIDTH; x++)
		{
			SDL_Rect rect = {
				minimapX + x * MINIMAP_SCALE,
				minimapY + y * MINIMAP_SCALE,
				MINIMAP_SCALE,
				MINIMAP_SCALE
			};
			if (state->map[x][y] != 0)
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			}
			SDL_RenderFillRect(renderer, &rect);
		}
	}

	// Draw the player
	int playerX = minimapX + (int)(state->posX * MINIMAP_SCALE);
	int playerY = minimapY + (int)(state->posY * MINIMAP_SCALE);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_Rect playerRect = {
		playerX,
		playerY ,
		4,
		4
	};
	SDL_RenderFillRect(renderer, &playerRect);
	dirX = playerX + (int)(state->dirX * MINIMAP_SCALE);
	dirY = playerY + (int)(state->dirY * MINIMAP_SCALE);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderDrawLine(renderer, playerX, playerY, dirX, dirY);
}
#include "ray.h"
/** 
 * handleInput - handles input 
 *
 * @event: SDL event 
 * @state: state of the game struct
 */
void handleInput(SDL_Event *event, RaycasterState *state)
{
	if (event->type == SDL_KEYDOWN)
	{
		switch (event->key.keysym.sym)
		{
		case SDLK_w:
			state->moveForward = true;
			break;
		case SDLK_s:
			state->moveBackward = true;
			break;
		case SDLK_a:
			state->strafeLeft = true;
			break;
		case SDLK_d:
			state->strafeRight = true;
			break;
		case SDLK_LEFT:
			state->rotateLeft = true;
			break;
		case SDLK_RIGHT:
			state->rotateRight = true;
			break;
		case SDLK_m:
			state->toggleMap = !state->toggleMap;
			break;
		}
	}
	else if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
		case SDLK_w:
			state->moveForward = false;
			break;
		case SDLK_s:
			state->moveBackward = false;
			break;
		case SDLK_a:
			state->strafeLeft = false;
			break;
		case SDLK_d:
			state->strafeRight = false;
			break;
		case SDLK_LEFT:
			state->rotateLeft = false;
			break;
		case SDLK_RIGHT:
			state->rotateRight = false;
			break;
		}
	}
}
/**
 * updatePosition - handles the updated position
 *
 * @state: game state struct
 * @deltaTime: delta time
 */
void updatePosition(RaycasterState *state, double deltaTime)
{
	double moveSpeed, rotSpeed, speedMultiplier;
	speedMultiplier = 0.005;

	moveSpeed = MOVE_SPEED * deltaTime * speedMultiplier;
	rotSpeed = ROTATE_SPEED *+ deltaTime * speedMultiplier;

	if(state->moveForward)
	{
		if(state->map[(int)(state->posX + state->dirX * moveSpeed)][(int)state->posY] == 0)
			state->posX += state->dirX * moveSpeed;
		if(state->map[(int)state->posX][(int)(state->posY + state->dirY * moveSpeed)] == 0)
			state->posY += state->dirY * moveSpeed;
	}
	if(state->moveBackward)
	{
		if(state->map[(int)(state->posX - state->dirX * moveSpeed)][(int)state->posY] == 0)
			state->posX -= state->dirX * moveSpeed;
		if(state->map[(int)state->posX][(int)(state->posY - state->dirY * moveSpeed)] == 0)
			state->posY -= state->dirY * moveSpeed;
	}
	if(state->rotateLeft)
	{
		double oldDirX = state->dirX;
		state->dirX = state->dirX * cos(rotSpeed) - state->dirY * sin(rotSpeed);
		state->dirY = oldDirX * sin(rotSpeed) + state->dirY * cos(rotSpeed);
		double oldPlaneX = state->planeX;
		state->planeX = state->planeX * cos(rotSpeed) - state->planeY * sin(rotSpeed);
		state->planeY = oldPlaneX * sin(rotSpeed) + state->planeY * cos(rotSpeed);
	}
	if(state->rotateRight)
	{
		double oldDirX = state->dirX;
		state->dirX = state->dirX * cos(-rotSpeed) - state->dirY * sin(-rotSpeed);
		state->dirY = oldDirX * sin(-rotSpeed) + state->dirY * cos(-rotSpeed);
		double oldPlaneX = state->planeX;
		state->planeX = state->planeX * cos(-rotSpeed) - state->planeY * sin(-rotSpeed);
		state->planeY = oldPlaneX * sin(-rotSpeed) + state->planeY * cos(-rotSpeed);
	}
	if (state->strafeLeft)
	{
		if (state->map[(int)(state->posX - state->dirX * moveSpeed)][(int)state->posY] == 0)
			state->posX -= state->dirY * moveSpeed;
		if (state->map[(int)state->posX][(int)(state->posY + state->dirX * moveSpeed)] == 0)
			state->posY += state->dirX * moveSpeed;
	}
	if (state->strafeRight)
	{
		if (state->map[(int)(state->posX + state->dirX * moveSpeed)][(int)state->posY] == 0)
			state->posX += state->dirY * moveSpeed;
		if (state->map[(int)state->posX][(int)(state->posY - state->dirX * moveSpeed)] == 0)
			state->posY -= state->dirX * moveSpeed;
	}
}
#include "ray.h"
#include <string.h>

#define MAX_LINE_LENGTH 1024

RaycasterState* parseMapFile(const char *filename)
{
	char line[MAX_LINE_LENGTH];
	char *c = line;
	int i = 0;
	int row = 0, max_width = 0, col;
	FILE *file = fopen(filename, "r");
	if (!file)
	{
		printf("Error: Could not open file %s\n", filename);
		return NULL;
	}
	RaycasterState *state = malloc(sizeof(RaycasterState));
	if (!state)
	{
		printf("Error: Could not allocate memory for RaycasterState\n");
		fclose(file);
		return NULL;
	}

	while (fgets(line, sizeof(line), file) && row < MAP_HEIGHT)
	{
		col = 0;
		for (i = 0; line[i] != '\n' && line[i] != '\0' && col < MAP_WIDTH; i++)
		{
			switch (line[i])
			{
		case '0': state->map[col][row] = 0; break;
		case '1': state->map[col][row] = 1; break;

		case '2': state->map[col][row] = 2; break;
		case '3': state->map[col][row] = 3; break; // Special wall type 2
                case '4': state->map[col][row] = 4; break; // Special wall type 3
                case '5': state->map[col][row] = 5; break; // Special wall type 4
                case '6': state->map[col][row] = 6; break; // Special wall type 5
                case '7': state->map[col][row] = 7; break; // Special wall type 6
                case '8': state->map[col][row] = 8; break; // Special wall type 7
                case 'P': // Player start position
                    state->posX = col + 0.5;
                    state->posY = row + 0.5;
                    state->map[col][row] = 0; // Assume player starts on empty space
                    break;
                default:
                    fprintf(stderr, "Unknown character in map: %c\n", *c);
                    state->map[col][row] = 0; // Treat unknown as empty space
            }
            col++;
        }
        if (col > max_width) max_width = col;
        row++;
    }

    fclose(file);

     state->dirX = -1;
    state->dirY = 0;
    state->planeX = 0;
    state->planeY = 0.66;
    state->moveForward = false;
    state->moveBackward = false;
    state->rotateLeft = false;
    state->rotateRight = false;

    // Validate map dimensions
    if (row != MAP_HEIGHT || max_width != MAP_WIDTH) {
        fprintf(stderr, "Invalid map dimensions. Expected %dx%d, got %dx%d\n", 
                MAP_WIDTH, MAP_HEIGHT, max_width, row);
        free(state);
        return NULL;
    }

    return state;
}
#include "ray.h"

static void calculateRayPosition(int x, RaycasterState* state, double* rayDirX, double* rayDirY) {
    double cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
    *rayDirX = state->dirX + state->planeX * cameraX;
    *rayDirY = state->dirY + state->planeY * cameraX;
}

static void performDDA(RaycasterState* state, double rayDirX, double rayDirY, int* mapX, int* mapY, double *perpWallDist, int* side) {
    int stepX, stepY, hit = 0;
    double sideDistX, sideDistY;
    double deltaDistX = fabs(1 / rayDirX);
    double deltaDistY = fabs(1 / rayDirY);

    *mapX = (int)state->posX;
    *mapY = (int)state->posY;

    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (state->posX - *mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (*mapX + 1.0 - state->posX) * deltaDistX;
    }
    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (state->posY - *mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (*mapY + 1.0 - state->posY) * deltaDistY;
    }

    while (hit == 0) {
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            *mapX += stepX;
            *side = 0;
        } else {
            sideDistY += deltaDistY;
            *mapY += stepY;
            *side = 1;
        }
        if (state->map[*mapX][*mapY] > 0) hit = 1;
    }
    if (*side == 0) *perpWallDist = (*mapX - state->posX + (1 - stepX) / 2) / rayDirX;
    else *perpWallDist = (*mapY - state->posY + (1 - stepY) / 2) / rayDirY;

}

static void drawWall(SDL_Renderer* renderer, int x, double perpWallDist, int side, RaycasterState* rcState, double rayDirX, double rayDirY, int mapX, int mapY) {
	int drawEnd, texNum, texX, texY, y; //texOffset;
	double wallX, step, texPos;
	Uint32 color;
	Uint8 r, g, b;
    int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
    int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;

    if (drawStart < 0) drawStart = 0;
    drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
    if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

    texNum = rcState->map[mapX][mapY] - 1;
    if (texNum < 0 || texNum >= 8) texNum = 0;
    if (side == 0) wallX = rcState->posY + perpWallDist * rayDirY;
    else           wallX = rcState->posX + perpWallDist * rayDirX;
    wallX -= floor(wallX);

    texX = (int)(wallX * (double)TEXWIDTH);
    if (side == 0 && rayDirX > 0) texX = TEXWIDTH - texX - 1;
    if (side == 1 && rayDirY < 0) texX = TEXWIDTH - texX - 1;

    step = 1.0 * TEXHEIGHT / lineHeight;
    texPos = (drawStart - SCREEN_HEIGHT / 2 + lineHeight / 2) * step;
//    texOffset = texX * TEXHEIGHT;
    for (y = drawStart; y < drawEnd; y++) {
	texY = (int)texPos & (TEXHEIGHT - 1);
	texPos += step;
	
	color = rcState->textures[texNum][texY * TEXWIDTH + texX];
	r = color & 0xFF;
	g = (color >> 8) & 0xFF;
	b = (color >> 16) & 0xFF;
	if (side == 1)
	{
		r = (Uint8)(r * 0.7);
		g = (Uint8)(g * 0.7);
		b = (Uint8)(b * 0.7);
	}
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderDrawPoint(renderer, x, y);
    }
}

void render(SDLState* sdlState, RaycasterState* rcState) {
	int x, mapX, mapY, side;
	double cameraX, rayDirX, rayDirY, perpWallDist;
    SDL_SetRenderDrawColor(sdlState->renderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlState->renderer);
    drawCeiling(sdlState->renderer, rcState);
    drawFloor(sdlState->renderer, rcState);

    for (x = 0; x < SCREEN_WIDTH; x++) {
	    cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
	rayDirX = rcState->dirX + rcState->planeX * cameraX;
	rayDirY = rcState->dirY + rcState->planeY * cameraX;

        calculateRayPosition(x, rcState, &rayDirX, &rayDirY);

        performDDA(rcState, rayDirX, rayDirY, &mapX, &mapY, &perpWallDist, &side);

        drawWall(sdlState->renderer, x, perpWallDist, side, rcState, rayDirX, rayDirY, mapX, mapY);
    }
    if (rcState->toggleMap)
    	drawMiniMap(sdlState->renderer, rcState);

    SDL_RenderPresent(sdlState->renderer);
}
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
#define NUM_TEXTURES 8
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
void drawMiniMap(SDL_Renderer *renderer, RaycasterState *state);
void textures_init(RaycasterState *rcState);
RaycasterState* parseMapFile(const char *filename);
void drawFloor(SDL_Renderer *renderer, RaycasterState *state);
void drawCeiling(SDL_Renderer *renderer, RaycasterState *state);
#endif // RAYCASTER_H

