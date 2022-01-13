// ---------------------------------------------------------
//   COMPLETE LEVEL DRAWING FUNCTION
// ---------------------------------------------------------


void DrawLevel()
{
    LevelMap_DrawBackground( &CurrentLevelMap );
    LevelMap_DrawForeground( &CurrentLevelMap );
    
    for( int i = 0; i < ExistingCoins; i++ )
      Coin_Draw( &Coins[ i ], &CurrentLevelMap.TopLeftOnScreen );
    
    for( int i = 0; i < ExistingConveyors; i++ )
      Conveyor_Draw( &Conveyors[ i ], &CurrentLevelMap.TopLeftOnScreen );
    
    for( int i = 0; i < ExistingSprings; i++ )
      Spring_Draw( &Springs[ i ], &CurrentLevelMap.TopLeftOnScreen );
    
    for( int i = 0; i < ExistingOnOffBlocks; i++ )
      OnOffBlock_Draw( &OnOffBlocks[ i ], &CurrentLevelMap.TopLeftOnScreen );
    
    Goal_Draw( &LevelGoal, &CurrentLevelMap.TopLeftOnScreen );
    
    // highlights should appear just below the player
    // but over any background objects
    for( int i = 0; i < 5; i++ )
      Highlight_Draw( &Highlights[ i ], &CurrentLevelMap.TopLeftOnScreen );
    
    // the player is drawn over all other objects
    Player_Draw( &Player1, &CurrentLevelMap.TopLeftOnScreen );
    
    // morning stars look wrong unless drawn over
    // the player since they can move over blocks
    for( int i = 0; i < ExistingMorningStars; i++ )
      MorningStar_Draw( &MorningStars[ i ], &CurrentLevelMap.TopLeftOnScreen );
}


// ---------------------------------------------------------
//   DRAWING GUI
// ---------------------------------------------------------


void DrawGUI()
{
    // draw the left frame
    select_texture( TextureGame );
    select_region( RegionGUIFrameLeft );
    draw_region_at( 0,0 );
    
    // write number of lives
    select_region( RegionGUIDigitZero + min(PlayerLives, 9) );
    draw_region_at( 46,6 );
    
    // write collected and total coins
    select_region( RegionGUIDigitZero + min(Player1.CollectedCoins, 9) );
    draw_region_at( 105,6 );
    
    select_region( RegionGUIDigitZero + min(ExistingCoins, 9) );
    draw_region_at( 139,6 );
    
    // ---------------------------------------------
    // draw the right frame
    select_region( RegionGUIFrameRight );
    draw_region_at( 639,0 );
    
    // when remaining time is 20 seconds or less, time blinks in orange
    if( RemainingTimeFrames <= (20*frames_per_second) )
      if( RemainingTimeFrames & 8 )
        set_multiply_color( make_color_rgb(255,128,0) );
    
    // decompose time limit in minutes and seconds
    int RemainingSeconds = RemainingTimeFrames / frames_per_second;
    int Minutes = RemainingSeconds / 60;
    int SecondsDigit1 = (RemainingSeconds - Minutes*60) / 10;
    int SecondsDigit2 = (RemainingSeconds - Minutes*60) % 10;
    
    // write 1 digit for minutes
    select_region( RegionGUIDigitZero + Minutes );
    draw_region_at( 569,6 );
    
    // write 2 digits for seconds
    select_region( RegionGUIDigitZero + SecondsDigit1 );
    draw_region_at( 600,6 );
    
    select_region( RegionGUIDigitZero + SecondsDigit2 );
    draw_region_at( 618,6 );
    
    // restore drawing options
    set_multiply_color( color_white );
}
