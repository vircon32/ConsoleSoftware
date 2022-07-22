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
      play_sound( SoundMenuAccept );
}


// ---------------------------------------------------------
//   TITLE: DRAWING FUNCTIONS
// ---------------------------------------------------------


void Title_DrawScene()
{
    select_texture( TextureMineSweeper );
    
    // draw background gradient
    select_region( RegionTitleGradient );
    set_drawing_scale( screen_width, 1 );
    draw_region_zoomed_at( 0, 0 );
    
    // draw logo using a globally timed sine effect
    int CurrentFrames = get_frame_counter();
    float Angle = (2*pi*CurrentFrames)/160;
    
    // draw logo and title
    select_region( RegionTitleLetters );
    draw_region_at( screen_width/2, 39 + 6*sin(Angle) );
    select_region( RegionTitleLogo );
    draw_region_at( screen_width/2, 131 + 6*sin(Angle+pi/5) );    
    
    // draw Carra and 2022
    select_region( Region2022 );
    draw_region_at( 4, screen_height - 5 );
    
    select_region( RegionCarra );
    draw_region_at( screen_width - 5, screen_height - 5 );
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
    // draw scene
    Title_DrawScene();
    
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
    // draw scene
    Title_DrawScene();
    
    // draw "press start" and have it blink
    if( (Title_ElapsedFrames/30) % 2 )
    {
        select_region( RegionTextPressStart );
        draw_region_at( screen_width/2, 313 );
    }
    
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
    // draw scene
    Title_DrawScene();
    
    // draw "press start" and have it blink
    if( (Title_ElapsedFrames/5) % 2 )
    {
        select_region( RegionTextPressStart );
        draw_region_at( screen_width/2, 313 );
    }
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * Title_ElapsedFrames / 45 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
    
    // change scene when finished
    if( Title_ElapsedFrames >= 60 )
    {
        GameScene = Scene_Menu;
        GameState = Menu_Initialize;
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
