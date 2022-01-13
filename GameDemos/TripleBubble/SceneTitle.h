// ---------------------------------------------------------
//   TITLE: STATE MANAGEMENT
// ---------------------------------------------------------


int Title_ElapsedFrames;

// ---------------------------------------------------------

void Title_ChangeState( int NewState )
{
    GameState = NewState;
    Title_ElapsedFrames = 0;
    
    // some states begin with a specific sound
    if( NewState == Title_BubbleUp )
      play_sound( SoundTitleBubble );

    else if( NewState == Title_Burst )
      play_sound( SoundTitleFanfare );

    else if( NewState == Title_StartPressed )
      play_sound( SoundTitleStart );
}


// ---------------------------------------------------------
//   TITLE: STAR FIELD
// ---------------------------------------------------------


Vector2D[ 30 ] Title_StarPositions;


void Title_CreateStar( Vector2D* Position )
{
    Position->x = 10 + rand() % 620;
    Position->y = 370;
}

void Title_UpdateStars()
{
    for( int i = 0; i < 30; ++i )
    {
        Title_StarPositions[i].y -= 6;
        
        if( Title_StarPositions[i].y < -10 )
          Title_CreateStar( &Title_StarPositions[i] );
    }
}

void Title_DrawStars()
{
    select_region( RegionTitleStar );
    
    for( int i = 0; i < 30; ++i )
      draw_region_at( Title_StarPositions[i].x, Title_StarPositions[i].y );
}


// ---------------------------------------------------------
//   TITLE: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Title_RunState_Initialize()
{
    select_texture( TextureTitle );
    
    // initialize star field
    for( int i = 0; i < 30; ++i )
    {
        Title_StarPositions[i].x = 10 + rand() % 620;
        Title_StarPositions[i].y = rand() % 360;
    }
    
    // transition immediately
    Title_ChangeState( Title_BubbleUp );
}

// ---------------------------------------------------------

void Title_RunState_BubbleUp()
{
    // draw background elements
    clear_screen( make_color_rgb( 22,49,54 ) );
    Title_DrawStars();
    Title_UpdateStars();
    
    select_region( RegionTitleStage );
    draw_region_at( 0,0 );
    
    // draw bubble moving up
    int BubbleY = 441 - 3 * Title_ElapsedFrames;
    select_region( RegionTitleBubble );
    draw_region_at( 319,BubbleY );
    
    // change state when finished
    if( BubbleY <= 201 )
      Title_ChangeState( Title_Burst );   
}

// ---------------------------------------------------------

void Title_RunState_Burst()
{
    // draw background elements
    clear_screen( make_color_rgb( 22,49,54 ) );
    Title_DrawStars();
    Title_UpdateStars();
    
    select_region( RegionTitleStage );
    draw_region_at( 0,0 );
    
    // draw bubble at final position
    select_region( RegionTitleBubble );
    draw_region_at( 319,201 );
    
    // make flash effect
    int AlphaLevel = min( 255, 15 * (15-abs( Title_ElapsedFrames-15 )) );
    clear_screen( make_color_rgba( 255,255,255,AlphaLevel ) );
    
    // change state when finished
    if( Title_ElapsedFrames >= 30 )
      Title_ChangeState( Title_Waiting );    
}

// ---------------------------------------------------------

void Title_RunState_Waiting()
{
    // draw background elements
    clear_screen( make_color_rgb( 22,49,54 ) );
    Title_DrawStars();
    Title_UpdateStars();
    
    select_region( RegionTitleStage );
    draw_region_at( 0,0 );
    
    // draw logo elements
    select_region( RegionLogoLeft );
    draw_region_at( 247,276 );
    
    select_region( RegionLogoRight );
    draw_region_at( 391,276 );
    
    select_region( RegionTitleBubble );
    draw_region_at( 319,201 );
    
    select_region( RegionTitleDemo );
    draw_region_at( 630,351 );
    
    // draw "press start" and have it blink
    if( (Title_ElapsedFrames/30) % 2 )
    {
        select_region( RegionPressStart );
        draw_region_at( 320,306 );
    }
    
    // change state when start pressed on any gamepad
    for( int i = 0; i < 3; i++ )
    {
        select_gamepad( i );
        
        if( gamepad_button_start() == 1 )
        {
            Title_ChangeState( Title_StartPressed );
            return;
        }
    }
}

// ---------------------------------------------------------

void Title_RunState_StartPressed()
{
    // draw background elements
    clear_screen( make_color_rgb( 22,49,54 ) );
    Title_DrawStars();
    
    select_region( RegionTitleStage );
    draw_region_at( 0,0 );
    
    // draw logo elements
    select_region( RegionLogoLeft );
    draw_region_at( 247,276 );
    
    select_region( RegionLogoRight );
    draw_region_at( 391,276 );
    
    select_region( RegionTitleBubble );
    draw_region_at( 319,201 );
    
    // draw "press start" and have it blink
    if( (Title_ElapsedFrames/5) % 2 )
    {
        select_region( RegionPressStart );
        draw_region_at( 320,306 );
    }
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * Title_ElapsedFrames / 45 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
    
    // change scene when finished
    if( Title_ElapsedFrames >= 60 )
    {
        GameScene = Scene_MainMenu;
        GameState = MainMenu_Initialize;
    }
}


// ---------------------------------------------------------
//   TITLE: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Title_RunStateMachine()
{
    if( GameState == Title_Initialize )
      Title_RunState_Initialize();
    
    else if( GameState == Title_BubbleUp )
      Title_RunState_BubbleUp();
    
    else if( GameState == Title_Burst )
      Title_RunState_Burst();
    
    else if( GameState == Title_Waiting )
      Title_RunState_Waiting();
    
    else if( GameState == Title_StartPressed )
      Title_RunState_StartPressed();

    // in any case, wait for next frame
    Title_ElapsedFrames++;
}
