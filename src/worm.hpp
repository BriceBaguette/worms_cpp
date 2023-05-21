#include<SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include "defs.hpp"
#include <list>
#include <map>
#include <algorithm>

class Worm {
    private:
        int x = 200;
        int y = 200;
        SDL_Texture* restSprite=NULL;
        std::vector<SDL_Texture*> movingSprite;
        SDL_Texture* fallingSprite = NULL;
        SDL_Texture* jetpackSprite = NULL;
        SDL_Texture* bazookaSprite = NULL;
        SDL_Texture* shotgunSprite = NULL;
        int vSpeed = WORM_FALLING_SPEED;
        int hSpeed = 0;       
        bool flip = false;
        int health = WORM_INITIAL_HEALTH;
        bool aiming = false;
        bool aiming_upwards = false;
        std::string weapon = "bazooka";
        std::map<std::string, int> weapon_amunition_map{{"bazooka", BAZOOKA_INITIAL_AMMO}, {"gun", SHOTGUN_INITIAL_AMMO}};
        std::map<std::string, int> weapon_reload_time_map{{"bazooka", 0}, {"gun", 0}};
        double weapon_angle = 0.;
    
    public:

        Worm(SDL_Renderer *renderer);

        // Function to load sprite        
        void loadAll(SDL_Renderer *renderer);
        SDL_Texture* loadMedia(SDL_Renderer *renderer,const char* path, int width, int height);

        // Function that manage the game behaviour and display of the worm
        void render(SDL_Renderer *renderer);
        void update(const std::list<SDL_Point>& points);

        // Function to control worm's behaviour
        bool checkCollision(const std::list<SDL_Point>& points);
        bool isWeaponReady();
        int getWeaponAmunition();
        double getWeaponAngle();
        std::string getWeapon();
        void setWeapon(const std::string& weapon);
        int setDamage(int damage);
        void setHSPeed(int speed);
        void setVSPeed(int speed);
        void setAiming(bool aiming, bool upwards);
        void fire();
        void close();
};