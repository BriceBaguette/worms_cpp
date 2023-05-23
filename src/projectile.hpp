#ifndef PROJECTILE
#define PROJECTILE

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cmath>
#include <tuple>
#include <vector>
#include "defs.hpp"

class Projectile{

    public:
        virtual std::vector<SDL_Point> getExplosionZone() = 0;
        virtual void render(SDL_Renderer *renderer);
        virtual bool update();
        virtual bool checkCollision(const std::vector<SDL_Point>& points);
        virtual bool checkCollision(const SDL_Rect& hitbox);
        virtual int getDamage();

        virtual ~Projectile() {
        SDL_DestroyTexture(sprite);
        }

    protected:
        double center_x;
        double center_y;
        double speed;
        double angle;
        bool flip;
        double h_speed;
        double v_speed;
        int mass;
        int cross_section_area;
        int width;
        int height;
        SDL_Texture* sprite;
        int damage_points;
        int explosion_radius;
        std::vector<SDL_Point> explosion_zone_template;

        virtual SDL_Texture* loadMedia(SDL_Renderer *renderer, const char* path, int width, int height);
        virtual void computeStartingPosition(std::tuple<bool, double, SDL_Rect, SDL_Rect>& fire_params);
        virtual void setExplosionZoneTemplate(int explosion_radius);
};

class Bullet : public Projectile{

    public:
        Bullet(std::tuple<bool, double, SDL_Rect, SDL_Rect>& , double speed, SDL_Renderer *renderer);
        std::vector<SDL_Point> getExplosionZone() override;
};

class Rocket : public Projectile{

    public:
        Rocket(std::tuple<bool, double, SDL_Rect, SDL_Rect>& fire_params, double speed, SDL_Renderer *renderer);
        std::vector<SDL_Point> getExplosionZone() override;
};

#endif