#include<SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

class Worm {
    private:
        int x_pos = 200;
        int y_pos = 200;
        SDL_Surface* wormSprite=NULL;
    
    public:

        Worm();
        void render();
        void event();
        bool loadMedia();
};