struct Tile
{
    int MineState;       // 0 = free, 1-8 = adjacent (numbers), 9 = mine
    int GraphicTile;
    int RevealDistance;  // 0 = not revealed, 1 = player chosen tile (1st step), 2-N: Number of steps to reach it
    bool Exposed;
    bool Flagged;
};

struct Board
{
    Tile[ 80 ][ 150 ] Tiles;
    int Width, Height;
    bool WrapAround;
    int NumberOfMines;
};

struct GameSession
{
    Board B;
    int SelectionX, SelectionY;
};

GameSession Game;

Board_Clear( Board* B )
{
    for( int y = 0; y < B->Height; y++ )
      for( int x = 0; x < B->Width; x++ )
        B->Tiles[ y ][ x ].MineState = 0;
    
    B->NumberOfMines = 0;
}

Board_PlaceMinesInArea( Board* B, int MinX, int MinY, int MaxX, int MaxY, int NumberOfMines )
{
    for( int i = 0; i < NumberOfMines; i++ )
    {
        int x, y;
        
        do
        {
            x = MinX + rand() % (MaxX - MinX + 1);
            y = MinY + rand() % (MaxY - MinY + 1);
        }
        while( B->Tiles[ y ][ x ].MineState == 9 );
        
        B->Tiles[ y ][ x ].MineState = 9;
        B->NumberOfMines++;
    }
}

Board_PlaceMinesFixed( Board* B )
{
    Board_PlaceMinesInArea( 
}

Board_PlaceMinesEndless( Board* B )
{
    for( int y = 0; y < 5; y++ )
      for( int x = 0; x < 5; x++ )
        Board_PlaceMinesInArea( B, x*30,y*16, x*30+29,y*16+15, 100 );
}

Board_DetermineTileStates()
{
    
}

Board_DetermineGraphicTiles( Board* B )
{
    for( int y = 0; y < B->Height; y++ )
      for( int x = 0; x < B->Width; x++ )
        B->Tiles[ y ][ x ].GraphicTile = rand() & 3;
}

Board_WriteTileState( Board* B, int TileX, int TileY )
{
    9->done
    other -> get (up to) 8 adjacent tiles, add
}

// tile revealing is propagating by recursing in 4 directions, 
// but it is done in discrete "steps", progressing in distance
// returns the maximum reveal distance (used by the scene to iterate)
int Board_RevealTile( Board* B, int TileX, int TileY, int CurrentDistance )
{
    // case 1: stay within board limits
    if( !B->WrapAround )
    {
        if( TileX < 0 || TileX >= B->Width  ) return 1;
        if( TileY < 0 || TileY >= B->Height ) return 1;
    }
    
    // case 2: apply board wrapping
    else
    {
        if( TileX < 0 ) TileX += B->Width;
        if( TileY < 0 ) TileY += B->Height;
        
        if( TileX >= B->Width  ) TileX += B->Width;
        if( TileY >= B->Height ) TileY += B->Height;
    }
    
    // for the root revealed cell first reset all reveal state
    if( CurrentDistance == 1 )
    {
        for( int y = 0; y < B->Height; y++ )
            for( int x = 0; x < B->Width; x++ )
            B->Tiles[ y ][ x ].RevealDistance = 0;
    }
    
    // do not rewrite distance in already calculated cells
    if( B->Tiles[ TileY ][ TileX ].RevealDistance > 0 )
      return 1;
    
    B->Tiles[ TileY ][ TileX ].RevealDistance = CurrentDistance;
    
    // for mine or adjacent tiles, stop recursion
    if( B->Tiles[ TileY ][ TileX ].MineState != 0 )
      return CurrentDistance;
    
    CurrentDistance++;
    
    // for free tiles, recurse to all 8 adjacent tiles
    int MaxDistance = CurrentDistance;
    MaxDistance = max( MaxDistance, Board_RevealTile( B, TileX-1, TileY-1, CurrentDistance ) );
    MaxDistance = max( MaxDistance, Board_RevealTile( B, TileX-1, TileY  , CurrentDistance ) );
    MaxDistance = max( MaxDistance, Board_RevealTile( B, TileX-1, TileY+1, CurrentDistance ) );
    MaxDistance = max( MaxDistance, Board_RevealTile( B, TileX  , TileY-1, CurrentDistance ) );
    MaxDistance = max( MaxDistance, Board_RevealTile( B, TileX  , TileY+1, CurrentDistance ) );
    MaxDistance = max( MaxDistance, Board_RevealTile( B, TileX+1, TileY-1, CurrentDistance ) );
    MaxDistance = max( MaxDistance, Board_RevealTile( B, TileX+1, TileY  , CurrentDistance ) );
    MaxDistance = max( MaxDistance, Board_RevealTile( B, TileX+1, TileY+1, CurrentDistance ) );
    
    return MaxDistance;
}
