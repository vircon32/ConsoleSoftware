// ---------------------------------------------------------
//   BOSS FOR END OF LEVEL 3
// ---------------------------------------------------------


enum Boss3States
{
    Boss3_Inactive,
    Boss3_Moving,
    Boss3_ShootingBullet,
    Boss3_ShootingCannon,
    Boss3_ShootingMissile,
    Boss3_Aiming,
    Boss3_Exploding,
    Boss3_BigExplosion,
    Boss3_Dead
};

// ---------------------------------------------------------

struct Boss3
{
    // physics
    Box ShapeBox;
    
    // initial state
    Vector2D StartPosition;
    int InitialHealth;
    
    // state variables
    Boss3States State;
    int Health;
    float CannonAngle;
    int FramesSinceLastHit;
    
    // action planning
    float TargetCannonAngle;
    int TargetXOffset;
    int ElapsedFrames;
};

// ---------------------------------------------------------

void Boss3_Create( Boss3* B, int TileX, int TileY )
{
    B->ShapeBox.Width = 90;
    B->ShapeBox.Height = 131;
    B->ShapeBox.Hotspot.x = -10;
    B->ShapeBox.Hotspot.y = 151;
    B->StartPosition.x = TileWidth * (TileX + 1);
    B->StartPosition.y = TileHeight * (TileY + 1) - 1;
    B->InitialHealth = 50;
}

// ---------------------------------------------------------

void Boss3_Reset( Boss3* B )
{
    Box_FullStop( &B->ShapeBox );
    B->ShapeBox.Position = B->StartPosition;
    B->ElapsedFrames = 0;
    B->FramesSinceLastHit = 60;
    B->State = Boss3_Inactive;
    B->Health = B->InitialHealth;
    B->CannonAngle = pi * -0.8;
    B->TargetCannonAngle = B->CannonAngle;
}

// ---------------------------------------------------------

void Boss3_ShootBullet( Boss3* B )
{
    Vector2D ShotPosition = { B->ShapeBox.Position.x - 15, B->ShapeBox.Position.y - 46 };
    Vector2D ShotVelocity = { -150, 0 };
    CreateEnemyShot( &ShotPosition, &ShotVelocity );
        
    B->State = Boss3_ShootingBullet;
    B->ElapsedFrames = 0;
}

// ---------------------------------------------------------

void Boss3_ShootCannon( Boss3* B )
{
    Vector2D ShotPosition = { B->ShapeBox.Position.x + 51, B->ShapeBox.Position.y - 105 };
    ShotPosition.x += 50 * cos( B->CannonAngle );
    ShotPosition.y += 50 * sin( B->CannonAngle );
    
    CreateCannonBullet( &ShotPosition, B->CannonAngle, 250 );
    
    B->State = Boss3_ShootingCannon;
    B->ElapsedFrames = 0;
}

// ---------------------------------------------------------

void Boss3_ShootMissile( Boss3* B )
{
    Vector2D MissilePosition = { B->ShapeBox.Position.x - 8, B->ShapeBox.Position.y - 80 };
    int PlayerMidY = (Box_Top( &Player1.ShapeBox ) + Box_Bottom( &Player1.ShapeBox )) / 2;
    
    float MissileAngle = pi*0.95;
    
    if( PlayerMidY < MissilePosition.y )
      MissileAngle = -pi*0.95;
    
    CreateMissile( &MissilePosition, MissileAngle, 250 );
    
    B->State = Boss3_ShootingMissile;
    B->ElapsedFrames = 0;
}

// ---------------------------------------------------------

void Boss3_DetectImpacts( Boss3* B )
{
    B->FramesSinceLastHit++;
    
    // detect impacts from player bullets
    for( int i = 0; i < 10; i++ )
    {
        if( !PlayerShots[ i ].Active )
          continue;
        
        if( Box_OverlapWith( &PlayerShots[ i ].ShapeBox, &B->ShapeBox ) )
        {
            // destroy player shot
            PlayerShots[ i ].Active = false;
            
            // reduce boss health
            B->Health--;
            
            // kill boss when no health remains
            if( B->Health <= 0 )
            {
                B->State = Boss3_Exploding;
                B->ElapsedFrames = 0;
            }
            
            play_sound( SoundEnemyHit );
            B->FramesSinceLastHit = 0;
            break;
        }
    }
    
    // kill the player on impact
    if( Box_OverlapWith( &Player1.ShapeBox, &B->ShapeBox ) )
      Gameplay_ChangeState( Gameplay_Death );
}

// ---------------------------------------------------------

void Boss3_Update( Boss3* B )
{
    B->ElapsedFrames++;
    
    switch( B->State )
    {
        case Boss3_Dead:
          return;
        
        case Boss3_Inactive:
        {
            // activate when the stage has finished scrolling
            int ScreenRight = CurrentLevelMap.LeftCameraLimit + screen_width;
            int LevelWidth = TileWidth * CurrentLevelMap.TilesInX;
            
            if( ScreenRight >= LevelWidth )
            {
                B->TargetXOffset = -150;
                B->State = Boss3_Moving;
                B->ElapsedFrames = 0;
            }
            
            break;
        }
        
        case Boss3_Moving:
        {
            int TargetXPosition = B->StartPosition.x + B->TargetXOffset;
            
            if( B->ShapeBox.Position.x < TargetXPosition )
              B->ShapeBox.Position.x += 2;
            
            else if( B->ShapeBox.Position.x > TargetXPosition )
              B->ShapeBox.Position.x -= 2;
              
            if( abs(B->ShapeBox.Position.x - TargetXPosition) < 2 )
            {
                B->ShapeBox.Position.x = TargetXPosition;
                Boss3_ShootMissile( B );
            }
            
            break;
        }
        
        case Boss3_ShootingMissile:
        {
            if( B->ElapsedFrames >= 30 )
            {
                // alternate between 2 angles
                if( B->TargetCannonAngle == pi * -0.8 )
                  B->TargetCannonAngle = pi * -0.9;
                else
                  B->TargetCannonAngle = pi * -0.8;
                
                B->State = Boss3_Aiming;
                B->ElapsedFrames = 0;
            }
            
            break;
        }
        
        case Boss3_Aiming:
        {
            if( B->CannonAngle < B->TargetCannonAngle )
              B->CannonAngle += pi/200;
            
            else if( B->CannonAngle > B->TargetCannonAngle )
              B->CannonAngle -= pi/200;
            
            if( fabs(B->CannonAngle - B->TargetCannonAngle) < (pi/200) )
            {
                B->CannonAngle = B->TargetCannonAngle;
                Boss3_ShootCannon( B );
            }

            break;
        }
        
        case Boss3_ShootingCannon:
        {
            if( B->ElapsedFrames >= 50 )
              Boss3_ShootBullet( B );
            
            break;            
        }
        
        case Boss3_ShootingBullet:
        {
            if( B->ElapsedFrames >= 15 )
            {
                // alternate between 2 positions
                if( B->TargetXOffset >= -150 )
                  B->TargetXOffset = -280;
                else
                  B->TargetXOffset = -150;
                
                B->State = Boss3_Moving;
                B->ElapsedFrames = 0;
            }
            
            break;
        }
        
        case Boss3_Exploding:
        {
            if( B->ElapsedFrames == 1 )
            {
                Vector2D HighlightPosition = { B->ShapeBox.Position.x + 56, B->ShapeBox.Position.y - 40 };
                CreateHighlight( &HighlightPosition );
                play_sound( SoundExplosionSmall );
            }
                
            if( B->ElapsedFrames == 15 )
            {
                Vector2D HighlightPosition = { B->ShapeBox.Position.x + 56, B->ShapeBox.Position.y - 80 };
                CreateHighlight( &HighlightPosition );
                play_sound( SoundExplosionSmall );
            }
                
            if( B->ElapsedFrames == 30 )
            {
                Vector2D HighlightPosition = { B->ShapeBox.Position.x + 56, B->ShapeBox.Position.y - 120 };
                CreateHighlight( &HighlightPosition );
                play_sound( SoundExplosionSmall );
            }
            
            if( B->ElapsedFrames >= 45 )
            {
                play_sound( SoundExplosionLarge );
                B->State = Boss3_BigExplosion;
                B->ElapsedFrames = 0;
            }
            
            break;
        }
        
        case Boss3_BigExplosion:
        {
            if( B->ElapsedFrames >= 60 )
            {
                B->State = Boss3_Dead;
                Gameplay_ChangeState( Gameplay_LevelClear );
            }
            
            break;
        }
    }
    
    // now, for active states we also check if
    // the boss is killed by a player shot,
    // or if it kills the player by contact
    if( B->State != Boss3_Inactive && B->State != Boss3_Dead
    &&  B->State != Boss3_Exploding && B->State != Boss3_BigExplosion )
      Boss3_DetectImpacts( B );
}

// ---------------------------------------------------------

void Boss3_Draw( Boss3* B, Vector2D* LevelTopLeftOnScreen )
{
    // don't draw inactive or dead bosses
    if( B->State == Boss3_Inactive || B->State == Boss3_Dead )
      return;
    
    int BossX = LevelTopLeftOnScreen->x + B->ShapeBox.Position.x;
    int BossY = LevelTopLeftOnScreen->y + B->ShapeBox.Position.y;
    
    // when boss is in its large explosion
    // it only shows a big highlighy
    if( B->State == Boss3_BigExplosion )
    {
        if( B->ElapsedFrames <= 24 )
        {
            int AnimationFrame = min( 5, B->ElapsedFrames/4 );
            select_region( FirstRegionHighlight + AnimationFrame );
            
            set_drawing_scale( 2.5, 2.5 );
            draw_region_zoomed_at( BossX + 56, BossY - 80 );
        }
        
        return;
    }
    
    // draw wheels rotating
    int MovedX = B->ShapeBox.Position.x - B->StartPosition.x;
    float WheelAngle = pi / 100.0 * MovedX;
    select_region( RegionBossWheelBlue );
    set_drawing_angle( WheelAngle );
    draw_region_rotated_at( BossX+29, BossY-22 );
    draw_region_rotated_at( BossX+29+50, BossY-22 );
    
    // draw cannon
    if( B->State == Boss3_ShootingCannon && B->ElapsedFrames < 5 )
      select_region( FirstRegionBossCannon+1 );
    else
      select_region( FirstRegionBossCannon );
    
    set_drawing_angle( B->CannonAngle );
    draw_region_rotated_at( BossX+51, BossY-105 );
    
    // draw body with an up-down offset
    // to visually create engine movement
    int OffsetY = 0;
    
    if( (get_frame_counter() / 4) % 2 )
      OffsetY = 1;
    
    select_region( RegionBoss3 );
    draw_region_at( BossX, BossY + OffsetY );
    
    // when boss is being hit, highlight its chassis
    if( B->FramesSinceLastHit < 4 )
    {
        set_blending_mode( blending_add );
        draw_region_at( BossX, BossY + OffsetY );
        set_blending_mode( blending_alpha );
    }
}
