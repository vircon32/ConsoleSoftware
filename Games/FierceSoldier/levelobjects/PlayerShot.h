// ---------------------------------------------------------
//   BULLETS SHOT BY THE PLAYER
// ---------------------------------------------------------


struct PlayerShot
{
    Box ShapeBox;
    int ElapsedFrames;
    bool Active;
};

// ---------------------------------------------------------

void PlayerShot_Create( PlayerShot* S, Vector2D* Position, Vector2D* Velocity )
{
    S->Active = true;
    S->ElapsedFrames = 0;
    S->ShapeBox.Position = *Position;
    S->ShapeBox.Velocity = *Velocity;
    S->ShapeBox.Width = 8;
    S->ShapeBox.Height = 8;
}

// ---------------------------------------------------------

void PlayerShot_Reset( PlayerShot* S )
{
    S->Active = false;
    S->ElapsedFrames = 0;
}

// ---------------------------------------------------------

void PlayerShot_Update( PlayerShot* S )
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

void PlayerShot_Draw( PlayerShot* S, Vector2D* LevelTopLeftOnScreen )
{
    // nothing to do when inactive
    if( !S->Active ) return;
    
    // calculate position on screen
    int PlayerShotX = LevelTopLeftOnScreen->x + S->ShapeBox.Position.x;
    int PlayerShotY = LevelTopLeftOnScreen->y + S->ShapeBox.Position.y;
    
    // draw current animation frame
    int AnimationFrame = (S->ElapsedFrames/4) % 2;
    select_region( FirstRegionPlayerBullet + AnimationFrame );
    draw_region_at( PlayerShotX, PlayerShotY );
    
    // if it exits the screen, deactivate it
    if( PlayerShotX <= -10 || PlayerShotX >= (screen_width + 10) )
    {
        S->ElapsedFrames = 0;
        S->Active = false;
    }
}


// ---------------------------------------------------------
//   POOL OF PLAYER SHOTS TO USE BY OTHER GAME OBJECTS
// ---------------------------------------------------------


PlayerShot[ 10 ] PlayerShots;

// ---------------------------------------------------------

void CreatePlayerShot( Vector2D* Position, bool GoesRight )
{
    for( int i = 0; i < 10; i++ )
      if( !PlayerShots[ i ].Active )
        {
            Vector2D ShotVelocity = { 400, 0 };
            if( !GoesRight ) ShotVelocity.x *= -1;
            PlayerShot_Create( &PlayerShots[ i ], Position, &ShotVelocity );
            return;
        }
}
