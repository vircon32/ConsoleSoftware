#include "Physics.h"


// ---------------------------------------------------------
//   BULLETS SHOT BY THE PLAYER'S PISTOL
// ---------------------------------------------------------


struct PistolShot
{
    Box ShapeBox;
    int ElapsedFrames;
    bool Active;
    bool IsBreaking;
};

// ---------------------------------------------------------

void PistolShot_Create( PistolShot* S, Vector2D* Position, Vector2D* Velocity )
{
    S->Active = true;
    S->IsBreaking = false;
    S->ElapsedFrames = 0;
    S->ShapeBox.Position = *Position;
    S->ShapeBox.Velocity = *Velocity;
    S->ShapeBox.Width = 8;
    S->ShapeBox.Height = 8;
}

// ---------------------------------------------------------

void PistolShot_Reset( PistolShot* S )
{
    S->Active = false;
    S->IsBreaking = false;
    S->ElapsedFrames = 0;
}

// ---------------------------------------------------------

void PistolShot_Update( PistolShot* S )
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
    }
    
    else
    {
        // move shot
        Box_Simulate( &S->ShapeBox );
        Box_Move( &S->ShapeBox );
        
        // get the tile at shot's position
        int* ShotTile = get_tile_at_point( &S->ShapeBox.Position, &MapLevel );
        
        // destroy crates (and the shot with them)
        if( ShotTile && *ShotTile == (int)Tile_Crate )
        {
            *ShotTile = (int)Tile_Empty;
            S->ElapsedFrames = 0;
            S->IsBreaking = true;
            
            play_sound( SoundBoxDestroyed );
            
            // center the highlight on the crate's tile
            Vector2D HighlightPosition;
            int TileX = (ShotTile - MapLevel.map) % MapLevel.array_width;
            int TileY = (ShotTile - MapLevel.map) / MapLevel.array_width;
            HighlightPosition.x = MapLevel.tiles->width  * (TileX + 0.5);
            HighlightPosition.y = MapLevel.tiles->height * (TileY + 0.5);
            CreateHighlight( &HighlightPosition );
            return;
        }
        
        // if it collides with the scenery, deactivate it
        if( ShotTile && TileHasLeftBoundary( *ShotTile ) )
        {
            S->ElapsedFrames = 0;
            S->IsBreaking = true;
            return;
        }
    }
}

// ---------------------------------------------------------

void PistolShot_Draw( PistolShot* S, Vector2D* LevelTopLeftOnScreen )
{
    // nothing to do when inactive
    if( !S->Active ) return;
    
    // calculate position on screen
    int PistolShotX = LevelTopLeftOnScreen->x + S->ShapeBox.Position.x;
    int PistolShotY = LevelTopLeftOnScreen->y + S->ShapeBox.Position.y;
    
    if( S->IsBreaking )
    {
        int AnimationFrame = min( S->ElapsedFrames/4, 3 );
        select_region( FirstRegionShotBreaking + AnimationFrame );
        draw_region_at( PistolShotX, PistolShotY );
    }
    
    else
    {
        // draw current animation frame
        int AnimationFrame = (S->ElapsedFrames/4) % 2;
        select_region( FirstRegionBullet + AnimationFrame );
        draw_region_at( PistolShotX, PistolShotY );
        
        // if it exits the screen, deactivate it
        if( PistolShotX <= -10 || PistolShotX >= (screen_width + 10) )
        {
            S->ElapsedFrames = 0;
            S->Active = false;
        }
    }
}


// ---------------------------------------------------------
//   POOL OF PISTOL SHOTS TO USE BY OTHER GAME OBJECTS
// ---------------------------------------------------------


PistolShot[ 10 ] PistolShots;

// ---------------------------------------------------------

void CreatePistolShot( Vector2D* Position, bool GoesRight )
{
    for( int i = 0; i < 10; i++ )
      if( !PistolShots[ i ].Active )
        {
            Vector2D ShotVelocity = { 400, 0 };
            if( !GoesRight ) ShotVelocity.x *= -1;
            PistolShot_Create( &PistolShots[ i ], Position, &ShotVelocity );
            return;
        }
}
