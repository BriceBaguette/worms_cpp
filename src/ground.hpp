#include "defs.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
#include <list>
#include <algorithm>

class Ground
{
private:
    std::list<SDL_Point> points; 
    int calculateY(int x);
public:
    Ground();
    void render(SDL_Renderer *renderer);
    void update();
    std::list<SDL_Point> getPoints();
    void destroyPoints(std::list<SDL_Point> destroyed_zone);
    void close();
};

