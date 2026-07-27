#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_image/SDL_image.h>
#include <stdlib.h>
#include "game.h"
#include "player.h"

void GameInit(SDL_Window** window, SDL_Renderer** renderer){
    //Initialisation of video, audio, gamepad is for consoles
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)){
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        exit(-1);
    }
    //Making the window and renderer
    if(!SDL_CreateWindowAndRenderer("Cat game", WIDTH, HEIGHT, 0, window, renderer)){//width first, then height
            SDL_Log("Failed: %s", SDL_GetError());
    }
    SDL_SetRenderVSync(*renderer, 1); //vsync here for framerate, 1 for 60Hz
}

void GameDestroy(SDL_Window* window, SDL_Renderer* renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void RunGame(SDL_Renderer* renderer){
    
    bool running = true;
    SDL_Event event;

    //Declaration of cat
    Player cat = PlayerInit(renderer);
    Uint64 last = SDL_GetPerformanceCounter();  //For delta time

    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_EVENT_QUIT){
                running=false;	//We stop if the player hits on "X"
            }
        }
        
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)(now-last)/(float)SDL_GetPerformanceFrequency();  //Delta time
        last = now;


        //making the keyboard movement
        const bool* keys = SDL_GetKeyboardState(NULL);
        
        //Lifetime of player cat
        PlayerUpdate(&cat, keys, dt);
        //setting the default background color to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);     //here's black
        SDL_RenderClear(renderer);
        PlayerRender(&cat, renderer);
        
        //OTHER GAME LOGIC HERE
        
        SDL_RenderPresent(renderer);
    }
    PlayerDestroy(&cat);
    SDL_DestroyTexture(cat.texture);
}