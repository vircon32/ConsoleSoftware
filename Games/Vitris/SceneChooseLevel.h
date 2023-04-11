// ---------------------------------------------------------
//   CHOOSE LEVEL: STATE MANAGEMENT
// ---------------------------------------------------------


int ChooseLevel_ElapsedFrames;
int ChooseLevel_ChosenOption;

// ---------------------------------------------------------

void ChooseLevel_ChangeState( int NewState )
{
    GameState = NewState;
    ChooseLevel_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( NewState == ChooseLevel_TitleUp )
      play_sound( SoundPieceDrop );
    
    else if( NewState == ChooseLevel_Chosen )
      play_sound( SoundPressStart );
}


// ---------------------------------------------------------
//   CHOOSE LEVEL: DRAWING FUNCTIONS
// ---------------------------------------------------------


void ChooseLevel_DrawScene( bool DrawOptions )
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
    
    tilemap_draw( &MapTitle );
    select_texture( TextureTitle );
    
    if( DrawOptions )
    {
        select_region( RegionChooseLevel );
        draw_region_at( screen_width/2, 194 );
        
        for( int i = 0; i < 4; ++i )
        {
            select_region( FirstRegionLevels + i );
            draw_region_at( 200 + 60 * i, 255 );
        }
    }
}


// ---------------------------------------------------------
//   CHOOSE LEVEL: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void ChooseLevel_RunState_Initialize()
{
    // reset title from map in ROM
    memcpy( &TitleMapEditable[0][0], &TitleMap[0][0], sizeof(TitleMapEditable) );
    MapTitle.topleft_y = 95;
    
    // set default choice
    ChooseLevel_ChosenOption = 0;
    
    // transition immediately
    ChooseLevel_ChangeState( ChooseLevel_TitleUp );
}

// ---------------------------------------------------------

void ChooseLevel_RunState_TitleUp()
{
    MapTitle.topleft_y -= 5;
    ChooseLevel_DrawScene( false );
    
    // transition when title is at the right height
    if( MapTitle.topleft_y <= 30 )
      ChooseLevel_ChangeState( ChooseLevel_Choosing );
}

// ---------------------------------------------------------

void ChooseLevel_RunState_Choosing()
{
    // draw scene
    ChooseLevel_DrawScene( true );
    
    // draw cursor animation
    int CursorFrame = (ChooseLevel_ElapsedFrames / 3) % 8;
    select_region( FirstRegionCursorUp + CursorFrame );
    draw_region_at( 227 + 60 * ChooseLevel_ChosenOption, 306 );
    
    // left and right change selection
    if( gamepad_left() == 1 )
    {
        if( ChooseLevel_ChosenOption >= 1 )
        {
            play_sound( SoundMenuMove );
            ChooseLevel_ChosenOption--;
        }
    }
    
    else if( gamepad_right() == 1 )
    {
        if( ChooseLevel_ChosenOption <= 2 )
        {
            ChooseLevel_ChosenOption++;
            play_sound( SoundMenuMove );
        }
    }
    
    // buttons A or Start confirm selection
    if( gamepad_button_start() == 1 || gamepad_button_a() == 1 )
    {
        ChooseLevel_ChangeState( ChooseLevel_Chosen );
        return;
    }
}

// ---------------------------------------------------------

void ChooseLevel_RunState_Chosen()
{
    // draw scene
    ChooseLevel_DrawScene( true );
    
    // highlight chosen option and make it blink
    if( (ChooseLevel_ElapsedFrames/5) % 2 )
    {
        set_blending_mode( blending_add );
        select_region( FirstRegionLevels + ChooseLevel_ChosenOption );
        draw_region_at( 200 + 60 * ChooseLevel_ChosenOption, 255 );
        set_blending_mode( blending_alpha );
    }
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * ChooseLevel_ElapsedFrames / 45 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
    
    // change scene when finished
    if( ChooseLevel_ElapsedFrames >= 60 )
    {
        GameScene = Scene_Gameplay;
        GameState = Gameplay_Initialize;
    }
}


// ---------------------------------------------------------
//   CHOOSE LEVEL: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void ChooseLevel_RunStateMachine()
{
    switch( GameState )
    {
        case ChooseLevel_Initialize:
            ChooseLevel_RunState_Initialize();
            break;
            
        case ChooseLevel_TitleUp:
            ChooseLevel_RunState_TitleUp();
            break;
            
        case ChooseLevel_Choosing:
            ChooseLevel_RunState_Choosing();
            break;
            
        case ChooseLevel_Chosen:
            ChooseLevel_RunState_Chosen();
            break;
    }
    
    // count the frames in current state
    ChooseLevel_ElapsedFrames++;
}


