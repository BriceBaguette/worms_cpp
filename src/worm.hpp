#ifndef WORM
#define WORM

#include<SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include "defs.hpp"
#include <map>
#include <tuple>
#include <algorithm>

class Worm {
    private:
        int x;
        int y;
        SDL_Texture* restSprite=NULL;
        std::vector<SDL_Texture*> movingSprite;
        SDL_Texture* fallingSprite = NULL;
        SDL_Texture* jetpackSprite = NULL;
        SDL_Texture* bazookaSprite = NULL;
        SDL_Texture* shotgunSprite = NULL;
        double vSpeed = WORM_FALLING_SPEED;
        double hSpeed = 0;       
        bool flip = false;
        int frames_before_fall = (int)(FALLING_STARTING_TIME * FRAMERATE);
        int health = WORM_INITIAL_HEALTH;
        bool aiming = false;
        bool aiming_upwards = false;
        std::string weapon = "bazooka";
        std::map<std::string, int> weapon_amunition_map{{"bazooka", BAZOOKA_INITIAL_AMMO}, {"gun", SHOTGUN_INITIAL_AMMO}};
        std::map<std::string, int> weapon_reload_time_map{{"bazooka", 0}, {"gun", 0}};
        double weapon_angle = 0.;

        SDL_Rect getCollisionHitbox();
        double checkHorizontalCollision(const std::list<SDL_Point>& points, const SDL_Rect other_worm_hitbox);
        double checkVerticalCollision(const std::list<SDL_Point>& points, const SDL_Rect other_worm_hitbox);
        double hasSteppedInVoid(const std::list<SDL_Point>& points);
        void climbSlope(const std::list<SDL_Point>& points, SDL_Rect collision_box);
    
    public:

        Worm(SDL_Renderer *renderer,int x,int y);

        // Function to load sprite        
        void loadAll(SDL_Renderer *renderer);
        SDL_Texture* loadMedia(SDL_Renderer *renderer,const char* path, int width, int height);

        // Function that manage the game behaviour and display of the worm
        void render(SDL_Renderer *renderer);
        void update(const std::list<SDL_Point>& points, const SDL_Rect other_worm_hitbox);

        // Function to control worm's behaviour
        bool checkCollision(const std::vector<SDL_Point>& points);
        SDL_Rect getHitbox();
        int getHealth();
        bool isWeaponReady();
        int getWeaponAmunition();
        double getWeaponAngle();
        std::string getWeapon();
        double getVSpeed();
        void setWeapon(const std::string& weapon);
        int setDamage(int damage);
        void setHSPeed(int speed);
        void setVSPeed(int speed);
        void setAiming(bool aiming, bool upwards);
        std::tuple<bool, double, SDL_Rect, SDL_Rect> fire();
        void close();
};

#endif