#include "worm.hpp"

Worm::Worm(SDL_Renderer *renderer)
{
    this->loadAll(renderer);
}

void Worm::setHSPeed(int speed){
    this->hSpeed = speed;
}

void Worm::setVSPeed(int speed){
    this->vSpeed = speed;
}

void Worm::update(){
    if(!(this->x + this->hSpeed < 0 || this->x + this->hSpeed > SCREEN_WIDTH - WORM_WIDTH))
        this->x += this->hSpeed;
    else this->hSpeed = 0;
    this->y += this->vSpeed;
}

void Worm::render(SDL_Renderer *renderer)
{
    SDL_Rect destinationRect = {this->x, this->y, WORM_WIDTH, WORM_HEIGHT };
    SDL_Texture* sprite = NULL;
    // Set the position and dimensions of the image
    if(this->hSpeed == 0 && this->vSpeed == 0){
        sprite = this->restSprite;
    }
    if(this->hSpeed != 0) {
        if(this->hSpeed > 0)
            this->flip = true;
        else    
            this->flip = false;
        sprite = this->movingSprite.front();
        std::reverse(this->movingSprite.begin(), this->movingSprite.end());
    }

    SDL_QueryTexture(sprite, nullptr, nullptr, &destinationRect.w, &destinationRect.h);
    if(this->flip){
        SDL_RenderCopyEx(renderer, sprite, nullptr, &destinationRect, 0, nullptr, SDL_FLIP_HORIZONTAL);
    }
    else{
        SDL_RenderCopyEx(renderer, sprite, nullptr, &destinationRect, 0, nullptr, SDL_FLIP_NONE);
    }
}

void Worm::loadAll(SDL_Renderer *renderer)
{
    this->restSprite = this->loadMedia(renderer, "./assets/assets/sprites/worm_rest.bmp");
    this->movingSprite.push_back(this->loadMedia(renderer, "./assets/assets/sprites/worm_moving1.bmp"));
    this->movingSprite.push_back(this->loadMedia(renderer, "./assets/assets/sprites/worm_moving2.bmp"));
    this->fallingSprite = this->loadMedia(renderer, "./assets/assets/sprites/worm_falling.bmp");
    this->jetpackSprite = this->loadMedia(renderer, "./assets/assets/sprites/worm_flying.bmp");
}

SDL_Texture *Worm::loadMedia(SDL_Renderer *renderer, const char *path)
{

    // Load splash image
    SDL_Surface *imageSurface = SDL_LoadBMP(path);
    if (!imageSurface)
    {
        std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
        exit(-1);
    }

    // Create texture from surface
    SDL_Texture *wormSprite = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    return wormSprite;
}