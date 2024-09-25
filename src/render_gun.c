#include "ray.h"
void renderGun(SDL_Renderer *renderer)
{
	int gunWidth = 50;
	int gunHeight = 100;

	SDL_Rect gunRect;
	gunRect.w = gunWidth;
	gunRect.h = gunHeight;
	gunRect.x = (SCREEN_WIDTH / 2) - (gunWidth / 2);
	gunRect.y = SCREEN_HEIGHT - gunHeight - 20;

	SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255); // Grey color for the gun
	SDL_RenderFillRect(renderer, &gunRect);
}
