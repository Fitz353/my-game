#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
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
    SDL_SetRenderVSync(*renderer, 1); //vsync here for framerate, 1 for 60Hz
}

void Destroy(SDL_Window* window, SDL_Renderer* renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}



void HandlePlayerIn(const bool* keys, Player* cat, float speed, float dt){
    
    speed = speed * dt;

    bool shift = keys[SDL_SCANCODE_LSHIFT];
    bool left = keys[SDL_SCANCODE_LEFT];
    bool right = keys[SDL_SCANCODE_RIGHT];
    bool up = keys[SDL_SCANCODE_UP];
    bool down = keys[SDL_SCANCODE_DOWN];
    bool moving = left || right || up || down;

    //Running
    if(shift && moving){
        speed*=2;
    }

    //Movement
    if(left){
        cat->x-=speed;
        cat->last_facing=FACING_LEFT;
    }
    if(right){
        cat->x+=speed;
        cat->last_facing=FACING_RIGHT;
    }
    if(up){
        cat->y-=speed;
        cat->last_facing=FACING_UP;
    }
    if(down){
        cat->y+=speed;
        cat->last_facing=FACING_DOWN;
    }

    PlayerState new_state;
    //Whats the next state
    if(left && shift){
        new_state=STATE_RUN_LEFT;
    }
    else if(right && shift){
        new_state=STATE_RUN_RIGHT;
    }
    else if (up && shift){
        new_state=STATE_RUN_UP;
    }
    else if (down && shift){
        new_state = STATE_RUN_DOWN;
    }
    else if (left){
        new_state=STATE_WALK_LEFT;
    }
    else if (right){
        new_state = STATE_WALK_RIGHT;
    }
    else if (up){
        new_state=STATE_WALK_UP;
    }
    else if (down){
        new_state=STATE_WALK_DOWN;
    }
    else{
        new_state=STATE_IDLE;
    }

    if(cat->direction!=new_state){
        cat->direction=new_state;
        cat->current_frame=0;
        cat->anim_timer=0.0;

        AnimData anim;
            if(new_state == STATE_IDLE){
                anim = cat->idle[cat->last_facing];
            } else {
                anim = cat->anims[new_state];
            }
  
            cat->current_row = anim.row;
            cat->total_frames = anim.total_frames;
            cat->anim_speed = anim.anim_speed;             
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
    Player cat = {                                                       
            .x = 100.0,                                                      
            .y = 100.0,                                                      
            .width = 192.0,                                                  
            .height = 192.0,                                                 
            .current_frame = 0,                                              
            .current_row = 19,                                               
            .total_frames = 5,                                               
            .anim_timer = 0.0,                                               
            .anim_speed = 0.3,                                               
            .direction = STATE_IDLE,                                         
            .last_facing = FACING_DOWN,                                      
            .anims = {                                                       
                [STATE_WALK_LEFT]  = {4, 6, 0.2},                            
                [STATE_WALK_RIGHT] = {5, 6, 0.2},
                [STATE_WALK_UP]    = {3, 6, 0.2},
                [STATE_WALK_DOWN]  = {2, 6, 0.2},
                [STATE_RUN_LEFT]   = {11, 5, 0.1},
                [STATE_RUN_RIGHT]  = {10, 5, 0.1},
                [STATE_RUN_UP]     = {9, 4, 0.1},
                [STATE_RUN_DOWN]   = {8, 4, 0.1},
            },
            .idle = {
                [FACING_DOWN]  = {19, 5, 0.3},
                [FACING_LEFT]  = {21, 5, 0.3},
                [FACING_RIGHT] = {22, 5, 0.3},
                [FACING_UP]    = {20, 5, 0.3},
            },
        };

    cat.texture=IMG_LoadTexture(renderer, "assets/fred_cat.png");
    
    if(!cat.texture){
        SDL_Log("Image fail: %s", SDL_GetError());
    }
    SDL_SetTextureScaleMode(cat.texture, SDL_SCALEMODE_NEAREST);

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
        float speed = 230.0;
        HandlePlayerIn(keys, &cat, speed, dt);      //Make the player move by the arrows
        KeepInside(&cat, 0, WIDTH, HEIGHT);     //Keep player in bounds
        
        cat.anim_timer+=dt;
        if(cat.anim_timer>=cat.anim_speed){
            cat.current_frame=(cat.current_frame+1)%cat.total_frames;
            cat.anim_timer=0;
        }




        //setting the default background color to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);     //here's black
        SDL_RenderClear(renderer);

        //Drawing the player
        SDL_FRect src = {cat.current_frame * 64, cat.current_row * 64, 64, 64};
        SDL_FRect dest = {cat.x, cat.y, cat.width, cat.height};     //Float rectangle, position and how big
        SDL_RenderTexture(renderer, cat.texture, &src, &dest);



        //OTHER GAME LOGIC HERE
        
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(cat.texture);
}