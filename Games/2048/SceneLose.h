// ---------------------------------------------------------
//   LOSE: STATE MANAGEMENT
// ---------------------------------------------------------


// state variables
int Lose_ElapsedFrames;

// ---------------------------------------------------------

void Lose_ChangeState( int NewState )
{
    GameState = NewState;
    Lose_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( NewState == Dialog_Highlight )
    {
        stop_channel( ChannelMusic );
        play_sound( SoundLose );
    }
    
    else if( NewState == Dialog_FadeOut )
      play_sound( SoundMenuAccept );
}


// ---------------------------------------------------------
//   LOSE: DRAWING FUNCTIONS
// ---------------------------------------------------------


void DrawLoseDialog( bool DrawCursor )
{
    select_region( RegionDialogLose );
    draw_region_at( 76,103 );
    
    select_region( RegionDialogExit );
    draw_region_at( 76,195 );
    
    if( DrawCursor )
    {
        // draw cursor animation
        int CursorFrame = (Lose_ElapsedFrames / 3) % 8;
        select_region( FirstRegionCursor + CursorFrame );
        draw_region_at( 167,224 );
    }
}


// ---------------------------------------------------------
//   LOSE: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Lose_RunState_Initialize()
{
    // disable any other highlights
    DirectionHighlightFrames = 0;
    TileHighlightFrames = 0;
    ScoreHighlightFrames = 0;
    
    // transition immediately
    Lose_ChangeState( Dialog_Highlight );
}

// ---------------------------------------------------------

void Lose_RunState_Highlight()
{
    // draw the scene
    DrawBackground();
    DrawGUI();
    DrawBoard();
    
    // do a blinking highlight
    if( !(Lose_ElapsedFrames & 16) )
      clear_screen( make_color_rgba( 255, 0, 0, 128 ) );
    
    // when finished, transition
    if( Lose_ElapsedFrames >= 125 )
      Lose_ChangeState( Dialog_Choose );
}

// ---------------------------------------------------------

void Lose_RunState_Choose()
{
    // buttons A or Start confirm selection
    if( gamepad_button_a() == 1 || gamepad_button_start() == 1 )
      Lose_ChangeState( Dialog_FadeOut );
    
    // draw the scene
    DrawBackground();
    DrawGUI();
    DrawLoseDialog( true );
}

// ---------------------------------------------------------

void Lose_RunState_FadeOut()
{
    // when fade-out ends, apply selection
    if( Lose_ElapsedFrames >= 45 )
    {
        // apply end of game
        GameScene = Scene_Title;
        GameState = Title_Initialize;
    }
    
    // draw scene
    DrawBackground();
    DrawGUI();
    DrawLoseDialog( false );
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * Lose_ElapsedFrames / 45 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
}


// ---------------------------------------------------------
//   LOSE: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Lose_RunStateMachine()
{
    if( GameState == Dialog_Initialize )
      Lose_RunState_Initialize();
    
    else if( GameState == Dialog_Highlight )
      Lose_RunState_Highlight();
    
    else if( GameState == Dialog_Choose )
      Lose_RunState_Choose();
    
    else if( GameState == Dialog_FadeOut )
      Lose_RunState_FadeOut();
    
    // count the frames in current state
    Lose_ElapsedFrames++;
}
