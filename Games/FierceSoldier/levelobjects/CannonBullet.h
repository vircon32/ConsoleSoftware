// ---------------------------------------------------------
//   ENEMY SHOTS - NORMAL BULLETS
// ---------------------------------------------------------


struct CannonBullet
{
    Box ShapeBox;
    int ElapsedFrames;
    bool Active;
};

// ---------------------------------------------------------

void CannonBullet_Create( CannonBullet* C, Vector2D* Position, float Angle, float Speed )
{
    C->Active = true;
    C->ElapsedFrames = 0;
    C->ShapeBox.Position = *Position;
    C->ShapeBox.Velocity.x = Speed * cos( Angle );
    C->ShapeBox.Velocity.y = Speed * sin( Angle );
    C->ShapeBox.Width = 18;
    C->ShapeBox.Height = 18;
    C->ShapeBox.Hotspot.x = 9;
    C->ShapeBox.Hotspot.y = 9;
}

// ---------------------------------------------------------

void CannonBullet_Reset( CannonBullet* C )
{
    C->Active = false;
    C->ElapsedFrames = 0;
}

// ---------------------------------------------------------

void CannonBullet_Update( CannonBullet* C )
{
    // nothing to do when inactive
    if( !C->Active ) return;
    
    // advance animation
    C->ElapsedFrames++;
    
    // apply a reduced gravity
    Box_BeginNewFrame( &C->ShapeBox );
    C->ShapeBox.Acceleration.y += 1200 * 0.2;
    
    // move
    Box_Simulate( &C->ShapeBox );
    Box_Move( &C->ShapeBox );
}

// ---------------------------------------------------------

void CannonBullet_Draw( CannonBullet* C, Vector2D* LevelTopLeftOnScreen )
{
    // nothing to do when inactive
    if( !C->Active ) return;
    
    // calculate position on screen
    int CannonBulletX = LevelTopLeftOnScreen->x + C->ShapeBox.Position.x;
    int CannonBulletY = LevelTopLeftOnScreen->y + C->ShapeBox.Position.y;
    
    // draw on screen
    select_region( RegionCannonBullet );
    draw_region_at( CannonBulletX, CannonBulletY );
    
    // if it exits the screen, deactivate it
    if( CannonBulletX <= -10 || CannonBulletX >= (screen_width + 10) )
    {
        C->ElapsedFrames = 0;
        C->Active = false;
    }
    
    else if( CannonBulletY <= -10 || CannonBulletY >= (screen_height + 10) )
    {
        C->ElapsedFrames = 0;
        C->Active = false;
    }
}


// ---------------------------------------------------------
//   POOL OF ENEMY SHOTS TO USE BY OTHER GAME OBJECTS
// ---------------------------------------------------------


CannonBullet[ 10 ] CannonBullets;

// ---------------------------------------------------------

void CreateCannonBullet( Vector2D* Position, float Angle, float Speed  )
{
    for( int i = 0; i < 10; i++ )
      if( !CannonBullets[ i ].Active )
        {
            CannonBullet_Create( &CannonBullets[ i ], Position, Angle, Speed );
            play_sound( SoundExplosionSmall );
            return;
        }
}
