#include <SDL3/SDL.h>
#define HEIGHT 650
#define WIDTH 500


int main(int argc, char* argv[]){
    //for compiler warnings
    (void) argc;
    (void) argv;

    //initialisation of video, audio, gamepad is for consoles
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)){
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return -1;
    }

    //making the window and renderer
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    if(!SDL_CreateWindowAndRenderer("Cat game", WIDTH, HEIGHT, 0, &window, &renderer)){//width first, then height
            SDL_Log("Failed: %s", SDL_GetError());
    }

    //making the closing by "X" mechanism, trapping in the while loop
    bool running = true;
    SDL_Event event;

    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_EVENT_QUIT){
                running=false;
            }
        }
        //setting the default background color to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);     //here's black
        SDL_RenderClear(renderer);

        //OTHER GAME LOGIC HERE
        
        SDL_RenderPresent(renderer);
    }







    SDL_Quit();
    return 0;
}