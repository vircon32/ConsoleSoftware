// ---------------------------------------------------------
//   ENEMY SHOTS - MISSILES
// ---------------------------------------------------------


struct Missile
{
    Box ShapeBox;
    int ElapsedFrames;
    bool Active;
};

// ---------------------------------------------------------

void Missile_Create( Missile* M, Vector2D* Position, float Angle, float Speed )
{
    M->Active = true;
    M->ElapsedFrames = 0;
    M->ShapeBox.Position = *Position;
    M->ShapeBox.Velocity.x = Speed * cos( Angle );
    M->ShapeBox.Velocity.y = Speed * sin( Angle );
    M->ShapeBox.Width = 13;
    M->ShapeBox.Height = 13;
    M->ShapeBox.Hotspot.x = 6;
    M->ShapeBox.Hotspot.y = 6;
}

// ---------------------------------------------------------

void Missile_Reset( Missile* M )
{
    M->Active = false;
    M->ElapsedFrames = 0;
}

// ---------------------------------------------------------

void Missile_Update( Missile* M )
{
    // nothing to do when inactive
    if( !M->Active ) return;
    
    // advance animation
    M->ElapsedFrames++;
    
    // move
    Box_Simulate( &M->ShapeBox );
    Box_Move( &M->ShapeBox );
}

// ---------------------------------------------------------

void Missile_Draw( Missile* M, Vector2D* LevelTopLeftOnScreen )
{
    // nothing to do when inactive
    if( !M->Active ) return;
    
    // calculate position on screen
    int MissileX = LevelTopLeftOnScreen->x + M->ShapeBox.Position.x;
    int MissileY = LevelTopLeftOnScreen->y + M->ShapeBox.Position.y;
    
    // draw current animation frame
    int AnimationFrame = (M->ElapsedFrames/3) % 2;
    select_region( FirstRegionMissile + AnimationFrame );
    set_drawing_angle( atan2( M->ShapeBox.Velocity.y, M->ShapeBox.Velocity.x ) );
    draw_region_rotated_at( MissileX, MissileY );
    
    // if it exits the screen, deactivate it
    if( MissileX <= -10 || MissileX >= (screen_width + 10) )
    {
        M->ElapsedFrames = 0;
        M->Active = false;
    }
    
    else if( MissileY <= -10 || MissileY >= (screen_height + 10) )
    {
        M->ElapsedFrames = 0;
        M->Active = false;
    }
}


// ---------------------------------------------------------
//   POOL OF ENEMY SHOTS TO USE BY OTHER GAME OBJECTS
// ---------------------------------------------------------


Missile[ 10 ] Missiles;

// ---------------------------------------------------------

void CreateMissile( Vector2D* Position, float Angle, float Speed  )
{
    for( int i = 0; i < 10; i++ )
      if( !Missiles[ i ].Active )
        {
            Missile_Create( &Missiles[ i ], Position, Angle, Speed );
            play_sound( SoundMissile );
            return;
        }
}
