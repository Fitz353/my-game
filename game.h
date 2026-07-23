#ifndef GAME_H
#define GAME_H
#define HEIGHT 650
#define WIDTH 500

    
#include <SDL3/SDL.h>
#include <stdbool.h>
    
    void Init(SDL_Window** window, SDL_Renderer** renderer);
    void Destroy(SDL_Window* window, SDL_Renderer* renderer);
    void RunGame(SDL_Renderer* renderer);

#endif