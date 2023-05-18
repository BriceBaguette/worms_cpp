#include "worm.hpp"

Worm::Worm(SDL_Renderer *renderer){
    if(!Worm::loadMedia(renderer)){
        exit(-1);
    }
    
}


void Worm::event(){

}

void Worm::render(SDL_Renderer *renderer){
    

    // Set the position and dimensions of the image
    SDL_Rect destinationRect = {this->x, this->y, 0, 0};
    SDL_QueryTexture(this->wormSprite, nullptr, nullptr, &destinationRect.w, &destinationRect.h);

    // Render the image
    SDL_RenderCopy(renderer, this->wormSprite, nullptr, &destinationRect);

}

bool Worm::loadMedia(SDL_Renderer *renderer){
// Loading success flag
    bool success = true;

    // Load splash image
    SDL_Surface* imageSurface = SDL_LoadBMP("./assets/assets/sprites/worm_rest.bmp");
    if (!imageSurface) {
        std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
        return 1;
    }

    // Create texture from surface
    wormSprite = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    return success;
}