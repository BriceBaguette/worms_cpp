#include <SDL2/SDL.h>
#include "worm.hpp"
#include "defs.hpp"
#include "ground.hpp"

class WindowApp
{
private:
    //The window we'll be rendering to
    SDL_Window* gWindow = NULL;
        
    //The surface contained by the window
    SDL_Renderer* renderer = NULL;

    //the surface of the sprite
    SDL_Surface* gSprite = NULL;

    Ground* ground = NULL;

    bool quit = false;

    Worm* worm1 = NULL;
public:
    WindowApp();
    bool init();
    void update();
    void close();
    void render();
    void event();
    bool getQuit();
};



