#include "ground.hpp"

Ground::Ground()
{
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        for(int y = calculateY(x); y<SCREEN_HEIGHT; y++){
            this->points.push_back({x, y});
        }
    }
}

int Ground::calculateY(int x)
{
    // Amplitude of the sine wave
    double amplitude = 20.0;

    // Frequency of the sine wave
    double frequency = 5.0 / SCREEN_WIDTH;

    // Phase shift of the sine wave
    double phaseShift = 0.0;

    return GROUND_HEIGHT + static_cast<int>(amplitude * sin(frequency * x * 2 * M_PI + phaseShift));
}

void Ground::render(SDL_Renderer *renderer)
{
    // Set the drawing color to white
    SDL_SetRenderDrawColor(renderer, 128, 96, 20, 255);
    SDL_RenderDrawPoints(renderer, &this->points[0], this->points.size());
}
std::vector<SDL_Point> Ground::getPoints(){
    return this->points;
}

void Ground::destroyPoints(std::list<SDL_Point> destroyed_zone){
    for (auto& point : destroyed_zone)
        this->points.remove_if([point](SDL_Point p){ return (p.x==point.x && p.y==point.y); });
}