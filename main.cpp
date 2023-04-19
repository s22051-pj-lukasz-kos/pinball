#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL.h>

bool handle_events() {
    SDL_Event e;
    while( SDL_PollEvent( &e ) != 0 ) {
        switch (e.type) {
            case SDL_QUIT:
                return false;
        }
    }
    return true;
}

int main() {
    SDL_Init(SDL_INIT_EVERYTHING );
    auto window = SDL_CreateWindow( "Pinball",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    640, 480,
                                    SDL_WINDOW_SHOWN );
    while (handle_events()) {
        SDL_Delay(10);
    }


    SDL_DestroyWindow( window );
    SDL_Quit();
    return 0;
}
