#include "ray.h"

void initGuns(RaycasterState *state)
{
	state->guns[0] = (Gun){0, 10, 2.0, 0.0}; /* pistol */
	state->guns[1] = (Gun){1, 20, 44.0, 0.0}; /* shotgun */
	state->guns[2] = (Gun){2, 5, 10.0, 0.0}; /* machine gun */

	state->currentGun = 0;

	 const char *gunTextureFiles[] = {"pistol.png", "shotgun.png", "machinegun.png"};
    
    for (int i = 0; i < MAX_GUNS; i++) {
        SDL_Surface* gunSurface = IMG_Load(gunTextureFiles[i]);
        if (gunSurface == NULL) {
            printf("Unable to load gun texture %s! SDL_image Error: %s\n", gunTextureFiles[i], IMG_GetError());
        } else {
            SDL_LockSurface(gunSurface);
            for (int y = 0; y < GUN_TEXTURE_HEIGHT; y++) {
                for (int x = 0; x < GUN_TEXTURE_WIDTH; x++) {
                    Uint32 pixel = ((Uint32*)gunSurface->pixels)[y * gunSurface->w + x];
                    state->gunTextures[i][GUN_TEXTURE_WIDTH * y + x] = pixel;
                }
            }
            SDL_UnlockSurface(gunSurface);
            SDL_FreeSurface(gunSurface);
        }
    }
}

void renderGun(SDL_Renderer *renderer, RaycasterState *state) {
    int gunTextureId = state->guns[state->currentGun].textureId;
    
    // Calculate the position to render the gun (centered at the bottom of the screen)
    int gunWidth = GUN_TEXTURE_WIDTH;
    int gunHeight = GUN_TEXTURE_HEIGHT;
    int gunX = (SCREEN_WIDTH - gunWidth) / 2;
    int gunY = SCREEN_HEIGHT - gunHeight;

    // Render the gun texture
    for (int y = 0; y < gunHeight; y++) {
        for (int x = 0; x < gunWidth; x++) {
            Uint32 color = state->gunTextures[gunTextureId][GUN_TEXTURE_WIDTH * y + x];
            // Assuming 0 is transparent
            if ((color & 0x00FFFFFF) != 0) {
                SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 255);
                SDL_RenderDrawPoint(renderer, gunX + x, gunY + y);
            }
        }
    }
}

void handleShooting(RaycasterState *state) {
    if (state->isShooting) {
        Gun *currentGun = &state->guns[state->currentGun];
        double currentTime = SDL_GetTicks() / 1000.0;  // Current time in seconds
        
        if (currentTime - currentGun->lastFireTime >= 1.0 / currentGun->fireRate) {
            // Perform shooting logic here
            printf("Bang! Gun %d fired.\n", state->currentGun);
            
            // TODO: Add hit detection and damage calculation for enemies
            
            currentGun->lastFireTime = currentTime;
        }
    }
}
