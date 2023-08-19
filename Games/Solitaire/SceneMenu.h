// ---------------------------------------------------------
//   MENU: STATE MANAGEMENT
// ---------------------------------------------------------


int Menu_ElapsedFrames;
int Menu_SelectedOption;

// ---------------------------------------------------------

void Menu_ChangeState( int NewState )
{
    GameState = NewState;
    Menu_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( NewState == Menu_FadeOut )
    {
        stop_channel( ChannelMusic );
        play_sound( SoundTitleStart );
    }
}


// ---------------------------------------------------------
//   MENU: GENERIC FUNCTIONS
// ---------------------------------------------------------


void Menu_DrawScene()
{
    // draw background
    clear_screen( make_color_rgb( 115, 130, 155 ) );
    select_region( RegionMenuSide );
    set_drawing_scale( 1, screen_height );
    draw_region_zoomed_at( 0, 0 );
    set_drawing_scale( -1, screen_height );
    draw_region_zoomed_at( screen_width - 1, 0 );
    
    // draw logo and title text
    select_region( RegionLogo );
    draw_region_at( 210, 16 );
    select_region( RegionChooseMode );
    draw_region_at( 198, 98 );
    
    // draw texts
    for( int Option = 0; Option < 3; ++Option )
    {
        select_region( FirstRegionModeTitles + Option );
        draw_region_at( 259, 159 + 66 * Option );
        
        select_region( FirstRegionModeDescriptions + Option );
        draw_region_at( 259, 194 + 66 * Option );
    }
}


// ---------------------------------------------------------
//   MENU: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Menu_RunState_Initialize()
{
    clear_screen( color_black );
    Menu_ChangeState( Menu_FadeIn );
    Menu_SelectedOption = Mode_Normal;
}

// ---------------------------------------------------------

void Menu_RunState_FadeIn()
{
    // fade-in effect
    Menu_DrawScene();
    
    // fade-in effect
    int BlackLevel = max( 0, 255 - 255 * Menu_ElapsedFrames / 30 );
    clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    
    if( Menu_ElapsedFrames >= 30 )
      Menu_ChangeState( Menu_Choose );    
}

// ---------------------------------------------------------

void Menu_RunState_Choose()
{
    Menu_DrawScene();
    
    // draw cursor animation at the correct option
    int CursorFrame = (Menu_ElapsedFrames / 3) % 8;
    select_region( FirstRegionCursorDialog + CursorFrame );
    draw_region_at( 246, 175 + 66 * Menu_SelectedOption );
    
    // process inputs
    select_gamepad( 0 );
    
    if( gamepad_up() == 1 )
    if( Menu_SelectedOption > Mode_Easy )
    {
        play_sound( SoundMenuMove );
        Menu_SelectedOption--;
    }
    
    if( gamepad_down() == 1 )
    if( Menu_SelectedOption < Mode_Hard )
    {
        play_sound( SoundMenuMove );
        Menu_SelectedOption++;
    }
    
    if( gamepad_button_start() == 1 )
      Menu_ChangeState( Menu_FadeOut );
}

// ---------------------------------------------------------

void Menu_RunState_FadeOut()
{
    Menu_DrawScene();
    
    // blink selection color
    if( !((Menu_ElapsedFrames/4) % 2) )
      set_multiply_color( color_red );
    else
      set_multiply_color( color_white );
    
    // draw selection highlight
    select_region( FirstRegionModeTitles + Menu_SelectedOption );
    draw_region_at( 259, 159 + 66 * Menu_SelectedOption );
    select_region( FirstRegionModeDescriptions + Menu_SelectedOption );
    draw_region_at( 259, 194 + 66 * Menu_SelectedOption );
    set_multiply_color( color_white );
    
    // fade-out effect
    int BlackLevel = min( 255, 255 * Menu_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    
    // select game scene for each mode
    if( Menu_ElapsedFrames >= 60 )
    {
        GameScene = Scene_Gameplay;
        GameState = Gameplay_Initialize;
        
        switch( Menu_SelectedOption )
        {
            case Mode_Normal:
                DrawnCards = 2;
                break;
            
            case Mode_Hard:
                DrawnCards = 3;
                break;
            
            // default to easy if something breaks
            default:
                DrawnCards = 1;
                break;
        }
    }   
}


// ---------------------------------------------------------
//   MENU: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Menu_RunStateMachine()
{
    if( GameState == Menu_Initialize )
      Menu_RunState_Initialize();
    
    else if( GameState == Menu_FadeIn )
      Menu_RunState_FadeIn();
    
    else if( GameState == Menu_Choose )
      Menu_RunState_Choose();
    
    else if( GameState == Menu_FadeOut )
      Menu_RunState_FadeOut();
    
    // in any case, wait for next frame
    Menu_ElapsedFrames++;
}
