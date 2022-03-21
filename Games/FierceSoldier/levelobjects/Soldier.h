// ---------------------------------------------------------
//   STANDING ENEMY SOLDIER
// ---------------------------------------------------------


enum SoldierStates
{
    Soldier_Inactive,
    Soldier_Waiting,
    Soldier_Shooting,
    Soldier_Dead
};

// ---------------------------------------------------------

struct Soldier
{
    // physics
    Box ShapeBox;
    
    // initial state
    Vector2D StartPosition;
    
    // state variables
    SoldierStates State;
    int FramesUntilNextAction;
    int DrawnRegion;
};

// ---------------------------------------------------------

void Soldier_Create( Soldier* S, int TileX, int TileY )
{
    S->ShapeBox.Width = 21;
    S->ShapeBox.Height = 82;
    S->ShapeBox.Hotspot.x = 10;
    S->ShapeBox.Hotspot.y = 81;
    S->StartPosition.x = TileWidth * (TileX + 0.5);
    S->StartPosition.y = TileHeight * (TileY + 1) - 1;
}

// ---------------------------------------------------------

void Soldier_Reset( Soldier* S )
{
    Box_FullStop( &S->ShapeBox );
    S->ShapeBox.Position = S->StartPosition;
    
    S->FramesUntilNextAction = 50;
    S->State = Soldier_Inactive;
    S->DrawnRegion = FirstRegionSoldier;
}

// ---------------------------------------------------------

void Soldier_Shoot( Soldier* S )
{
    Vector2D ShotPosition = { S->ShapeBox.Position.x - 35, S->ShapeBox.Position.y - 53 };
    Vector2D ShotVelocity = { -200, 0 };
    CreateEnemyShot( &ShotPosition, &ShotVelocity );
    
    S->State = Soldier_Shooting;
    S->FramesUntilNextAction = 5;
}

// ---------------------------------------------------------

void Soldier_DetectImpacts( Soldier* S )
{
    // detect impacts from player bullets
    for( int i = 0; i < 10; i++ )
    {
        if( !PlayerShots[ i ].Active )
          continue;
        
        if( Box_OverlapWith( &PlayerShots[ i ].ShapeBox, &S->ShapeBox ) )
        {
            // destroy player shot
            PlayerShots[ i ].Active = false;
            
            // kill soldier
            S->State = Soldier_Dead;
            Vector2D HighlightPosition = { S->ShapeBox.Position.x, S->ShapeBox.Position.y - 40 };
            CreateHighlight( &HighlightPosition );
            play_sound( SoundEnemyDeath );
            break;
        }
    }
    
    // kill the player on impact
    if( Box_OverlapWith( &Player1.ShapeBox, &S->ShapeBox ) )
      Gameplay_ChangeState( Gameplay_Death );
}

// ---------------------------------------------------------

void Soldier_Update( Soldier* S )
{
    switch( S->State )
    {
        case Soldier_Dead:
          return;
        
        case Soldier_Inactive:
        {
            // activate when the screen has scrolled close enough
            int SoldierLeft = Box_Left( &S->ShapeBox );
            int ScreenRight = CurrentLevelMap.LeftCameraLimit + screen_width;
            
            if( ScreenRight >= SoldierLeft )
              S->State = Soldier_Waiting;
            
            break;
        }
        
        case Soldier_Waiting:
        {
            S->FramesUntilNextAction--;
            
            if( S->FramesUntilNextAction <= 0 )
              Soldier_Shoot( S );
            
            break;
        }
        
        case Soldier_Shooting:
        {
            S->FramesUntilNextAction--;
            
            if( S->FramesUntilNextAction <= 0 )
            {
                S->State = Soldier_Waiting;
                S->FramesUntilNextAction = 60 + rand() % 100;
            }
            
            break;
        }
        
        default: break;
    }
    
    // now, for active states we also check if
    // the soldier is killed by a player shot,
    // or if it kills the player by contact
    if( S->State == Soldier_Waiting || S->State == Soldier_Shooting )
      Soldier_DetectImpacts( S );
    
    // kill silently when they are left
    // behind at the far left
    if( S->State != Soldier_Inactive )
    {
        int SoldierRight = Box_Right( &S->ShapeBox );
        int ScreenLeft = CurrentLevelMap.LeftCameraLimit;
        
        if( SoldierRight <= ScreenLeft )
          S->State = Soldier_Dead;
    }
    
    // choosing the drawn region is very simple here
    S->DrawnRegion = FirstRegionSoldier;
    
    if( S->State == Soldier_Shooting )
      S->DrawnRegion++;
}

// ---------------------------------------------------------

void Soldier_Draw( Soldier* S, Vector2D* LevelTopLeftOnScreen )
{
    // don't draw inactive or dead soldiers
    if( S->State == Soldier_Inactive || S->State == Soldier_Dead )
      return;
    
    int SoldierX = LevelTopLeftOnScreen->x + S->ShapeBox.Position.x;
    int SoldierY = LevelTopLeftOnScreen->y + S->ShapeBox.Position.y;
    
    select_region( S->DrawnRegion );
    draw_region_at( SoldierX, SoldierY );
}

