#include <SDL3_image/SDL_image.h>
#include "player.h"
#include "game.h"  
#include "map.h"

Player PlayerInit(SDL_Renderer* renderer) { //this function is for the player cat for now
        Player player = {
            .x = 763.0f,
            .y = 338.5f,
            .width = 128.0f,
            .height = 128.0f,
            .col_offset_x = 44.0f,
            .col_offset_y = 60.0f,
            .col_width = 40.0f,
            .col_height = 30.0f,
            .current_frame = 0,
            .current_row = 19,
            .total_frames = 5,
            .anim_timer = 0.0f,
            .anim_speed = 0.3f,
            .direction = STATE_IDLE,
            .last_facing = FACING_DOWN,
            .anims = {
                [STATE_WALK_LEFT]  = {4, 6, 0.2f},
                [STATE_WALK_RIGHT] = {5, 6, 0.2f},
                [STATE_WALK_UP]    = {3, 6, 0.2f},
                [STATE_WALK_DOWN]  = {2, 6, 0.2f},
                [STATE_RUN_LEFT]   = {11, 5, 0.1f},
                [STATE_RUN_RIGHT]  = {10, 5, 0.1f},
                [STATE_RUN_UP]     = {9, 4, 0.1f},
                [STATE_RUN_DOWN]   = {8, 4, 0.1f},
            },
            .idle = {
                [FACING_DOWN]  = {19, 5, 0.3f},
                [FACING_LEFT]  = {21, 5, 0.3f},
                [FACING_RIGHT] = {22, 5, 0.3f},
                [FACING_UP]    = {20, 5, 0.3f},
            },
        };
    
        player.texture = IMG_LoadTexture(renderer, "assets/fred_cat.png");
        if (!player.texture) {
            SDL_Log("Failed to load player texture: %s", SDL_GetError());
        }
        SDL_SetTextureScaleMode(player.texture, SDL_SCALEMODE_NEAREST);
    
        return player;
    }

void PlayerUpdate(Player* player, const bool* keys, float dt){
        float speed = CAT_SPEED * dt;

        bool shift = keys[SDL_SCANCODE_LSHIFT];
        bool left = keys[SDL_SCANCODE_LEFT];
        bool right = keys[SDL_SCANCODE_RIGHT];
        bool up = keys[SDL_SCANCODE_UP];
        bool down = keys[SDL_SCANCODE_DOWN];
        bool moving = left || right || up || down;

        if (shift && moving) {
            speed *= 2; // Speed boost restored
        }

        bool can_left  = left  && (player->x + player->col_offset_x > 0);
        bool can_right = right && (player->x + player->col_offset_x + player->col_width < WIDTH);
        bool can_up    = up    && (player->y + player->col_offset_y > 0);
        bool can_down  = down  && (player->y + player->col_offset_y + player->col_height < HEIGHT);

        if(can_left && can_right){
            if(player->last_facing==FACING_LEFT) can_right=false;
            else can_left=false;
        }
        if(can_up && can_down){
            if(player->last_facing==FACING_DOWN) can_up=false;
            else can_down=false;
        }

        float next_x = player->x;
        float next_y = player->y;
        bool moved_left = false, moved_right = false, moved_up = false, moved_down = false;

        if (can_left)  { next_x -= speed; player->last_facing = FACING_LEFT; }
        if (can_right) { next_x += speed; player->last_facing = FACING_RIGHT; }

        // Slide X
        if (!MapCheckCollision(next_x + player->col_offset_x, player->y + player->col_offset_y, player->col_width,
  player->col_height, (float)WIDTH, (float)HEIGHT)) {
            player->x = next_x;
            if (can_left) moved_left = true;
            if (can_right) moved_right = true;
        }

        if (can_up)    { next_y -= speed; player->last_facing = FACING_UP; }
        if (can_down)  { next_y += speed; player->last_facing = FACING_DOWN; }

        // Slide Y
        if (!MapCheckCollision(player->x + player->col_offset_x, next_y + player->col_offset_y, player->col_width,
  player->col_height, (float)WIDTH, (float)HEIGHT)) {
            player->y = next_y;
            if (can_up) moved_up = true;
            if (can_down) moved_down = true;
        }

        PlayerState new_state;
        // Use moved_* variables here so animation stops when hitting a wall
        if (moved_left && shift)      new_state = STATE_RUN_LEFT;
        else if (moved_right && shift) new_state = STATE_RUN_RIGHT;
        else if (moved_up && shift)   new_state = STATE_RUN_UP;
        else if (moved_down && shift) new_state = STATE_RUN_DOWN;
        else if (moved_left)          new_state = STATE_WALK_LEFT;
        else if (moved_right)         new_state = STATE_WALK_RIGHT;
        else if (moved_up)            new_state = STATE_WALK_UP;
        else if (moved_down)          new_state = STATE_WALK_DOWN;
        else                          new_state = STATE_IDLE;

        if (player->direction != new_state) {
            player->direction = new_state;
            player->current_frame = 0;
            player->anim_timer = 0.0f;

            AnimData anim = (new_state == STATE_IDLE) ? player->idle[player->last_facing] : player->anims[new_state];
            player->current_row = anim.row;
            player->total_frames = anim.total_frames;
            player->anim_speed = anim.anim_speed;
        }

        // Keep Inside boundary check
        if (player->x + player->col_offset_x < 0) player->x = 0 - player->col_offset_x;
        if (player->y + player->col_offset_y < 0) player->y = 0 - player->col_offset_y;
        if (player->x + player->col_offset_x + player->col_width > WIDTH) player->x = WIDTH - player->col_width -
  player->col_offset_x;
        if (player->y + player->col_offset_y + player->col_height > HEIGHT) player->y = HEIGHT - player->col_height -
  player->col_offset_y;

        player->anim_timer += dt;
        if (player->anim_timer >= player->anim_speed) {
            player->current_frame = (player->current_frame + 1) % player->total_frames;
            player->anim_timer = 0.0f;
        }
    }

void PlayerRender(Player* player, SDL_Renderer* renderer, float cam_x, float cam_y, float zoom) {
        SDL_FRect src = {player->current_frame * 64.0f, player->current_row * 64.0f, 64.0f, 64.0f};
        SDL_FRect dest = {
            (player->x - cam_x) * zoom,
            (player->y - cam_y) * zoom,
            player->width * zoom,
            player->height * zoom
        };
        SDL_RenderTexture(renderer, player->texture, &src, &dest);

        SDL_FRect debug_col = {
            (player->x + player->col_offset_x - cam_x) * zoom,
            (player->y + player->col_offset_y - cam_y) * zoom,
            player->col_width * zoom,
            player->col_height * zoom
        };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderRect(renderer, &debug_col);
    }
    
void PlayerDestroy(Player* player) {
    if (player->texture) {
        SDL_DestroyTexture(player->texture);
        player->texture = NULL;
    }
}

