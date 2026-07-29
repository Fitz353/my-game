#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

void MapLoad(const char* filepath, SDL_Renderer* renderer);
void MapRender(SDL_Renderer* renderer, float target_w, float target_h);
void MapDestroy(void);  