#include <SDL2/SDL.h>
#include "worm.hpp"
#include "defs.hpp"
#include "environment.hpp"
#include "projectile.hpp"
#include "vector"
#include <SDL2/SDL_ttf.h>

class WindowApp
{
private:
    //The window we'll be rendering to
    SDL_Window* gWindow = NULL;
        
    //The surface contained by the window
    SDL_Renderer* renderer = NULL;

    TTF_Font* gFont = nullptr;

    double timer = TIMER;

    SDL_Texture* timerText = nullptr;

    SDL_Color* power_bar_empty_color;

    SDL_Color* power_bar_full_color;

    SDL_Texture* player1Health = nullptr;

    SDL_Texture* player2Health = nullptr;

    SDL_Texture* player1Name = nullptr;

    SDL_Texture* player2Name = nullptr;

    Ground* ground = NULL;

    Platform* platform1 = NULL;

    Platform* platform2 = NULL;

    bool quit = false;

    Worm* worm1 = NULL;

    Worm* worm2 = NULL;

    Worm* curr_worm = NULL;

    bool curr_worm_in_air = false;

    bool curr_worm_shooting = false;
    
    bool curr_worm_has_aimed = false;

    int shooting_power = MIN_SHOOTING_POWER;

    Projectile* curr_projectile = nullptr;

    void explodeProjectile(bool hit);


    bool init();
    void loadFont(const std::string& fontPath, int fontSize);
    void renderText(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y);
    void renderBar();
    SDL_Texture* createTextTexture(SDL_Renderer *renderer, const std::string &text, SDL_Color textColor, int surfaceWidth, int surfaceHeight);
    

public:
    WindowApp();
    void update();
    void close();
    void render();
    void event();
    bool getQuit();
};



