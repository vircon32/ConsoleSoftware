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
    if( NewState == Dialog_Choose )
      play_sound( SoundQuitGame );
      
    else if( NewState == Dialog_FadeOut )
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
    draw_region_at( screen_width/2, screen_height/2 );
    
    if( DrawCursor )
    {
        // draw cursor animation at the correct option
        int CursorFrame = (QuitGame_ElapsedFrames / 3) % 8;
        select_region( FirstRegionCursorMenu + CursorFrame );
        
        if( QuitGame_SelectedOption == 1 )
          draw_region_at( 378,208 );
        else
          draw_region_at( 226,208 );
    }
}

// ---------------------------------------------------------

void DrawGameScene()
{
    if( GameMode == ModeEasy )
      GameEasy_DrawScene();
    
    else if( GameMode == ModeMedium )
      GameMedium_DrawScene();
    
    else if( GameMode == ModeHard )
      GameHard_DrawScene();
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
    QuitGame_ChangeState( Dialog_Choose );
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
            GameScene = Scene_GameEasy + GameMode;
            GameState = Gameplay_Waiting;
            
            play_channel( ChannelMusic );
            play_sound( SoundMenuCancel );
        }
        
        // apply end of game
        else QuitGame_ChangeState( Dialog_FadeOut );
    }
    
    // draw the scene
    DrawGameScene();
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
    DrawGameScene();
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
    if( GameState == Dialog_Initialize )
      QuitGame_RunState_Initialize();
    
    else if( GameState == Dialog_Choose )
      QuitGame_RunState_Choose();
    
    else if( GameState == Dialog_FadeOut )
      QuitGame_RunState_FadeOut();
    
    // count the frames in current state
    QuitGame_ElapsedFrames++;
}
