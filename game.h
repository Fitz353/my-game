#ifndef GAME_H
#define GAME_H
#define HEIGHT 900
#define WIDTH 1600

//Struct of the player with it's attributes
#include <SDL3/SDL.h>
#include <stdbool.h>

typedef enum{
    STATE_IDLE,
    STATE_WALK_LEFT,
    STATE_WALK_RIGHT,
    STATE_WALK_UP,
    STATE_WALK_DOWN,
    STATE_RUN_LEFT,
    STATE_RUN_RIGHT,
    STATE_RUN_UP,
    STATE_RUN_DOWN,
}PlayerState;

typedef enum{
    FACING_DOWN,//idle
    FACING_LEFT,
    FACING_RIGHT,
    FACING_UP,
}FacingDir;

typedef struct{
    int row;
    int total_frames;
    float anim_speed;
}AnimData;

typedef struct{
    float x, y;
    float width, height;
    SDL_Texture* texture;
    int current_frame, current_row;
    int total_frames;
    float anim_timer;
    float anim_speed;
    PlayerState direction; 
    FacingDir last_facing;
    AnimData anims[STATE_RUN_DOWN+1];
    AnimData idle[FACING_UP+1];
}Player;

    void Init(SDL_Window** window, SDL_Renderer** renderer);
    void Destroy(SDL_Window* window, SDL_Renderer* renderer);
    void RunGame(SDL_Renderer* renderer);

#endif