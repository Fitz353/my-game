#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdlib.h>
#include "game.h"

int main(int argc, char* argv[]){
    //For compiler warnings
    (void) argc;
    (void) argv;

    //Initializing window and other stuff
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    Init(&window, &renderer);

    //Game running and core logic here
    RunGame(renderer);

    //Discarding and quitting the game
    Destroy(window, renderer);
    return 0;
}















