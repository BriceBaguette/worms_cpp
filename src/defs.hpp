#ifndef DEFS
#define DEFS

#define SCREEN_WIDTH   1280
#define SCREEN_HEIGHT  720
#define TIMER 30*30
#define FRAMERATE 30                        //frame/sec
#define FALLING_STARTING_TIME 0.3           //sec

#define WORM_SPEED_MODIFIER 75             //pixel/sec
#define WORM_FALLING_SPEED 100             //pixel/sec
#define WORM_JETPACK_SPEED -75             //pixel/sec
#define WORM_WIDTH 30
#define WORM_HEIGHT 50
#define GROUND_HEIGHT 420
#define WORM_INITIAL_HEALTH 100
#define WORM_INITIAL_WEAPON "bazooka"

#define GROUND_HEIGHT 420
#define GROUND_HEIGHT 420

#define ROCKET_MASS 5                       //kg
#define ROCKET_CROSS_SECTIONAL_AREA 1       //pixel
#define ROCKET_WIDTH 20                     //pixel
#define ROCKET_HEIGHT 10                    //pixel

#define BULLET_MASS 2                       //kg
#define BULLET_CROSS_SECTIONAL_AREA 1       //pixel
#define BULLET_WIDTH 10                     //pixel
#define BULLET_HEIGHT 5                     //pixel

#define GRAVITY 500                         //pixel/sec^2
#define AIR_DENSITY 1.                      //kg/m^3
#define DRAG_COEFFICIENT 0.001

#define WEAPON_WIDTH 30                     //pixel
#define WEAPON_HEIGHT 15                    //pixel
#define WORM_WEAPON_CENTERS_DISTANCE_X -10  //pixels
#define WORM_WEAPON_CENTERS_DISTANCE_Y 10   //pixels

#define BAZOOKA_RELOADING_DELAY 30          //sec
#define BAZOOKA_INITIAL_AMMO 2
#define BAZOOKA_DAMAGE_POINTS 10
#define BAZOOKA_RADIUS_DAMAGE 24            //pixel

#define SHOTGUN_RELOADING_DELAY 20          //sec
#define SHOTGUN_INITIAL_AMMO 4
#define SHOTGUN_DAMAGE_POINTS 20
#define SHOTGUN_RADIUS_DAMAGE 0             //pixel

#define SHOOTING_ANGLE_STEP 10              //deg
#define SHOOTING_POWER_STEP 10              //pixel
#define MIN_SHOOTING_POWER 0                //pixel/sec
#define MAX_SHOOTING_POWER 500              //pixel/sec

#define PLATFORM_WIDTH 300                  //pixel
#define PLATFORM_HEIGHT 10                  //pixel

#define PLAYER_NAME_WIDTH 35                //pixel
#define PLAYER_NAME_HEIGHT 25               //pixel
#define PLAYER_HEALTH_WIDTH 70              //pixel
#define PLAYER_HEALTH_HEIGHT 50             //pixel

#define PLAYER1_NAME_X 30                   //pixel index
#define PLAYER1_NAME_Y 10                   //pixel index
#define PLAYER2_NAME_X 1180                 //pixel index
#define PLAYER2_NAME_Y 10                   //pixel index
#define PLAYER1_HEALTH_X 30                 //pixel index
#define PLAYER1_HEALTH_Y 20                 //pixel index
#define PLAYER2_HEALTH_X 1180               //pixel index
#define PLAYER2_HEALTH_Y 20                 //pixel index

#define POWER_BAR_X 730                     //pixel index
#define POWER_BAR_Y 20                      //pixel index
#define POWER_BAR_WIDTH 400                 //pixel
#define POWER_BAR_HEIGHT 50                 //pixel

#define TIMER_X 620                         //pixel index
#define TIMER_Y 20                          //pixel index
#define TIMER_WIDTH 70                      //pixel
#define TIMER_HEIGHT 50                     //pixel

#define PLAYER1_AMUNITION_X 30              //pixel index
#define PLAYER1_AMUNITION_Y 80              //pixel index
#define PLAYER2_AMUNITION_X 1180            //pixel index
#define PLAYER2_AMUNITION_Y 80              //pixel index

#define AMUNITION_DISPLAY_ANGLE 45.         //deg
#define AMUNITION_DISPLAY_SEPARATION 20     //pixel

#endif
