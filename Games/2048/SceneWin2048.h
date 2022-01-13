// ---------------------------------------------------------
//   WIN 2048: STATE MANAGEMENT
// ---------------------------------------------------------


// state variables
int Win2048_ElapsedFrames;
int Win2048_SelectedOption;

// ---------------------------------------------------------

void Win2048_ChangeState( int NewState )
{
    GameState = NewState;
    Win2048_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( NewState == Dialog_Highlight )
    {
        stop_channel( ChannelMusic );
        play_sound( SoundWin );
    }
    
    else if( NewState == Dialog_FadeOut )
      play_sound( SoundMenuCancel );
}


// ---------------------------------------------------------
//   WIN 2048: DRAWING FUNCTIONS
// ---------------------------------------------------------


void DrawWin2048Dialog( bool DrawCursor )
{
    select_region( RegionDialogWin );
    draw_region_at( 76,103 );
    
    select_region( RegionDialogYesNo );
    draw_region_at( 76,195 );
    
    select_region( RegionTile2 + 10 );
    draw_region_at( 124,151 );
    
    select_region( RegionTextContinue );
    draw_region_at( 178,169 );
    
    if( DrawCursor )
    {
        // draw cursor animation at the correct option
        int CursorFrame = (Win2048_ElapsedFrames / 3) % 8;
        select_region( FirstRegionCursor + CursorFrame );
        
        if( Win2048_SelectedOption == 1 )
          draw_region_at( 271,224 );
        else
          draw_region_at( 119,224 );
    }
}


// ---------------------------------------------------------
//   WIN 2048: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Win2048_RunState_Initialize()
{
    // disable any other highlights
    DirectionHighlightFrames = 0;
    TileHighlightFrames = 0;
    ScoreHighlightFrames = 0;
    
    // initialize selection
    // (set continue by default, to avoid mistakes)
    Win2048_SelectedOption = 0;
    
    // transition immediately
    Win2048_ChangeState( Dialog_Highlight );
}

// ---------------------------------------------------------

void Win2048_RunState_Highlight()
{
    // draw the scene
    DrawBackground();
    DrawGUI();
    DrawBoard();
    
    // do a blinking highlight for all 2048 tiles
    if( Win2048_ElapsedFrames & 16 )
    {
        set_blending_mode( blending_add );
        select_region( RegionTileHighlight );
        
        for( int y = 0; y < 4; y++ )
          for( int x = 0; x < 4; x++ )
            if( CurrentBoard[ y ][ x ] == 11 )   // 2 ^ 11 = 2048
              draw_region_at( 100 + 75*x, 67 + 75*y );
              
        set_blending_mode( blending_alpha );
    }
    
    // when finished, transition
    if( Win2048_ElapsedFrames >= 125 )
      Win2048_ChangeState( Dialog_Choose );
}

// ---------------------------------------------------------

void Win2048_RunState_Choose()
{
    // left and right change selection
    if( gamepad_left() == 1 )
    {
        if( Win2048_SelectedOption >= 1 )
        {
            Win2048_SelectedOption = 0;
            play_sound( SoundMenuMove );
        }
    }
    
    else if( gamepad_right() == 1 )
    {
        if( Win2048_SelectedOption <= 0 )
        {
            Win2048_SelectedOption = 1;
            play_sound( SoundMenuMove );
        }
    }
    
    // buttons A or Start confirm selection
    if( gamepad_button_a() == 1 || gamepad_button_start() == 1 )
    {
        // go back to continue the game
        if( Win2048_SelectedOption == 0 )
        {
            GameScene = Scene_Gameplay;
            Gameplay_ChangeState( Gameplay_AddTile );
            
            play_sound( SoundMenuAccept );
            play_sound_in_channel( MusicGameplay, ChannelMusic );
        }
        
        // apply end of game
        else Win2048_ChangeState( Dialog_FadeOut );
    }
    
    // draw the scene
    DrawBackground();
    DrawGUI();
    DrawWin2048Dialog( true );
}

// ---------------------------------------------------------

void Win2048_RunState_FadeOut()
{
    // when fade-out ends, apply selection
    if( Win2048_ElapsedFrames >= 45 )
    {
        // apply end of game
        GameScene = Scene_Title;
        GameState = Title_Initialize;
    }
    
    // draw scene
    DrawBackground();
    DrawGUI();
    DrawWin2048Dialog( false );
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * Win2048_ElapsedFrames / 45 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
}


// ---------------------------------------------------------
//   WIN 2048: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Win2048_RunStateMachine()
{
    if( GameState == Dialog_Initialize )
      Win2048_RunState_Initialize();
    
    else if( GameState == Dialog_Highlight )
      Win2048_RunState_Highlight();
    
    else if( GameState == Dialog_Choose )
      Win2048_RunState_Choose();
    
    else if( GameState == Dialog_FadeOut )
      Win2048_RunState_FadeOut();
    
    // count the frames in current state
    Win2048_ElapsedFrames++;
}
