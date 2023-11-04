// ---------------------------------------------------------
//   BASIC FUNCTIONS FOR GAME LOGIC
// ---------------------------------------------------------


void LoadMaze()
{
    // make a copy of the original maze,
    // so that we can modify it in the game
    memcpy( &MazeArray, &OriginalMazeArray, sizeof(OriginalMazeArray) );
    
    // detect the player start position tile
    for( int TileY = 0; TileY < MAZE_HEIGHT; ++TileY )
      for( int TileX = 0; TileX < MAZE_WIDTH; ++TileX )
        if( MazeArray[ TileY ][ TileX ] == TilePlayer )
        {
            Player_Create( &Player1, TileX, TileY );
            MazeArray[ TileY ][ TileX ] = TileEmpty;
            return;
        }
}

// ---------------------------------------------------------

// set initial state for the whole game
void ResetGameSession()
{
    // load the level map
    LoadMaze();
    
    // reset the player
    Player_Reset( &Player1 );
    
    // set initial game state
    GameState = StateStart;
    RemainingTimeFrames = 30 * frames_per_second;
    CollectedGems = 0;
}

// ---------------------------------------------------------

void ProcessTileUnderPlayer( int TileX, int TileY )
{
    int* Tile = &MazeArray[ TileY ][ TileX ];
    
    // collect a red gem
    if( *Tile == TileRedGem )
    {
        *Tile = TileEmpty;
        CollectedGems += 1;
        play_sound( SoundCollectRedGem );
        return;
    }
    
    // collect a red gem
    if( *Tile == TileBlueGem )
    {
        *Tile = TileEmpty;
        CollectedGems += 5;
        play_sound( SoundCollectBlueGem );
        return;
    }
    
    // collect extra time
    if( *Tile == TileTime )
    {
        *Tile = TileEmpty;
        RemainingTimeFrames += 10 * frames_per_second;
        play_sound( SoundCollectTime );
        return;
    }
    
    // arrive at the goal
    if( *Tile == TileGoal )
    {
        play_sound( SoundGoal );
        GameState = StateGoal;
        return;
    }
    
    // die from spikes
    if( *Tile == TileSpikes )
    {
        play_sound( SoundDeath );
        GameState = StateDeath;
        return;
    }
}

// ---------------------------------------------------------

// when the player steps over certain tiles,
// gameplay events will happen; this function
// will process all tiles under the player
void ProcessPlayerPosition()
{
    // find all tiles covered by the player
    int MinTileX = box_left  ( &Player1.Shape ) / TilesMaze.width;
    int MaxTileX = box_right ( &Player1.Shape ) / TilesMaze.width;
    int MinTileY = box_top   ( &Player1.Shape ) / TilesMaze.height;
    int MaxTileY = box_bottom( &Player1.Shape ) / TilesMaze.height;
    
    // process all of them
    for( int TileY = MinTileY; TileY <= MaxTileY; ++TileY )
      for( int TileX = MinTileX; TileX <= MaxTileX; ++TileX )
        ProcessTileUnderPlayer( TileX, TileY );
}


// ---------------------------------------------------------
//   GAME DRAWING FUNCTIONS
// ---------------------------------------------------------


void DrawGame()
{
    // focus camera on the player, but keep
    // the map always filling the whole screen
    MapMaze.camera_position = Player1.Shape.position;
    tilemap_clip_camera_position( &MapMaze );
    
    // draw the map
    tilemap_draw_from_camera( &MapMaze );
    
    // draw the player over the map
    Player_Draw( &Player1 );
}

// ---------------------------------------------------------

void DrawGUI()
{
    select_texture( TextureGemMaze );
    
    // draw frame for collected gems at top-left corner
    select_region( RegionGuiGems );
    draw_region_at( 0, 0 );
    
    // draw the number of collected gems, always using 2 digits
    numberfont_print_int_fixed( &FontNumbers, 51, 6, CollectedGems, 2 );
    
    // draw remaining time at top-right corner
    select_region( RegionGuiTime );
    draw_region_at( screen_width-1, 0 );
    
    // draw the number of remaining seconds, always using 2 digits
    int RemainingSeconds = RemainingTimeFrames / frames_per_second;
    numberfont_print_int_fixed( &FontNumbers, 599, 6, RemainingSeconds, 2 );
}


// ---------------------------------------------------------
//   GAME UPDATE FUNCTIONS FOR EACH GAME STATE
// ---------------------------------------------------------


void RunState_Start()
{
    // draw the scene without GUI
    DrawGame();
    
    // darken the screen
    clear_screen( make_color_rgba( 0, 0, 0, 128 ) );
    
    // draw the press start text
    select_region( RegionTextStart );
    draw_region_at( screen_width/2, screen_height/2 );
    
    // init play if start is pressed
    if( gamepad_button_start() == 1 )
    {
        play_sound( SoundStart );
        GameState = StatePlay;
    }
}

// ---------------------------------------------------------

void RunState_Play()
{
    // update player movement
    Player_Update( &Player1 );
    
    // check if player steps on special tiles
    ProcessPlayerPosition();
    
    // decrease time, and check for time out
    RemainingTimeFrames--;
    
    if( RemainingTimeFrames <= 0 )
    {
        play_sound( SoundTimeOut );
        GameState = StateDeath;
    }
    
    // draw the scene with GUI
    DrawGame();
    DrawGUI();
}

// ---------------------------------------------------------

void RunState_Death()
{
    // draw the scene with GUI
    DrawGame();
    DrawGUI();
    
    // darken the screen
    clear_screen( make_color_rgba( 64, 0, 0, 128 ) );
    
    // show a death message
    select_region( RegionTextDeath );
    draw_region_at( screen_width/2, screen_height/2 );
    
    // reset game if start is pressed
    if( gamepad_button_start() == 1 )
      ResetGameSession();
}

// ---------------------------------------------------------

void RunState_Goal()
{
    // draw the scene with GUI
    DrawGame();
    DrawGUI();
    
    // lighten the screen
    clear_screen( make_color_rgba( 200, 255, 255, 128 ) );
    
    // show a clear message
    select_region( RegionTextClear );
    draw_region_at( screen_width/2, 160 );
    
    // show the number of collected gems, always using 2 digits
    numberfont_print_int_fixed( &FontNumbers, 304, 220, CollectedGems, 2 );
    
    // reset game if start is pressed
    if( gamepad_button_start() == 1 )
      ResetGameSession();
}
