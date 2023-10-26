// *****************************************************************************
    // include project headers
    #include "Player.h"
    #include "Definitions.h"
// *****************************************************************************


// ---------------------------------------------------------
//   PLAYER FUNCTIONS IMPLEMENTATION
// ---------------------------------------------------------


void Player_Create( Player* P, int TileX, int TileY, int ID )
{
    P->ShapeBox.Width = 44;
    P->ShapeBox.Height = 44;
    P->ShapeBox.Hotspot.x = 22;
    P->ShapeBox.Hotspot.y = 22;
    P->StartPosition.x = TileWidth * (TileX + 0.5);
    P->StartPosition.y = TileHeight * (TileY + 0.5);
    P->PlayerID = P->GamepadID = ID;
}

// ---------------------------------------------------------

void Player_Reset( Player* P )
{
    Box_FullStop( &P->ShapeBox );
    P->ShapeBox.Position = P->StartPosition;
    
    P->State = Player_Moving;
    P->Health = 20;
    P->ElapsedFrames = 0;
    P->RemainingHitFrames = 0;
    P->RemainingShotFrames = 0;
    
    P->BodyDirectionX = 0;
    P->BodyDirectionY = 1;
    
    if( P->StartPosition.y > screen_height/2 )
      P->BodyDirectionY = P->TurretDirectionY = -1;
  
    P->TurretDirectionX = P->BodyDirectionX;
    P->TurretDirectionY = P->BodyDirectionY;
}

// ---------------------------------------------------------

void Player_Shoot( Player* P )
{
    P->RemainingShotFrames = 5;
    
    Vector2D ShotPosition = P->ShapeBox.Position;
    ShotPosition.x += 32 * P->TurretDirectionX;
    ShotPosition.y += 32 * P->TurretDirectionY;
    
    Vector2D ShotVelocity;
    ShotVelocity.x += BulletSpeed * P->TurretDirectionX;
    ShotVelocity.y += BulletSpeed * P->TurretDirectionY;
    
    if( ShotVelocity.x && ShotVelocity.y )
    {
        ShotVelocity.x *= 0.707;
        ShotVelocity.y *= 0.707;
    }
    
    CreateTankShot( &ShotPosition, &ShotVelocity, P->PlayerID );
    play_sound( SoundShoot );
}

// ---------------------------------------------------------

void Player_GetHit( Player* P )
{
    P->Health -= 1;
    
    if( P->Health <= 0 )
    {
        P->Health = 0;
        P->State = Player_Dying;
        P->ElapsedFrames = 0;
        Box_FullStop( &P->ShapeBox );
        return;
    }
    
    P->RemainingHitFrames = 5;
    play_sound( SoundTankHit );
}

// ---------------------------------------------------------

void Player_DetermineMovement( Player* P )
{
    P->ElapsedFrames++;
    
    if( P->State == Player_RespawnWait )
    {
        if( P->ElapsedFrames >= (60 * RespawnSeconds) )
        {
            Player_Reset( P );
            play_sound( SoundRespawn );
            CreateHighlight( &P->ShapeBox.Position, 2 );
        }
        
        return;
    }
    
    // get hit by bullets
    for( int i = 0; i < MAX_TANKSHOTS; ++i )
    {
        if( !TankShots[ i ].Active || TankShots[ i ].IsBreaking )
          continue;
        
        if( TankShots[ i ].PlayerID == P->PlayerID )
          continue;
        
        // allow hitting an exploding tank too
        if( Box_OverlapWith( &TankShots[ i ].ShapeBox, &P->ShapeBox ) )
        {
            TankShots[ i ].ElapsedFrames = 0;
            TankShots[ i ].IsBreaking = true;
            
            if( P->State == Player_Moving )
            {
                Player_GetHit( P );
                
                if( P->State == Player_Dying )
                  break;
            }
        }
    }
    
    if( P->State == Player_Dying )
    {
        if( P->ElapsedFrames > 60 )
        {
            // larger explosion
            play_sound( SoundExplosionLarge );
            CreateHighlight( &P->ShapeBox.Position, 2 );
            
            // reset position for respawn countdown
            P->State = Player_RespawnWait;
            P->ElapsedFrames = 0;
            P->ShapeBox.Position = P->StartPosition;
        }
        
        // smaller explosions
        else if( (P->ElapsedFrames % 10) == 0 )
        {
            Vector2D HighlightPosition = P->ShapeBox.Position;
            HighlightPosition.x += -20 + rand() % 40;
            HighlightPosition.y += -20 + rand() % 40;
            CreateHighlight( &HighlightPosition, 1 );
            play_sound( SoundExplosionSmall );
        }
        
        return;
    }
    
    if( P->RemainingShotFrames > 0 )
      P->RemainingShotFrames--;
    
    if( P->RemainingHitFrames > 0 )
      P->RemainingHitFrames--;
    
    // reset player physics for this next frame
    Box_BeginNewFrame( &P->ShapeBox );
    
    // check movement inputs
    int DirectionX, DirectionY;
    select_gamepad( P->GamepadID );
    gamepad_direction( &DirectionX, &DirectionY );
    
    // only update player direction with active inputs
    // (player will keep facing at the last pressed direction)
    if( DirectionX != 0 || DirectionY != 0 )
    {
        P->BodyDirectionX = DirectionX;
        P->BodyDirectionY = DirectionY;
    }
    
    // apply player ground movement
    P->ShapeBox.Velocity.x = TankSpeed * DirectionX;
    P->ShapeBox.Velocity.y = TankSpeed * DirectionY;
    
    if( DirectionX && DirectionY )
    {
        P->ShapeBox.Velocity.x *= 0.707;
        P->ShapeBox.Velocity.y *= 0.707;
    }
    
    // simulate tentative player motion for this frame
    Box_Simulate( &P->ShapeBox );
    
    // button A shoots right
    if( gamepad_button_a() == 1 )
    {
        P->TurretDirectionX = 1;
        P->TurretDirectionY = 0;
        Player_Shoot( P );
    }
    
    // button B shoots down
    else if( gamepad_button_b() == 1 )
    {
        P->TurretDirectionX = 0;
        P->TurretDirectionY = 1;
        Player_Shoot( P );
    }
    
    // button X shoots up
    else if( gamepad_button_x() == 1 )
    {
        P->TurretDirectionX = 0;
        P->TurretDirectionY = -1;
        Player_Shoot( P );
    }
    
    // button Y shoots left
    else if( gamepad_button_y() == 1 )
    {
        P->TurretDirectionX = -1;
        P->TurretDirectionY = 0;
        Player_Shoot( P );
    }
}

// ---------------------------------------------------------

void Player_ApplyMovement( Player* P )
{
    // actually apply projected movement
    Box_Move( &P->ShapeBox );
}

// ---------------------------------------------------------

void Player_Draw( Player* P, Vector2D* LevelTopLeftOnScreen )
{
    int PlayerX = LevelTopLeftOnScreen->x + P->ShapeBox.Position.x;
    int PlayerY = LevelTopLeftOnScreen->y + P->ShapeBox.Position.y;
    
    if( P->State == Player_RespawnWait )
    {
        // draw respawn countdown
        int RemainingSeconds = (((60*RespawnSeconds) - P->ElapsedFrames) / 60) + 1;
        select_region( FirstRegionNumbers + RemainingSeconds );
        draw_region_at( PlayerX, PlayerY );
        
        return;
    }
    
    // draw shadow
    if( P->BodyDirectionY == 0 || P->BodyDirectionX == 0 )
      select_region( RegionShadowTank );
    else
      select_region( RegionShadowTankDiagonal );
    
    draw_region_at( PlayerX, PlayerY );
    
    // draw body
    float BodyAngle = atan2( P->BodyDirectionY, P->BodyDirectionX );
    select_region( FirstRegionTankBodiesRight + P->PlayerID );
    set_drawing_angle( BodyAngle );
    draw_region_rotated_at( PlayerX, PlayerY );
    
    // draw turret
    float TurretAngle = atan2( P->TurretDirectionY, P->TurretDirectionX );
    
    if( P->RemainingShotFrames > 0 )
      select_region( FirstRegionTurrets + 2*P->PlayerID + 1 );
    else
      select_region( FirstRegionTurrets + 2*P->PlayerID );
      
    set_drawing_angle( TurretAngle + pi/2 );
    draw_region_rotated_at( PlayerX, PlayerY );
    
    // highlight when hit
    if( P->RemainingHitFrames > 0 )
    {
        set_blending_mode( blending_add );
        
        select_region( FirstRegionTankBodiesRight + P->PlayerID );
        set_drawing_angle( BodyAngle );
        draw_region_rotated_at( PlayerX, PlayerY );
        
        if( P->RemainingShotFrames > 0 )
          select_region( FirstRegionTurrets + 2*P->PlayerID + 1 );
        else
          select_region( FirstRegionTurrets + 2*P->PlayerID );
          
        set_drawing_angle( TurretAngle + pi/2 );
        draw_region_rotated_at( PlayerX, PlayerY );
        
        set_blending_mode( blending_alpha );
    }
}
