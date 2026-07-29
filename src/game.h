#ifndef GAME_H
#define GAME_H
#define HEIGHT 900
#define WIDTH 1600


//Struct of the player with it's attributes
#include <SDL3/SDL.h>
#include <stdbool.h>

    void GameInit(SDL_Window** window, SDL_Renderer** renderer);
    void GameDestroy(SDL_Window* window, SDL_Renderer* renderer);
    void RunGame(SDL_Renderer* renderer);

#endif