// ---------------------------------------------------------
//   QUIT GAME: STATE MANAGEMENT
// ---------------------------------------------------------


// state variables
int QuitGame_ElapsedFrames;
int QuitGame_SelectedOption;

// ---------------------------------------------------------

void QuitGame_ChangeState( int NewState )
{
    GameState = NewState;
    QuitGame_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( NewState == QuitGame_Choose )
      play_sound( SoundQuitGame );
      
    else if( NewState == QuitGame_FadeOut )
    {
        stop_channel( ChannelMusic );
        play_sound( SoundMenuAccept );
    }
}


// ---------------------------------------------------------
//   QUIT GAME: DRAWING FUNCTIONS
// ---------------------------------------------------------


void DrawQuitGameDialog( bool DrawCursor )
{
    select_region( RegionDialogQuit );
    draw_region_at( 363, screen_height/2 );
    
    if( DrawCursor )
    {
        // draw cursor animation at the correct option
        int CursorFrame = (QuitGame_ElapsedFrames / 3) % 8;
        select_region( FirstRegionCursorDialog + CursorFrame );
        
        if( QuitGame_SelectedOption == 1 )
          draw_region_at( 421,208 );
        else
          draw_region_at( 269,208 );
    }
}


// ---------------------------------------------------------
//   QUIT GAME: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void QuitGame_RunState_Initialize()
{
    // initialize selection
    // (set cancel by default, to avoid mistakes)
    QuitGame_SelectedOption = 1;
    
    // transition immediately
    QuitGame_ChangeState( QuitGame_Choose );
}

// ---------------------------------------------------------

void QuitGame_RunState_Choose()
{
    // left and right change selection
    if( gamepad_left() == 1 )
    {
        if( QuitGame_SelectedOption >= 1 )
        {
            QuitGame_SelectedOption = 0;
            play_sound( SoundMenuMove );
        }
    }
    
    else if( gamepad_right() == 1 )
    {
        if( QuitGame_SelectedOption <= 0 )
        {
            QuitGame_SelectedOption = 1;
            play_sound( SoundMenuMove );
        }
    }
    
    // buttons A or Start confirm selection
    if( gamepad_button_a() == 1 || gamepad_button_start() == 1 )
    {
        // cancel end of game
        if( QuitGame_SelectedOption == 1 )
        {
            GameScene = Scene_Gameplay;
            GameState = Gameplay_Waiting;
            
            play_channel( ChannelMusic );
            play_sound( SoundMenuCancel );
        }
        
        // apply end of game
        else QuitGame_ChangeState( QuitGame_FadeOut );
    }
    
    // draw the scene
    Gameplay_DrawScene();
    DrawQuitGameDialog( true );
}

// ---------------------------------------------------------

void QuitGame_RunState_FadeOut()
{
    // when fade-out ends, apply selection
    if( QuitGame_ElapsedFrames >= 45 )
    {
        // apply end of game
        GameScene = Scene_Title;
        GameState = Title_Initialize;
    }
    
    // draw scene
    Gameplay_DrawScene();
    DrawQuitGameDialog( false );
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * QuitGame_ElapsedFrames / 45 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
}


// ---------------------------------------------------------
//   QUIT GAME: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void QuitGame_RunStateMachine()
{
    if( GameState == QuitGame_Initialize )
      QuitGame_RunState_Initialize();
    
    else if( GameState == QuitGame_Choose )
      QuitGame_RunState_Choose();
    
    else if( GameState == QuitGame_FadeOut )
      QuitGame_RunState_FadeOut();
    
    // count the frames in current state
    QuitGame_ElapsedFrames++;
}
