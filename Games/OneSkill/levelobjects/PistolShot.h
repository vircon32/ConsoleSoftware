// ---------------------------------------------------------
//   BULLETS SHOT BY THE PLAYER'S PISTOL
// ---------------------------------------------------------


struct PistolShot
{
    Box ShapeBox;
    int ElapsedFrames;
    bool Active;
};

// ---------------------------------------------------------

void PistolShot_Create( PistolShot* S, Vector2D* Position, Vector2D* Velocity )
{
    S->Active = true;
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
    S->ElapsedFrames = 0;
}

// ---------------------------------------------------------

void PistolShot_Update( PistolShot* S )
{
    // nothing to do when inactive
    if( !S->Active ) return;
    
    // advance animation
    S->ElapsedFrames++;
    
    // move
    Box_Simulate( &S->ShapeBox );
    Box_Move( &S->ShapeBox );
}

// ---------------------------------------------------------

void PistolShot_Draw( PistolShot* S, Vector2D* LevelTopLeftOnScreen )
{
    // nothing to do when inactive
    if( !S->Active ) return;
    
    // calculate position on screen
    int PistolShotX = LevelTopLeftOnScreen->x + S->ShapeBox.Position.x;
    int PistolShotY = LevelTopLeftOnScreen->y + S->ShapeBox.Position.y;
    
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
