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
    if( NewState == Win_Highlight )
      stop_channel( ChannelMusic );
    
    else if( NewState == Win_Choose )
      play_sound( SoundWin );
    
    else if( NewState == Win_FadeOut )
      play_sound( SoundMenuAccept );
}


// ---------------------------------------------------------
//   WIN: DRAWING FUNCTIONS
// ---------------------------------------------------------


void DrawWinDialog( bool DrawCursor )
{
    select_region( RegionDialogWin );
    draw_region_at( 363, screen_height/2 );
    
    if( DrawCursor )
    {
        // draw cursor animation
        int CursorFrame = (Win_ElapsedFrames / 3) % 8;
        select_region( FirstRegionCursorDialog + CursorFrame );
        draw_region_at( 317,208 );
    }
}


// ---------------------------------------------------------
//   WIN: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Win_RunState_Initialize()
{
    ResetActions();
    
    // transition immediately
    Win_ChangeState( Win_Highlight );
}

// ---------------------------------------------------------

void Win_RunState_Highlight()
{
    // draw the scene
    Gameplay_DrawScene();
    
    // highlight the 4 suits in order
    // playing a sound tune
    int Suit = Win_ElapsedFrames / 20;
    
    if( Win_ElapsedFrames % 20 == 1 )
    {
        play_sound_in_channel( SoundWinHighlight, ChannelMusic );
        select_channel( ChannelMusic );
        set_channel_speed( pow( 1.06, 2*(Suit-1) ) );
    }
    
    set_blending_mode( blending_add );
    select_region( RegionWinHighlight );
    draw_region_at( SuitStacksFirstX + Suit * SuitStacksStepX, SuitStacksY );
    set_blending_mode( blending_alpha );
    
    // when finished, transition
    if( Win_ElapsedFrames >= 79 )
    {
        Win_ChangeState( Win_Choose );
        
        // restore music channel to normal speed
        select_channel( ChannelMusic );
        set_channel_speed( 1.0 );
    }
}

// ---------------------------------------------------------

void Win_RunState_Choose()
{
    // buttons A or Start confirm selection
    if( gamepad_button_a() == 1 || gamepad_button_start() == 1 )
      Win_ChangeState( Win_FadeOut );
    
    // darken the scene
    Gameplay_DrawScene();
    clear_screen( make_color_rgba( 0,0,0,128 ) );
    
    // draw the dialog on top
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
    Gameplay_DrawScene();
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
    if( GameState == Win_Initialize )
      Win_RunState_Initialize();
    
    else if( GameState == Win_Highlight )
      Win_RunState_Highlight();
    
    else if( GameState == Win_Choose )
      Win_RunState_Choose();
    
    else if( GameState == Win_FadeOut )
      Win_RunState_FadeOut();
    
    // count the frames in current state
    Win_ElapsedFrames++;
}
