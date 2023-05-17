#include<SDL2/SDL>
#include <SDL2/SDL_image.h>
#include <iostream>
class Worm {
    private:

        int x_pos = 200;
        int y_pos = 200;
        SDL_Surface* wormSprite=NULL;
    
    public:

        worm();
        void render();
        void event();
        void loadMedia();

}