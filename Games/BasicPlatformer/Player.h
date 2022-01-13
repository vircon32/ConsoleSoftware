// player motion constants
#define Gravity 1000
#define JumpImpulse 480
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
    bool ControlsEnabled;
    int DrawnRegion;                 // the currently displayed region
    int FramesToCancelJump;   // 
    int DirectionX;
    
    // collectables
    int CollectedCoins;
    int CollectedKeys;
};

// ---------------------------------------------------------

void Player_Create( Player* P, int TileX, int TileY )
{
    P->ShapeBox.Width = 32;
    P->ShapeBox.Height = 35;
    P->ShapeBox.Hotspot.x = 15;
    P->ShapeBox.Hotspot.y = 34;
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
    P->FramesToCancelJump = 0;
    P->DrawnRegion = RegionPlayerIdle;
    P->DirectionX = 0;
    
    P->CollectedCoins = 0;
    P->CollectedKeys = 0;
    
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
    // reset player physics for this next frame
    Box_BeginNewFrame( &P->ShapeBox );
    P->PreviousGroundSpeedX = P->GroundSpeedX;
    P->GroundSpeedX = 0;
    
    // apply gravity, up to a certain terminal velocity
    if( P->ShapeBox.Velocity.y < MaxFallSpeed )
      P->ShapeBox.Acceleration.y += Gravity;
    
    // check movement inputs
    int DirectionY;
    gamepad_direction( &P->DirectionX, &DirectionY );
    
    // CASE 1: apply player ground movement
    if( P->ShapeBox.PreviousContacts.Bottom )
    {
        float RelativeSpeedX = P->ShapeBox.Velocity.x - P->PreviousGroundSpeedX;
        
        if( P->DirectionX < 0 )
        {
            if( RelativeSpeedX >= -MaxSpeedX )
              P->ShapeBox.Acceleration.x -= GroundAcceleration;
        }
        
        else if( P->DirectionX > 0 )
        {
            if( RelativeSpeedX <= MaxSpeedX )
              P->ShapeBox.Acceleration.x += GroundAcceleration;
        }
        
        // let friction apply when either:
        // - player is not applying motion
        // - player is changing direction
        // - player is walking faster than its limit speed
        if( (P->DirectionX == 0) || (P->DirectionX * RelativeSpeedX < 0) || (fabs(RelativeSpeedX) > MaxSpeedX) )
          Player_ApplyFriction( P );
    }
    
    // CASE 2: apply player air movement
    else
    {
        // upon horizontal inputs, apply reduced motion force
        if( P->DirectionX > 0 )
        {
            if( P->ShapeBox.Velocity.x < MaxSpeedX )
              P->ShapeBox.Acceleration.x += AirAcceleration;
        }
        
        else if( P->DirectionX< 0 )
        {
            if( P->ShapeBox.Velocity.x > -MaxSpeedX )
              P->ShapeBox.Acceleration.x -= AirAcceleration;
        }
    }
    
    // simulate tentative player motion for this frame
    Box_Simulate( &P->ShapeBox );
    
    // jump is an upward impulse
    if( gamepad_button_a() == 1 )
      if( P->ShapeBox.PreviousContacts.Bottom )
        {
            P->ShapeBox.DeltaVelocity.y -= JumpImpulse;
            P->FramesToCancelJump = JumpCancelFrames;
            play_sound( SoundJump );
        }
    
    if( P->FramesToCancelJump > 0 )
      P->FramesToCancelJump--;
    
    // allow jumps to be lower by releasing the button
    if( gamepad_button_a() == -1 )
      if( P->FramesToCancelJump > 0 )
      {
          P->ShapeBox.DeltaVelocity.y -= P->ShapeBox.Velocity.y * JumpCancelFactor;
          P->FramesToCancelJump = 0;
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

void Player_UpdateAnimation( Player* P )
{
    // select animation frames
    P->DrawnRegion = RegionPlayerIdle;
    
    // case 1: death
    if( GameState == Gameplay_Death )
    {
        P->DrawnRegion = RegionPlayerDeath;
        return;
    }
    
    // case 2: level clear
    if( GameState == Gameplay_Goal )
    {
        if( (Gameplay_ElapsedFrames & 16) == 0 )
          P->DrawnRegion = RegionPlayerVictory;
        
        return;
    }
    
    bool FacingSide = (P->DirectionX != 0 || abs( P->ShapeBox.Velocity.x ) > 40);
    
    // possible states on ground
    if( P->ShapeBox.Contacts.Bottom )
    {
        if( P->ShapeBox.Contacts.Left || P->ShapeBox.Contacts.Right )
          P->DrawnRegion = RegionPlayerPush;
        
        else if( FacingSide )
        {
            int StepsFromLeft = P->ShapeBox.Position.x / 12;
            P->DrawnRegion = FirstRegionPlayerWalk + StepsFromLeft % 4;
        }
    }
    
    // possible states in air
    else
    {
        if( P->ShapeBox.Velocity.y < 0 )
        {
            if( FacingSide )
              P->DrawnRegion = RegionPlayerJumpSide;
            else
              P->DrawnRegion = RegionPlayerJumpFront;
        }
        
        else
        {
            if( FacingSide )
              P->DrawnRegion = RegionPlayerFallSide;
            else
              P->DrawnRegion = RegionPlayerFallFront;
        }
    }
}

// ---------------------------------------------------------

void Player_Draw( Player* P, Vector2D* LevelTopLeftOnScreen )
{
    int PlayerX = LevelTopLeftOnScreen->x + P->ShapeBox.Position.x;
    int PlayerY = LevelTopLeftOnScreen->y + P->ShapeBox.Position.y;
    
    // invert image on X when facing left
    int ZoomX = 1;
    
    if( P->DirectionX < 0 || (P->DirectionX == 0 && P->ShapeBox.Velocity.x < 15) )
      ZoomX = -1;
      
    set_drawing_scale( ZoomX, 1 );
    select_region( P->DrawnRegion );
    draw_region_zoomed_at( PlayerX, PlayerY );
    
    // now draw any collected keys
    select_region( RegionCollectedKey );
    int KeyX = PlayerX - 10;
    int KeyY = PlayerY - 44;
    
    for( int i = 0; i < P->CollectedKeys; ++i )
    {
        draw_region_at( KeyX, KeyY );
        KeyX -= 5;
        KeyY -= 20;
    }
}
