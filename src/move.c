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
		case SDLK_SPACE:
			state->isShooting = true;
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
		case SDLK_SPACE:
			state->isShooting = false;
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
    rotSpeed = ROTATE_SPEED * deltaTime * speedMultiplier;
    double moveX = 0;
    double moveY = 0;

    // Forward and backward movement
	if (state->moveForward || state->moveBackward)
    {
        double moveDir = state->moveForward ? 1 : -1;
        moveX += state->dirX * moveDir;
        moveY += state->dirY * moveDir;
    }
    if (state->strafeLeft || state->strafeRight)
    {
        double strafeDir = state->strafeRight ? 1 : -1;
        moveX += state->dirY * strafeDir;
        moveY += -state->dirX * strafeDir;
    }

    // Normalize movement vector if moving diagonally
    if (moveX != 0 && moveY != 0)
    {
        double length = sqrt(moveX * moveX + moveY * moveY);
        moveX /= length;
        moveY /= length;
    }

    // Apply movement
    if (state->map[(int)(state->posX + moveX * moveSpeed)][(int)state->posY] == 0)
        state->posX += moveX * moveSpeed;
    if (state->map[(int)state->posX][(int)(state->posY + moveY * moveSpeed)] == 0)
        state->posY += moveY * moveSpeed;
       // Rotation
    if(state->rotateLeft || state->rotateRight)
    {
        double rotDir = state->rotateLeft ? 1 : -1;
        double oldDirX = state->dirX;
        state->dirX = state->dirX * cos(rotSpeed * rotDir) - state->dirY * sin(rotSpeed * rotDir);
        state->dirY = oldDirX * sin(rotSpeed * rotDir) + state->dirY * cos(rotSpeed * rotDir);
        double oldPlaneX = state->planeX;
        state->planeX = state->planeX * cos(rotSpeed * rotDir) - state->planeY * sin(rotSpeed * rotDir);
        state->planeY = oldPlaneX * sin(rotSpeed * rotDir) + state->planeY * cos(rotSpeed * rotDir);
    }
}
