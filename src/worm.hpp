#include<SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include "defs.hpp"
#include <vector>
#include <algorithm>

class Worm {
    private:
        int x = 200;
        int y = 200;
        SDL_Texture* restSprite=NULL;
        std::vector<SDL_Texture*> movingSprite;
        SDL_Texture* fallingSprite = NULL;
        SDL_Texture* jetpackSprite = NULL;
        int vSpeed = 0;
        int hSpeed = 0;       
        bool flip = false;
    
    public:

        Worm(SDL_Renderer *renderer);

        // Function to load sprite        
        void loadAll(SDL_Renderer *renderer);
        SDL_Texture* loadMedia(SDL_Renderer *renderer,const char* path);

        // Function that manage the game behaviour and display of the worm
        void render(SDL_Renderer *renderer);
        void update();
        void setHSPeed(int speed);
        void setVSPeed(int speed);
        void close();
};