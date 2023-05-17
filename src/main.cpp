/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include "window.hpp"


int main( int argc, char* args[] )
{
    WindowApp app;
    while(!app.getQuit()){
        app.render();
        app.event();
    }
    return 0;
}