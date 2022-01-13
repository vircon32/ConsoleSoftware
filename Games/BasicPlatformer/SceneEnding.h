// ---------------------------------------------------------
//   ENDING: SUPPORT FUNCTIONS
// ---------------------------------------------------------


bool AnyButtonPressed()
{
    for( int Gamepad = 0; Gamepad < 4; Gamepad++ )
    {
        select_gamepad( Gamepad );
        
        if( gamepad_button_a() == 1 ) return true;
        if( gamepad_button_b() == 1 ) return true;
        if( gamepad_button_x() == 1 ) return true;
        if( gamepad_button_y() == 1 ) return true;
        if( gamepad_button_l() == 1 ) return true;
        if( gamepad_button_r() == 1 ) return true;
        if( gamepad_button_start() == 1 ) return true;
    }
    
    return false;
}


// ---------------------------------------------------------
//   ENDING: STATE MANAGEMENT
// ---------------------------------------------------------


int Ending_ElapsedFrames;

// ---------------------------------------------------------

void Ending_ChangeState( int NewState )
{
    GameState = NewState;
    Ending_ElapsedFrames = 0;
}


// ---------------------------------------------------------
//   ENDING: DRAW FUNCTIONS
// ---------------------------------------------------------


void Ending_DrawScene( bool DrawTexts, int WhitenessLevel )
{
    // solid background
    clear_screen( make_color_rgb(170,200,150) );
    
    // draw character with its shadow
    select_texture( TextureGame );
    select_region( RegionCharacterShadow );
    draw_region_at( screen_width/2,245 );
    
    select_texture( TextureGame );
    select_region( RegionCharacterWin );
    draw_region_at( screen_width/2,212 );
    
    set_multiply_color( color_white );
    
    // when requested, draw texts above and below the character
    if( DrawTexts )
    {
        DrawTextCentered( screen_width/2,94, "YOU WIN!" );
        DrawTextCentered( screen_width/2,305, "THANK YOU FOR PLAYING" );
    }
    
    // darken the drawn scene
    if( WhitenessLevel < 255 )
      clear_screen( make_color_rgba( 0,0,0,255-max(0,WhitenessLevel) ) );
    
    // restore graphic options
    select_texture( TextureGame );
}


// ---------------------------------------------------------
//    ENDING: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Ending_RunState_Initialize()
{
    // stop music and play ending sound
    stop_channel( ChannelMusic );
    play_sound( MusicEnding );
    
    // transition immediately
    Ending_ChangeState( Ending_FadeIn );
}

// ---------------------------------------------------------

void Ending_RunState_FadeIn()
{
    // draw scene with a fade-in from black
    int WhitenessLevel = max( 0, 255 * Ending_ElapsedFrames / 60 );
    Ending_DrawScene( false, WhitenessLevel );
    
    // transition when finished
    if( Ending_ElapsedFrames >= 60 )
      Ending_ChangeState( Ending_TextAppears );
}

// ---------------------------------------------------------

void Ending_RunState_TextAppears()
{
    // we will wait a minimum time before exiting
    bool CanExit = (Ending_ElapsedFrames >= 96);
    
    // draw scene with a text blink effect
    bool ShowText = ((Ending_ElapsedFrames & 16) == 0);
    Ending_DrawScene( CanExit || ShowText, 255 );
    
    // transition when any button is pressed,
    // but wait a minimum time first
    if( CanExit )
      if( AnyButtonPressed() )
        Ending_ChangeState( Ending_FadeOut );
}

// ---------------------------------------------------------

void Ending_RunState_FadeOut()
{
    // draw scene with a fade-in to black
    int BlacknessLevel = max( 0, 255 * Ending_ElapsedFrames / 90 );
    Ending_DrawScene( true, 255 - BlacknessLevel );
    
    // when done, go back to title
    if( Ending_ElapsedFrames >= 60 )
    {
        select_texture( TextureGame );
        GameScene = Scene_Title;
        Title_ChangeState( Title_Initialize );
    }
}


// ---------------------------------------------------------
//   ENDING: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Ending_RunStateMachine()
{
    if( GameState == Ending_Initialize )
      Ending_RunState_Initialize();
    
    else if( GameState == Ending_FadeIn )
      Ending_RunState_FadeIn();
    
    else if( GameState == Ending_TextAppears )
      Ending_RunState_TextAppears();
    
    else if( GameState == Ending_FadeOut )
      Ending_RunState_FadeOut();
    
    // count the frames in current state
    Ending_ElapsedFrames++;
}
