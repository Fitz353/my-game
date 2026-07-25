#include <SDL3/SDL.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3_image/SDL_image.h>
#include <stdlib.h>
#include "game.h"

void Init(SDL_Window** window, SDL_Renderer** renderer){
    //Initialisation of video, audio, gamepad is for consoles
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)){
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        exit(-1);
    }
    //Making the window and renderer
    if(!SDL_CreateWindowAndRenderer("Cat game", WIDTH, HEIGHT, 0, window, renderer)){//width first, then height
            SDL_Log("Failed: %s", SDL_GetError());
    }
}

void Destroy(SDL_Window* window, SDL_Renderer* renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void HandlePlayerIn(const bool* keys, Player* cat, float speed){
    
    if(keys[SDL_SCANCODE_LSHIFT] && (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_DOWN])){
        speed*=2;
    }
    if(keys[SDL_SCANCODE_LEFT]){
        cat->x-=speed;
    }
    if(keys[SDL_SCANCODE_RIGHT]){
        cat->x+=speed;
    }
    if(keys[SDL_SCANCODE_UP]){
        cat->y-=speed;
    }
    if(keys[SDL_SCANCODE_DOWN]){
        cat->y+=speed;
    }
    
}

void KeepInside(Player* player, float min, float max_x, float max_y){
    //checking the player is still in the window, or where we want
    if(player->x<min){
        player->x=min;
    }
    if(player->y<min){
        player->y=min;
    }
    if(player->x + player->width>max_x){
        player->x=max_x - player->width;
    }
    if(player->y + player->height>max_y){
        player->y=max_y - player->height;
    }
}

void RunGame(SDL_Renderer* renderer){
    bool running = true;
    SDL_Event event;
    //Main game loop

    //Declaration of cat
    Player cat;
    cat.x=100.0;
    cat.y=100.0;
    cat.width=150.0;
    cat.height=150.0;
    cat.texture=IMG_LoadTexture(renderer, "assets/cat.png");
    if(!cat.texture){
        SDL_Log("Image fail: %s", SDL_GetError());
    }

    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_EVENT_QUIT){
                running=false;	//We stop if the player hits on "X"
            }
        }
        
        //making the keyboard movement
        const bool* keys = SDL_GetKeyboardState(NULL);
        float speed = 2.5;
        HandlePlayerIn(keys, &cat, speed);      //Make the player move by the arrows
        KeepInside(&cat, 0, WIDTH, HEIGHT);     //Keep player in bounds





        //setting the default background color to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);     //here's black
        SDL_RenderClear(renderer);

        //Drawing the player
        SDL_FRect dest = {cat.x, cat.y, cat.width, cat.height};     //Float rectangle, position and how big
        SDL_RenderTexture(renderer, cat.texture, NULL, &dest);



        //OTHER GAME LOGIC HERE
        
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(cat.texture);
}