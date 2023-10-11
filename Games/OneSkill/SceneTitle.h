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
    if( NewState == Title_SlideDown )
      play_sound( SoundTitleSlide );
    
    else if( NewState == Title_Waiting )
    {
        play_sound( SoundTitleLogo );
        play_sound_in_channel( MusicTitle, ChannelMusic );
    }
    
    else if( NewState == Title_StartPressed )
    {
        stop_channel( ChannelMusic );
        play_sound( SoundPressStart );
    }
}


// ---------------------------------------------------------
//   TITLE: DRAWING FUNCTIONS
// ---------------------------------------------------------


void Title_DrawScene( bool DrawLogo )
{
    select_texture( TextureTitle );
    
    // draw background
    select_region( RegionBackgroundStrip );
    set_drawing_scale( screen_width, 1 );
    draw_region_zoomed_at( 0, 0 );
    
    // draw Carra and 2023
    select_region( Region2023 );
    draw_region_at( 46, 339 );
    
    select_region( RegionCarra );
    draw_region_at( 415, 339 );
    
    // draw center separator lines
    if( !DrawLogo )
    {
        select_region( RegionDiagonalLine );
        draw_region_at( 115, 359 );
        draw_region_at( 397, 359 );
        return;
    }
    
    // draw center band
    select_region( RegionBlueBand );
    draw_region_at( 115, 0 );
    
    // draw logo
    select_region( RegionLogo );
    draw_region_at( 191, 65 );
    
    // draw rotating arrows
    int CurrentFrames = get_frame_counter();
    float ArrowsAngle = -(pi * 2.0) * ((CurrentFrames % 120) / 120.0);
    set_drawing_angle( ArrowsAngle );
    select_region( RegionSwitchArrows );
    draw_region_rotated_at( 401, 67 );
    
    // draw skill frames
    select_region( RegionSkillFrame );
    draw_region_at( 68, 65 );
    draw_region_at( 29, 181 );
    draw_region_at( 523, 65 );
    draw_region_at( 484, 181 );
    
    // draw skill animations
    int AnimationFrame = ( CurrentFrames / 8 ) % 4;
    select_texture( TextureGameplay );
    select_region( FirstRegionBootsWalk + AnimationFrame );
    draw_region_at( 68 + 49, 65 + 65 );
    select_region( FirstRegionPistolWalk + AnimationFrame );
    draw_region_at( 29 + 49, 181 + 65 );
    select_region( FirstRegionWandWalk + AnimationFrame );
    draw_region_at( 523 + 49, 65 + 65 );
    select_region( FirstRegionCapeWalk + AnimationFrame );
    draw_region_at( 484 + 49, 181 + 65 );
}


// ---------------------------------------------------------
//   TITLE: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Title_RunState_Initialize()
{
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
      Title_ChangeState( Title_SlideDown );
}

// ---------------------------------------------------------

void Title_RunState_SlideDown()
{
    // draw scene with no logo
    Title_DrawScene( false );
    select_texture( TextureTitle );
    
    // draw center strip sliding
    select_region( RegionBlueBand );
    draw_region_at( 115 + 120 - 2*Title_ElapsedFrames, 0 - 360 + 6*Title_ElapsedFrames );
    
    // change state when finished
    if( Title_ElapsedFrames >= 60 )
      Title_ChangeState( Title_Waiting );   
}

// ---------------------------------------------------------

void Title_RunState_Waiting()
{
    // draw full scene
    Title_DrawScene( true );
    select_texture( TextureTitle );
    
    // draw "press start" and have it blink
    if( (Title_ElapsedFrames/30) % 2 )
    {
        select_region( RegionPressStart );
        draw_region_at( 157, 315 );
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
    // draw full scene
    Title_DrawScene( true );
    select_texture( TextureTitle );
    
    // draw "press start" and have it blink
    if( (Title_ElapsedFrames/5) % 2 )
    {
        select_region( RegionPressStart );
        draw_region_at( 157, 315 );
    }
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * Title_ElapsedFrames / 45 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
    
    // change scene when finished
    if( Title_ElapsedFrames >= 60 )
    {
        GameScene = Scene_Intro;
        GameState = Story_Initialize;
    }
}


// ---------------------------------------------------------
//   TITLE: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Title_RunStateMachine()
{
    switch( GameState )
    {
        case Title_Initialize:
          Title_RunState_Initialize();
          break;
        
        case Title_FadeIn:
          Title_RunState_FadeIn();
          break;
        
        case Title_SlideDown:
          Title_RunState_SlideDown();
          break;
        
        case Title_Waiting:
          Title_RunState_Waiting();
          break;
        
        case Title_StartPressed:
          Title_RunState_StartPressed();
          break;
    }
    
    // count the frames in current state
    Title_ElapsedFrames++;
}
