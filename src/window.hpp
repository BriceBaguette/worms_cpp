#include <SDL2/SDL.h>
#include "defs.hpp"

class WindowApp
{
private:
    //The window we'll be rendering to
    SDL_Window* gWindow = NULL;
        
    //The surface contained by the window
    SDL_Surface* gScreenSurface = NULL;

    SDL_Surface* gSprite = NULL;

public:
    WindowApp();
    ~WindowApp();
    bool init();
    bool loadMedia();
    void close();
};



