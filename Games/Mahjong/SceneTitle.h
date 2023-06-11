// ---------------------------------------------------------
//   TITLE: STATE MANAGEMENT
// ---------------------------------------------------------


int Title_ElapsedFrames;

// screen position for the 4 tiles
struct Position{ int x, y; };
Position[ 4 ] TilePositions;

// initial tile positions (off-screen)
Position[ 4 ] InitialTilePositions =
{
    { 113, 283 - 300 }, { 172, 312 - 450 },
    { 231, 340 - 600 }, { 172, 279 - 750 }
};

// final fall position for the tiles
int[ 4 ] FinalTileHeights = { 283, 312, 340, 279 };

// ---------------------------------------------------------

void Title_ChangeState( int NewState )
{
    GameState = NewState;
    Title_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( NewState == Title_StartPressed )
    {
        stop_channel( ChannelMusic );
        play_sound( SoundPressStart );
    }
    
    else if( NewState == Title_Flash )
      play_sound( SoundTitleLogo );
    
    else if( NewState == Title_Waiting )
      play_sound_in_channel( MusicTitle, ChannelMusic );
}


// ---------------------------------------------------------
//   TITLE: DRAWING FUNCTIONS
// ---------------------------------------------------------


void Title_DrawScene( bool DrawLogo )
{
    // draw background
    select_region( RegionTableMat );
    draw_region_at( 0, 0 );
    
    // draw left black triangle
    select_region( RegionBlackSlope );
    
    for( int y = 0; y < 201; ++y )
    {
        set_drawing_scale( 201-y, 1 );
        draw_region_zoomed_at( 0, y );
    }
    
    // draw right black triangle
    for( int y = 0; y < 119; ++y )
    {
        int ScaleX = 119 - y;
        set_drawing_scale( ScaleX, 1 );
        draw_region_zoomed_at( screen_width-1 - 2*ScaleX, y );
    }
    
    // draw our 4 tiles
    for( int i = 0; i < 4; i++ )
    {
        select_region( FirstRegionTitleTiles + (i % 2) );
        draw_region_at( TilePositions[ i ].x, TilePositions[ i ].y );
    }
    
    // when logo is requested, show extra elements
    if( DrawLogo )
    {
        select_region( RegionLogo );
        draw_region_at( 296, 105 );
        
        select_region( RegionLogoChinese );
        draw_region_at( 346, 37 );
        
        select_region( Region2023 );
        draw_region_at( 4, screen_height-5 );
        
        select_region( RegionCarra );
        draw_region_at( screen_width-5, screen_height-5 );
    }
}


// ---------------------------------------------------------
//   TITLE: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Title_RunState_Initialize()
{
    // initial configurations
    select_gamepad( 0 );
    select_texture( TextureMahjong );
    
    // reset tiles
    memcpy( TilePositions, InitialTilePositions, sizeof(TilePositions) );
    
    // transition immediately
    Title_ChangeState( Title_FadeIn );
}

// ---------------------------------------------------------

void Title_RunState_FadeIn()
{
    // draw scene
    Title_DrawScene( false );
    
    // fade-in effect
    int BlackLevel = max( 0, 255 - 255 * Title_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    
    // change state when finished
    if( Title_ElapsedFrames >= 60 )
      Title_ChangeState( Title_TilesFall );
}

// ---------------------------------------------------------

void Title_RunState_TilesFall()
{
    // draw scene
    Title_DrawScene( false );
    
    // process falling tiles
    for( int i = 0; i < 4; ++i )
    {
        // make tile fall down to its final height
        TilePositions[ i ].y = min( FinalTileHeights[ i ], TilePositions[ i ].y + 8 );
        
        // make a sound when it ends falling
        if( TilePositions[ i ].y < FinalTileHeights[ i ] )
          if( FinalTileHeights[ i ] - TilePositions[ i ].y <= 8 )
            play_sound( SoundTileCollide );
    }
    
    // we allow to skip the falling with the start button
    if( gamepad_button_start() == 1 )
    {
        // set final tile positions
        for( int i = 0; i < 4; ++i )
          TilePositions[ i ].y = FinalTileHeights[ i ];
        
        // change state
        Title_ChangeState( Title_Flash );
        return;
    }
    
    // change scene when finished
    if( TilePositions[ 3 ].y >= FinalTileHeights[ 3 ] )
      Title_ChangeState( Title_Flash );
}

// ---------------------------------------------------------

void Title_RunState_Flash()
{
    // draw scene
    Title_DrawScene( true );
    
    // fade from white
    int WhiteLevel = 128 * Title_ElapsedFrames / 6;
    clear_screen( make_color_rgba( 255,255,255,WhiteLevel ) );
    
    // change scene when finished
    if( Title_ElapsedFrames >= 6 )
        Title_ChangeState( Title_Waiting );
}

// ---------------------------------------------------------

void Title_RunState_Waiting()
{
    // draw scene
    Title_DrawScene( true );
    
    // draw "press start" and have it blink
    if( (Title_ElapsedFrames/30) % 2 )
    {
        select_region( RegionPressStart );
        draw_region_at( 382, 278 );
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
    // draw scene
    Title_DrawScene( true );
    
    // draw "press start" and have it blink
    if( (Title_ElapsedFrames/5) % 2 )
    {
        select_region( RegionPressStart );
        draw_region_at( 382, 278 );
    }
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * Title_ElapsedFrames / 45 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
    
    // change scene when finished
    if( Title_ElapsedFrames >= 60 )
    {
        GameScene = Scene_ChooseLayout;
        GameState = ChooseLayout_Initialize;
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
            
        case Title_TilesFall:
            Title_RunState_TilesFall();
            break;
            
        case Title_Flash:
            Title_RunState_Flash();
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
