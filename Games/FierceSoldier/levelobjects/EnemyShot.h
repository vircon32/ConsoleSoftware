// ---------------------------------------------------------
//   ENEMY SHOTS - NORMAL BULLETS
// ---------------------------------------------------------


struct EnemyShot
{
    Box ShapeBox;
    int ElapsedFrames;
    bool Active;
};

// ---------------------------------------------------------

void EnemyShot_Create( EnemyShot* S, Vector2D* Position, Vector2D* Velocity )
{
    S->Active = true;
    S->ElapsedFrames = 0;
    S->ShapeBox.Position = *Position;
    S->ShapeBox.Velocity = *Velocity;
    S->ShapeBox.Width = 8;
    S->ShapeBox.Height = 8;
    S->ShapeBox.Hotspot.x = 4;
    S->ShapeBox.Hotspot.y = 4;
}

// ---------------------------------------------------------

void EnemyShot_Reset( EnemyShot* S )
{
    S->Active = false;
    S->ElapsedFrames = 0;
}

// ---------------------------------------------------------

void EnemyShot_Update( EnemyShot* S )
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

void EnemyShot_Draw( EnemyShot* S, Vector2D* LevelTopLeftOnScreen )
{
    // nothing to do when inactive
    if( !S->Active ) return;
    
    // calculate position on screen
    int EnemyShotX = LevelTopLeftOnScreen->x + S->ShapeBox.Position.x;
    int EnemyShotY = LevelTopLeftOnScreen->y + S->ShapeBox.Position.y;
    
    // draw current animation frame
    int AnimationFrame = (S->ElapsedFrames/4) % 2;
    select_region( FirstRegionEnemyBullet + AnimationFrame );
    draw_region_at( EnemyShotX, EnemyShotY );
    
    // if it exits the screen, deactivate it
    if( EnemyShotX <= -10 || EnemyShotX >= (screen_width + 10) )
    {
        S->ElapsedFrames = 0;
        S->Active = false;
    }
    
    else if( EnemyShotY <= -10 || EnemyShotY >= (screen_height + 10) )
    {
        S->ElapsedFrames = 0;
        S->Active = false;
    }
}


// ---------------------------------------------------------
//   POOL OF ENEMY SHOTS TO USE BY OTHER GAME OBJECTS
// ---------------------------------------------------------


EnemyShot[ 15 ] EnemyShots;

// ---------------------------------------------------------

void CreateEnemyShot( Vector2D* Position, Vector2D* Velocity )
{
    for( int i = 0; i < 15; i++ )
      if( !EnemyShots[ i ].Active )
        {
            EnemyShot_Create( &EnemyShots[ i ], Position, Velocity );
            play_sound( SoundEnemyShoot );
            return;
        }
}
