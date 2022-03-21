// ---------------------------------------------------------
//   BOSS FOR END OF LEVEL 1
// ---------------------------------------------------------


enum Boss1States
{
    Boss1_Inactive,
    Boss1_Moving,
    Boss1_ShootingBullet1,
    Boss1_ShootingBullet2,
    Boss1_ShootingCannon,
    Boss1_Aiming,
    Boss1_Exploding,
    Boss1_BigExplosion,
    Boss1_Dead
};

// ---------------------------------------------------------

struct Boss1
{
    // physics
    Box ShapeBox;
    
    // initial state
    Vector2D StartPosition;
    int InitialHealth;
    
    // state variables
    Boss1States State;
    int Health;
    float CannonAngle;
    int FramesSinceLastHit;
    
    // action planning
    float TargetCannonAngle;
    int TargetXOffset;
    int ElapsedFrames;
};

// ---------------------------------------------------------

void Boss1_Create( Boss1* B, int TileX, int TileY )
{
    B->ShapeBox.Width = 140;
    B->ShapeBox.Height = 107;
    B->ShapeBox.Hotspot.x = -10;
    B->ShapeBox.Hotspot.y = 86;
    B->StartPosition.x = TileWidth * (TileX + 1);
    B->StartPosition.y = TileHeight * (TileY + 1) - 1;
    B->InitialHealth = 30;
}

// ---------------------------------------------------------

void Boss1_Reset( Boss1* B )
{
    Box_FullStop( &B->ShapeBox );
    B->ShapeBox.Position = B->StartPosition;
    B->ElapsedFrames = 0;
    B->FramesSinceLastHit = 60;
    B->State = Boss1_Inactive;
    B->Health = B->InitialHealth;
    B->CannonAngle = pi * -0.7;
    B->TargetCannonAngle = B->CannonAngle;
}

// ---------------------------------------------------------

void Boss1_ShootBullet( Boss1* B )
{
    Vector2D ShotPosition = { B->ShapeBox.Position.x - 15, B->ShapeBox.Position.y - 46 };
    Vector2D ShotVelocity = { -150, 0 };
    CreateEnemyShot( &ShotPosition, &ShotVelocity );
    
    if( B->State == Boss1_Moving )
      B->State = Boss1_ShootingBullet1;
    else
      B->State = Boss1_ShootingBullet2;
      
    B->ElapsedFrames = 0;
}

// ---------------------------------------------------------

void Boss1_ShootCannon( Boss1* B )
{
    Vector2D ShotPosition = { B->ShapeBox.Position.x + 51, B->ShapeBox.Position.y - 61 };
    ShotPosition.x += 50 * cos( B->CannonAngle );
    ShotPosition.y += 50 * sin( B->CannonAngle );
    
    CreateCannonBullet( &ShotPosition, B->CannonAngle, 250 );
    
    B->State = Boss1_ShootingCannon;
    B->ElapsedFrames = 0;
}

// ---------------------------------------------------------

void Boss1_DetectImpacts( Boss1* B )
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
                B->State = Boss1_Exploding;
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

void Boss1_Update( Boss1* B )
{
    B->ElapsedFrames++;
    
    switch( B->State )
    {
        case Boss1_Dead:
          return;
        
        case Boss1_Inactive:
        {
            // activate when the stage has finished scrolling
            int ScreenRight = CurrentLevelMap.LeftCameraLimit + screen_width;
            int LevelWidth = TileWidth * CurrentLevelMap.TilesInX;
            
            if( ScreenRight >= LevelWidth )
            {
                B->TargetXOffset = -200;
                B->State = Boss1_Moving;
                B->ElapsedFrames = 0;
            }
            
            break;
        }
        
        case Boss1_Moving:
        {
            int TargetXPosition = B->StartPosition.x + B->TargetXOffset;
            
            if( B->ShapeBox.Position.x < TargetXPosition )
              B->ShapeBox.Position.x += 2;
            
            else if( B->ShapeBox.Position.x > TargetXPosition )
              B->ShapeBox.Position.x -= 2;
              
            if( abs(B->ShapeBox.Position.x - TargetXPosition) < 2 )
            {
                B->ShapeBox.Position.x = TargetXPosition;
                Boss1_ShootBullet( B );
            }
            
            break;
        }
        
        case Boss1_ShootingBullet1:
        {
            if( B->ElapsedFrames >= 60 )
            {
                // alternate between 2 angles
                if( B->TargetCannonAngle == pi * -0.7 )
                  B->TargetCannonAngle = pi * -0.8;
                else
                  B->TargetCannonAngle = pi * -0.7;
                
                B->State = Boss1_Aiming;
                B->ElapsedFrames = 0;
            }
            
            break;
        }
        
        case Boss1_Aiming:
        {
            if( B->CannonAngle < B->TargetCannonAngle )
              B->CannonAngle += pi/200;
            
            else if( B->CannonAngle > B->TargetCannonAngle )
              B->CannonAngle -= pi/200;
            
            if( fabs(B->CannonAngle - B->TargetCannonAngle) < (pi/200) )
            {
                B->CannonAngle = B->TargetCannonAngle;
                Boss1_ShootCannon( B );
            }

            break;
        }
        
        case Boss1_ShootingCannon:
        {
            if( B->ElapsedFrames >= 60 )
              Boss1_ShootBullet( B );
            
            break;            
        }
        
        case Boss1_ShootingBullet2:
        {
            if( B->ElapsedFrames >= 30 )
            {
                // alternate between 2 positions
                if( B->TargetXOffset >= -200 )
                  B->TargetXOffset = -350;
                else
                  B->TargetXOffset = -150;
                
                B->State = Boss1_Moving;
                B->ElapsedFrames = 0;
            }
            
            break;
        }
        
        case Boss1_Exploding:
        {
            if( B->ElapsedFrames == 1 )
            {
                Vector2D HighlightPosition = { B->ShapeBox.Position.x + 36, B->ShapeBox.Position.y - 60 };
                CreateHighlight( &HighlightPosition );
                play_sound( SoundExplosionSmall );
            }
                
            if( B->ElapsedFrames == 15 )
            {
                Vector2D HighlightPosition = { B->ShapeBox.Position.x + 76, B->ShapeBox.Position.y - 60 };
                CreateHighlight( &HighlightPosition );
                play_sound( SoundExplosionSmall );
            }
                
            if( B->ElapsedFrames == 30 )
            {
                Vector2D HighlightPosition = { B->ShapeBox.Position.x + 116, B->ShapeBox.Position.y - 60 };
                CreateHighlight( &HighlightPosition );
                play_sound( SoundExplosionSmall );
            }
            
            if( B->ElapsedFrames >= 45 )
            {
                play_sound( SoundExplosionLarge );
                B->State = Boss1_BigExplosion;
                B->ElapsedFrames = 0;
            }
            
            break;
        }
        
        case Boss1_BigExplosion:
        {
            if( B->ElapsedFrames >= 60 )
            {
                B->State = Boss1_Dead;
                Gameplay_ChangeState( Gameplay_LevelClear );
            }
            
            break;
        }
    }
    
    // now, for active states we also check if
    // the boss is killed by a player shot,
    // or if it kills the player by contact
    if( B->State != Boss1_Inactive && B->State != Boss1_Dead
    &&  B->State != Boss1_Exploding && B->State != Boss1_BigExplosion )
      Boss1_DetectImpacts( B );
}

// ---------------------------------------------------------

void Boss1_Draw( Boss1* B, Vector2D* LevelTopLeftOnScreen )
{
    // don't draw inactive or dead bosses
    if( B->State == Boss1_Inactive || B->State == Boss1_Dead )
      return;
    
    int BossX = LevelTopLeftOnScreen->x + B->ShapeBox.Position.x;
    int BossY = LevelTopLeftOnScreen->y + B->ShapeBox.Position.y;
    
    // when boss is in its large explosion
    // it only shows a big highlighy
    if( B->State == Boss1_BigExplosion )
    {
        if( B->ElapsedFrames <= 24 )
        {
            int AnimationFrame = min( 5, B->ElapsedFrames/4 );
            select_region( FirstRegionHighlight + AnimationFrame );
            
            set_drawing_scale( 2.5, 2.5 );
            draw_region_zoomed_at( BossX + 80, BossY - 60 );
        }
        
        return;
    }
    
    // draw wheels rotating
    int MovedX = B->ShapeBox.Position.x - B->StartPosition.x;
    float WheelAngle = pi / 100.0 * MovedX;
    select_region( RegionBossWheel );
    set_drawing_angle( WheelAngle );
    draw_region_rotated_at( BossX+29, BossY-22 );
    draw_region_rotated_at( BossX+29+50, BossY-22 );
    draw_region_rotated_at( BossX+29+100, BossY-22 );
    
    // draw cannon
    if( B->State == Boss1_ShootingCannon && B->ElapsedFrames < 5 )
      select_region( FirstRegionBossCannon+1 );
    else
      select_region( FirstRegionBossCannon );
    
    set_drawing_angle( B->CannonAngle );
    draw_region_rotated_at( BossX+51, BossY-61 );
    
    // draw body with an up-down offset
    // to visually create engine movement
    int OffsetY = 0;
    
    if( (get_frame_counter() / 4) % 2 )
      OffsetY = 1;
    
    select_region( RegionBoss1 );
    draw_region_at( BossX, BossY + OffsetY );
    
    // when boss is being hit, highlight its chassis
    if( B->FramesSinceLastHit < 4 )
    {
        set_blending_mode( blending_add );
        draw_region_at( BossX, BossY + OffsetY );
        set_blending_mode( blending_alpha );
    }
}
