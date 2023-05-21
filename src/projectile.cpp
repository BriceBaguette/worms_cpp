#include "projectile.hpp"

void Projectile::render(SDL_Renderer *renderer){  

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_Rect destinationRect = {(int) this->x, (int) this->y, this->width, this->height };
    // Set the position and dimensions of the image
    SDL_QueryTexture(this->sprite, nullptr, nullptr, &destinationRect.w, &destinationRect.h);
    SDL_RenderCopyEx(renderer, this->sprite, nullptr, &destinationRect, this->angle, nullptr, SDL_FLIP_NONE);
}

void Projectile::update(){
    //update position accordingly to previous step speed
    this->x += this->h_speed;
    this->y += this->v_speed;
    //compute vertical speed variation accordingly to gravity (dv = a*dt)
    double dy_gravity =  GRAVITY * 1/FRAMERATE;
    //compute speed variation due to drag forces (dv = F/m * dt)
    double drag_force = -1/2 * AIR_DENSITY * this->cross_section_area * DRAG_COEFFICIENT * pow(this->speed, 2);
    double dx_drag = drag_force*cos(this->angle * M_PI/180.0) / this->mass * 1/FRAMERATE;
    double dy_drag = drag_force*-sin(this->angle * M_PI/180.0) / this->mass * 1/FRAMERATE;
    //apply those variations to the axial speeds
    this->h_speed += dx_drag;
    this->v_speed += (dy_drag + dy_gravity);
    //recompute speed and angle with new values of axial speeds (v = sqrt(v_h^2 + v_v^2) and angle = arctan(v_v/v_h))
    this->speed = sqrt(pow(this->h_speed, 2) + pow(this->v_speed, 2));
    this->angle = atan(-this->v_speed/this->h_speed) *180.0/M_PI;
}

SDL_Texture* Projectile::loadMedia(SDL_Renderer *renderer, const char* path, int width, int height){

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
    SDL_Texture *sprite = SDL_CreateTextureFromSurface(renderer, resizedSurface);
    SDL_FreeSurface(resizedSurface);
    SDL_FreeSurface(imageSurface);
    return sprite;
}

Bullet::Bullet(int x, int y, int speed, int angle, SDL_Renderer *renderer){

    this->x = x;
    this->y = y;
    this->speed = speed;
    this->angle = angle;
    this->h_speed = speed * cos(angle * M_PI/180.0);
    this->v_speed = speed * -sin(angle * M_PI/180.0);        //as height idices are defined from top to bottom
    this->mass = BULLET_MASS;
    this->cross_section_area = BULLET_CROSS_SECTIONAL_AREA;
    this->width = BULLET_WIDTH;
    this->height = BULLET_HEIGHT;
    this->sprite = Projectile::loadMedia(renderer, "./assets/assets/sprites/projectile_bullet.bmp", this->width, this->height);
}

int Bullet::get_radius_damage(){
    
    return SHOTGUN_RADIUS_DAMAGE;
}

Rocket::Rocket(int x, int y, int speed, int angle, SDL_Renderer *renderer){

    this->x = x;
    this->y = y;
    this->speed = speed;
    this->angle = angle;
    this->h_speed = speed * cos(angle * M_PI/180.0);
    this->v_speed = speed * -sin(angle * M_PI/180.0);        //as height idices are defined from top to bottom
    this->mass = ROCKET_MASS;
    this->cross_section_area = ROCKET_CROSS_SECTIONAL_AREA;
    this->width = ROCKET_WIDTH;
    this->height = ROCKET_HEIGHT;
    this->sprite = this->loadMedia(renderer, "./assets/assets/sprites/projectile_rocket.bmp", this->width, this->height);
}

int Rocket::get_radius_damage(){
    
    return BAZOOKA_RADIUS_DAMAGE;
}