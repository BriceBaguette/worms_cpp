#include <SDL2/SDL.h>
#include "defs.hpp"

class WindowApp
{
private:
    //The window we'll be rendering to
    SDL_Window* gWindow = NULL;
        
    //The surface contained by the window
    SDL_Renderer* renderer = NULL;

    //the surface of the sprite
    SDL_Surface* gSprite = NULL;

    bool quit = false;
public:
    WindowApp();
    bool init();
    void close();
    void render();
    void event();
    bool getQuit();
};



