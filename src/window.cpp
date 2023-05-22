#include "window.hpp"

WindowApp::WindowApp()
{
    // Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
        exit(-1);
    }
    this->worm1 = new Worm(this->renderer);
    this->ground = new Ground();
}

bool WindowApp::init()
{
    {
        // Initialization flag
        bool success = true;

        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            // Create window
            this->gWindow = SDL_CreateWindow("Worms", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
            if (gWindow == NULL)
            {
                printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
                success = false;
            }
            this->renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
        }

        if (TTF_Init() == -1)
        {
            exit(-1);
        }
        loadFont("./assets/assets/font/OpenSans-Bold.ttf", 28);
        this->timerText = createTextTexture(this->renderer, std::to_string((int) (timer/FRAME_RATE)),{255,0,0, 255}, 70,50);
        return success;
    }
}

void WindowApp::loadFont(const std::string &fontPath, int fontSize)
{
    gFont = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (gFont == nullptr)
    {
        exit(-1);
    }
}

SDL_Texture* WindowApp::createTextTexture(SDL_Renderer* renderer, const std::string& text, SDL_Color textColor, int surfaceWidth, int surfaceHeight)
{
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(gFont, text.c_str(), textColor, surfaceWidth);
    if (textSurface == nullptr)
    {
        std::cout << "Failed to render text surface. TTF Error: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    // Create a new surface with the desired width and height
    SDL_Surface* scaledSurface = SDL_CreateRGBSurfaceWithFormat(0, surfaceWidth, surfaceHeight, 32, textSurface->format->format);
    if (scaledSurface == nullptr)
    {
        std::cout << "Failed to create scaled surface. SDL Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return nullptr;
    }

    // Copy the text surface onto the scaled surface, scaling it if necessary
    SDL_Rect targetRect = {0, 0, surfaceWidth, surfaceHeight};
    SDL_BlitScaled(textSurface, nullptr, scaledSurface, &targetRect);

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, scaledSurface);
    if (textTexture == nullptr)
    {
        std::cout << "Failed to create texture from rendered text. SDL Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        SDL_FreeSurface(scaledSurface);
        return nullptr;
    }

    SDL_FreeSurface(textSurface);
    SDL_FreeSurface(scaledSurface);

    return textTexture;
}

void WindowApp::renderText(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y)
{
    SDL_Rect renderQuad = {x, y, 0, 0};
    SDL_QueryTexture(texture, nullptr, nullptr, &renderQuad.w, &renderQuad.h);
    SDL_RenderCopy(renderer, texture, nullptr, &renderQuad);
}

void WindowApp::close()
{
    this->ground->render(this->renderer);
    // Destroy renderer
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    // Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}

void WindowApp::render()
{
    // Clear the screen
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 255, 255);
    SDL_RenderClear(this->renderer);

    // Render the scene
    this->worm1->render(this->renderer);
    this->ground->render(this->renderer);
    this->renderText(this->renderer, this->timerText, 620,20);

    // Update the screen
    SDL_RenderPresent(this->renderer);
}

void WindowApp::update()
{
    this->timer--;
    this->timerText = createTextTexture(this->renderer, std::to_string((int)(this->timer/FRAME_RATE)), {255,0,0, 255}, 70,50);
    this->worm1->update(ground->getPoints());
}

void WindowApp::event()
{

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {

        switch (event.type)
        {
        case SDL_QUIT:
            this->quit = true;
            break;
        case SDL_KEYDOWN:
            // Keydown event
            // Check the specific key
            switch (event.key.keysym.sym)
            {
            case SDLK_LEFT:
                // Move image to the left
                this->worm1->setHSPeed(-WORM_SPEED_MODIFIER);
                break;
            case SDLK_RIGHT:
                // Move image to the right
                this->worm1->setHSPeed(WORM_SPEED_MODIFIER);
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            // Keyup event
            // Check the specific key
            switch (event.key.keysym.sym)
            {
            case SDLK_LEFT:
                this->worm1->setHSPeed(0);
                break;
            case SDLK_RIGHT:
                // Stop animation
                this->worm1->setHSPeed(0);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

bool WindowApp::getQuit()
{
    return this->quit;
}