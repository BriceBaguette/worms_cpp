#include<SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

class Worm {
    private:
        int x = 200;
        int y = 200;
        SDL_Texture* wormSprite=NULL;
    
    public:

        Worm(SDL_Renderer *renderer);
        void render(SDL_Renderer *renderer);
        void event();
        bool loadMedia(SDL_Renderer *renderer);
        void close();
};