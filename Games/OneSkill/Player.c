// *****************************************************************************
    // include project headers
    #include "Player.h"
    #include "Definitions.h"
// *****************************************************************************


// ---------------------------------------------------------
//   PLAYER FUNCTIONS IMPLEMENTATION
// ---------------------------------------------------------


void Player_Create( Player* P, int TileX, int TileY )
{
    P->ShapeBox.Width = 23;
    P->ShapeBox.Height = 37;
    P->ShapeBox.Hotspot.x = 11;
    P->ShapeBox.Hotspot.y = 37;
    P->StartPosition.x = TileWidth * (TileX + 0.5);
    P->StartPosition.y = TileHeight * (TileY + 1);
}

// ---------------------------------------------------------

void Player_Reset( Player* P )
{
    Box_FullStop( &P->ShapeBox );
    P->ShapeBox.Position = P->StartPosition;
    
    P->PreviousGroundSpeedX = 0;
    P->GroundSpeedX = 0;
    
    P->Skill = Skill_None;
    P->FramesToCancelJump = 0;
    P->DrawnRegion = FirstRegionNeutralIdle;
    P->FacingDirectionX = 1;
    
    // do this to avoid detecting landing on frame 1
    P->ShapeBox.Contacts.Bottom = true;
    P->ShapeBox.PreviousContacts.Bottom = true;
    P->ShapeBox.Velocity.y = 5;
    
    P->IsDead = false;
    P->DidDoubleJump = false;
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
    int DirectionX, DirectionY;
    gamepad_direction( &DirectionX, &DirectionY );
    
    // only update player direction with active X inputs
    // (player will keep facing at the last pressed X direction)
    if( DirectionX != 0 ) P->FacingDirectionX = DirectionX;
    
    // CASE 1: apply player ground movement
    if( P->ShapeBox.PreviousContacts.Bottom )
    {
        float RelativeSpeedX = P->ShapeBox.Velocity.x - P->PreviousGroundSpeedX;
        
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
        
        // let friction apply when either:
        // - player is not applying motion
        // - player is changing direction
        // - player is walking faster than its limit speed
        if( (DirectionX == 0) || (DirectionX * RelativeSpeedX < 0) || (fabs(RelativeSpeedX) > MaxSpeedX) )
          Player_ApplyFriction( P );
    }
    
    // CASE 2: apply player air movement
    else
    {
        // upon horizontal inputs, apply reduced motion force
        if( DirectionX > 0 )
        {
            if( P->ShapeBox.Velocity.x < MaxSpeedX )
              P->ShapeBox.Acceleration.x += AirAcceleration;
        }
        
        else if( DirectionX< 0 )
        {
            if( P->ShapeBox.Velocity.x > -MaxSpeedX )
              P->ShapeBox.Acceleration.x -= AirAcceleration;
        }
    }
    
    // simulate tentative player motion for this frame
    Box_Simulate( &P->ShapeBox );
    
    // button A makes player jump
    if( gamepad_button_a() == 1 )
    {
        bool CanJump = P->ShapeBox.PreviousContacts.Bottom;
        
        // with cape, allow a single double jump
        if( !CanJump && P->Skill == Skill_Cape && !P->DidDoubleJump )
        {
            // don't allow both jumps to be too close
            if( P->FramesToCancelJump == 0 )
            {
                CanJump = true;
                P->DidDoubleJump = true;
            }
        }
        
        // apply jump as an fixed upwards speed
        if( CanJump )
        {
            P->ShapeBox.Velocity.y = 0;
            P->ShapeBox.DeltaVelocity.y = -JumpImpulse;
            P->FramesToCancelJump = JumpCancelFrames;
            
            if( P->DidDoubleJump )
            {
                play_sound( SoundDoubleJump );
                
                // create a highlight under player's feet
                Vector2D HighlightPosition = P->ShapeBox.Position;
                HighlightPosition.y -= 5;
                CreateHighlight( &HighlightPosition );
            }
            else
              play_sound( SoundJump );
        }
        
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
    
    // button B with the pistol shoots a bullet
    if( gamepad_button_b() == 1 && P->Skill == Skill_Pistol )
    {
        Vector2D ShotPosition = P->ShapeBox.Position;
        ShotPosition.x += 32 * P->FacingDirectionX;
        ShotPosition.y -= 23;
        CreatePistolShot( &ShotPosition, P->FacingDirectionX > 0 );
        play_sound( SoundShoot );
    }
    
    // button B with the wand uses magic
    if( gamepad_button_b() == 1 && P->Skill == Skill_Wand )
    {
        // create a highlight on the wand's tip
        Vector2D HighlightPosition;
        HighlightPosition.x = P->ShapeBox.Position.x + 23 * P->FacingDirectionX;
        HighlightPosition.y = P->ShapeBox.Position.y - 33;
        CreateHighlight( &HighlightPosition );
        
        // play sound for wand magic
        play_sound( SoundWandMagic );
        
        // switch all on-off blocks on the whole room
        for( int TileY = 0; TileY <= MapLevel.map_height; ++TileY )
        for( int TileX = 0; TileX <= MapLevel.map_width;  ++TileX )
        {
            int* Tile = &MapLevel.map[ MapLevel.array_width * TileY + TileX ];
            
            if( *Tile == (int)Tile_OnBlock )
              *Tile = (int)Tile_OffBlock;
            else if( *Tile == (int)Tile_OffBlock )
              *Tile = (int)Tile_OnBlock;
        }
    }
}

// ---------------------------------------------------------

void Player_ApplyMovement( Player* P )
{
    // actually apply projected movement
    Box_Move( &P->ShapeBox );
    
    // detect player landing
    if( !P->ShapeBox.PreviousContacts.Bottom && P->ShapeBox.Contacts.Bottom )
    {
        // renew double jump
        P->DidDoubleJump = false;
        
        // use a different sound for boots
        if( P->Skill == Skill_Boots )
          play_sound( SoundBootsLand );
        else
          play_sound( SoundLand );
    }
}

// ---------------------------------------------------------

void Player_UpdateAnimation( Player* P )
{
    // select animation frames
    int IdleAnimationFrame = (Gameplay_ElapsedFrames / 20) % 2;
    P->DrawnRegion = FirstRegionNeutralIdle + (2 * Player1.Skill) + IdleAnimationFrame;
    
    // case 1: death
    if( GameState == Gameplay_Death )
    {
        P->DrawnRegion = RegionPlayerDeath;
        return;
    }
    
    // case 2: level or room clear
    if( GameState == Gameplay_LevelClear || GameState == Gameplay_RoomClear )
    {
        int AnimationFrame = min( Gameplay_ElapsedFrames / 5, 6 );
        P->DrawnRegion = FirstRegionEnterDoor + AnimationFrame;
        return;
    }
    
    bool MovingOnX = (abs( P->ShapeBox.Velocity.x ) > 40);
    
    // possible states on ground
    if( P->ShapeBox.Contacts.Bottom )
    {
        if( MovingOnX )
        {
            // determine step animation from walked distance on X
            int StepsFromLeft = P->ShapeBox.Position.x / 12;
            int WalkAnimationFrame = StepsFromLeft % 4;
            
            // we need to invert animation direction when going
            // left or else it looks like walking backwards
            if( P->FacingDirectionX < 0 ) WalkAnimationFrame = 3 - WalkAnimationFrame;
            
            P->DrawnRegion = FirstRegionNeutralWalk + (4 * Player1.Skill) + WalkAnimationFrame;
        }
    }
    
    // possible states in air
    else
    {
        if( P->ShapeBox.Velocity.y < 0 )
          P->DrawnRegion = RegionNeutralJumpUp + Player1.Skill;
        
        else
          P->DrawnRegion = RegionNeutralJumpDown + Player1.Skill;
    }
}

// ---------------------------------------------------------

void Player_Draw( Player* P, Vector2D* LevelTopLeftOnScreen )
{
    int PlayerX = LevelTopLeftOnScreen->x + P->ShapeBox.Position.x;
    int PlayerY = LevelTopLeftOnScreen->y + P->ShapeBox.Position.y;
    
    // invert image on X when facing left
    int ZoomX = 1;
    
    if( P->FacingDirectionX < 0 )
      ZoomX = -1;
      
    set_drawing_scale( ZoomX, 1 );
    select_region( P->DrawnRegion );
    draw_region_zoomed_at( PlayerX, PlayerY );
}
