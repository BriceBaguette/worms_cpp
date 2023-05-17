#include "window.hpp"

WindowApp::WindowApp()
{
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Apply the image
            SDL_BlitSurface( gSprite, NULL, gScreenSurface, NULL );
            //Update the surface
            SDL_UpdateWindowSurface( gWindow );
            //Hack to get window to stay up
            SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }
        }
    }

    //Free resources and close SDL
    close();
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
            gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
            if (gWindow == NULL)
            {
                printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                // Get window surface
                gScreenSurface = SDL_GetWindowSurface(gWindow);
            }
        }

        return success;
    }
}

bool WindowApp::loadMedia()
{
    // Loading success flag
    bool success = true;

    // Load splash image
    gSprite = SDL_LoadBMP("./assets/assets/sprites/worm_rest.bmp");
    if (gSprite == NULL)
    {
        printf("Unable to load image %s! SDL Error: %s\n", "./assets/assets/sprites/worm_rest.bmp", SDL_GetError());
        success = false;
    }

    return success;
}

void WindowApp::close()
{
    //Deallocate surface
    SDL_FreeSurface( gSprite );
    gSprite = NULL;

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}
