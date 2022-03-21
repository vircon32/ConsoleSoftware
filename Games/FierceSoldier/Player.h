// player motion constants
#define Gravity 1200
#define JumpImpulse 500
#define MaxSpeedX 150
#define MaxFallSpeed 450
#define AirAcceleration 300
#define GroundAcceleration 600
#define GroundFriction 800


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
    bool ControlsEnabled;
    int DrawnRegion;                 // the currently displayed region
    int FramesSinceLastShot;
    bool FacingLeft;
    bool Ducking;
};

// ---------------------------------------------------------

void Player_Create( Player* P, int TileX, int TileY )
{
    P->ShapeBox.Width = 21;
    P->ShapeBox.Height = 82;
    P->ShapeBox.Hotspot.x = 10;
    P->ShapeBox.Hotspot.y = 81;
    P->StartPosition.x = TileWidth * (TileX + 0.5);
    P->StartPosition.y = TileHeight * (TileY + 1) - 1;
}

// ---------------------------------------------------------

void Player_Reset( Player* P )
{
    Box_FullStop( &P->ShapeBox );
    P->ShapeBox.Position = P->StartPosition;
    
    P->PreviousGroundSpeedX = 0;
    P->GroundSpeedX = 0;
    
    P->ControlsEnabled = true;
    P->DrawnRegion = FirstRegionPlayerStanding;
    P->FacingLeft = false;
    P->Ducking = false;
    P->FramesSinceLastShot = 60;
    
    // do this to avoid detecting landing on frame 1
    P->ShapeBox.Contacts.Bottom = true;
    P->ShapeBox.PreviousContacts.Bottom = true;
    P->ShapeBox.Velocity.y = 5;
}

// ---------------------------------------------------------

void Player_ApplyFriction( Player* P )
{
    // friction is not applied in the air
    if( !P->ShapeBox.PreviousContacts.Bottom )
      return;
    
    // friction is proportional to the player's speed relative to the ground
    float RelativeSpeedX = P->ShapeBox.Velocity.x - P->PreviousGroundSpeedX;
    
    if( RelativeSpeedX == 0 )
      return;
    
    // determine friction
    int SpeedSign = 1;
    if( RelativeSpeedX < 0 ) SpeedSign = -1;
    float LimitAcceleration = fabs( RelativeSpeedX / frame_time );
    
    // apply friction
    P->ShapeBox.Acceleration.x += -SpeedSign * min( GroundFriction, LimitAcceleration );
}

// ---------------------------------------------------------

void Player_DetermineMovement( Player* P )
{
    P->FramesSinceLastShot++;
    
    // reset player physics for this next frame
    Box_BeginNewFrame( &P->ShapeBox );
    P->PreviousGroundSpeedX = P->GroundSpeedX;
    P->GroundSpeedX = 0;
    
    // apply gravity, up to a certain terminal velocity
    if( P->ShapeBox.Velocity.y < MaxFallSpeed )
      P->ShapeBox.Acceleration.y += Gravity;
    
    // check movement inputs
    int DirectionX, DirectionY;
    gamepad_direction( &DirectionX, &DirectionY );
    
    // staying still does not change facing side
    if( DirectionX > 0 ) P->FacingLeft = false;
    if( DirectionX < 0 ) P->FacingLeft = true;
    
    // CASE 1: apply player ground movement
    if( P->ShapeBox.PreviousContacts.Bottom )
    {
        float RelativeSpeedX = P->ShapeBox.Velocity.x - P->PreviousGroundSpeedX;
        
        // when ducking player cannot advance
        if( !P->Ducking )
        {
            if( DirectionX < 0 )
            {
                if( RelativeSpeedX >= -MaxSpeedX )
                  P->ShapeBox.Acceleration.x -= GroundAcceleration;
            }
            
            else if( DirectionX > 0 )
            {
                if( RelativeSpeedX <= MaxSpeedX )
                  P->ShapeBox.Acceleration.x += GroundAcceleration;
            }
        }
        
        // let friction apply when either:
        // - player is ducking
        // - player is not applying motion
        // - player is changing direction
        // - player is walking faster than its limit speed
        if( P->Ducking || (DirectionX == 0) || (DirectionX * RelativeSpeedX < 0) || (fabs(RelativeSpeedX) > MaxSpeedX) )
          Player_ApplyFriction( P );
    }
    
    // CASE 2: apply player air movement
    else
    {
        // being airborne automatically unducks
        P->Ducking = false;
        
        // upon horizontal inputs, apply reduced motion force
        if( DirectionX > 0 )
        {
            if( P->ShapeBox.Velocity.x < MaxSpeedX )
              P->ShapeBox.Acceleration.x += AirAcceleration;
        }
        
        else if( DirectionX < 0 )
        {
            if( P->ShapeBox.Velocity.x > -MaxSpeedX )
              P->ShapeBox.Acceleration.x -= AirAcceleration;
        }
    }
    
    // simulate tentative player motion for this frame
    Box_Simulate( &P->ShapeBox );
    
    // some actions can only be done on land
    if( P->ShapeBox.PreviousContacts.Bottom )
    {
        // pressing down ducks
        // (and releasing down unducks)
        P->Ducking = (gamepad_down() > 0);
        
        // pressing A can have different effects:
        if( gamepad_button_a() == 1 )
        {
            // while ducking, it descends (passes its bottom boundary)
            // but we will disable it unless the player is at least
            // 2 tiles higher than the lowest tile (would make no sense)
            if( P->Ducking )
            {
                int PlayerBottom = P->ShapeBox.Position.y - P->ShapeBox.Hotspot.y + P->ShapeBox.Height;
                int StageBottom = screen_height;  // cheat because we can't access CurrentLevelMap from here
                
                if( StageBottom - PlayerBottom > 100 )
                {
                    P->ShapeBox.Position.y += 2;
                    P->Ducking = false;
                }                
            }
            
            // standing, it jumps (an upward impulse)
            else
            {
                P->ShapeBox.DeltaVelocity.y -= JumpImpulse;
                play_sound( SoundJump );
                
                // cannot jump and duck
                P->Ducking = false;
            }
        }
    }
    
    // shoot horizontally
    if( gamepad_button_x() == 1 )
    {
        Vector2D ShotPosition = P->ShapeBox.Position;
        ShotPosition.y -= 53;
        
        // adjust when facing left
        if( P->FacingLeft ) 
          ShotPosition.x -= 35;          
        else 
          ShotPosition.x += 35;          
      
        // adjust when ducking
        if( P->Ducking > 0 )
          if( P->ShapeBox.PreviousContacts.Bottom )
            ShotPosition.y += 19;
        
        // adjust when jumping
        if( !P->ShapeBox.PreviousContacts.Bottom )
          ShotPosition.y += 6;
        
        CreatePlayerShot( &ShotPosition, !P->FacingLeft );
        play_sound( SoundShoot );
        P->FramesSinceLastShot = 0;
    }
    
    // when ducking, lower the hitbox
    if( P->Ducking )
    {
        P->ShapeBox.Height = 57;
        P->ShapeBox.Hotspot.y = 56;
    }
    else
    {
        P->ShapeBox.Height = 82;
        P->ShapeBox.Hotspot.y = 81;
    }
}

// ---------------------------------------------------------

void Player_ApplyMovement( Player* P )
{
    // actually apply projected movement
    Box_Move( &P->ShapeBox );
    
    // detect player landing
    if( !P->ShapeBox.PreviousContacts.Bottom && P->ShapeBox.Contacts.Bottom )
      play_sound( SoundLand );
}

// ---------------------------------------------------------

void Player_DetectShotImpacts( Player* P )
{
    // detect impacts from regular bullets
    for( int i = 0; i < 15; i++ )
    {
        if( !EnemyShots[ i ].Active )
          continue;
        
        if( Box_OverlapWith( &EnemyShots[ i ].ShapeBox, &P->ShapeBox ) )
        {
            Gameplay_ChangeState( Gameplay_Death );
            return;
        }
    }
    
    // detect impacts from cannon bullets
    for( int i = 0; i < 10; i++ )
    {
        if( !CannonBullets[ i ].Active )
          continue;
        
        if( Box_OverlapWith( &CannonBullets[ i ].ShapeBox, &P->ShapeBox ) )
        {
            Gameplay_ChangeState( Gameplay_Death );
            return;
        }
    }
    
    // detect impacts from missiles
    for( int i = 0; i < 10; i++ )
    {
        if( !Missiles[ i ].Active )
          continue;
        
        if( Box_OverlapWith( &Missiles[ i ].ShapeBox, &P->ShapeBox ) )
        {
            Gameplay_ChangeState( Gameplay_Death );
            return;
        }
    }
}

// ---------------------------------------------------------

void Player_UpdateAnimation( Player* P )
{
    // select animation frames
    P->DrawnRegion = FirstRegionPlayerStanding;
    
    // case 1: death
    if( GameState == Gameplay_Death )
    {
        P->DrawnRegion = RegionPlayerDeath;
        return;
    }
    
    // case 2: level clear
    if( GameState == Gameplay_LevelClear )
    {
        if( (Gameplay_ElapsedFrames & 16) != 0 )
          P->DrawnRegion = RegionPlayerVictory;
        
        return;
    }
    
    // common for next cases
    bool PlayerShooting = (P->FramesSinceLastShot < 5);
    
    // case 3: jumping
    if( !P->ShapeBox.Contacts.Bottom )
    {
        P->DrawnRegion = FirstRegionPlayerJump;
        if( PlayerShooting ) P->DrawnRegion++;
        return;
    }
    
    // case 4: ducking
    if( P->Ducking )
    {
        P->DrawnRegion = FirstRegionPlayerDuck;
        if( PlayerShooting ) P->DrawnRegion++;
        return;
    }
    
    // case 5: walking
    if( abs( P->ShapeBox.Velocity.x ) > 15 )
    {
        int StepsFromLeft = P->ShapeBox.Position.x / 16;
        P->DrawnRegion = FirstRegionPlayerWalk + StepsFromLeft % 4;
        if( PlayerShooting ) P->DrawnRegion += 4;
        return;
    }
    
    // case 6 (default): standing
    P->DrawnRegion = FirstRegionPlayerStanding;
    if( PlayerShooting ) P->DrawnRegion++;
}

// ---------------------------------------------------------

void Player_Draw( Player* P, Vector2D* LevelTopLeftOnScreen )
{
    int PlayerX = LevelTopLeftOnScreen->x + P->ShapeBox.Position.x;
    int PlayerY = LevelTopLeftOnScreen->y + P->ShapeBox.Position.y;
    
    // invert image on X when facing left
    int ZoomX = 1;
    
    if( P->FacingLeft )
      ZoomX = -1;
      
    set_drawing_scale( ZoomX, 1 );
    select_region( P->DrawnRegion );
    draw_region_zoomed_at( PlayerX, PlayerY );
}
