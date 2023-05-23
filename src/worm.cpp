#include "worm.hpp"

Worm::Worm(SDL_Renderer *renderer, int xCoord,int yCoord) :  x(xCoord), y(yCoord) 
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

int Worm::getHealth(){
    return this->health;
}

double Worm::getVSpeed(){
    return this->vSpeed;
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

std::tuple<bool, double, SDL_Rect, SDL_Rect> Worm::fire(){
    double angle = this->weapon_angle;
    SDL_Rect worm_rect = {(int)this->x, (int)this->y, WORM_WIDTH, WORM_HEIGHT };
    int mod = this->flip ? -1. : 1.;
    double weapon_center_x = this->x + WORM_WIDTH/2. + mod*WORM_WEAPON_CENTERS_DISTANCE_X;
    double weapon_center_y = this->y + WORM_HEIGHT/2. + WORM_WEAPON_CENTERS_DISTANCE_Y;
    int weapon_x = (int)(weapon_center_x - WEAPON_WIDTH/2.);
    int weapon_y = (int)(weapon_center_y - WEAPON_HEIGHT/2.);
    SDL_Rect weapon_rect = {weapon_x, weapon_y, WEAPON_WIDTH, WEAPON_HEIGHT };
    std::tuple<bool, double, SDL_Rect, SDL_Rect> ret_val = {this->flip, angle, worm_rect, weapon_rect};

    this->weapon_amunition_map[this->weapon]--;
    this->weapon_angle = 0;
    if (!this->weapon.compare("bazooka"))
        this->weapon_reload_time_map[weapon] = BAZOOKA_RELOADING_DELAY * FRAMERATE;
    else
        this->weapon_reload_time_map[weapon] = SHOTGUN_RELOADING_DELAY * FRAMERATE;

    return ret_val;
}

bool Worm::isWeaponReady(){
    if(this->weapon_reload_time_map[weapon] == 0)
        return true;
    return false;
}

bool Worm::checkCollision(const std::vect<SDL_Point>& points) {
    SDL_Rect rect = this->getCollisionHitbox();
    for (const SDL_Point& point : points) {
        if (point.x >= rect.x && point.x < (rect.x + rect.w)&&
            point.y >= rect.y && point.y < (rect.y + rect.h)) {
            return true;  // Collision detected
        }
    }
    return false;  // No collision
}

SDL_Rect Worm::getHitbox(){
    SDL_Rect rect = {(int)this->x, (int)this->y, WORM_WIDTH, WORM_HEIGHT};
    return rect;
}

SDL_Rect Worm::getCollisionHitbox(){
    int x = this->x;
    if(this->flip){
        x += 10;
    }
    SDL_Rect rect = {x, (int)this->y+10, WORM_WIDTH-10, WORM_HEIGHT-15};
    return rect;
}

double Worm::checkHorizontalCollision(const std::list<SDL_Point>& points, const SDL_Rect other_worm_hitbox){
    if(this->hSpeed==0 || (this->hSpeed < 0 && this->x == 0) || (this->hSpeed>0 && this->x + WORM_WIDTH == SCREEN_WIDTH))
        return 0;

    int mod = this->flip ? 1 : -1;
    SDL_Rect box = this->getCollisionHitbox();
    box.x += mod * -1;
    for (const SDL_Point& point : points)
        if (point.x >= box.x && point.x < (box.x + box.w)&&
            point.y >= box.y && point.y < (box.y + box.h))
                return 0;
    box.x -= mod * -1;

    double dx = this->hSpeed / (double)FRAMERATE;
    box.x += mod * -1 * dx;
    bool ok = false;
    bool ground_collision = true;
    bool worm_collision = true;
    bool screen_border_collision = true;

    while(!ok && (abs(dx) <= SCREEN_WIDTH)){
        ok = true;
        if (ground_collision){
            ground_collision = false;
            for (const SDL_Point& point : points) {
                if (point.x >= box.x && point.x < (box.x + box.w)&&
                    point.y >= box.y && point.y < (box.y + box.h)) {
                        box.x += mod * -1;
                        dx += mod * -1;
                        ok = false;
                        ground_collision = true;
                        break;
                }
            }
        }
        
        if (worm_collision){
            worm_collision = false;
            if (SDL_HasIntersection(&box, &other_worm_hitbox)){
                box.x += mod * -1;
                dx += mod * -1;
                ok = false;
                worm_collision = true;
            }
        }

        if (screen_border_collision){
            screen_border_collision = false;
            if (box.x < 0 || box.x + WORM_WIDTH > SCREEN_WIDTH){
                box.x += mod * -1;
                dx += mod * -1;
                ok = false;
                screen_border_collision = true;
            }
        }
    }

    if (ok){
        climbSlope(points, box);
        return dx;
    }
    else{
        this->x = 0;
        this->y = 0;
        return 0;
    }
}

void Worm::climbSlope(const std::list<SDL_Point>& points, SDL_Rect collision_box){
    SDL_Rect margin_box = {collision_box.x, collision_box.y+collision_box.h, collision_box.w, WORM_HEIGHT - (collision_box.y-(int)this->y) - collision_box.h};
    int dy = 0;
    bool ok = false;
    while(!ok){
        ok = true;
        for (const SDL_Point& point : points) {
            if (point.x >= margin_box.x && point.x < (margin_box.x + margin_box.w)&&
                point.y >= margin_box.y && point.y < (margin_box.y + margin_box.h)) {
                    collision_box.y -= 1;
                    margin_box.y -= 1;
                    ok = false;
                    break;
            }
        }
        if (!ok){
            for (const SDL_Point& point : points) {
                if (point.x >= collision_box.x && point.x < (collision_box.x + collision_box.w)&&
                    point.y >= collision_box.y && point.y < (collision_box.y + collision_box.h)) {
                        this->y += dy;
                        return;
                }
            }
        }
        dy -= 1;
    }
}

double Worm::checkVerticalCollision(const std::list<SDL_Point>& points, const SDL_Rect other_worm_hitbox){
    if(this->vSpeed==0 || (this->vSpeed < 0 && this->y == 0) || (this->vSpeed > 0 && this->y + WORM_HEIGHT == SCREEN_HEIGHT))
        return 0;

    int mod = this->vSpeed>0 ? 1 : -1;
    SDL_Rect collision_box = this->getCollisionHitbox();
    SDL_Rect box = this->getHitbox();
    SDL_Rect use_box = mod>0 ? box : collision_box;

    double dy = this->vSpeed / (double)FRAMERATE;
    use_box.y += dy;
    bool ok = false;
    bool ground_collision = true;
    bool worm_collision = true;
    bool screen_border_collision = true;

    while(!ok && (abs(dy) <= SCREEN_HEIGHT)){
        ok = true;
        if (ground_collision){
            ground_collision = false;
            for (const SDL_Point& point : points) {
                if (point.x >= use_box.x && point.x < (use_box.x + use_box.w)&&
                    point.y >= use_box.y && point.y < (use_box.y + use_box.h)) {
                        use_box.y += mod * -1;
                        dy += mod * -1;
                        ok = false;
                        ground_collision = true;
                        break;
                }
            }
        }

        if (worm_collision){
            worm_collision = false;
            if (SDL_HasIntersection(&use_box, &other_worm_hitbox)){
                use_box.y += mod * -1;
                dy += mod * -1;
                ok = false;
                worm_collision = true;
            }
        }

        if (screen_border_collision){
            screen_border_collision = false;
            if (box.y < 0 || box.y + WORM_HEIGHT > SCREEN_HEIGHT){
                box.y += mod * -1;
                dy += mod * -1;
                ok = false;
                screen_border_collision = true;
            }
        }
    }

    if (!ok){
        this->x = 0;
        this->y = 0;
        return 0;
    }
    
    return dy;
}

double Worm::hasSteppedInVoid(const std::list<SDL_Point>& points){
    SDL_Rect collision_box = getCollisionHitbox();
    SDL_Rect margin_box = {collision_box.x, collision_box.y+collision_box.h, collision_box.w, WORM_HEIGHT - (collision_box.y-(int)this->y) - collision_box.h};
    double dy = 0;
    bool ok = true;
    while(ok && dy <= WORM_FALLING_SPEED/(double)FRAMERATE){
        for (const SDL_Point& point : points) {
            if (point.x >= margin_box.x && point.x < (margin_box.x + margin_box.w)&&
                point.y >= margin_box.y && point.y < (margin_box.y + margin_box.h)) {
                    ok = false;
                    break;
            }
        }
        if (ok){
            dy++;
            margin_box.y++;
        }
    }

    if (!ok)  
        return (--dy);
    return WORM_FALLING_SPEED/(double)FRAMERATE;
}

void Worm::update(const std::list<SDL_Point>& points, const SDL_Rect other_worm_hitbox){
    if (this->hSpeed != 0){
        double dx = checkHorizontalCollision(points, other_worm_hitbox);
        if (this->vSpeed>=0){
            this->x += dx;
        }
    }

    if (this->vSpeed > 0){
        if (this->frames_before_fall>0)
            this->frames_before_fall--;
    }
    else
        this->frames_before_fall = (int)(FALLING_STARTING_TIME * FRAMERATE);

    if (this->vSpeed != 0){
        double dy = checkVerticalCollision(points, other_worm_hitbox);
        this->y += dy;
        if (this->vSpeed>0 && dy<this->vSpeed/(double)FRAMERATE){
            this->vSpeed = 0;
        }
    }
    else {
        double dy = hasSteppedInVoid(points);
        this->y += dy;
        if (dy > 0 && dy == WORM_FALLING_SPEED/(double)FRAMERATE)
            this->vSpeed = WORM_FALLING_SPEED;
    }

    if (this->weapon_reload_time_map["bazooka"] > 0)
        this->weapon_reload_time_map["bazooka"]--;
    if (this->weapon_reload_time_map["gun"] > 0)
        this->weapon_reload_time_map["gun"]--;

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
    SDL_Rect destinationRect = {(int)this->x, (int)this->y, WORM_WIDTH, WORM_HEIGHT };
    SDL_Texture* sprite = NULL;
    SDL_Texture* weaponSprite = NULL;
    SDL_Rect weapon_destination_rect;
    bool show_weapon = false;
    // Set the position and dimensions of the image
    if(this->hSpeed == 0 && (this->vSpeed == 0 || (this->vSpeed > 0 && this->frames_before_fall > 0))){
        sprite = this->restSprite;
        show_weapon = true;
    }
    if(this->hSpeed != 0) {
        if(this->hSpeed > 0)
            this->flip = true;
        else    
            this->flip = false;
        sprite = this->movingSprite.front();
        if(this->vSpeed != WORM_FALLING_SPEED && this->vSpeed != WORM_JETPACK_SPEED)
            show_weapon = true;
        std::reverse(this->movingSprite.begin(), this->movingSprite.end());
    }

    if(this->vSpeed > 0 && this->frames_before_fall == 0){
        sprite = this->fallingSprite;
    }

    if(this->vSpeed < 0){
        sprite = this->jetpackSprite;
    }

    if (show_weapon){
        if(!this->weapon.compare("bazooka"))
            weaponSprite = this->bazookaSprite;
        else
            weaponSprite = this->shotgunSprite;

        int mod = this->flip ? -1. : 1.;

        double weapon_center_x = this->x + WORM_WIDTH/2. + mod*WORM_WEAPON_CENTERS_DISTANCE_X;
        double weapon_center_y = this->y + WORM_HEIGHT/2. + (double)WORM_WEAPON_CENTERS_DISTANCE_Y;
        int weapon_x = (int)(weapon_center_x - WEAPON_WIDTH/2.);
        int weapon_y = (int)(weapon_center_y - WEAPON_HEIGHT/2.);
        weapon_destination_rect = {weapon_x, weapon_y, WEAPON_WIDTH, WEAPON_HEIGHT };

        SDL_QueryTexture(weaponSprite, nullptr, nullptr, &weapon_destination_rect.w, &weapon_destination_rect.h);
    }

    if(this->vSpeed < 0){
        sprite = this->jetpackSprite;
    }

    if (show_weapon){
        if(!this->weapon.compare("bazooka"))
            weaponSprite = this->bazookaSprite;
        else
            weaponSprite = this->shotgunSprite;

        int mod = this->flip ? -1. : 1.;

        double weapon_center_x = this->x + WORM_WIDTH/2. + mod*WORM_WEAPON_CENTERS_DISTANCE_X;
        double weapon_center_y = this->y + WORM_HEIGHT/2. + (double)WORM_WEAPON_CENTERS_DISTANCE_Y;
        int weapon_x = (int)(weapon_center_x - WEAPON_WIDTH/2.);
        int weapon_y = (int)(weapon_center_y - WEAPON_HEIGHT/2.);
        weapon_destination_rect = {weapon_x, weapon_y, WEAPON_WIDTH, WEAPON_HEIGHT };

        SDL_QueryTexture(weaponSprite, nullptr, nullptr, &weapon_destination_rect.w, &weapon_destination_rect.h);
    }

    SDL_QueryTexture(sprite, nullptr, nullptr, &destinationRect.w, &destinationRect.h);
    if(this->flip){
        SDL_RenderCopyEx(renderer, sprite, nullptr, &destinationRect, 0, nullptr, SDL_FLIP_HORIZONTAL);
        if(show_weapon)
            SDL_RenderCopyEx(renderer, 
                            weaponSprite, 
                            nullptr, 
                            &weapon_destination_rect, 
                            -this->weapon_angle, 
                            nullptr, 
                            SDL_FLIP_HORIZONTAL);
    }
    else{
        SDL_RenderCopyEx(renderer, sprite, nullptr, &destinationRect, 0, nullptr, SDL_FLIP_NONE);
        if(show_weapon)
            SDL_RenderCopyEx(renderer, 
                            weaponSprite, 
                            nullptr, 
                            &weapon_destination_rect, 
                            this->weapon_angle, 
                            nullptr, 
                            SDL_FLIP_NONE);
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

void Worm::close() {
    // Free the textures
    if (restSprite)
        SDL_DestroyTexture(restSprite);
    for (SDL_Texture* texture : movingSprite)
        SDL_DestroyTexture(texture);
    if (fallingSprite)
        SDL_DestroyTexture(fallingSprite);
    if (jetpackSprite)
        SDL_DestroyTexture(jetpackSprite);
    if (bazookaSprite)
        SDL_DestroyTexture(bazookaSprite);
    if (shotgunSprite)
        SDL_DestroyTexture(shotgunSprite);
}