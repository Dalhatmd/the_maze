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
