// ---------------------------------------------------------
//   MAIN MENU: STATE MANAGEMENT
// ---------------------------------------------------------


int MainMenu_ElapsedFrames;
int MainMenu_SelectedOption = 0;
int MainMenu_SelectedPlayers = 1;
Vector2D[ 4 ] MenuBubblePositions;
float[ 4 ] MenuBubbleSpeeds;

// ---------------------------------------------------------

void MainMenu_ChangeState( int NewState )
{
    GameState = NewState;
    MainMenu_ElapsedFrames = 0;
}


// ---------------------------------------------------------
//   MAIN MENU: GENERIC FUNCTIONS
// ---------------------------------------------------------


void MainMenu_Draw()
{
    // draw background color
    clear_screen( make_color_rgb( 75, 75, 102 ) );
    
    // draw moving bubbles
    select_region( RegionMenuBubble );
    
    for( int i = 3; i >= 0; --i )
      draw_region_at( MenuBubblePositions[ i ].x,MenuBubblePositions[ i ].y );
    
    // draw title
    select_region( RegionMenuTitle );
    draw_region_at( 0,0 );
    
    // draw texts
    for( int i = 0; i < 4; ++i )
    {
        select_region( RegionMenuTriple + i );
        draw_region_at( 191, 172 + 44*i );
    }
    
    // draw foreground
    select_region( RegionMenuWall );
    draw_region_at( 123,359 );
    draw_region_at( 501,359 );
    
    // draw selection highlights
    select_region( RegionMenuSelection );
    draw_region_at( 191, 172 + 44 * MainMenu_SelectedOption );
    
    set_multiply_color( color_cyan );
    select_region( RegionMenuTriple + MainMenu_SelectedOption );
    draw_region_at( 191, 172 + 44*MainMenu_SelectedOption );
    
    // restore light level
    set_multiply_color( color_white );
}

// ---------------------------------------------------------

void MainMenu_SimulateBubbles()
{
    // simulate gravity and movement
    for( int i = 0; i < 4; ++i )
    {
        MenuBubbleSpeeds[ i ] += 15 / 60.0;
        MenuBubblePositions[ i ].y += MenuBubbleSpeeds[ i ] / 60.0;
    }
    
    // bounce bottom bubbles on the floor
    if( MenuBubblePositions[ 1 ].y > 360 )
    {
        MenuBubblePositions[ 1 ].y = 360;
        MenuBubbleSpeeds[ 1 ] *= -1;
    }
    
    if( MenuBubblePositions[ 3 ].y > 360 )
    {
        MenuBubblePositions[ 3 ].y = 360;
        MenuBubbleSpeeds[ 3 ] *= -1;
    }
    
    // bounce top bubbles on bottom ones
    if( MenuBubblePositions[ 0 ].y > (MenuBubblePositions[ 1 ].y - 120) )
    {
        MenuBubblePositions[ 0 ].y = MenuBubblePositions[ 1 ].y - 120;
        
        if( MenuBubbleSpeeds[ 0 ] > 0 )
          MenuBubbleSpeeds[ 0 ] *= -1;
    }
    
    if( MenuBubblePositions[ 2 ].y > (MenuBubblePositions[ 3 ].y - 120) )
    {
        MenuBubblePositions[ 2 ].y = MenuBubblePositions[ 3 ].y - 120;
        
        if( MenuBubbleSpeeds[ 2 ] > 0 )
          MenuBubbleSpeeds[ 2 ] *= -1;
    }
}


// ---------------------------------------------------------
//   MAIN MENU: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void MainMenu_RunState_Initialize()
{
    select_texture( TextureMenus );
    
    // initialize bubble positions
    MenuBubblePositions[ 0 ].x = MenuBubblePositions[ 1 ].x = 61;
    MenuBubblePositions[ 2 ].x = MenuBubblePositions[ 3 ].x = 578;

    MenuBubblePositions[ 0 ].y = 206;
    MenuBubblePositions[ 1 ].y = 345;
    MenuBubblePositions[ 2 ].y = 215;
    MenuBubblePositions[ 3 ].y = 359;
    
    // initialize bubble speeds
    MenuBubbleSpeeds[ 0 ] = MenuBubbleSpeeds[ 2 ] = -6;
    MenuBubbleSpeeds[ 1 ] = 3;
    MenuBubbleSpeeds[ 3 ] = -5;

    clear_screen( color_black );
    MainMenu_ChangeState( MainMenu_FadeIn );
}

// ---------------------------------------------------------

void MainMenu_RunState_FadeIn()
{
    // fade-in effect
    MainMenu_SimulateBubbles();
    MainMenu_Draw();
    
    // fade-in effect
    int BlackLevel = max( 0, 255 - 255 * MainMenu_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    
    if( MainMenu_ElapsedFrames >= 60 )
      MainMenu_ChangeState( MainMenu_Choose );    
}

// ---------------------------------------------------------

void MainMenu_RunState_Choose()
{
    MainMenu_SimulateBubbles();
    MainMenu_Draw();
    
    // process inputs
    select_gamepad( 0 );
    
    if( gamepad_up() == 1 )
    if( MainMenu_SelectedOption > 0 )
    {
        play_sound( SoundMenuMove );
        MainMenu_SelectedOption--;
    }
    
    if( gamepad_down() == 1 )
    if( MainMenu_SelectedOption < 1 )
    {
        play_sound( SoundMenuMove );
        MainMenu_SelectedOption++;
    }
    
    if( gamepad_button_start() == 1 )
    {
        play_sound( SoundMenuAccept );
        MainMenu_ChangeState( MainMenu_FadeOut );
    }
}

// ---------------------------------------------------------

void MainMenu_RunState_SetPlayers()
{
    
}

// ---------------------------------------------------------

void MainMenu_RunState_FadeOut()
{
    MainMenu_Draw();
    
    // blink selection color
    if( !((MainMenu_ElapsedFrames/4) % 2) )
      set_multiply_color( color_red );
    else
      set_multiply_color( color_white );
    
    // draw selection highlight
    select_region( RegionMenuTriple + MainMenu_SelectedOption );
    draw_region_at( 191, 172 + 44*MainMenu_SelectedOption );
    set_multiply_color( color_white );
    
    // fade-out effect
    int BlackLevel = min( 255, 255 * MainMenu_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    
    if( MainMenu_ElapsedFrames >= 60 )
    {
        if( MainMenu_SelectedOption == 0 )
        {
            GameScene = Scene_ArcadeTriple;
            ArcadeTriple_ChangeState( Arcade_Initialize );
        }
        
        else
        {
            GameScene = Scene_ArcadeClassic;
            ArcadeClassic_ChangeState( Arcade_Initialize );
        }
    }   
}


// ---------------------------------------------------------
//   MAIN MENU: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void MainMenu_RunStateMachine()
{
    if( GameState == MainMenu_Initialize )
      MainMenu_RunState_Initialize();
    
    else if( GameState == MainMenu_FadeIn )
      MainMenu_RunState_FadeIn();
    
    else if( GameState == MainMenu_Choose )
      MainMenu_RunState_Choose();
    
    else if( GameState == MainMenu_SetPlayers )
      MainMenu_RunState_SetPlayers();
    
    else if( GameState == MainMenu_FadeOut )
      MainMenu_RunState_FadeOut();
    
    // in any case, wait for next frame
    MainMenu_ElapsedFrames++;
}
