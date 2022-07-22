// ---------------------------------------------------------
//   WIN: STATE MANAGEMENT
// ---------------------------------------------------------


// state variables
int Win_ElapsedFrames;

// ---------------------------------------------------------

void Win_ChangeState( int NewState )
{
    GameState = NewState;
    Win_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( NewState == Dialog_Highlight )
    {
        stop_channel( ChannelMusic );
        play_sound( SoundWin );
    }
    
    else if( NewState == Dialog_FadeOut )
      play_sound( SoundMenuAccept );
}


// ---------------------------------------------------------
//   WIN: DRAWING FUNCTIONS
// ---------------------------------------------------------


void DrawWinDialog( bool DrawCursor )
{
    select_region( RegionDialogWin );
    draw_region_at( screen_width/2, screen_height/2 );
    
    if( DrawCursor )
    {
        // draw cursor animation
        int CursorFrame = (Win_ElapsedFrames / 3) % 8;
        select_region( FirstRegionCursorMenu + CursorFrame );
        draw_region_at( 274,208 );
    }
}


// ---------------------------------------------------------
//   WIN: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Win_RunState_Initialize()
{
    // transition immediately
    Win_ChangeState( Dialog_Highlight );
}

// ---------------------------------------------------------

void Win_RunState_Highlight()
{
    // draw the scene
    DrawGameScene();
    
    // do a blinking highlight
    if( !(Win_ElapsedFrames & 8) )
      clear_screen( make_color_rgba( 255, 255, 255, 64 ) );
    
    // when finished, transition
    if( Win_ElapsedFrames >= 47 )
      Win_ChangeState( Dialog_Choose );
}

// ---------------------------------------------------------

void Win_RunState_Choose()
{
    // buttons A or Start confirm selection
    if( gamepad_button_a() == 1 || gamepad_button_start() == 1 )
      Win_ChangeState( Dialog_FadeOut );
    
    // draw the scene
    DrawGameScene();
    DrawWinDialog( true );
}

// ---------------------------------------------------------

void Win_RunState_FadeOut()
{
    // when fade-out ends, apply selection
    if( Win_ElapsedFrames >= 45 )
    {
        // apply end of game
        GameScene = Scene_Title;
        GameState = Title_Initialize;
    }
    
    // draw scene
    DrawGameScene();
    DrawWinDialog( false );
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * Win_ElapsedFrames / 45 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
}


// ---------------------------------------------------------
//   WIN: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Win_RunStateMachine()
{
    if( GameState == Dialog_Initialize )
      Win_RunState_Initialize();
    
    else if( GameState == Dialog_Highlight )
      Win_RunState_Highlight();
    
    else if( GameState == Dialog_Choose )
      Win_RunState_Choose();
    
    else if( GameState == Dialog_FadeOut )
      Win_RunState_FadeOut();
    
    // count the frames in current state
    Win_ElapsedFrames++;
}
