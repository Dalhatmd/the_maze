#ifndef MAIN_H
#define MAIN_H
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAP_WIDTH 8
#define MAP_HEIGHT 8
#define TILE_SIZE 64

/**
 * struct Player - player struct
 *
 * @x: x position of Player
 * @y: y position of player
 * @dirX: direction x of Player
 * @dirY: direction y of player
 * @planeX: plane x of player
 * @planeY: plane y of Player
 * @moveSpeed: speed of player
 * @rotSpeed: rotation speed of player
 */
typedef struct Player
{
	double x;
	double y;
	double dirX;
	double dirY;
	double planeX;
	double planeY;
	double moveSpeed;
	double rotSpeed;
} Player;

void drawline(SDL_Renderer *renderer, int x, int drawS, int drawE, int color);
void castRays(SDL_Renderer *renderer, Player *player);
void drawMap(SDL_Renderer *renderer, Player *player);
void rotatePlayer(Player *player, double rotSpeed);
void movePlayer(Player *player, double moveSpeed, int forward, int strafe);

#endif /* MAZE_H */
