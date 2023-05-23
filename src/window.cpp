#include "window.hpp"

WindowApp::WindowApp()
{
    // Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
        exit(-1);
    }
    
    this->worm1 = new Worm(this->renderer, 70, 0, true);
    this->worm2 = new Worm(this->renderer, 1100, 0, false);

    this->timer_text = createTextTexture(this->renderer, std::to_string((int)(timer / FRAMERATE)), {255, 0, 0, 255});

    this->player1_name = createTextTexture(this->renderer, "player1", {0, 0, 0, 255});
    this->player2_name = createTextTexture(this->renderer, "player2", {0, 0, 0, 255});
    this->player1_health = createTextTexture(this->renderer, std::to_string(this->worm1->getHealth()), {0, 0, 0, 255});
    this->player2_health = createTextTexture(this->renderer, std::to_string(this->worm2->getHealth()), {0, 0, 0, 255});
    this->bullet_sprite = this->loadMedia(this->renderer, "./assets/assets/sprites/projectile_bullet.bmp", BULLET_WIDTH, BULLET_HEIGHT);
    this->rocket_sprite = this->loadMedia(this->renderer, "./assets/assets/sprites/projectile_rocket.bmp", ROCKET_WIDTH, ROCKET_HEIGHT);

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
        loadFont("./assets/assets/font/OpenSans-Bold.ttf", 50);
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

SDL_Texture *WindowApp::createTextTexture(SDL_Renderer *renderer, const std::string &text, SDL_Color textColor)
{
    SDL_Surface *text_surface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);
    if (text_surface == nullptr)
    {
        std::cout << "Failed to render text surface. TTF Error: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (text_texture == nullptr)
    {
        std::cout << "Failed to create texture from rendered text. SDL Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(text_surface);
        return nullptr;
    }

    SDL_FreeSurface(text_surface);

    return text_texture;
}

SDL_Texture* WindowApp::loadMedia(SDL_Renderer *renderer, const char* path, int width, int height){

    // Load splash image
    SDL_Surface *imageSurface = SDL_LoadBMP(path);
    if (!imageSurface)
    {
        std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
        exit(-1);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // Create texture from surface
    SDL_Surface* resizedSurface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_BlitScaled(imageSurface, nullptr, resizedSurface, nullptr);
    SDL_Texture *sprite = SDL_CreateTextureFromSurface(renderer, resizedSurface);
    SDL_QueryTexture(sprite, nullptr, nullptr, &width, &height);
    SDL_FreeSurface(resizedSurface);
    SDL_FreeSurface(imageSurface);
    return sprite;
}

void WindowApp::renderBar()
{
    int full_rect_width = (int)(POWER_BAR_WIDTH * this->shooting_power / (double)MAX_SHOOTING_POWER);
    SDL_Rect full_rect = {POWER_BAR_X, POWER_BAR_Y, full_rect_width, POWER_BAR_HEIGHT};

    int empty_rect_x = POWER_BAR_X + full_rect.w;
    int empty_rect_width = POWER_BAR_WIDTH - full_rect_width;
    SDL_Rect empty_rect = {empty_rect_x, POWER_BAR_Y, empty_rect_width, POWER_BAR_HEIGHT};

    SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(this->renderer, &full_rect);

    SDL_SetRenderDrawColor(this->renderer, 211, 211, 211, 255);
    SDL_RenderFillRect(this->renderer, &empty_rect);
}

void WindowApp::renderWormsAmunitions()
{
    std::string worm1_weapon = this->worm1->getWeapon();
    int worm1_amunitions = this->worm1->getWeaponAmunition();
    std::string worm2_weapon = this->worm2->getWeapon();
    int worm2_amunitions = this->worm2->getWeaponAmunition();
    SDL_Rect rect;
    SDL_Texture* sprite;

    if (!worm1_weapon.compare("bazooka")){
        rect = {PLAYER1_AMUNITION_X, PLAYER1_AMUNITION_Y, ROCKET_WIDTH, ROCKET_HEIGHT};
        sprite = this->rocket_sprite;    
    }
    else{
        rect = {PLAYER1_AMUNITION_X, PLAYER1_AMUNITION_Y, BULLET_WIDTH, BULLET_HEIGHT};
        sprite = this->bullet_sprite;    
    }

    for (int i=0; i<worm1_amunitions; i++){
        SDL_RenderCopyEx(this->renderer, sprite, nullptr, &rect, AMUNITION_DISPLAY_ANGLE, nullptr, SDL_FLIP_NONE);
        rect.x += AMUNITION_DISPLAY_SEPARATION;
    }
    
    if (!worm2_weapon.compare("bazooka")){
        rect = {PLAYER2_AMUNITION_X, PLAYER2_AMUNITION_Y, ROCKET_WIDTH, ROCKET_HEIGHT};
        sprite = this->rocket_sprite;    
    }
    else{
        rect = {PLAYER2_AMUNITION_X, PLAYER2_AMUNITION_Y, BULLET_WIDTH, BULLET_HEIGHT};
        sprite = this->bullet_sprite;    
    }

    for (int i=0; i<worm2_amunitions; i++){
        SDL_RenderCopyEx(this->renderer, sprite, nullptr, &rect, AMUNITION_DISPLAY_ANGLE, nullptr, SDL_FLIP_NONE);
        rect.x += AMUNITION_DISPLAY_SEPARATION;
    }
}

void WindowApp::close()
{
    if (curr_projectile != nullptr)
        delete curr_projectile;
    delete ground;
    delete worm1;
    delete worm2;
    SDL_DestroyTexture(timer_text);
    SDL_DestroyTexture(player1_health);
    SDL_DestroyTexture(player2_health);
    SDL_DestroyTexture(player1_name);
    SDL_DestroyTexture(player2_name);
    SDL_DestroyTexture(bullet_sprite);
    SDL_DestroyTexture(rocket_sprite);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gWindow);
    TTF_CloseFont(gFont);
    SDL_Quit();
}

void WindowApp::render()
{
    // Clear the screen
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 255, 255);
    SDL_RenderClear(this->renderer);

    // Render the scene
    this->ground->render(this->renderer);

    this->worm1->render(this->renderer);
    this->worm2->render(this->renderer);

    if (this->curr_projectile != nullptr)
        this->curr_projectile->render(this->renderer);
    
    SDL_RenderCopy(this->renderer, this->timer_text, NULL, &this->timer_rect);
    SDL_RenderCopy(this->renderer, this->player1_name, NULL, &this->player1_name_rect);
    SDL_RenderCopy(this->renderer, this->player2_name, NULL, &this->player2_name_rect);
    SDL_RenderCopy(this->renderer, this->player1_health, NULL, &this->player1_health_rect);
    SDL_RenderCopy(this->renderer, this->player2_health, NULL, &this->player2_health_rect);

    this->renderBar();

    this->renderWormsAmunitions();

    if (this->quit){
        std::string str;
        if(this->worm1->getHealth() == 0)
            str = "Player 2 won the game!";
        else    
            str = "Player 1 won the game!";

        SDL_Texture* end_game = createTextTexture(this->renderer, str, {0, 0, 0, 255});
        SDL_Rect rect = {490, 335, 300, 50};
        SDL_RenderCopy(this->renderer, end_game, NULL, &rect);
        SDL_DestroyTexture(end_game);
    }
    // Update the screen
    SDL_RenderPresent(this->renderer);
}

void WindowApp::update()
{
    if (this->timer == 0)
    {   
        this->curr_worm->setHSPeed(0);
        this->curr_worm->setVSPeed(WORM_FALLING_SPEED);
        if (this->curr_worm == this->worm1)
            this->curr_worm = this->worm2;
        else
            this->curr_worm = this->worm1;

        curr_worm_shooting = false;

        curr_worm_has_aimed = false;

        shooting_power = MIN_SHOOTING_POWER;
        
        if (curr_projectile != nullptr)
            this->explodeProjectile(false, false);

        this->timer = TIMER;
    }

    this->timer--;
    SDL_DestroyTexture(this->timer_text);
    this->timer_text = createTextTexture(this->renderer, std::to_string((int)(this->timer/FRAMERATE)), {255,0,0, 255});

    this->worm1->update(ground->getPoints(), this->worm2->getHitbox());
    if (this->worm1->getHealth() <= 0)
        this->quit = true;
    this->worm2->update(ground->getPoints(), this->worm1->getHitbox());
    if (this->worm2->getHealth() <= 0)
        this->quit = true;

    if (this->curr_projectile != nullptr && !this->curr_projectile->update())
        explodeProjectile(false, false);

    if(this->curr_projectile != nullptr){
        SDL_Rect hitbox1 = this->worm1->getHitbox();
        SDL_Rect hitbox2 = this->worm2->getHitbox();
        bool hit1 = this->curr_projectile->checkCollision(hitbox1);
        bool hit2 = this->curr_projectile->checkCollision(hitbox2);
        if (hit1 || hit2 || this->curr_projectile->checkCollision(ground->getPoints()))
        {
            explodeProjectile(hit1, hit2);
        }
    }

    if (this->curr_worm_shooting && this->curr_worm_has_aimed)
    {
        this->shooting_power += SHOOTING_POWER_STEP;
        if (this->shooting_power > MAX_SHOOTING_POWER)
            this->shooting_power = MIN_SHOOTING_POWER;
    }

    if (this->quit)
        this->render();
}

void WindowApp::event()
{

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {

        switch (event.type)
        {
        case SDL_QUIT:
            this->hard_quit = true;
            break;
        case SDL_KEYDOWN:
            // Keydown event
            // Check the specific key
            switch (event.key.keysym.sym)
            {
            case SDLK_LEFT:
                // Move image to the left
                if (this->curr_worm->getVSpeed() != WORM_JETPACK_SPEED)
                    this->curr_worm->setHSPeed(-WORM_SPEED_MODIFIER);
                break;
            case SDLK_RIGHT:
                // Move image to the right
                if (this->curr_worm->getVSpeed() != WORM_JETPACK_SPEED)
                    this->curr_worm->setHSPeed(WORM_SPEED_MODIFIER);
                break;
            case SDLK_SPACE:
                if (this->curr_worm->getVSpeed() == 0){
                    //If the playing worm is using its jetpack or falling, it cannot shoot
                    if(!this->curr_worm_shooting && this->curr_worm->isWeaponReady() && this->curr_worm->getWeaponAmunition() > 0){
                        //Starts the shooting process
                        this->curr_worm_shooting = true;
                    }
                    else if (this->curr_worm_shooting && !this->curr_worm_has_aimed)
                    {
                        // Starts the fire power gauging process
                        this->curr_worm_has_aimed = true;
                    }
                    else if (this->curr_worm_shooting && this->curr_worm_has_aimed)
                    {
                        // Finalize the shooting process
                        std::tuple<bool, double, SDL_Rect, SDL_Rect> fire_params = this->curr_worm->fire();
                        std::string weapon = this->curr_worm->getWeapon();
                        if (!weapon.compare("bazooka"))
                            this->curr_projectile = new Rocket(fire_params, this->shooting_power, this->renderer);
                        else
                            this->curr_projectile = new Bullet(fire_params, this->shooting_power, this->renderer);

                        SDL_Rect hitbox1 = this->worm1->getHitbox();
                        bool hit1 = this->curr_projectile->checkCollision(hitbox1);
                        SDL_Rect hitbox2 = this->worm2->getHitbox();
                        bool hit2 = this->curr_projectile->checkCollision(hitbox2);
                        if (hit1 || hit2 || this->curr_projectile->checkCollision(ground->getPoints()))
                        {
                            explodeProjectile(hit1, hit2);
                        }
                        
                        this->shooting_power = MIN_SHOOTING_POWER;
                        this->curr_worm_shooting = false;
                        this->curr_worm_has_aimed = false;
                    }
                }
                break;
            case SDLK_UP:
                if (this->curr_worm_shooting && !this->curr_worm_has_aimed)
                {
                    // If at aiming step of shooting process, set current worm to aim upwards
                    this->curr_worm->setAiming(true, true);
                }
                else if(!this->curr_worm_shooting){
                    this->curr_worm->setVSPeed(WORM_JETPACK_SPEED);
                    this->curr_worm->setHSPeed(0);
                }
                break;
            case SDLK_DOWN:
                if (this->curr_worm_shooting && !this->curr_worm_has_aimed)
                {
                    // If at aiming step of shooting process, set current worm to aim downwards
                    this->curr_worm->setAiming(true, false);
                }
                break;
            case SDLK_1:
                // Worm selects weapon 1 (bazooka)
                if (!this->curr_worm_shooting)
                    this->curr_worm->setWeapon("bazooka");
                break;
            case SDLK_2:
                // Worm selects weapon 2 (shotgun)
                if (!this->curr_worm_shooting)
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
                this->curr_worm->setHSPeed(0);
                break;
            case SDLK_RIGHT:
                // Stop animation
                this->curr_worm->setHSPeed(0);
                break;
            case SDLK_UP:
                if (this->curr_worm_shooting && !this->curr_worm_has_aimed)
                {
                    // If at aiming step of shooting process, set current worm to stop aiming upwards
                    this->curr_worm->setAiming(false, false);
                }
                else if(!this->curr_worm_shooting){
                    this->curr_worm->setVSPeed(0);
                }
                break;
            case SDLK_DOWN:
                if (this->curr_worm_shooting && !this->curr_worm_has_aimed)
                {
                    // If at aiming step of shooting process, set current worm to stop aiming downwards
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

bool WindowApp::getHardQuit()
{
    return this->hard_quit;
}

void WindowApp::explodeProjectile(bool hit1, bool hit2)
{
    std::vector<SDL_Point> explosion_zone = this->curr_projectile->getExplosionZone();
    this->ground->destroyPoints(explosion_zone);
    for (int i=1; i<3; i++){
        Worm* worm;
        bool hit;
        if (i == 1){
            worm = this->worm1;
            hit = hit1;
        }
        else {
            worm = this->worm2;
            hit = hit2;
        }

        if (hit || worm->checkCollision(explosion_zone))
        {
            worm->setDamage(this->curr_projectile->getDamage());
            if (worm->getHealth() <= 0)
                this->quit = true;
        }
    }
    
    SDL_DestroyTexture(this->player1_health);
    SDL_DestroyTexture(this->player2_health);
    this->player1_health = createTextTexture(this->renderer, std::to_string(this->worm1->getHealth()), {0, 0, 0, 255});
    this->player2_health = createTextTexture(this->renderer, std::to_string(this->worm2->getHealth()), {0, 0, 0, 255});

    delete this->curr_projectile;
    this->curr_projectile = nullptr;
}