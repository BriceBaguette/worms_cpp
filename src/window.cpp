#include "window.hpp"

WindowApp::WindowApp()
{
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
        exit(-1);
    }
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
            gWindow = SDL_CreateWindow("Worms", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
            if (gWindow == NULL)
            {
                printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
                success = false;
            }
            renderer = SDL_CreateRenderer(gWindow, -1, 0);
        }

        return success;
    }
}

void WindowApp::close()
{
    //Deallocate surface
    SDL_FreeSurface( gSprite );
    gSprite = NULL;

    //Destroy renderer
    SDL_DestroyRenderer( renderer );
    renderer = NULL;

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

void WindowApp::render()
{
    // Clear the screen
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->renderer);

    // Render the scene
    // ...

    // Update the screen
    SDL_RenderPresent(this->renderer);
}

void WindowApp::event(){
    SDL_Event e;  
    while( SDL_PollEvent( &e ) )
        { 
            if( e.type == SDL_QUIT ) 
                    this->quit = true;
        } 
}

bool WindowApp::getQuit(){
    return this->quit;
}