int Ending_ElapsedFrames = 0;


// ---------------------------------------------------------
//   ENDING: GENERAL FUNCTIONS
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

void Ending_Draw()
{
    // draw the fixed elements
    select_texture( TextureCutscenes );
    clear_screen( make_color_rgb( 120, 160, 120 ) );
    
    select_region( RegionEndingText );
    draw_region_at( 319,156 );
    
    // apply blink effect to this part
    if( Ending_ElapsedFrames & 8 )
      set_multiply_color( make_gray( 192 ) );
    
    select_region( RegionEndingComeBack );
    draw_region_at( 319,188 );
    set_multiply_color( color_white );
    
    // draw character animation
    int AnimationRegion = FirstRegionEndingChar;
    
    if( Ending_ElapsedFrames & 16 )
      AnimationRegion++;
    
    select_region( AnimationRegion );
    draw_region_at( 315,319 );
        
}


// ---------------------------------------------------------
//   ENDING: STATE MACHINE
// ---------------------------------------------------------


void Ending_ChangeState( int NewState )
{
    GameState = NewState;
    Ending_ElapsedFrames = 0;
}

// ---------------------------------------------------------

void Ending_RunState_Initialize()
{
    // (nothing to initialize for now)
    //
    
    // transition immediately
    Ending_ChangeState( Ending_Wait );
}

// ---------------------------------------------------------

void Ending_RunState_Wait()
{
    // draw the scene
    Ending_Draw();
    
    // after a button is pressed, go back to title
    if( Ending_ElapsedFrames > 60 && AnyButtonPressed() )
      Ending_ChangeState( Ending_FadeOut );
}

// ---------------------------------------------------------

void Ending_RunState_FadeOut()
{
    // draw the scene
    Ending_Draw();
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * Ending_ElapsedFrames / 45 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
    
    // transition when finished
    if( Ending_ElapsedFrames >= 60 )
    {
        GameScene = Scene_Title;
        GameState = Title_Initialize;
    }
}

// ---------------------------------------------------------

void Ending_RunStateMachine()
{
    if( GameState == Ending_Initialize )
      Ending_RunState_Initialize();
    
    else if( GameState == Ending_Wait )
      Ending_RunState_Wait();
    
    else if( GameState == Ending_FadeOut )
      Ending_RunState_FadeOut();
    
    // count the elapse time
    Ending_ElapsedFrames++;
}
