#include "defs.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

class Environment
{
protected:
    std::vector<SDL_Point> points; 

public:
    Environment();
    
    std::vector<SDL_Point> getPoints();
    virtual void render(SDL_Renderer* renderer);
    virtual void destroyPoints(std::vector<SDL_Point>& destroyed_zone);
    virtual void close();
};

class Ground: public Environment
{
private:
    int calculateY(int x);
public:
    Ground();
};

class Platform: public Environment
{
public:
    Platform(int x, int y);
};

