// ---------------------------------------------------------
//   SCENE MANIPULATION FUNCTIONS
// ---------------------------------------------------------


// this initializes the scene to known values
void ResetGameScene()
{
    GameState.PlayerPosition.x = 0;
    GameState.PlayerPosition.y = 2;
    GameState.ItemPosition.x = 2;
    GameState.ItemPosition.y = 2;
    GameState.Score = 0;
}

// ---------------------------------------------------------

// this initializes the scene to known values
void CreateNewItem()
{
    GameState.ItemPosition.x = rand() % TilesInX;
    GameState.ItemPosition.y = rand() % TilesInY;
}

// ---------------------------------------------------------

// forces an object position into the game field
void ClampPosition( Point* Position )
{
    clamp( &Position->x, 0, TilesInX-1 );
    clamp( &Position->y, 0, TilesInY-1 );
}


// ---------------------------------------------------------
//   SCENE DRAWING FUNCTIONS
// ---------------------------------------------------------


void DrawGameScene()
{
    clear_screen( color_black );
    
    // draw left indicators
    select_region( RegionLoad );
    draw_region_at( 33, 116 );
    
    select_region( RegionSave );
    draw_region_at( 33, 143 );    
    
    select_region( RegionDogHouse );
    draw_region_at( 0, 187 );
    
    // draw empty tiles
    select_region( RegionTile );
    for( int y = 0; y < TilesInY; y++ )
      for( int x = 0; x < TilesInX; x++ )
        draw_region_at( 142 + TileSize*x, 22 + TileSize*y );
    
    // write current score
    PrintScore( 19, 228, GameState.Score );
    
    // draw game objects
    select_region( RegionDog );
    draw_region_at( 142 + TileSize*GameState.PlayerPosition.x, 22 + TileSize*GameState.PlayerPosition.y );
    
    select_region( RegionBone );
    draw_region_at( 142 + TileSize*GameState.ItemPosition.x, 22 + TileSize*GameState.ItemPosition.y );
}
