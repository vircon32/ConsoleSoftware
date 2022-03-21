// ---------------------------------------------------------
//   MISSILE LAUNCHER
// ---------------------------------------------------------


enum LauncherStates
{
    Launcher_Inactive,
    Launcher_Waiting,
    Launcher_Shooting,
    Launcher_Dead
};

// ---------------------------------------------------------

struct Launcher
{
    // physics
    Box ShapeBox;
    
    // initial state
    Vector2D StartPosition;
    int InitialHealth;
    
    // state variables
    LauncherStates State;
    int FramesUntilNextAction;
    int FramesSinceLastHit;
    int Health;
};

// ---------------------------------------------------------

void Launcher_Create( Launcher* L, int TileX, int TileY )
{
    L->ShapeBox.Width = 38;
    L->ShapeBox.Height = 75;
    L->ShapeBox.Hotspot.x = 0;
    L->ShapeBox.Hotspot.y = 74;
    L->StartPosition.x = TileWidth * TileX;
    L->StartPosition.y = TileHeight * (TileY + 1) - 1;
    L->InitialHealth = 10;
}

// ---------------------------------------------------------

void Launcher_Reset( Launcher* L )
{
    Box_FullStop( &L->ShapeBox );
    L->ShapeBox.Position = L->StartPosition;
    
    L->FramesUntilNextAction = 60;
    L->FramesSinceLastHit = 60;
    L->State = Launcher_Inactive;
    L->Health = L->InitialHealth;
}

// ---------------------------------------------------------

void Launcher_Shoot( Launcher* L )
{
    Vector2D MissilePosition = { L->ShapeBox.Position.x - 36, L->ShapeBox.Position.y - 53 };
    CreateMissile( &MissilePosition, pi, 200 );
    
    L->State = Launcher_Shooting;
    L->FramesUntilNextAction = 5;
}

// ---------------------------------------------------------

void Launcher_DetectImpacts( Launcher* L )
{
    L->FramesSinceLastHit++;
    
    // detect impacts from player bullets
    for( int i = 0; i < 10; i++ )
    {
        if( !PlayerShots[ i ].Active )
          continue;
        
        if( Box_OverlapWith( &PlayerShots[ i ].ShapeBox, &L->ShapeBox ) )
        {
            // destroy player shot
            PlayerShots[ i ].Active = false;
            
            // reduce launcher health
            L->Health--;
            
            // kill launcher when no health remains
            if( L->Health <= 0 )
            {
                L->State = Launcher_Dead;
                Vector2D HighlightPosition = { L->ShapeBox.Position.x+15, L->ShapeBox.Position.y - 40 };
                CreateHighlight( &HighlightPosition );
                play_sound( SoundExplosionSmall );
            }
            
            play_sound( SoundEnemyHit );
            L->FramesSinceLastHit = 0;
            break;
        }
    }
    
    // kill the player on impact
    if( Box_OverlapWith( &Player1.ShapeBox, &L->ShapeBox ) )
      Gameplay_ChangeState( Gameplay_Death );
}

// ---------------------------------------------------------

void Launcher_Update( Launcher* L )
{
    switch( L->State )
    {
        case Launcher_Dead:
          return;
        
        case Launcher_Inactive:
        {
            // activate when the screen has scrolled close enough
            int LauncherLeft = Box_Left( &L->ShapeBox );
            int ScreenRight = CurrentLevelMap.LeftCameraLimit + screen_width;
            
            if( ScreenRight >= LauncherLeft )
              L->State = Launcher_Waiting;
            
            break;
        }
        
        case Launcher_Waiting:
        {
            L->FramesUntilNextAction--;
            
            if( L->FramesUntilNextAction <= 0 )
              Launcher_Shoot( L );
            
            break;
        }
        
        case Launcher_Shooting:
        {
            L->FramesUntilNextAction--;
            
            if( L->FramesUntilNextAction <= 0 )
            {
                L->State = Launcher_Waiting;
                L->FramesUntilNextAction = 150;
            }
            
            break;
        }
        
        default: break;
    }
    
    // now, for active states we also check if
    // the launcher is killed by a player shot,
    // or if it kills the player by contact
    if( L->State == Launcher_Waiting || L->State == Launcher_Shooting )
      Launcher_DetectImpacts( L );
    
    // kill silently when they are left
    // behind at the far left
    if( L->State != Launcher_Inactive )
    {
        int LauncherRight = Box_Right( &L->ShapeBox );
        int ScreenLeft = CurrentLevelMap.LeftCameraLimit;
        
        if( LauncherRight <= ScreenLeft )
          L->State = Launcher_Dead;
    }
}

// ---------------------------------------------------------

void Launcher_Draw( Launcher* L, Vector2D* LevelTopLeftOnScreen )
{
    // don't draw inactive or dead launchers
    if( L->State == Launcher_Inactive || L->State == Launcher_Dead )
      return;
    
    int LauncherX = LevelTopLeftOnScreen->x + L->ShapeBox.Position.x;
    int LauncherY = LevelTopLeftOnScreen->y + L->ShapeBox.Position.y;
    
    select_region( RegionMissileLauncher );
    draw_region_at( LauncherX, LauncherY );
    
    // when launcher is being hit, highlight it
    if( L->FramesSinceLastHit < 4 )
    {
        set_blending_mode( blending_add );
        draw_region_at( LauncherX, LauncherY );
        set_blending_mode( blending_alpha );
    }
}

