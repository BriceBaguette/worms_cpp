#include "worm.hpp"

Worm::Worm(){
    if(!Worm::loadMedia()){
        exit(-1);
    }
}


void Worm::event(){

}

void Worm::render(){

}

bool Worm::loadMedia(){
// Loading success flag
    bool success = true;

    // Load splash image
    wormSprite = SDL_LoadBMP("./assets/assets/sprites/worm_rest.bmp");
    if (wormSprite == NULL)
    {
        printf("Unable to load image %s! SDL Error: %s\n", "./assets/assets/sprites/worm_rest.bmp", SDL_GetError());
        success = false;
    }

    return success;

}