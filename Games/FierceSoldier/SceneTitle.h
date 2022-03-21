// ---------------------------------------------------------
//   TITLE: STATE MANAGEMENT
// ---------------------------------------------------------


int Title_ElapsedFrames;

// ---------------------------------------------------------

void Title_ChangeState( int NewState )
{
    GameState = NewState;
    Title_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( NewState == Title_StartPressed )
    {
        stop_channel( ChannelMusic );
        play_sound( SoundStartGame );
    }
}


// ---------------------------------------------------------
//   TITLE: DRAWING FUNCTIONS
// ---------------------------------------------------------


void Title_DrawScene( bool DrawLogo )
{
    select_texture( TextureTitle );
    
    // draw background
    clear_screen( make_color_rgb( 55,55,75 ) );
    select_region( RegionTitleBackground );
    draw_region_at( 0,359 );
    
    // when requested, draw logo
    if( DrawLogo )
    {
        // make logo outline blink
        // (use global frame counter to ensure
        // continuous blink between states)
        int DrawnRegion = FirstRegionLogo + (get_frame_counter() / 25) % 2;
        
        select_region( DrawnRegion );
        draw_region_at( 142,37 );
    }
}


// ---------------------------------------------------------
//   TITLE: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Title_RunState_Initialize()
{
    // play title music
    play_sound_in_channel( MusicTitle, ChannelMusic );
    
    // transition immediately
    Title_ChangeState( Title_FadeIn );
}

// ---------------------------------------------------------

void Title_RunState_FadeIn()
{
    // draw scene without logo
    Title_DrawScene( false );
    
    // fade-in effect
    int BlackLevel = max( 0, 255 - 255 * Title_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    
    // change state when finished
    if( Title_ElapsedFrames >= 60 )
      Title_ChangeState( Title_Waiting );   
}

// ---------------------------------------------------------

void Title_RunState_Waiting()
{
    // draw scene with logo
    Title_DrawScene( true );
    
    // draw "press start" and have it blink
    if( (Title_ElapsedFrames/40) % 2 )
      FontLarge_PrintTextFromLeft( 300,252, "PRESS START" );
    
    // change state when start pressed on first gamepad
    select_gamepad( 0 );
        
    if( gamepad_button_start() == 1 )
    {
        Title_ChangeState( Title_StartPressed );
        return;
    }
}

// ---------------------------------------------------------

void Title_RunState_StartPressed()
{
    // draw scene with logo
    Title_DrawScene( true );
    
    // draw "press start" and have it blink
    if( (Title_ElapsedFrames/5) % 2 )
      FontLarge_PrintTextFromLeft( 300,252, "PRESS START" );
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * Title_ElapsedFrames / 45 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
    
    // change scene when finished
    if( Title_ElapsedFrames >= 60 )
    {
        GameScene = Scene_Intro;
        GameState = Story_Initialize;
        
        // ensure a new game session
        ResetGameSession();
    }
}


// ---------------------------------------------------------
//   TITLE: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Title_RunStateMachine()
{
    if( GameState == Title_Initialize )
      Title_RunState_Initialize();
    
    else if( GameState == Title_FadeIn )
      Title_RunState_FadeIn();
    
    else if( GameState == Title_Waiting )
      Title_RunState_Waiting();
    
    else if( GameState == Title_StartPressed )
      Title_RunState_StartPressed();

    // count the frames in current state
    Title_ElapsedFrames++;
}
