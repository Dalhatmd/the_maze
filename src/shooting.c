#include "ray.h"
void handleShooting(RaycasterState *rcState)
{
//	Gun *gun = &rcState->guns[rcState->currentGun];
	//Uint32 currentTime = SDL_GetTicks() / 1000;

	for (int i = 0; i < MAX_BULLETS; i++) {
            if (!rcState->bullets[i].active) {
                rcState->bullets[i].x = rcState->posX;
                rcState->bullets[i].y = rcState->posY;
                rcState->bullets[i].dx = rcState->dirX * BULLET_SPEED;
                rcState->bullets[i].dy = rcState->dirY * BULLET_SPEED;
                rcState->bullets[i].active = true;
                rcState->bullets[i].speed = BULLET_SPEED;
                rcState->bullets[i].damage = BULLET_DAMAGE;
                rcState->bullets[i].maxRange = BULLET_MAX_RANGE;
                rcState->bullets[i].lifeTime = BULLET_LIFETIME;
                rcState->bullets[i].timeAlive = 0;
                rcState->numBullets++;
                break;
            }
        }
        rcState->isShooting = false;
}


void shootBullet(RaycasterState *rcState) {
    if (rcState->numBullets < MAX_BULLETS) {
        Bullet *bullet = &rcState->bullets[rcState->numBullets++];

        // Calculate the initial position and direction of the bullet
        double rayDirX, rayDirY;
        calculateRayPosition(0, rcState, &rayDirX, &rayDirY);

        bullet->x = rcState->posX + rcState->dirX * 0.5;
        bullet->y = rcState->posY + rcState->dirY * 0.5;
        bullet->dx = rayDirX;
        bullet->dy = rayDirY;
        bullet->speed = 20.0;
        bullet->active = true;
    }
}
void renderBullets(SDL_Renderer* renderer, RaycasterState* rcState) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (rcState->bullets[i].active) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawPoint(renderer, (int)rcState->bullets[i].x, (int)rcState->bullets[i].y);
        }
    }
}
void moveBullets(RaycasterState* rcState, double deltaTime) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (rcState->bullets[i].active) {
            rcState->bullets[i].x += rcState->bullets[i].dx * deltaTime;
            rcState->bullets[i].y += rcState->bullets[i].dy * deltaTime;
            rcState->bullets[i].timeAlive += deltaTime;
            if (rcState->bullets[i].timeAlive >= rcState->bullets[i].lifeTime || 
                pow(rcState->bullets[i].x - rcState->posX, 2) + pow(rcState->bullets[i].y - rcState->posY, 2) > pow(rcState->bullets[i].maxRange, 2)) {
                rcState->bullets[i].active = false;
                rcState->numBullets--;
            }
        }
    }
}
