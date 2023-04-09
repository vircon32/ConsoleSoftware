// ---------------------------------------------------------
//   GAME OVER: STATE MANAGEMENT
// ---------------------------------------------------------


// state variables
int GameOver_ElapsedFrames;

// ---------------------------------------------------------

void GameOver_ChangeState( int NewState )
{
    GameState = NewState;
    GameOver_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( NewState == GameOver_Choose )
      play_sound( SoundGameOver );
    
    else if( NewState == GameOver_FadeOut )
      play_sound( SoundMenuAccept );
}


// ---------------------------------------------------------
//   GAME OVER: DRAWING FUNCTIONS
// ---------------------------------------------------------


void GameOver_DrawDialog( bool DrawCursor )
{
    select_region( RegionDialogGameOver );
    draw_region_at( screen_width/2, screen_height/2 );
    
    if( DrawCursor )
    {
        // draw cursor animation
        int CursorFrame = (GameOver_ElapsedFrames / 3) % 8;
        select_region( FirstRegionCursorRight + CursorFrame );
        draw_region_at( 274,210 );
    }
}


// ---------------------------------------------------------
//   GAME OVER: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void GameOver_RunState_Initialize()
{
    // transition immediately
    GameOver_ChangeState( GameOver_Choose );
}

// ---------------------------------------------------------

void GameOver_RunState_Choose()
{
    // buttons A or Start confirm selection
    if( gamepad_button_a() == 1 || gamepad_button_start() == 1 )
      GameOver_ChangeState( GameOver_FadeOut );
    
    // darken the scene
    Gameplay_DrawScene();
    clear_screen( make_color_rgba( 0,0,0,128 ) );
    
    // draw the dialog on top
    GameOver_DrawDialog( true );
}

// ---------------------------------------------------------

void GameOver_RunState_FadeOut()
{
    // when fade-out ends, apply selection
    if( GameOver_ElapsedFrames >= 45 )
    {
        // apply end of game
        GameScene = Scene_Title;
        GameState = Title_Initialize;
    }
    
    // draw scene
    Gameplay_DrawScene();
    GameOver_DrawDialog( false );
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * GameOver_ElapsedFrames / 45 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
}


// ---------------------------------------------------------
//   GAME OVER: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void GameOver_RunStateMachine()
{
    if( GameState == GameOver_Initialize )
      GameOver_RunState_Initialize();
    
    else if( GameState == GameOver_Choose )
      GameOver_RunState_Choose();
    
    else if( GameState == GameOver_FadeOut )
      GameOver_RunState_FadeOut();
    
    // count the frames in current state
    GameOver_ElapsedFrames++;
}
