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
    this->curr_worm = this->worm1;
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
    if (this->curr_projectile!=nullptr)
        this->curr_projectile->render(this->renderer);

    // Update the screen
    SDL_RenderPresent(this->renderer);
}

void WindowApp::update()
{
    this->worm1->update(ground->getPoints());
    if(this->curr_projectile != nullptr){
        if (this->curr_projectile->update()){
            std::list<SDL_Point> explosion_zone = this->curr_projectile->getExplosionZone();
            this->ground->destroyPoints(explosion_zone);
            if (this->worm1->checkCollision(explosion_zone)){
                this->worm1->setDamage(this->curr_projectile->getDamage());
                if (this->worm1->getHealth() <= 0)
                    this->quit = true;
            }
            delete this->curr_projectile;
            this->curr_projectile = nullptr;
        }
        SDL_Rect hitbox = this->worm1->getHitbox();
        bool hit = this->curr_projectile->checkCollision(hitbox);
        if(hit || this->curr_projectile->checkCollision(ground->getPoints())){
            std::list<SDL_Point> explosion_zone = this->curr_projectile->getExplosionZone();
            this->ground->destroyPoints(explosion_zone);
            if (hit || this->worm1->checkCollision(explosion_zone)){
                this->worm1->setDamage(this->curr_projectile->getDamage());
                if (this->worm1->getHealth() <= 0)
                    this->quit = true;
            }
            delete this->curr_projectile;
            this->curr_projectile = nullptr;
        }
    }

    if(this->curr_worm_shooting && this->curr_worm_has_aimed){
        this->shooting_power += SHOOTING_POWER_STEP;
        if(this->shooting_power > MAX_SHOOTING_POWER)
            this->shooting_power = MIN_SHOOTING_POWER;
    }
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
            case SDLK_SPACE:
                if (!this->curr_worm_in_air){
                    //If the playing worm is using its jetpack or falling, it cannot shoot
                    if(!this->curr_worm_shooting && this->curr_worm->isWeaponReady() && this->curr_worm->getWeaponAmunition()>0){
                        //Starts the shooting process
                        this->curr_worm_shooting = true;
                    }
                    else if(this->curr_worm_shooting && !this->curr_worm_has_aimed){
                        //Starts the fire power gauging process
                        this->curr_worm_has_aimed = true;
                    }
                    else if(this->curr_worm_shooting && this->curr_worm_has_aimed){
                        //Finalize the shooting process
                        std::tuple<bool, double, SDL_Rect, SDL_Rect> fire_params = this->curr_worm->fire();
                        std::string weapon = this->curr_worm->getWeapon();
                        if(!weapon.compare("bazooka"))
                            this->curr_projectile = new Rocket(fire_params, this->shooting_power, this->renderer);
                        else
                            this->curr_projectile = new Bullet(fire_params, this->shooting_power, this->renderer);
                            
                        this->curr_worm_shooting = false;
                        this->curr_worm_has_aimed = false;
                    }
                }
                break;
            case SDLK_UP:
                if(this->curr_worm_shooting && !this->curr_worm_has_aimed){
                    //If at aiming step of shooting process, set current worm to aim upwards
                    this->curr_worm->setAiming(true, true);
                }
                break;
            case SDLK_DOWN:
                if(this->curr_worm_shooting && !this->curr_worm_has_aimed){
                    //If at aiming step of shooting process, set current worm to aim downwards
                    this->curr_worm->setAiming(true, false);
                }
                break;
            case SDLK_1:
                //Worm selects weapon 1 (bazooka)
                if(!this->curr_worm_shooting)
                    this->curr_worm->setWeapon("bazooka");
                break;
            case SDLK_2:
                //Worm selects weapon 2 (shotgun)
                if(!this->curr_worm_shooting)
                    this->curr_worm->setWeapon("gun");
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
            case SDLK_UP:
                if(this->curr_worm_shooting && !this->curr_worm_has_aimed){
                    //If at aiming step of shooting process, set current worm to stop aiming upwards
                    this->curr_worm->setAiming(false, false);
                }
                break;
            case SDLK_DOWN:
                if(this->curr_worm_shooting && !this->curr_worm_has_aimed){
                    //If at aiming step of shooting process, set current worm to stop aiming downwards
                    this->curr_worm->setAiming(false, false);
                }
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