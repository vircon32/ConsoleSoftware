// ---------------------------------------------------------
//   COMPLETE LEVEL DRAWING FUNCTION
// ---------------------------------------------------------


void DrawLevel()
{
    // draw the map itself
    LevelMap_DrawBackground( &CurrentLevelMap );
    LevelMap_DrawForeground( &CurrentLevelMap );
    
    // draw object lists
    for( int i = 0; i < ExistingSoldiers; i++ )
      Soldier_Draw( &Soldiers[ i ], &CurrentLevelMap.TopLeftOnScreen );
    
    for( int i = 0; i < ExistingTurrets; i++ )
      Turret_Draw( &Turrets[ i ], &CurrentLevelMap.TopLeftOnScreen );
      
    for( int i = 0; i < ExistingLaunchers; i++ )
      Launcher_Draw( &Launchers[ i ], &CurrentLevelMap.TopLeftOnScreen );
    
    // draw temporary objects
    for( int i = 0; i < 10; i++ )
      PlayerShot_Draw( &PlayerShots[ i ], &CurrentLevelMap.TopLeftOnScreen );
    
    for( int i = 0; i < 15; i++ )
      EnemyShot_Draw( &EnemyShots[ i ], &CurrentLevelMap.TopLeftOnScreen );
    
    for( int i = 0; i < 10; i++ )
      CannonBullet_Draw( &CannonBullets[ i ], &CurrentLevelMap.TopLeftOnScreen );
    
    for( int i = 0; i < 10; i++ )
      Missile_Draw( &Missiles[ i ], &CurrentLevelMap.TopLeftOnScreen );
    
    // draw bosses
    switch( LevelNumber )
    {
        case 1:
            Boss1_Draw( &Level1Boss, &CurrentLevelMap.TopLeftOnScreen  );
            break;
        case 2:
            Boss2_Draw( &Level2Boss, &CurrentLevelMap.TopLeftOnScreen  );
            break;
        case 3:
            Boss3_Draw( &Level3Boss, &CurrentLevelMap.TopLeftOnScreen  );
            break;
    }
    
    // highlights should appear just below the player
    // but over any background objects
    for( int i = 0; i < 5; i++ )
      Highlight_Draw( &Highlights[ i ], &CurrentLevelMap.TopLeftOnScreen );
    
    // the player is drawn over all other objects
    Player_Draw( &Player1, &CurrentLevelMap.TopLeftOnScreen );
}


// ---------------------------------------------------------
//   DRAWING GUI
// ---------------------------------------------------------


void DrawGUI()
{
    // draw the left frame
    select_texture( TextureGameplay );
    select_region( RegionGUIFrame );
    draw_region_at( 0,0 );
    
    // write number of lives
    select_region( FirstRegionNumbers + min(PlayerLives, 9) );
    draw_region_at( 47,7 );
}
