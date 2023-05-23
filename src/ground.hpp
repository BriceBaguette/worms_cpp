#ifndef GROUND
#define GROUND

#include "defs.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

class Ground
{
private:
    std::vector<SDL_Point> points;

    int calculateY(int x);
    void addPlateform(int x, int y);

public:
    Ground();

    virtual ~Ground(){
        points.clear();
    };
    
    std::vector<SDL_Point> getPoints();
    virtual void render(SDL_Renderer* renderer);
    virtual void destroyPoints(std::vector<SDL_Point>& destroyed_zone);
};

#endif