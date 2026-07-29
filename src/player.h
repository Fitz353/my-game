#ifndef PLAYER_H
#define PLAYER_H
#define CAT_SPEED 230.0f

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
    FACING_DOWN,    //idle
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
    float col_offset_x, col_offset_y;   //both for collision
    float col_width, col_height;    //collision box delimiters
    PlayerState direction; 
    FacingDir last_facing;
    AnimData anims[STATE_RUN_DOWN+1];   //+1 because enum uses 0-indexing
    AnimData idle[FACING_UP+1];
}Player;

Player PlayerInit(SDL_Renderer* renderer);
void PlayerUpdate(Player* player, const bool* keys, float dt, float world_w, float world_h);
void PlayerRender(Player* player, SDL_Renderer* renderer, float cam_x, float cam_y, float zoom);
void PlayerDestroy(Player* player);

#endif