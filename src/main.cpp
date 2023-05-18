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
        app.render();
        app.event();
        SDL_Delay(33);
    }
    return 0;
}