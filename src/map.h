#pragma once
#include <SDL3/SDL.h>
#include <stdbool.h>

int MapGetTileCols(void);
int MapGetTileRows(void);
void MapLoad(const char* filepath, SDL_Renderer* renderer);
void MapRender(SDL_Renderer* renderer, float target_w, float target_h, float cam_x, float cam_y, float zoom, bool render_tops);
void MapDestroy(void);
bool MapCheckCollision(float x, float y, float w, float h, float target_w, float target_h);