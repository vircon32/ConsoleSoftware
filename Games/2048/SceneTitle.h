// ---------------------------------------------------------
//   TITLE: VARIABLES
// ---------------------------------------------------------


int[ 16 ] Title_TilesX;
int[ 16 ] Title_TilesY;


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
    {
        play_sound( SoundTitleLogo );
        play_sound_in_channel( MusicTitle, ChannelMusic );
    }
    
    else if( NewState == Title_StartPressed )
    {
        stop_channel( ChannelMusic );
        play_sound( SoundTitleStart );
    }
}


// ---------------------------------------------------------
//   TITLE: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Title_RunState_Initialize()
{
    // fill in tile positions
    for( int i = 0; i < 6; i++ )
    {
        Title_TilesX[ i ] = 125 + 77*i;
        Title_TilesY[ i ] = 60;
    }
    
    for( int i = 8; i < 14; i++ )
    {
        Title_TilesX[ i ] = Title_TilesX[ 13-i ];
        Title_TilesY[ i ] = 60 + 77*3;
    }
    
    Title_TilesX[ 7 ] = Title_TilesX[ 6 ] = Title_TilesX[ 5 ];
    Title_TilesX[ 15 ] = Title_TilesX[ 14 ] = Title_TilesX[ 0 ];
    
    Title_TilesY[ 6 ] = Title_TilesY[ 15 ] = 60 + 77*1;
    Title_TilesY[ 7 ] = Title_TilesY[ 14 ] = 60 + 77*2;
    
    // transition immediately
    Title_ChangeState( Title_TilesAppear );
}

// ---------------------------------------------------------

void Title_RunState_TilesAppear()
{
    // draw background
    clear_screen( make_color_rgb( 50,130,100 ) );
    
    // draw tiles progressively
    int NumberOfTiles = min( 16, Title_ElapsedFrames / 5 );
    
    for( int i = 0; i < NumberOfTiles; i++ )
    {
        select_region( RegionTile2 + i );
        draw_region_at( Title_TilesX[ i ], Title_TilesY[ i ] );
    }
    
    // when a tile appears, make a sound
    if( !(Title_ElapsedFrames % 5) )
      play_sound( SoundTitleTiles );
    
    // change state when finished
    if( Title_ElapsedFrames >= 84 )
      Title_ChangeState( Title_Waiting );   
}

// ---------------------------------------------------------

void Title_RunState_Waiting()
{
    // draw background
    clear_screen( make_color_rgb( 50,130,100 ) );
    
    // draw logo
    select_region( RegionTitleLogo );
    draw_region_at( 196,115 );
    
    // draw "press start" and have it blink
    if( (Title_ElapsedFrames/30) % 2 )
    {
        select_region( RegionPressStart );
        draw_region_at( 208,196 );
    }
    
    // draw tiles
    for( int i = 0; i < 16; i++ )
    {
        select_region( RegionTile2 + i );
        draw_region_at( Title_TilesX[ i ], Title_TilesY[ i ] );
    }
    
    // have tiles blink in rotation
    int LitTile = (Title_ElapsedFrames/10) % 16;
    
    select_region( RegionTileHighlight );
    set_blending_mode( blending_add );
    draw_region_at( Title_TilesX[ LitTile ], Title_TilesY[ LitTile ] );
    set_blending_mode( blending_alpha );
    
    // write date and author
    select_region( Region2021 );
    draw_region_at( 2, 357 );
    
    select_region( RegionCarra );
    draw_region_at( 637, 357 );
    
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
    // draw background
    clear_screen( make_color_rgb( 50,130,100 ) );
    
    // draw logo
    select_region( RegionTitleLogo );
    draw_region_at( 196,115 );
    
    // draw "press start" and have it blink
    if( (Title_ElapsedFrames/5) % 2 )
    {
        select_region( RegionPressStart );
        draw_region_at( 208,196 );
    }
    
    // draw tiles
    for( int i = 0; i < 16; i++ )
    {
        select_region( RegionTile2 + i );
        draw_region_at( Title_TilesX[ i ], Title_TilesY[ i ] );
    }
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * Title_ElapsedFrames / 45 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
    
    // change scene when finished
    if( Title_ElapsedFrames >= 60 )
    {
        GameScene = Scene_Gameplay;
        GameState = Gameplay_Initialize;
    }
}


// ---------------------------------------------------------
//   TITLE: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Title_RunStateMachine()
{
    if( GameState == Title_Initialize )
      Title_RunState_Initialize();
    
    else if( GameState == Title_TilesAppear )
      Title_RunState_TilesAppear();
    
    else if( GameState == Title_Waiting )
      Title_RunState_Waiting();
    
    else if( GameState == Title_StartPressed )
      Title_RunState_StartPressed();

    // count the frames in current state
    Title_ElapsedFrames++;
}
