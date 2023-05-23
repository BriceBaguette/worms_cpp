/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include "window.hpp"


int main()
{
    WindowApp app;
    while(!app.getQuit()){
        int frameStart = SDL_GetTicks();
        app.render();
        app.event();
        app.update();
        int targetFrameTime = (1000/FRAMERATE);
        int frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < targetFrameTime) {
            SDL_Delay(targetFrameTime - frameTime);
        }
    }
    return 0;
}