// *****************************************************************************
    // start include guard
    #ifndef PLAYER_H
    #define PLAYER_H
    
    // include project headers
    #include "Box.h"
// *****************************************************************************


// player motion constants
#define Gravity 1000
#define JumpImpulse 460
#define JumpCancelFrames 30
#define JumpCancelFactor 0.4
#define MaxSpeedX 150
#define MaxFallSpeed 450
#define AirAcceleration 300
#define GroundAcceleration 400
#define GroundFriction 600


// ---------------------------------------------------------
//   CONTROLLABLE PLAYER CHARACTER
// ---------------------------------------------------------


struct Player
{
    // physics
    Box ShapeBox;
    float GroundSpeedX;
    float PreviousGroundSpeedX;
    
    // initial state
    Vector2D StartPosition;
    
    // state variables
    PlayerSkills Skill;
    int DrawnRegion;          // the currently displayed texture region
    int FramesToCancelJump; 
    int FacingDirectionX;
    bool IsDead;
    bool DidDoubleJump;
};

// ---------------------------------------------------------

// functions for player treatment
void Player_Create( Player* P, int TileX, int TileY );
void Player_Reset( Player* P );
void Player_ApplyFriction( Player* P );
void Player_DetermineMovement( Player* P );
void Player_ApplyMovement( Player* P );
void Player_UpdateAnimation( Player* P );
void Player_Draw( Player* P, Vector2D* LevelTopLeftOnScreen );


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
