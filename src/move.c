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
			state->rotateLeft = true;
			break;
		case SDLK_d:
			state->rotateRight = true;
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
			state->rotateLeft = false;
			break;
		case SDLK_d:
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
}
