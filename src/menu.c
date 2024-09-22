#include "ray.h"
#include <SDL2/SDL_ttf.h>

void cleanupMenu(MenuItem *menuItems, int count, TTF_Font *font) {
    for (int i = 0; i < count; i++) {
        SDL_DestroyTexture(menuItems[i].texture);
    }
    TTF_CloseFont(font);
}

int showMenu(SDL_Renderer *renderer) {
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        return -1;
    }

    TTF_Font *font = TTF_OpenFont("fonts/Roboto-Black.ttf", FONT_SIZE);
    if (!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        return -1;
    }

    MenuItem menuItems[MENU_ITEMS] = {
        {"Start Game", NULL, {0}},
        {"Options", NULL, {0}},
        {"Quit", NULL, {0}}
    };

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Color highlightColor = {255, 255, 0, 255};

    for (int i = 0; i < MENU_ITEMS; i++) {
        SDL_Surface *surface = TTF_RenderText_Solid(font, menuItems[i].text, textColor);
        menuItems[i].texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        SDL_QueryTexture(menuItems[i].texture, NULL, NULL, &menuItems[i].rect.w, &menuItems[i].rect.h);
        menuItems[i].rect.x = (SCREEN_WIDTH - menuItems[i].rect.w) / 2;
        menuItems[i].rect.y = (SCREEN_HEIGHT / 2) + (i - MENU_ITEMS/2) * 50;
    }

    int selectedItem = 0;
    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
                selectedItem = -1;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        selectedItem = (selectedItem - 1 + MENU_ITEMS) % MENU_ITEMS;
                        break;
                    case SDLK_DOWN:
                        selectedItem = (selectedItem + 1) % MENU_ITEMS;
                        break;
                    case SDLK_RETURN:
                        quit = true;
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < MENU_ITEMS; i++) {
            SDL_Color currentColor = (i == selectedItem) ? highlightColor : textColor;
            SDL_Surface *surface = TTF_RenderText_Solid(font, menuItems[i].text, currentColor);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            SDL_RenderCopy(renderer, texture, NULL, &menuItems[i].rect);
            SDL_DestroyTexture(texture);
        }

        SDL_RenderPresent(renderer);
    }

    cleanupMenu(menuItems, MENU_ITEMS, font);
    TTF_Quit();

    return selectedItem;
}
