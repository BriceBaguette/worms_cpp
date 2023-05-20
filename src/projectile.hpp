#ifndef PROJECTILE
#define PROJECTILE

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cmath>
#include "defs.hpp"

class Projectile{

    public:
        virtual int get_radius_damage() = 0;
        virtual void render(SDL_Renderer *renderer);
        virtual void update();

    protected:
        double x;
        double y;
        double speed;
        double angle;
        double h_speed;
        double v_speed;
        int mass;
        int cross_section_area;
        int width;
        int height;
        SDL_Texture* sprite;

        virtual SDL_Texture* loadMedia(SDL_Renderer *renderer, const char* path, int width, int height);
};

class Bullet : public Projectile{

    public:
        Bullet(int x, int y, int speed, int angle, SDL_Renderer *renderer);
        int get_radius_damage() override;
};

class Rocket : public Projectile{

    public:
        Rocket(int x, int y, int speed, int angle, SDL_Renderer *renderer);
        int get_radius_damage() override;
};

#endif