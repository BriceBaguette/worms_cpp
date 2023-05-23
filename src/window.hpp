#ifndef WINDOW
#define WINDOW

#include <SDL2/SDL.h>
#include "worm.hpp"
#include "defs.hpp"
#include "ground.hpp"
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

    SDL_Texture* timer_text = nullptr;
    SDL_Rect timer_rect = {TIMER_X, TIMER_Y, TIMER_WIDTH, TIMER_HEIGHT};
    SDL_Texture* player1_health = nullptr;
    SDL_Rect player1_health_rect = {PLAYER1_HEALTH_X, PLAYER1_HEALTH_Y, PLAYER_HEALTH_WIDTH, PLAYER_HEALTH_HEIGHT};
    SDL_Texture* player2_health = nullptr;
    SDL_Rect player2_health_rect = {PLAYER2_HEALTH_X, PLAYER2_HEALTH_Y, PLAYER_HEALTH_WIDTH, PLAYER_HEALTH_HEIGHT};
    SDL_Texture* player1_name = nullptr;
    SDL_Rect player1_name_rect = {PLAYER1_NAME_X, PLAYER1_NAME_Y, PLAYER_NAME_WIDTH, PLAYER_NAME_HEIGHT};
    SDL_Texture* player2_name = nullptr;
    SDL_Rect player2_name_rect = {PLAYER2_NAME_X, PLAYER2_NAME_Y, PLAYER_NAME_WIDTH, PLAYER_NAME_HEIGHT};
    SDL_Texture* bullet_sprite;
    SDL_Texture* rocket_sprite;

    Ground* ground = NULL;

    bool quit = false;
    bool hard_quit = false;

    Worm* worm1 = NULL;
    Worm* worm2 = NULL;
    Worm* curr_worm = NULL;
    bool curr_worm_shooting = false;
    bool curr_worm_has_aimed = false;
    Projectile* curr_projectile = nullptr;

    int shooting_power = MIN_SHOOTING_POWER;

    void explodeProjectile(bool hit1, bool hit2);
    bool init();
    void loadFont(const std::string& fontPath, int fontSize);
    SDL_Texture* loadMedia(SDL_Renderer *renderer, const char* path, int width, int height);
    void renderBar();
    void renderWormsAmunitions();
    SDL_Texture* createTextTexture(SDL_Renderer *renderer, const std::string &text, SDL_Color textColor);
    

public:
    WindowApp();
    void close();
    void update();
    void render();
    void event();
    bool getQuit();
    bool getHardQuit();
};

#endif