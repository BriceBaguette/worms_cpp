#include "worm.hpp"

Worm::Worm(SDL_Renderer *renderer)
{
    this->loadAll(renderer);
}

int Worm::getWeaponAmunition(){
    return this->weapon_amunition_map[this->weapon];
}

double Worm::getWeaponAngle(){
    return this->weapon_angle;
}

std::string Worm::getWeapon(){
    return this->weapon;
}

void Worm::setWeapon(const std::string& weapon){
    this->weapon = weapon;
}

int Worm::setDamage(int damage){
    this->health -= damage;
    return this->health;
}

void Worm::setHSPeed(int speed){
    this->hSpeed = speed;
}

void Worm::setVSPeed(int speed){
    this->vSpeed = speed;
}

void Worm::setAiming(bool aiming, bool upwards){
    this->aiming = aiming;
    this->aiming_upwards = upwards;
}

void Worm::fire(){
    this->weapon_amunition_map[this->weapon]--;
    this->weapon_angle = 0;
}

bool Worm::checkCollision(const std::list<SDL_Point>& points) {
    SDL_Rect rect = {this->x, this->y,WORM_WIDTH, WORM_HEIGHT};
    for (const SDL_Point& point : points) {
        if (point.x >= rect.x && point.x < (rect.x + rect.w)&&
            point.y >= rect.y && point.y < (rect.y + rect.h)) {
            this->y = point.y - WORM_HEIGHT;
            return true;  // Collision detected
        }
    }
    return false;  // No collision
}

void Worm::update(const std::list<SDL_Point>& points){
    if(!(this->x + this->hSpeed < 0 || this->x + this->hSpeed > SCREEN_WIDTH - WORM_WIDTH) && !(this->vSpeed<0))
        this->x += this->hSpeed;
    this->y += this->vSpeed;
    if(checkCollision(points)){
        this->vSpeed = 0;
    }
    else if(this->vSpeed == 0){
        this->vSpeed = 2;
    }
    else if((this->vSpeed >=2)){
        this->vSpeed = WORM_FALLING_SPEED;
    }

    if(this->aiming){
        if(this->aiming_upwards){
            this->weapon_angle += SHOOTING_ANGLE_STEP;
            if(this->weapon_angle > 90)
                this->weapon_angle = 90;
        }
        else{
            this->weapon_angle -= SHOOTING_ANGLE_STEP;
            if(this->weapon_angle < -90)
                this->weapon_angle = -90;
        }
    }
}

void Worm::render(SDL_Renderer *renderer)
{   

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_Rect destinationRect = {this->x, this->y, WORM_WIDTH, WORM_HEIGHT };
    SDL_Texture* sprite = NULL;
    bool show_weapon = false;
    // Set the position and dimensions of the image
    if(this->hSpeed == 0 && (this->vSpeed == 0 || this->vSpeed == 2)){
        sprite = this->restSprite;
        show_weapon = true;
    }
    if(this->hSpeed != 0) {
        if(this->hSpeed > 0)
            this->flip = true;
        else    
            this->flip = false;
        sprite = this->movingSprite.front();
        show_weapon = true;
        std::reverse(this->movingSprite.begin(), this->movingSprite.end());
    }

    if(this->vSpeed == WORM_FALLING_SPEED){
        sprite = this->fallingSprite;
    }

    SDL_QueryTexture(sprite, nullptr, nullptr, &destinationRect.w, &destinationRect.h);
    if(this->flip){
        SDL_RenderCopyEx(renderer, sprite, nullptr, &destinationRect, 0, nullptr, SDL_FLIP_HORIZONTAL);
        if(show_weapon){
            SDL_Texture* weaponSprite = NULL;
            if(!this->weapon.compare("bazooka"))
                weaponSprite = this->bazookaSprite;
            else
                weaponSprite = this->shotgunSprite;

            int weapon_center_x = this->x + WORM_WIDTH/2 - WORM_WEAPON_CENTERS_DISTANCE_X;
            int weapon_center_y = this->y + WORM_HEIGHT/2 + WORM_WEAPON_CENTERS_DISTANCE_Y;
            int weapon_x = weapon_center_x - WEAPON_WIDTH/2;
            int weapon_y = weapon_center_y - WEAPON_HEIGHT/2;
            SDL_Rect weapon_destination_rect = {weapon_x, weapon_y, WEAPON_WIDTH, WEAPON_HEIGHT };

            SDL_QueryTexture(weaponSprite, nullptr, nullptr, &weapon_destination_rect.w, &weapon_destination_rect.h);
            SDL_RenderCopyEx(renderer, 
                            weaponSprite, 
                            nullptr, 
                            &weapon_destination_rect, 
                            -this->weapon_angle, 
                            nullptr, 
                            SDL_FLIP_HORIZONTAL);
        }
    }
    else{
        SDL_RenderCopyEx(renderer, sprite, nullptr, &destinationRect, 0, nullptr, SDL_FLIP_NONE);
        if(show_weapon){
            SDL_Texture* weaponSprite = NULL;
            if(!this->weapon.compare("bazooka"))
                weaponSprite = this->bazookaSprite;
            else
                weaponSprite = this->shotgunSprite;

            int weapon_center_x = this->x + WORM_WIDTH/2 + WORM_WEAPON_CENTERS_DISTANCE_X;
            int weapon_center_y = this->y + WORM_HEIGHT/2 + WORM_WEAPON_CENTERS_DISTANCE_Y;
            int weapon_x = weapon_center_x - WEAPON_WIDTH/2;
            int weapon_y = weapon_center_y - WEAPON_HEIGHT/2;
            SDL_Rect weapon_destination_rect = {weapon_x, weapon_y, WEAPON_WIDTH, WEAPON_HEIGHT };
            
            SDL_QueryTexture(weaponSprite, nullptr, nullptr, &weapon_destination_rect.w, &weapon_destination_rect.h);
            SDL_RenderCopyEx(renderer, 
                            weaponSprite, 
                            nullptr, 
                            &weapon_destination_rect, 
                            this->weapon_angle, 
                            nullptr, 
                            SDL_FLIP_NONE);
        }
    }
}

void Worm::loadAll(SDL_Renderer *renderer)
{
    this->restSprite = this->loadMedia(renderer, "./assets/assets/sprites/worm_rest.bmp", WORM_WIDTH, WORM_HEIGHT);
    this->movingSprite.push_back(this->loadMedia(renderer, "./assets/assets/sprites/worm_moving1.bmp", WORM_WIDTH, WORM_HEIGHT));
    this->movingSprite.push_back(this->loadMedia(renderer, "./assets/assets/sprites/worm_moving2.bmp", WORM_WIDTH, WORM_HEIGHT));
    this->fallingSprite = this->loadMedia(renderer, "./assets/assets/sprites/worm_falling.bmp", WORM_WIDTH, WORM_HEIGHT);
    this->jetpackSprite = this->loadMedia(renderer, "./assets/assets/sprites/worm_flying.bmp", WORM_WIDTH, WORM_HEIGHT);
    this->bazookaSprite = this->loadMedia(renderer, "./assets/assets/sprites/weapon_bazooka.bmp", WEAPON_WIDTH, WEAPON_HEIGHT);
    this->shotgunSprite = this->loadMedia(renderer, "./assets/assets/sprites/weapon_gun.bmp", WEAPON_WIDTH, WEAPON_HEIGHT);
}

SDL_Texture *Worm::loadMedia(SDL_Renderer *renderer, const char *path, int width, int height)
{

    // Load splash image
    SDL_Surface *imageSurface = SDL_LoadBMP(path);
    if (!imageSurface)
    {
        std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
        exit(-1);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // Create texture from surface
    SDL_Surface* resizedSurface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_BlitScaled(imageSurface, nullptr, resizedSurface, nullptr);
    SDL_Texture *wormSprite = SDL_CreateTextureFromSurface(renderer, resizedSurface);
    SDL_FreeSurface(resizedSurface);
    SDL_FreeSurface(imageSurface);
    return wormSprite;
}