// *****************************************************************************
    // start include guard
    #ifndef PLAYER_H
    #define PLAYER_H
    
    // include project headers
    #include "Box.h"
// *****************************************************************************


// player motion constants
#define TankSpeed 100
#define BulletSpeed 500
#define InvincibilityFrames 150
#define RespawnSeconds 5


// ---------------------------------------------------------
//   CONTROLLABLE PLAYER CHARACTER
// ---------------------------------------------------------


enum PlayerStates
{
    Player_RespawnWait,
    Player_Moving,
    Player_Dying
};

// ---------------------------------------------------------

struct Player
{
    // initial state
    Vector2D StartPosition;
    int PlayerID;   // 0 to 3
    int GamepadID;  // 0 to 3
    
    // physics
    Box ShapeBox;
    
    // state variables
    PlayerStates State;
    int ElapsedFrames;
    int Health;
    
    // graphical state
    int RemainingHitFrames;
    int RemainingShotFrames;
    int BodyDirectionX, BodyDirectionY;
    int TurretDirectionX, TurretDirectionY;
};

// ---------------------------------------------------------

// functions for player treatment
void Player_Create( Player* P, int TileX, int TileY, int PlayerID );
void Player_Reset( Player* P );
void Player_DetermineMovement( Player* P );
void Player_ApplyMovement( Player* P );
void Player_Draw( Player* P, Vector2D* LevelTopLeftOnScreen );


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
