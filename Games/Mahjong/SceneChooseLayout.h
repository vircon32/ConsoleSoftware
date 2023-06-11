// ---------------------------------------------------------
//   CHOOSE LAYOUT: STATE MANAGEMENT
// ---------------------------------------------------------


int ChooseLayout_ElapsedFrames;
int ChooseLayout_ChosenOption;

// screen position for the 4 layout options
Position[ 4 ] OptionPositions =
{
    { 149,  95 }, { 341,  95 },
    { 149, 237 }, { 341, 237 }
};

// ---------------------------------------------------------

void ChooseLayout_ChangeState( int NewState )
{
    GameState = NewState;
    ChooseLayout_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( NewState == ChooseLayout_Chosen )
      play_sound( SoundPressStart );
}


// ---------------------------------------------------------
//   CHOOSE LAYOUT: DRAWING FUNCTIONS
// ---------------------------------------------------------


void ChooseLayout_DrawScene( bool DrawOptions )
{
    // draw background
    select_region( RegionTableMat );
    draw_region_at( 0, 0 );
    
    // draw left and right shadow
    select_region( RegionGUIShadow );
    set_drawing_scale( 1, screen_height );
    draw_region_zoomed_at( 0, 0 );
    
    set_drawing_scale( -1, screen_height );
    draw_region_zoomed_at( screen_width-1, 0 );
    
    // when requested, draw additional elements
    if( DrawOptions )
    {
        // draw choose text
        select_region( RegionChooseLayout );
        draw_region_at( screen_width/2, 15 );
        
        for( int i = 0; i < 4; ++i )
        {
            // name
            select_region( FirstRegionLayoutNames + i );
            draw_region_at( OptionPositions[ i ].x, OptionPositions[ i ].y - 8 );
            
            // image
            select_region( FirstRegionLayoutImages + i );
            draw_region_at( OptionPositions[ i ].x, OptionPositions[ i ].y );
            
            // frame
            select_region( RegionLayoutHighlight );
            draw_region();
        }
        
        // draw selection highlight
        if( (ChooseLayout_ElapsedFrames / 8) % 2 )
          set_multiply_color( color_red );
        else set_multiply_color( color_cyan );
        
        select_region( RegionLayoutHighlight );
        draw_region_at( OptionPositions[ ChooseLayout_ChosenOption ].x, OptionPositions[ ChooseLayout_ChosenOption ].y );
        
        set_multiply_color( color_white );
    }
}


// ---------------------------------------------------------
//   CHOOSE LAYOUT: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void ChooseLayout_RunState_Initialize()
{
    // set default choice
    ChooseLayout_ChosenOption = 0;
    
    // transition immediately
    ChooseLayout_ChangeState( ChooseLayout_FadeIn );
}

// ---------------------------------------------------------

void ChooseLayout_RunState_FadeIn()
{
    // draw scene
    ChooseLayout_DrawScene( false );
    
    // fade-in effect
    int BlackLevel = max( 0, 255 - 255 * ChooseLayout_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    
    // change state when finished
    if( ChooseLayout_ElapsedFrames >= 60 )
      ChooseLayout_ChangeState( ChooseLayout_Choosing );
}

// ---------------------------------------------------------

void ChooseLayout_RunState_Choosing()
{
    // draw scene
    ChooseLayout_DrawScene( true );
    
    // d-pad changes selection
    if( gamepad_left() == 1 )
    {
        if( ChooseLayout_ChosenOption & 1 )
        {
            play_sound( SoundMenuMove );
            ChooseLayout_ChosenOption--;
        }
    }
    
    else if( gamepad_right() == 1 )
    {
        if( !(ChooseLayout_ChosenOption & 1) )
        {
            ChooseLayout_ChosenOption++;
            play_sound( SoundMenuMove );
        }
    }
    
    else if( gamepad_up() == 1 )
    {
        if( ChooseLayout_ChosenOption >= 2 )
        {
            ChooseLayout_ChosenOption -= 2;
            play_sound( SoundMenuMove );
        }
    }
    
    else if( gamepad_down() == 1 )
    {
        if( ChooseLayout_ChosenOption < 2 )
        {
            ChooseLayout_ChosenOption += 2;
            play_sound( SoundMenuMove );
        }
    }
    
    // just in case, limit selection range
    if( ChooseLayout_ChosenOption < 0 ) ChooseLayout_ChosenOption = 0;
    if( ChooseLayout_ChosenOption > 3 ) ChooseLayout_ChosenOption = 3;
    
    // buttons A or Start confirm selection
    if( gamepad_button_start() == 1 || gamepad_button_a() == 1 )
    {
        ChooseLayout_ChangeState( ChooseLayout_Chosen );
        return;
    }
}

// ---------------------------------------------------------

void ChooseLayout_RunState_Chosen()
{
    // draw scene
    ChooseLayout_DrawScene( true );
    
    // highlight chosen layout and make it blink
    if( (ChooseLayout_ElapsedFrames/4) % 2 )
    {
        set_blending_mode( blending_add );
        select_region( FirstRegionLayoutImages + ChooseLayout_ChosenOption );
        draw_region_at( OptionPositions[ ChooseLayout_ChosenOption ].x, OptionPositions[ ChooseLayout_ChosenOption ].y );
        set_blending_mode( blending_alpha );
    }
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * ChooseLayout_ElapsedFrames / 45 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
    
    // change scene when finished
    if( ChooseLayout_ElapsedFrames >= 60 )
    {
        // load the chosen layout
        switch( ChooseLayout_ChosenOption )
        {
            case 0:  CopyLayout( &LayoutTurtle   ); break;
            case 1:  CopyLayout( &LayoutBridge   ); break;
            case 3:  CopyLayout( &LayoutPyramids ); break;
            case 2:  CopyLayout( &LayoutCastle   ); break;
            default: CopyLayout( &LayoutTurtle   ); break;
        }
        
        // go to gameplay
        GameScene = Scene_Gameplay;
        GameState = Gameplay_Initialize;
    }
}


// ---------------------------------------------------------
//   CHOOSE LAYOUT: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void ChooseLayout_RunStateMachine()
{
    switch( GameState )
    {
        case ChooseLayout_Initialize:
            ChooseLayout_RunState_Initialize();
            break;
            
        case ChooseLayout_FadeIn:
            ChooseLayout_RunState_FadeIn();
            break;
            
        case ChooseLayout_Choosing:
            ChooseLayout_RunState_Choosing();
            break;
            
        case ChooseLayout_Chosen:
            ChooseLayout_RunState_Chosen();
            break;
    }
    
    // count the frames in current state
    ChooseLayout_ElapsedFrames++;
}


