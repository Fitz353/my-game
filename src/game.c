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
#include "map.h"

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
    MapLoad("tiled/map2.tmj", renderer);

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
        
        // 1.666f keeps your current look. Make bigger if you want bigger world scale.
            float map_scale = 1.666f;
            float world_w = MapGetTileCols() * 32.0f * map_scale;
            float world_h = MapGetTileRows() * 32.0f * map_scale;

            PlayerUpdate(&cat, keys, dt, world_w, world_h);

            float zoom = 2.0f;
            float view_w = WIDTH / zoom;
            float view_h = HEIGHT / zoom;

            float cam_x = cat.x + (cat.width / 2.0f) - (view_w / 2.0f);
            float cam_y = cat.y + (cat.height / 2.0f) - (view_h / 2.0f);

            // Clamping or Centering
            if (world_w < view_w) cam_x = -(view_w - world_w) / 2.0f; // Center small rooms
            else {
                float max_x = world_w - view_w;
                if (cam_x < 0) cam_x = 0;
                if (cam_x > max_x) cam_x = max_x;
            }

            if (world_h < view_h) cam_y = -(view_h - world_h) / 2.0f; // Center small rooms
            else {
                float max_y = world_h - view_h;
                if (cam_y < 0) cam_y = 0;
                if (cam_y > max_y) cam_y = max_y;
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            MapRender(renderer, world_w, world_h, cam_x, cam_y, zoom, false);
            PlayerRender(&cat, renderer, cam_x, cam_y, zoom);
            MapRender(renderer, world_w, world_h, cam_x, cam_y, zoom, true);

        //OTHER GAME LOGIC HERE
        
        SDL_RenderPresent(renderer);
    }

    PlayerDestroy(&cat);
    MapDestroy();
}