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
    if( NewState == Title_Waiting )
      play_sound_in_channel( MusicTitle, ChannelMusic );
    
    else if( NewState == Title_StartPressed )
    {
        stop_channel( ChannelMusic );
        play_sound( SoundPressStart );
    }
}


// ---------------------------------------------------------
//   TITLE: DRAWING FUNCTIONS
// ---------------------------------------------------------


// we will need to copy title map into RAM,
// since we will edit it to make colors cycle
int[ TitleMapHeight ][ TitleMapWidth ] TitleMapEditable;

// ---------------------------------------------------------

void Title_DrawScene()
{
    // draw background color
    clear_screen( color_black );
    
    // draw left and right margins
    select_texture( TextureTitle );
    set_drawing_scale( 1, screen_height );
    
    select_region( RegionMarginLeft );
    draw_region_zoomed_at( 0, 0 );
    
    select_region( RegionMarginRight );
    draw_region_zoomed_at( screen_width-1, 0 );
    
    // draw info texts
    select_region( Region2023 );
    draw_region_at( 26, screen_height-5 );
    
    select_region( RegionCarra );
    draw_region_at( screen_width-27, screen_height-5 );
    
    // draw title map
    tilemap_draw( &MapTitle );
}


// ---------------------------------------------------------
//   TITLE: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Title_RunState_Initialize()
{
    // copy title map into RAM
    memcpy( &TitleMapEditable[0][0], &TitleMap[0][0], sizeof(TitleMapEditable) );
    MapTitle.topleft_y = 95;
    
    // initial configurations
    select_gamepad( 0 );
    
    // transition immediately
    Title_ChangeState( Title_Waiting );
}

// ---------------------------------------------------------

void Title_RunState_Waiting()
{
    // make tiles in map lower row cycle left
    if( (Title_ElapsedFrames % 8) == 0 )
    {
        int[ 3 ] LowerRowLeftTiles;
        memcpy( LowerRowLeftTiles, &TitleMapEditable[ TitleMapHeight-1 ][ 0 ], 3 );
        memcpy( &TitleMapEditable[ TitleMapHeight-1 ][ 0 ], &TitleMapEditable[ TitleMapHeight-1 ][ 3 ], TitleMapWidth - 3 );
        memcpy( &TitleMapEditable[ TitleMapHeight-1 ][ TitleMapWidth-3 ], LowerRowLeftTiles, 3 );
    }
    
    // draw scene
    Title_DrawScene();
    
    // draw "press start" and have it blink
    if( (Title_ElapsedFrames/30) % 2 )
    {
        select_texture( TextureTitle );
        select_region( RegionPressStart );
        draw_region_at( screen_width/2, 291 );
    }
    
    // change state when start pressed on first gamepad
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
        // fade out to black only the "press start" text
        int BlacknessLevel = min( 255, 255 * Title_ElapsedFrames / 45 );
        set_multiply_color( make_color_rgba( 255,255,255,255 - BlacknessLevel ) );
        
        select_texture( TextureTitle );
        select_region( RegionPressStart );
        draw_region_at( screen_width/2, 291 );
        
        set_multiply_color( color_white );
    }
    
    // change scene when finished
    if( Title_ElapsedFrames >= 60 )
    {
        GameScene = Scene_ChooseLevel;
        GameState = ChooseLevel_Initialize;
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


