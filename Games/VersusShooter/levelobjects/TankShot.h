#include "Physics.h"


// ---------------------------------------------------------
//   BULLETS SHOT BY THE PLAYER'S TANK
// ---------------------------------------------------------


struct TankShot
{
    Box ShapeBox;
    int ElapsedFrames;
    bool Active;
    bool IsBreaking;
    int PlayerID;      // the player that shot it
};

// ---------------------------------------------------------

void TankShot_Create( TankShot* S, int PlayerID, Vector2D* Position, Vector2D* Velocity )
{
    S->Active = true;
    S->IsBreaking = false;
    S->PlayerID = PlayerID;
    S->ElapsedFrames = 0;
    S->ShapeBox.Position = *Position;
    S->ShapeBox.Velocity = *Velocity;
    S->ShapeBox.Width = 8;
    S->ShapeBox.Height = 8;
    S->ShapeBox.Hotspot.x = 4;
    S->ShapeBox.Hotspot.y = 4;
}

// ---------------------------------------------------------

void TankShot_Reset( TankShot* S )
{
    S->Active = false;
    S->IsBreaking = false;
    S->ElapsedFrames = 0;
}

// ---------------------------------------------------------

void TankShot_Update( TankShot* S )
{
    // nothing to do when inactive
    if( !S->Active ) return;
    
    // advance animation
    S->ElapsedFrames++;
    
    if( S->IsBreaking )
    {
        // if shot finishes break animation, deactivate it
        if( S->ElapsedFrames >= 16 )
        {
            S->ElapsedFrames = 0;
            S->Active = false;
            S->IsBreaking = false;
        }
        
        return;
    }
    
    // move shot
    Box_Simulate( &S->ShapeBox );
    Box_Move( &S->ShapeBox );
    
    // get the tile at shot's position
    int* ShotTile = get_tile_at_point( &S->ShapeBox.Position, &MapLevel );
    
    // if it collides with the scenery, deactivate it
    if( ShotTile && TileIsSolid( *ShotTile ) )
    {
        S->ElapsedFrames = 0;
        S->IsBreaking = true;
        return;
    }
}

// ---------------------------------------------------------

void TankShot_Draw( TankShot* S, Vector2D* LevelTopLeftOnScreen )
{
    // nothing to do when inactive
    if( !S->Active ) return;
    
    // calculate position on screen
    int TankShotX = LevelTopLeftOnScreen->x + S->ShapeBox.Position.x;
    int TankShotY = LevelTopLeftOnScreen->y + S->ShapeBox.Position.y;
    
    if( S->IsBreaking )
    {
        int AnimationFrame = min( S->ElapsedFrames/4, 3 );
        select_region( FirstRegionShotBreaking + AnimationFrame );
        draw_region_at( TankShotX, TankShotY );
    }
    
    else
    {
        // draw current animation frame
        int AnimationFrame = (S->ElapsedFrames/4) % 2;
        select_region( FirstRegionBullets + 2 * S->PlayerID + AnimationFrame );
        draw_region_at( TankShotX, TankShotY );
        
        // if it exits the screen, deactivate it
        if( TankShotX <= -10 || TankShotX >= (screen_width + 10) )
        {
            S->ElapsedFrames = 0;
            S->Active = false;
        }
        
        if( TankShotY <= -10 || TankShotY >= (screen_height + 10) )
        {
            S->ElapsedFrames = 0;
            S->Active = false;
        }
    }
}


// ---------------------------------------------------------
//   POOL OF TANK SHOTS TO USE BY OTHER GAME OBJECTS
// ---------------------------------------------------------


#define MAX_TANKSHOTS 80
TankShot[ MAX_TANKSHOTS ] TankShots;

// ---------------------------------------------------------

void CreateTankShot( Vector2D* Position, Vector2D* Velocity, int PlayerID )
{
    for( int i = 0; i < MAX_TANKSHOTS; i++ )
      if( !TankShots[ i ].Active )
        {
            TankShot_Create( &TankShots[ i ], PlayerID, Position, Velocity );
            return;
        }
}
