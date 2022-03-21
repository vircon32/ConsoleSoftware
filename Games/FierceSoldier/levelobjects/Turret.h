// ---------------------------------------------------------
//   ROTATING TURRET
// ---------------------------------------------------------


enum TurretStates
{
    Turret_Inactive,
    Turret_Waiting,
    Turret_Aiming,
    Turret_Shooting,
    Turret_Dead
};

// ---------------------------------------------------------

struct Turret
{
    // physics
    Box ShapeBox;
    
    // initial state
    Vector2D StartPosition;
    int InitialHealth;
    
    // state variables
    TurretStates State;
    float CannonAngle;
    int FramesSinceLastHit;
    int Health;
    
    // action planning
    float TargetCannonAngle;
    int FramesUntilNextAction;
};

// ---------------------------------------------------------

void Turret_Create( Turret* T, int TileX, int TileY )
{
    T->ShapeBox.Width = 44;
    T->ShapeBox.Height = 44;
    T->ShapeBox.Hotspot.x = 22;
    T->ShapeBox.Hotspot.y = 22;
    T->StartPosition.x = TileWidth * (TileX + 1) - 1;
    T->StartPosition.y = TileHeight * (TileY + 1) - 1;
    T->InitialHealth = 6;
    T->CannonAngle = pi;
    T->TargetCannonAngle = T->CannonAngle;
}

// ---------------------------------------------------------

void Turret_Reset( Turret* T )
{
    Box_FullStop( &T->ShapeBox );
    T->ShapeBox.Position = T->StartPosition;
    
    T->FramesUntilNextAction = 30;
    T->FramesSinceLastHit = 60;
    T->State = Turret_Inactive;
    T->Health = T->InitialHealth;
}

// ---------------------------------------------------------

void Turret_Shoot( Turret* T )
{
    Vector2D ShotPosition1 = T->ShapeBox.Position;
    ShotPosition1.x += 40 * cos( T->CannonAngle - pi/20 );
    ShotPosition1.y += 40 * sin( T->CannonAngle - pi/20 );
    
    Vector2D ShotPosition2 = T->ShapeBox.Position;
    ShotPosition2.x += 40 * cos( T->CannonAngle + pi/20 );
    ShotPosition2.y += 40 * sin( T->CannonAngle + pi/20 );    
    
    Vector2D ShotVelocity;
    ShotVelocity.x = 200 * cos( T->CannonAngle );
    ShotVelocity.y = 200 * sin( T->CannonAngle );    
    
    CreateEnemyShot( &ShotPosition1, &ShotVelocity );
    CreateEnemyShot( &ShotPosition2, &ShotVelocity );
    
    T->State = Turret_Shooting;
    T->FramesUntilNextAction = 5;
}

// ---------------------------------------------------------

void Turret_DetectImpacts( Turret* T )
{
    T->FramesSinceLastHit++;
    
    // detect impacts from player bullets
    for( int i = 0; i < 10; i++ )
    {
        if( !PlayerShots[ i ].Active )
          continue;
        
        if( Box_OverlapWith( &PlayerShots[ i ].ShapeBox, &T->ShapeBox ) )
        {
            // destroy player shot
            PlayerShots[ i ].Active = false;
            
            // reduce turret health
            T->Health--;
            
            // kill turret when no health remains
            if( T->Health <= 0 )
            {
                T->State = Turret_Dead;
                CreateHighlight( &T->ShapeBox.Position );
                play_sound( SoundExplosionSmall );
            }
            
            play_sound( SoundEnemyHit );
            T->FramesSinceLastHit = 0;
            break;
        }
    }
    
    // kill the player on impact
    if( Box_OverlapWith( &Player1.ShapeBox, &T->ShapeBox ) )
      Gameplay_ChangeState( Gameplay_Death );
}

// ---------------------------------------------------------

void Turret_Update( Turret* T )
{
    switch( T->State )
    {
        case Turret_Dead:
          return;
        
        case Turret_Inactive:
        {
            // activate when the screen has scrolled close enough
            int TurretLeft = Box_Left( &T->ShapeBox );
            int ScreenRight = CurrentLevelMap.LeftCameraLimit + screen_width;
            
            if( ScreenRight >= TurretLeft )
              T->State = Turret_Waiting;
            
            break;
        }
        
        case Turret_Waiting:
        {
            T->FramesUntilNextAction--;
            
            if( T->FramesUntilNextAction <= 0 )
            {
                T->State = Turret_Aiming;
                
                // determine turret->player angle
                Vector2D PlayerPosition;
                PlayerPosition.x = (Box_Left(&Player1.ShapeBox) + Box_Right(&Player1.ShapeBox))/2;
                PlayerPosition.y = (Box_Top(&Player1.ShapeBox) + Box_Bottom(&Player1.ShapeBox))/2;
                
                float PlayerAngle = atan2
                (
                    PlayerPosition.y - T->ShapeBox.Position.y,
                    PlayerPosition.x - T->ShapeBox.Position.x
                );
                
                // atan2 is discontinuous at pi, which is not good here
                // so check if we should go the other way around
                float PlayerAngle2 = PlayerAngle + 2*pi;
                float Distance1 = fabs(PlayerAngle - T->CannonAngle);
                float Distance2 = fabs(PlayerAngle2 - T->CannonAngle);
                
                if( Distance2 < Distance1 )
                  T->TargetCannonAngle = PlayerAngle2;
                else
                  T->TargetCannonAngle = PlayerAngle;
            }
            
            break;
        }
        
        case Turret_Aiming:
        {
            // try to approximate that angle
            if( T->CannonAngle < T->TargetCannonAngle )
              T->CannonAngle += pi/200;
            
            else if( T->CannonAngle > T->TargetCannonAngle )
              T->CannonAngle -= pi/200;
            
            // when close enough, lock and shoot
            if( fabs(T->CannonAngle - T->TargetCannonAngle) < (pi/200) )
            {
                T->CannonAngle = T->TargetCannonAngle;
                Turret_Shoot( T );
            }
            
            break;
        }
        
        case Turret_Shooting:
        {
            T->FramesUntilNextAction--;
            
            if( T->FramesUntilNextAction <= 0 )
            {
                T->State = Turret_Waiting;
                T->FramesUntilNextAction = 150;
            }
            
            break;
        }
        
        default: break;
    }
    
    // now, for active states we also check if
    // the turret is killed by a player shot,
    // or if it kills the player by contact
    if( T->State != Turret_Inactive )
      Turret_DetectImpacts( T );
    
    // kill silently when they are left
    // behind at the far left
    if( T->State != Turret_Inactive )
    {
        int TurretRight = Box_Right( &T->ShapeBox );
        int ScreenLeft = CurrentLevelMap.LeftCameraLimit;
        
        if( TurretRight <= ScreenLeft )
          T->State = Turret_Dead;
    }
}

// ---------------------------------------------------------

void Turret_Draw( Turret* T, Vector2D* LevelTopLeftOnScreen )
{
    // don't draw inactive or dead turrets
    if( T->State == Turret_Inactive || T->State == Turret_Dead )
      return;
    
    int TurretX = LevelTopLeftOnScreen->x + T->ShapeBox.Position.x;
    int TurretY = LevelTopLeftOnScreen->y + T->ShapeBox.Position.y;
    
    // draw the fixed base
    select_region( RegionTurretBase );
    draw_region_at( TurretX, TurretY );
    
    // draw cannon with its rotation
    select_region( RegionTurretCannon );
    set_drawing_angle( T->CannonAngle );
    draw_region_rotated_at( TurretX, TurretY );
    
    // when turret is being hit, highlight it
    if( T->FramesSinceLastHit < 4 )
    {
        set_blending_mode( blending_add );
        select_region( RegionTurretBase );
        draw_region();
        select_region( RegionTurretCannon );
        draw_region_rotated();
        set_blending_mode( blending_alpha );
    }
}

