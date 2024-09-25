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
    int y, x;

    /* Draw the minimap */
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

    /* Draw the player */
    int playerX = minimapX + (int)(state->posX * MINIMAP_SCALE);
    int playerY = minimapY + (int)(state->posY * MINIMAP_SCALE);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect playerRect = {
        playerX - 2,
        playerY - 2,
        4,
        4
    };
    SDL_RenderFillRect(renderer, &playerRect);

    /* Draw the field of view */
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 128);
    int fovStartX = playerX;
    int fovStartY = playerY;
    int fovEndX = playerX + (int)(state->dirX * MINIMAP_SCALE * state->planeX * 2);
    int fovEndY = playerY + (int)(state->dirY * MINIMAP_SCALE * state->planeY * 2);
    SDL_RenderDrawLine(renderer, fovStartX, fovStartY, fovEndX, fovEndY);
    int fovAngle = (int)(atan2(state->planeY, state->planeX) * 180.0 / M_PI);
    SDL_RenderDrawLine(renderer, playerX, playerY, playerX + (int)(MINIMAP_SCALE * cos((fovAngle - 30) * M_PI / 180)), playerY + (int)(MINIMAP_SCALE * sin((fovAngle - 30) * M_PI / 180)));
    SDL_RenderDrawLine(renderer, playerX, playerY, playerX + (int)(MINIMAP_SCALE * cos((fovAngle + 30) * M_PI / 180)), playerY + (int)(MINIMAP_SCALE * sin((fovAngle + 30) * M_PI / 180)));
}
