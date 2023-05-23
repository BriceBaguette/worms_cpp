#include "environment.hpp"

Environment::Environment()
{
}

std::vector<SDL_Point> Environment::getPoints()
{
    return this->points;
}

void Environment::render(SDL_Renderer* renderer)
{
    // Set the drawing color to white
    SDL_SetRenderDrawColor(renderer, 128, 96, 20, 255);
    SDL_RenderDrawPoints(renderer, &points[0], points.size());
}

void Environment::destroyPoints(std::vector<SDL_Point>& destroyed_zone)
{
    points.erase(std::remove_if(points.begin(), points.end(),
        [&](const SDL_Point& p) {
            for (const auto& point : destroyed_zone) {
                if (p.x == point.x && p.y == point.y) {
                    return true;
                }
            }
            return false;
        }),
        points.end()
    );
}

void Environment::close()
{
    points.clear();
}

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

Platform::Platform(int x, int y)
{
    for (int i = 0; i < PLATFORM_WIDTH; i++)
    {
        for (int j = 0; j < PLATFORM_HEIGHT; j++)
        {
            this->points.push_back({x + i, y + j});
        }
    }
}
