#include "projectile.hpp"

void Projectile::render(SDL_Renderer *renderer){  

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_Rect destinationRect = {(int) this->center_x - this->width/2, (int) this->center_y - this->height/2, this->width, this->height };
    // Set the position and dimensions of the image
    SDL_QueryTexture(this->sprite, nullptr, nullptr, &destinationRect.w, &destinationRect.h);
    if (this->flip)
        SDL_RenderCopyEx(renderer, this->sprite, nullptr, &destinationRect, -this->angle, nullptr, SDL_FLIP_HORIZONTAL);
    else
        SDL_RenderCopyEx(renderer, this->sprite, nullptr, &destinationRect, this->angle, nullptr, SDL_FLIP_NONE);
}

bool Projectile::update(){
    //update position accordingly to previous step speed
    this->center_x += this->h_speed / (double)FRAMERATE;
    this->center_y += this->v_speed / (double)FRAMERATE;
    //if the projectile steps out of the screen, it will be destroyed;
    if (this->center_x - this->width/2 < 0 || this->center_x+this->width/2>=SCREEN_WIDTH)
        return false;
    if (this->center_y - this->height/2 < 0 || this->center_y+this->height/2>=SCREEN_WIDTH)
        return false;

    //compute vertical speed variation accordingly to gravity (dv = a*dt)
    double dy_gravity =  GRAVITY/(double)FRAMERATE;
    //compute speed variation due to drag forces (dv = F/m * dt)
    int mod = -1.;
    if (this->flip)
        mod = 1.;
    double drag_force = -1/2. * AIR_DENSITY * this->cross_section_area * DRAG_COEFFICIENT * pow(this->speed, 2);
    double dx_drag = mod * drag_force*cos(this->angle * M_PI/180.0) / (double)this->mass * 1/(double)FRAMERATE;
    double dy_drag = drag_force*-sin(this->angle * M_PI/180.0) / (double)this->mass * 1/(double)FRAMERATE;
    //apply those variations to the axial speeds
    this->h_speed += dx_drag;
    this->v_speed += dy_drag + dy_gravity;
    //recompute speed and angle with new values of axial speeds (v = sqrt(v_h^2 + v_v^2) and angle = arctan(v_v/v_h))
    this->speed = sqrt(pow(this->h_speed, 2) + pow(this->v_speed, 2));

    double angle = atan(-this->v_speed/this->h_speed) *180.0/M_PI;
    if(!this->flip){
        angle *= -1;
    }
    this->angle = angle;
    
    return true;
}

bool Projectile::checkCollision(const std::vector<SDL_Point>& points){

    SDL_Rect projectile_hitbox;
    if((this->angle>45 && this->angle<135) || (this->angle<-45 && this->angle>-135))    //in that case the hit box is rotated by 90 degrees to be closer to the true orientation of the projectile
        projectile_hitbox = {(int)(this->center_x-this->height/2.), (int)(this->center_y-this->width/2.), this->height, this->width};
    else
        projectile_hitbox = {(int)(this->center_x-this->width/2.), (int)(this->center_y-this->height/2.), this->width, this->height};

    for (const SDL_Point& point : points) {
        if (point.x >= projectile_hitbox.x && point.x < (projectile_hitbox.x + projectile_hitbox.w)&&
            point.y >= projectile_hitbox.y && point.y < (projectile_hitbox.y + projectile_hitbox.h)) {
            return true;  // Collision detected
        }
    }
    return false;  // No collision
}

bool Projectile::checkCollision(const SDL_Rect& hitbox){

    SDL_Rect projectile_hitbox;
    if((this->angle>45 && this->angle<135) || (this->angle<-45 && this->angle>-135))    //in that case the hit box is rotated by 90 degrees to be closer to the true orientation of the projectile
        projectile_hitbox = {(int)this->center_x-this->height/2, (int)this->center_y-this->width/2, this->height, this->width};
    else
        projectile_hitbox = {(int)this->center_x-this->width/2, (int)this->center_y-this->height/2, this->width, this->height};

    return SDL_HasIntersection(&projectile_hitbox, &hitbox);
}

int Projectile::getDamage(){
    return this->damage_points;
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

void Projectile::computeStartingPosition(std::tuple<bool, double, SDL_Rect, SDL_Rect>& fire_params){
    //Define where the starting position of the projectile should be considering the angle, worm and weapon positions
    this->flip = std::get<0>(fire_params);
    this->angle = std::get<1>(fire_params);
    SDL_Rect worm_rect = std::get<2>(fire_params);
    SDL_Rect weapon_rect = std::get<3>(fire_params);

    double worm_center_x = worm_rect.x + worm_rect.w/2.;
    double worm_center_y = worm_rect.y + worm_rect.h/2.;
    double weapon_center_x = weapon_rect.x + weapon_rect.w/2.;
    double weapon_center_y = weapon_rect.y + weapon_rect.h/2.;

    if (this->angle == 0){
        if (this->flip){
            this->center_x = (double) worm_center_x + worm_rect.w/2. + this->width/2.;
            this->center_y = weapon_center_y;
        }
        else{
            this->center_x = (double) worm_center_x - worm_rect.w/2. + this->width/2.;
            this->center_y = weapon_center_y;
        }
        return;
    }
    if (abs(this->angle) == 90){

        int mod = 1.;
        if (this->angle > 0)
            mod = -1.;
        
        this->center_x = weapon_center_x;
        this->center_y = (double) worm_center_y + mod * worm_rect.h/2. + mod * this->width/2.;

        return;
    }

    int mod = -1;
    if (this->flip)
        mod = 1;

    double slope = (mod*-1.) * tan(this->angle * M_PI/180.0);
    double c_x = weapon_center_x + mod*1;
    bool ok = false;

    while(!ok){

        double c_y = slope * (c_x - weapon_center_x) +  weapon_center_y;
        this->center_x = c_x;
        this->center_y = c_y;
        if (!this->checkCollision(worm_rect)){
            ok = true;
        }

        c_x += mod * 1;
    }
}

void Projectile::setExplosionZoneTemplate(int explosion_radius){
    
    std::vector<SDL_Point> explosion_zone_template;
    if(explosion_radius == 0)
        this->explosion_zone_template = explosion_zone_template;

    else {

        for (int i=-explosion_radius+1; i<explosion_radius-1; i++){
            for (int j=-explosion_radius+1; j<explosion_radius-1; j++){

                double dist_center = sqrt((double)(i*i + j*j));
                if(dist_center+0.5<explosion_radius)
                    explosion_zone_template.push_back({i, j});
            }
        }

        this->explosion_zone_template = explosion_zone_template;
    }
}

Bullet::Bullet(std::tuple<bool, double, SDL_Rect, SDL_Rect>& fire_params, double speed, SDL_Renderer *renderer){
    
    
    this->width = BULLET_WIDTH;
    this->height = BULLET_HEIGHT;
    this->computeStartingPosition(fire_params);
    this->speed = speed;
    double mod = 1.;
    if(!this->flip)
        mod = -1.;
    this->h_speed = mod * this->speed * cos(this->angle * M_PI/180.0);
    this->v_speed = this->speed * -sin(this->angle * M_PI/180.0);        //as height idices are defined from top to bottom
    this->mass = BULLET_MASS;
    this->cross_section_area = BULLET_CROSS_SECTIONAL_AREA;
    this->sprite = this->loadMedia(renderer, "./assets/assets/sprites/projectile_bullet.bmp", this->width, this->height);
    this->damage_points = SHOTGUN_DAMAGE_POINTS;
    this->explosion_radius = SHOTGUN_RADIUS_DAMAGE;
    this->setExplosionZoneTemplate(this->explosion_radius);
}

std::vector<SDL_Point> Bullet::getExplosionZone(){
    
    std::vector<SDL_Point> vector;
    return vector;
}

Rocket::Rocket(std::tuple<bool, double, SDL_Rect, SDL_Rect>& fire_params, double speed, SDL_Renderer *renderer){

    this->width = ROCKET_WIDTH;
    this->height = ROCKET_HEIGHT;
    this->computeStartingPosition(fire_params);
    this->speed = speed;
    double mod = 1.;
    if(!this->flip)
        mod = -1.;
    this->h_speed = mod * this->speed * cos(this->angle * M_PI/180.0);
    this->v_speed = this->speed * -sin(this->angle * M_PI/180.0);        //as height idices are defined from top to bottom
    this->mass = ROCKET_MASS;
    this->cross_section_area = ROCKET_CROSS_SECTIONAL_AREA;
    this->sprite = this->loadMedia(renderer, "./assets/assets/sprites/projectile_rocket.bmp", this->width, this->height);
    this->damage_points = BAZOOKA_DAMAGE_POINTS;
    this->explosion_radius = BAZOOKA_RADIUS_DAMAGE;
    this->setExplosionZoneTemplate(this->explosion_radius);
}

std::vector<SDL_Point> Rocket::getExplosionZone(){

    std::vector<SDL_Point> vector;

    for (const auto& point :  this->explosion_zone_template)
    {
        vector.push_back({point.x+(int)this->center_x, point.y+(int)this->center_y});
    }

    return vector;
}