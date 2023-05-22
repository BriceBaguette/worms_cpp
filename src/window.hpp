#include <SDL2/SDL.h>
#include "worm.hpp"
#include "defs.hpp"
#include "ground.hpp"
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

    SDL_Texture* player1Health = nullptr;

    SDL_Texture* player2Health = nullptr;

    SDL_Texture* player1Name = nullptr;

    SDL_Texture* player2name = nullptr;

    Ground* ground = NULL;

    bool quit = false;

    Worm* worm1 = NULL;

    bool init();
    void loadFont(const std::string& fontPath, int fontSize);
    void renderText(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y);
    SDL_Texture* createTextTexture(SDL_Renderer *renderer, const std::string &text, SDL_Color textColor, int surfaceWidth, int surfaceHeight);
    

public:
    WindowApp();
    void update();
    void close();
    void render();
    void event();
    bool getQuit();
};



