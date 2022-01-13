// ---------------------------------------------------------
//   WIN 65536: STATE MANAGEMENT
// ---------------------------------------------------------


// state variables
int Win65536_ElapsedFrames;

// ---------------------------------------------------------

void Win65536_ChangeState( int NewState )
{
    GameState = NewState;
    Win65536_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( NewState == Dialog_Highlight )
    {
        stop_channel( ChannelMusic );
        play_sound( SoundWin );
    }
    
    else if( NewState == Dialog_FadeOut )
      play_sound( SoundMenuAccept );
}


// ---------------------------------------------------------
//   WIN 65536: Gameplay_ChangeState( Gameplay_AddTile ) FUNCTIONS
// ---------------------------------------------------------


void DrawWin65536Dialog( bool DrawCursor )
{
    select_region( RegionDialogWin );
    draw_region_at( 76,103 );
    
    select_region( RegionDialogExit );
    draw_region_at( 76,195 );
    
    select_region( RegionTile2 + 15 );
    draw_region_at( 124,151 );
    
    select_region( RegionTextEnded );
    draw_region_at( 178,169 );
    
    if( DrawCursor )
    {
        // draw cursor animation
        int CursorFrame = (Win65536_ElapsedFrames / 3) % 8;
        select_region( FirstRegionCursor + CursorFrame );
        draw_region_at( 167,224 );
    }
}


// ---------------------------------------------------------
//   WIN 65536: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Win65536_RunState_Initialize()
{
    // disable any other highlights
    DirectionHighlightFrames = 0;
    TileHighlightFrames = 0;
    ScoreHighlightFrames = 0;
    
    // transition immediately
    Win65536_ChangeState( Dialog_Highlight );
}

// ---------------------------------------------------------

void Win65536_RunState_Highlight()
{
    // draw the scene
    DrawBackground();
    DrawGUI();
    DrawBoard();
    
    // do a blinking highlight for all 65536 tiles
    if( Win65536_ElapsedFrames & 16 )
    {
        set_blending_mode( blending_add );
        select_region( RegionTileHighlight );
        
        for( int y = 0; y < 4; y++ )
          for( int x = 0; x < 4; x++ )
            if( CurrentBoard[ y ][ x ] == 16 )   // 2 ^ 16 = 65536
              draw_region_at( 100 + 75*x, 67 + 75*y );
              
        set_blending_mode( blending_alpha );
    }
    
    // when finished, transition
    if( Win65536_ElapsedFrames >= 125 )
      Win65536_ChangeState( Dialog_Choose );
}

// ---------------------------------------------------------

void Win65536_RunState_Choose()
{
    // buttons A or Start confirm selection
    if( gamepad_button_a() == 1 || gamepad_button_start() == 1 )
      Win65536_ChangeState( Dialog_FadeOut );
    
    // draw the scene
    DrawBackground();
    DrawGUI();
    DrawWin65536Dialog( true );
}

// ---------------------------------------------------------

void Win65536_RunState_FadeOut()
{
    // when fade-out ends, apply selection
    if( Win65536_ElapsedFrames >= 45 )
    {
        // apply end of game
        GameScene = Scene_Title;
        GameState = Title_Initialize;
    }
    
    // draw scene
    DrawBackground();
    DrawGUI();
    DrawWin65536Dialog( false );
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * Win65536_ElapsedFrames / 45 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
}


// ---------------------------------------------------------
//   WIN 65536: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Win65536_RunStateMachine()
{
    if( GameState == Dialog_Initialize )
      Win65536_RunState_Initialize();
    
    else if( GameState == Dialog_Highlight )
      Win65536_RunState_Highlight();
    
    else if( GameState == Dialog_Choose )
      Win65536_RunState_Choose();
    
    else if( GameState == Dialog_FadeOut )
      Win65536_RunState_FadeOut();
    
    // count the frames in current state
    Win65536_ElapsedFrames++;
}
