#include <SDL2/SDL.h>
#include <iostream>
#include <ostream>


using namespace std;


SDL_Window *window;
SDL_Surface *winSurface;
SDL_Event event;

bool pixels[2048];

bool init(){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        cerr << "SDL_Init Error: " << SDL_GetError() << endl;
        return 1;
    }

    //force software rendering
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");

    window = SDL_CreateWindow("SDL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN);
    if(!window){
        cerr << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return false;
    }
    winSurface = SDL_GetWindowSurface(window);
    if(!winSurface){
        cerr << "SDL_GetWindowSurface Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return false;
    }
    return true;
}

void kill(){
    SDL_DestroyWindow(window);
    window = NULL;
    winSurface = NULL;
    SDL_Quit();
}

int main(){
    if(!init()){
        kill();
        return 1;
    }

    while(){
        SDL_PollEvent(&event);
        switch(event.type){
            case SDL_WINDOWEVENT:
                switch(event.window.type){
                    SDL_WINDOWEVENT_CLOSE:
                        kill();
                        break;
                }
                break;
            default:

                cerr << "Unknown event: " << string(event.type) << endl;
                cout << "Hello world" << endl;
            
        }
        //1.Get input
        //2.update logic
        //3.render image

        
    }
    
    SDL_Delay(10000);



    return 0;
}

