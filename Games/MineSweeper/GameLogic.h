// ---------------------------------------------------------
//   SINGLE TILE "CLASS"
// ---------------------------------------------------------


struct Tile
{
    int MineState;       // 0 = free tile, 1-8 = adjacent to mine (numbers), 9 = has mine
    int GraphicTile;
    int RevealDistance;  // 0 = not revealed, 1 = player chosen tile (1st step), 2 or greater: succesive steps
    bool Revealed;
    bool Flagged;
};

// ---------------------------------------------------------

void Tile_Clear( Tile* T )
{
    // clear all game indicators
    T->MineState = 0;
    T->Revealed = false;
    T->Flagged = false;
    
    // clear reveal distance
    T->RevealDistance = 0;
    
    // randomize graphic tile variation 
    T->GraphicTile = rand() & 3;
}


// ---------------------------------------------------------
//   FULL BOARD "CLASS"
// ---------------------------------------------------------


struct Board
{
    Tile[ 16 ][ 30 ] Tiles;
    int Width, Height;
    int NumberOfMines;
    int NumberOfFlags;
    int NumberOfTilesRevealed;
    int SelectionX, SelectionY;
    bool MinesArePlaced;
    bool MineExploded;
};

// our single board instance, declared as a global
Board GameBoard;

// ---------------------------------------------------------

void Board_Clear( Board* B )
{
    // clear every tile
    for( int y = 0; y < B->Height; y++ )
      for( int x = 0; x < B->Width; x++ )
        Tile_Clear( &B->Tiles[ y ][ x ] );
    
    // clear all board-level stats
    B->NumberOfFlags = 0;
    B->NumberOfTilesRevealed = 0;
    B->MinesArePlaced = false;
    B->MineExploded = false;
    
    // place initial selection at center
    B->SelectionX = B->Width / 2;
    B->SelectionY = B->Height / 2;
}

// ---------------------------------------------------------

void Board_PlaceSingleMine( Board* B, int TileX, int TileY )
{
    // place the mine
    B->Tiles[ TileY ][ TileX ].MineState = 9;
    
    // check for board limits
    bool LeftExists = (TileX > 0);
    bool TopExists  = (TileY > 0);
    bool RightExists  = (TileX < (B->Width  - 1));
    bool BottomExists = (TileY < (B->Height - 1));
    
    // update all 8 adjacent tiles
    if( LeftExists )
    {
        if( B->Tiles[ TileY ][ TileX-1 ].MineState < 9 )
          B->Tiles[ TileY ][ TileX-1 ].MineState++;
        
        if( TopExists )
          if( B->Tiles[ TileY-1 ][ TileX-1 ].MineState < 9 )
            B->Tiles[ TileY-1 ][ TileX-1 ].MineState++;
        
        if( BottomExists )
          if( B->Tiles[ TileY+1 ][ TileX-1 ].MineState < 9 )
            B->Tiles[ TileY+1 ][ TileX-1 ].MineState++;
    }
    
    if( RightExists )
    {
        if( B->Tiles[ TileY ][ TileX+1 ].MineState < 9 )
          B->Tiles[ TileY ][ TileX+1 ].MineState++;
        
        if( TopExists )
          if( B->Tiles[ TileY-1 ][ TileX+1 ].MineState < 9 )  
            B->Tiles[ TileY-1 ][ TileX+1 ].MineState++;
            
        if( BottomExists )
          if( B->Tiles[ TileY+1 ][ TileX+1 ].MineState < 9 )  
            B->Tiles[ TileY+1 ][ TileX+1 ].MineState++;
    }
    
    if( TopExists )
      if( B->Tiles[ TileY-1 ][ TileX ].MineState < 9 )
        B->Tiles[ TileY-1 ][ TileX ].MineState++;
    
    if( BottomExists )
      if( B->Tiles[ TileY+1 ][ TileX ].MineState < 9 )
        B->Tiles[ TileY+1 ][ TileX ].MineState++;
}

// ---------------------------------------------------------

// mines are placed only when the player chooses a
// first reveal tile, to ensure that the first tile is free
void Board_PlaceMines( Board* B, int ChosenTileX, int ChosenTileY )
{
    for( int i = 0; i < B->NumberOfMines; i++ )
    {
        int MineX, MineY;
        
        // keep re-choosing mine tile until a suitable position is found
        while( true )
        {
            MineX = rand() % B->Width;
            MineY = rand() % B->Height;
            
            // chosen tile has to be mine free
            if( B->Tiles[ MineY ][ MineX ].MineState == 9 )
              continue;
            
            // chosen tile has to be at least 2 tiles away from any mine)
            int DistanceX = abs( MineX - ChosenTileX );
            int DistanceY = abs( MineY - ChosenTileY );
            if( DistanceX < 2 && DistanceY < 2 ) continue;
            
            // if we reach here, this tile was valid
            break;
        }
        
        // place this mine
        Board_PlaceSingleMine( B, MineX, MineY );
    }
    
    // mark the board as having mines
    B->MinesArePlaced = true;
}

// ---------------------------------------------------------

void Board_RevealTile( Board* B, Tile* T )
{
    T->Revealed = true;
    B->NumberOfTilesRevealed++;
    
    // it makes no sense for revealed tiles to be flagged
    if( T->Flagged )
    {
        T->Flagged = false;
        B->NumberOfFlags--;
    }
}

// ---------------------------------------------------------

// this is the recursive function for tile reveal propagation
// {TileX, TileY} are the board coordinates of the tile to where reveal is being propagated
// {DeltaX, DeltaY} is the direction where we are propagating the reveal
// returns the the maximum reveal distance in this propagation path
int Board_PropagateReveal( Board* B, int TileX, int TileY )
{
    // do not propagate off board limits
    if( TileX < 0 || TileX >= B->Width  ) return 0;
    if( TileY < 0 || TileY >= B->Height ) return 0;
    
    // do not propagate to already revealed tiles
    if( B->Tiles[ TileY ][ TileX ].Revealed )
      return 0;
    
    // do not propagate to the same tiles twice
    if( B->Tiles[ TileY ][ TileX ].RevealDistance != 0 )
      return 0;
    
    // annotate current tile's reveal distance
    // (but don't actually reveal it yet!)
    int DistanceX = 1 + abs( TileX - B->SelectionX );
    int DistanceY = 1 + abs( TileY - B->SelectionY );
    int CurrentDistance  = max( DistanceX, DistanceY );
    B->Tiles[ TileY ][ TileX ].RevealDistance = CurrentDistance;
    
    // for mines or their adjacent tiles, stop propagation here
    if( B->Tiles[ TileY ][ TileX ].MineState != 0 )
      return CurrentDistance;
    
    // for free tiles, recurse to all 8 adjacent tiles
    int MaxDistance = CurrentDistance;
    MaxDistance = max( MaxDistance, Board_PropagateReveal( B, TileX-1, TileY-1 ) );
    MaxDistance = max( MaxDistance, Board_PropagateReveal( B, TileX  , TileY-1 ) );
    MaxDistance = max( MaxDistance, Board_PropagateReveal( B, TileX+1, TileY-1 ) );
    MaxDistance = max( MaxDistance, Board_PropagateReveal( B, TileX-1, TileY   ) );
    MaxDistance = max( MaxDistance, Board_PropagateReveal( B, TileX+1, TileY   ) );
    MaxDistance = max( MaxDistance, Board_PropagateReveal( B, TileX-1, TileY+1 ) );
    MaxDistance = max( MaxDistance, Board_PropagateReveal( B, TileX  , TileY+1 ) );
    MaxDistance = max( MaxDistance, Board_PropagateReveal( B, TileX+1, TileY+1 ) );
    
    return MaxDistance;
}

// ---------------------------------------------------------

// tile revealing is propagating by recursing in 8 directions, 
// but it is done in discrete "steps", progressing in distance
// returns the maximum reveal distance (used by the scene to iterate)
int Board_DetermineTilesToReveal( Board* B, int TileX, int TileY )
{
    // first delete any previous reveal distances
    for( int y = 0; y < B->Height; y++ )
      for( int x = 0; x < B->Width; x++ )
        B->Tiles[ y ][ x ].RevealDistance = 0;
    
    // annotate this root tile with reveal distance 1
    // (but don't actually reveal it yet!)
    int CurrentDistance = 1;
    B->Tiles[ TileY ][ TileX ].RevealDistance = CurrentDistance;
    
    // for mine or adjacent tiles, stop recursion
    if( B->Tiles[ TileY ][ TileX ].MineState != 0 )
      return CurrentDistance;
    
    // for free tiles, recurse to all 8 adjacent tiles
    int MaxDistance = CurrentDistance;
    MaxDistance = max( MaxDistance, Board_PropagateReveal( B, TileX-1, TileY-1 ) );
    MaxDistance = max( MaxDistance, Board_PropagateReveal( B, TileX  , TileY-1 ) );
    MaxDistance = max( MaxDistance, Board_PropagateReveal( B, TileX+1, TileY-1 ) );
    MaxDistance = max( MaxDistance, Board_PropagateReveal( B, TileX-1, TileY   ) );
    MaxDistance = max( MaxDistance, Board_PropagateReveal( B, TileX+1, TileY   ) );
    MaxDistance = max( MaxDistance, Board_PropagateReveal( B, TileX-1, TileY+1 ) );
    MaxDistance = max( MaxDistance, Board_PropagateReveal( B, TileX  , TileY+1 ) );
    MaxDistance = max( MaxDistance, Board_PropagateReveal( B, TileX+1, TileY+1 ) );
    
    return MaxDistance;
}

// ---------------------------------------------------------

// determines if we have already won the game
bool Board_IsSolved( Board* B )
{
    // board is solved when we have revealed
    // all tiles that don't contain a mine
    int TotalTiles = B->Width * B->Height;
    return (B->NumberOfTilesRevealed == (TotalTiles - B->NumberOfMines));
}
