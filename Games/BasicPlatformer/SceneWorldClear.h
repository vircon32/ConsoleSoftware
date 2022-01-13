// ---------------------------------------------------------
//   WORLD CLEAR: STATE MANAGEMENT
// ---------------------------------------------------------


int WorldClear_ElapsedFrames;
int[ 10 ] WorldClear_WorldText;

// ---------------------------------------------------------

void WorldClear_ChangeState( int NewState )
{
    GameState = NewState;
    WorldClear_ElapsedFrames = 0;
}


// ---------------------------------------------------------
//    WORLD CLEAR: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void WorldClear_RunState_Initialize()
{
    // stop music and play clear sound
    stop_channel( ChannelMusic );
    play_sound( MusicWorldClear );
    
    // form the world text by replacing the digit
    strcpy( WorldClear_WorldText, "WORLD 0" );
    WorldClear_WorldText[ 6 ] = '0' + WorldNumber;
    
    // transition immediately
    WorldClear_ChangeState( WorldClear_TextDown );
}

// ---------------------------------------------------------

void WorldClear_RunState_TextDown()
{
    // fade-in from black
    clear_screen( color_black );
    int OpacityLevel = max( 0, 255 * WorldClear_ElapsedFrames / 60 );
    set_multiply_color( make_color_rgba( 255,255,255,OpacityLevel ) );
    
    // draw background
    select_texture( TextureGame );
    select_region( RegionWorldClearBackground );
    set_drawing_scale( 8,8 );
    draw_region_zoomed_at( 0,0 );
    
    // draw separating bar
    select_region( RegionWorldClearBar );
    draw_region_at( screen_width/2,175 );
    
    // draw world text descending from top
    int WorldTextBaseY = -38 + WorldClear_ElapsedFrames * 4;
    DrawTextCentered( screen_width/2, WorldTextBaseY, WorldClear_WorldText );
    
    // transition when text reaches its target height
    if( WorldTextBaseY >= 162 )
    {
        set_multiply_color( color_white );
        WorldClear_ChangeState( WorldClear_Waiting );
    }
}

// ---------------------------------------------------------

void WorldClear_RunState_Waiting()
{
    // draw background
    select_texture( TextureGame );
    select_region( RegionWorldClearBackground );
    set_drawing_scale( 8,8 );
    draw_region_zoomed_at( 0,0 );
    
    // draw separating bar
    select_region( RegionWorldClearBar );
    draw_region_at( screen_width/2,175 );
    
    // draw texts above and below the bar,
    // but have a blinking effect below
    DrawTextCentered( screen_width/2,162 , WorldClear_WorldText );
    
    if( ((WorldClear_ElapsedFrames & 8) == 0) || WorldClear_ElapsedFrames >= 80 )
      DrawTextCentered( screen_width/2,229 , "CLEAR!" );
    
    // transition when finished
    if( WorldClear_ElapsedFrames >= 120 )
      WorldClear_ChangeState( WorldClear_FadeOut );
}

// ---------------------------------------------------------

void WorldClear_RunState_FadeOut()
{
    // draw background
    select_texture( TextureGame );
    select_region( RegionWorldClearBackground );
    set_drawing_scale( 8,8 );
    draw_region_zoomed_at( 0,0 );
    
    // draw separating bar
    select_region( RegionWorldClearBar );
    draw_region_at( screen_width/2,175 );
    
    // draw texts above and below the bar,
    DrawTextCentered( screen_width/2,162 , WorldClear_WorldText );
    DrawTextCentered( screen_width/2,229 , "CLEAR!" );
    
    // restore graphic settings
    select_texture( TextureGame );
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * WorldClear_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
    
    // when done, change scene depending on the cleared world
    if( WorldClear_ElapsedFrames >= 60 )
    {
        // if there are no more worlds, show credits
        if( WorldNumber >= NumberOfWorlds )
        {
            GameScene = Scene_Credits;
            Credits_ChangeState( Credits_Initialize );
        }
        
        // otherwise begin gameplay on next world
        else
        {
            WorldNumber++;
            LevelNumber = 1;
            GameScene = Scene_Gameplay;
            Gameplay_ChangeState( Gameplay_Initialize );
            
            // reset lives (9 tries per world)
            PlayerLives = LivesPerWorld;
        }
    }
}


// ---------------------------------------------------------
//   WORLD CLEAR: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void WorldClear_RunStateMachine()
{
    if( GameState == WorldClear_Initialize )
      WorldClear_RunState_Initialize();
    
    else if( GameState == WorldClear_TextDown )
      WorldClear_RunState_TextDown();
    
    else if( GameState == WorldClear_Waiting )
      WorldClear_RunState_Waiting();
    
    else if( GameState == WorldClear_FadeOut )
      WorldClear_RunState_FadeOut();
    
    // count the frames in current state
    WorldClear_ElapsedFrames++;
}
