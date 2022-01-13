// ---------------------------------------------------------
//   GAME SESSION DATA
// ---------------------------------------------------------


// board states
int[ 4 ][ 4 ] CurrentBoard, NextBoard;

// indicates which tiles are the result of
// a merge in the last move (this is used
// to determine points and draw highlights)
int[ 4 ][ 4 ] MergedTiles;

// next added tile (needs to be determined and
// stored before actually being added to the board)
int NextTileRow, NextTileColumn;
int NextTileValue;

// information about the move being made
int MoveX = 0, MoveY = 0;
int LastMoveWasValid = false;

// general game statistics
int PlayedFrames;
int Score;
int Moves;

// specific game information
int HighestTileEver;    // needed to know when winning tiles have just been created
int LastScoreIncrease;  // will be displayed at the score counter


// ---------------------------------------------------------
//   GAME LOGIC: BASIC FUNCTIONS
// ---------------------------------------------------------


// this check is used to know if free tiles exist
bool IsBoardFull()
{
    for( int y = 0; y < 4; y++ )
      for( int x = 0; x < 4; x++ )
        if( CurrentBoard[y][x] == 0 )
          return false;
        
    return true;
}

// ---------------------------------------------------------

// it determines a new tile and its location
// but does not actually add it to the board
// (scene transitions need this intermediate step)
void CreateNewTile()
{
    // we need some empty space for the tile
    if( IsBoardFull() )
    {
        NextTileValue = 0;
        return;
    }
    
    // keep choosing a tile until a free one is found
    while( true )
    {
        NextTileRow = rand() % 4;
        NextTileColumn = rand() % 4;
        
        if( CurrentBoard[ NextTileRow ][ NextTileColumn ] <= 0 )
          break;
    }
    
    // there is a 10% chance of creating a 4;
    // otherwise a 2 tile is created
    if( rand() % 10 )  NextTileValue = 1;
    else               NextTileValue = 2;
}

// ---------------------------------------------------------

// sets the initial game state for a new game
void ResetGameState()
{
    // clear any board contents
    memset( &CurrentBoard, 0, sizeof(CurrentBoard) );
    memset( &NextBoard, 0, sizeof(NextBoard) );
    memset( &MergedTiles, 0, sizeof(MergedTiles) );
    
    // randomize
    srand( get_time() );
    
    // reset game session data
    PlayedFrames = 0;
    Score = 0;
    Moves = 0;
    HighestTileEver = 1;
    LastScoreIncrease = 0;
    
    // populate board with 2 initial tiles
    for( int i = 0; i < 2; i++ )
    {
        CreateNewTile();
        CurrentBoard[ NextTileRow ][ NextTileColumn ] = NextTileValue;
        HighestTileEver = max( HighestTileEver, NextTileValue );
    }
}

// ---------------------------------------------------------

// when there are no longer any valid moves
// then the current game ends and player loses
bool AreThereValidMoves()
{
    // if some tile is empty, there are valid moves
    if( !IsBoardFull() )
      return true;
    
    // on a full board, if adjacent pairs exist,
    // it is a valid move to merge them
    
    // check for horizontal pairs
    for( int y = 0; y < 4; y++ )
      for( int x = 0; x < 3; x++ )
        if( CurrentBoard[y][x] == CurrentBoard[y][x+1] )
          return true;
    
    // check for vertical pairs
    for( int x = 0; x < 4; x++ )
      for( int y = 0; y < 3; y++ )
        if( CurrentBoard[y][x] == CurrentBoard[y+1][x] )
          return true;
    
    // no valid moves exist
    return false;
}

// ---------------------------------------------------------

// returns true if a new highest tile was just reached
bool UpdateHighestTileEver()
{
    int LastHighestTile = HighestTileEver;
    
    for( int y = 0; y < 4; y++ )
      for( int x = 0; x < 4; x++ )
        HighestTileEver = max( HighestTileEver, CurrentBoard[y][x] );
    
    return (HighestTileEver > LastHighestTile);
}

// ---------------------------------------------------------

// calculates points corresponding to all merged tiles in the last move
void DetermineScoreIncrease()
{
    LastScoreIncrease = 0;
    
    for( int y = 0; y < 4; y++ )
      for( int x = 0; x < 4; x++ )
        if( MergedTiles[y][x] > 0 )
          LastScoreIncrease += (1 << MergedTiles[y][x] );  // calculate powers of 2 as bit shifts
    
    // now, actually score those points
    Score += LastScoreIncrease;
}


// ---------------------------------------------------------
//   "CLASS" FOR TILE TRANSITIONS
// ---------------------------------------------------------


struct TileTransition
{
    int Value;                  // as power of 2; 0 = no tile
    int PositionX, PositionY;   // current coordinates, local to board
    int FinalX, FinalY;         // final coordinates, local to board
    int Opacity;                // 0 to 255
    bool IsMergeBottom;         // bottom part of the merge (will disappear)
    bool IsMergeTop;            // top part of the merge (will transform)
    bool IsMoving;              // set to false when position = final position
};

// we have a tile transition for every board position
TileTransition[ 4 ][ 4 ] TransitionBoard;

// ---------------------------------------------------------

// creates a transition based on a board position
void TileTransition_Initialize( int BoardX, int BoardY )
{
    TileTransition* Transition = &TransitionBoard[ BoardY ][ BoardX ];
    
    // tile value
    Transition->Value = CurrentBoard[ BoardY ][ BoardX ];
    
    // initial and final position
    Transition->FinalX = Transition->PositionX = 100 + 75 * BoardX;
    Transition->FinalY = Transition->PositionY = 67  + 75 * BoardY;
    
    // default behavior flags
    Transition->IsMergeBottom = false;
    Transition->IsMergeTop = false;
    Transition->IsMoving = false;
    
    // full opacity
    Transition->Opacity = 255;
}

// ---------------------------------------------------------

// determines the end of a horizontal transition
void TileTransition_SetFinalBoardX( TileTransition* Transition, int BoardX )
{
    Transition->FinalX = 100 + 75 * BoardX;
    Transition->IsMoving = (Transition->PositionX != Transition->FinalX);
}

// ---------------------------------------------------------

// determines the end of a vertical transition
void TileTransition_SetFinalBoardY( TileTransition* Transition, int BoardY )
{
    Transition->FinalY = 67 + 75 * BoardY;
    Transition->IsMoving = (Transition->PositionY != Transition->FinalY);
}

// ---------------------------------------------------------

// returns true if the current tile has just stopped
bool TileTransition_Simulate( TileTransition* Transition )
{
    if( Transition->Value <= 0 )
      return false;
    
    // merge bottom tiles will disappear
    if( Transition->IsMergeBottom )
      if( Transition->Opacity > 128 )
        Transition->Opacity -= 15;
    
    // move tile until it reaches its final position
    if( Transition->IsMoving )
    {
        Transition->PositionX += 25* MoveX;
        Transition->PositionY += 25* MoveY;
        
        if( Transition->PositionX == Transition->FinalX )
          if( Transition->PositionY == Transition->FinalY )
          {
              // stop current tile
              Transition->IsMoving = false;
              return true;
          }
    }
    
    return false;
}


// ---------------------------------------------------------
//   GAME LOGIC: MOVE RESOLUTION FUNCTIONS
// ---------------------------------------------------------


// These functions determine the next board state for a
// move in the specified direction. Also, they calculate
// the produced tile transitions in the whole board


void ExecuteMoveLeft()
{
    // every row is independent (movement is in 1D)
    for( int y = 0; y < 4; y++ )
    {
        // first, create a list with all active tiles in the row, in order
        int[ 4 ] PositionsInRow;
        memset( PositionsInRow, -1, sizeof(PositionsInRow) );  // -1 signals an empty tile
        int* PositionInRow = &PositionsInRow[ 0 ];
        
        // iterate from left to right
        for( int x = 0; x < 4; x++ )
          if( CurrentBoard[ y ][ x ] > 0 )
            *(PositionInRow++) = x;
        
        // we will move all tiles to the left, in order
        int NextX = 0;
        
        // iterate from left to right
        for( int i = 0; i < 4; i++ )
        {
            // CASE 1: empty tile
            // --> row has ended
            if( PositionsInRow[ i ] < 0 )
              break;
            
            // read current tile value
            int CurrentTileValue = CurrentBoard[ y ][ PositionsInRow[ i ] ];
            
            // current moving tile
            TileTransition* CurrentTransition = &TransitionBoard[ y ][ PositionsInRow[ i ] ];
            
            // CASE 2: last tile (not pairable)
            // --> move tile to the leftmost space
            if( i == 3 || PositionsInRow[ i+1 ] < 0 )
            {
                NextBoard[ y ][ NextX ] = CurrentTileValue;
                TileTransition_SetFinalBoardX( CurrentTransition, NextX );
                
                NextX++;
                continue;
            }
            
            // read next tile value
            int NextTileValue = CurrentBoard[ y ][ PositionsInRow[ i+1 ] ];
            
            // next moving tile
            TileTransition* NextTransition = &TransitionBoard[ y ][ PositionsInRow[ i+1 ] ];
            
            // CASE 3: a non matching pair
            // --> move tile to the leftmost space
            if( CurrentTileValue != NextTileValue )
            {
                NextBoard[ y ][ NextX ] = CurrentTileValue;
                TileTransition_SetFinalBoardX( CurrentTransition, NextX );
                
                NextX++;
            }
            
            // CASE 4: pair to be merged
            // --> merge both tiles into the next tile value
            else
            {
                // determine next board state
                NextBoard[ y ][ NextX ] = CurrentTileValue + 1;
                
                // prepare transitions
                TileTransition_SetFinalBoardX( CurrentTransition, NextX );
                CurrentTransition->IsMergeBottom = true;
                
                TileTransition_SetFinalBoardX( NextTransition, NextX );
                NextTransition->IsMergeTop = true;
                
                // signal any new merges
                MergedTiles[ y ][ NextX ] = CurrentTileValue + 1;
                
                // skip next tile, we already processed 2
                NextX++;
                i++;
            }
        }
    }
}

// ---------------------------------------------------------

void ExecuteMoveRight()
{
    // every row is independent (movement is in 1D)
    for( int y = 0; y < 4; y++ )
    {
        // first, create a list with all active tiles in the row, in order
        int[ 4 ] PositionsInRow;
        memset( PositionsInRow, -1, sizeof(PositionsInRow) );  // -1 signals an empty tile
        int* PositionInRow = &PositionsInRow[ 3 ];
        
        // iterate from right to left
        for( int x = 3; x >= 0; x-- )
          if( CurrentBoard[ y ][ x ] > 0 )
            *(PositionInRow--) = x;
        
        // we will move all tiles to the right, in order
        int NextX = 3;
        
        // iterate from right to left
        for( int i = 3; i >= 0; i-- )
        {
            // CASE 1: empty tile
            // --> row has ended
            if( PositionsInRow[ i ] < 0 )
              break;
            
            // read current tile value
            int CurrentTileValue = CurrentBoard[ y ][ PositionsInRow[ i ] ];
            
            // current moving tile
            TileTransition* CurrentTransition = &TransitionBoard[ y ][ PositionsInRow[ i ] ];
            
            // CASE 2: last tile (not pairable)
            // --> move tile to the rightmost space
            if( i == 0 || PositionsInRow[ i-1 ] < 0 )
            {
                NextBoard[ y ][ NextX ] = CurrentTileValue;
                TileTransition_SetFinalBoardX( CurrentTransition, NextX );
                
                NextX--;
                continue;
            }
            
            // read next tile value
            int NextTileValue = CurrentBoard[ y ][ PositionsInRow[ i-1 ] ];
            
            // next moving tile
            TileTransition* NextTransition = &TransitionBoard[ y ][ PositionsInRow[ i-1 ] ];
            
            // CASE 3: a non matching pair
            // --> move tile to the rightmost space
            if( CurrentTileValue != NextTileValue )
            {
                NextBoard[ y ][ NextX ] = CurrentTileValue;
                TileTransition_SetFinalBoardX( CurrentTransition, NextX );
                
                NextX--;
            }

            // CASE 4: pair to be merged
            // --> merge both tiles into the next tile value
            else
            {
                // determine next board state
                NextBoard[ y ][ NextX ] = CurrentTileValue + 1;
                
                // prepare transitions
                TileTransition_SetFinalBoardX( CurrentTransition, NextX );
                CurrentTransition->IsMergeBottom = true;
                
                TileTransition_SetFinalBoardX( NextTransition, NextX );
                NextTransition->IsMergeTop = true;
                
                // signal any new merges
                MergedTiles[ y ][ NextX ] = CurrentTileValue + 1;
                
                // skip next tile, we already processed 2
                NextX--;
                i--;
            }
        }
    }
}

// ---------------------------------------------------------

void ExecuteMoveUp()
{
    // every column is independent (movement is in 1D)
    for( int x = 0; x < 4; x++ )
    {
        // first, create a list with all active tiles in the column, in order
        int[ 4 ] PositionsInColumn;
        memset( PositionsInColumn, -1, sizeof(PositionsInColumn) );  // -1 signals an empty tile
        int* TileInColumn = &PositionsInColumn[ 0 ];
        
        // iterate from top to bottom
        for( int y = 0; y < 4; y++ )
          if( CurrentBoard[ y ][ x ] > 0 )
            *(TileInColumn++) = y;
        
        // we will move all tiles to the top, in order
        int NextY = 0;
        
        // iterate from top to bottom
        for( int i = 0; i < 4; i++ )
        {
            // CASE 1: empty tile
            // --> column has ended
            if( PositionsInColumn[ i ] < 0 )
              break;
            
            // read current tile value
            int CurrentTileValue = CurrentBoard[ PositionsInColumn[ i ] ][ x ];
            
            // current moving tile
            TileTransition* CurrentTransition = &TransitionBoard[ PositionsInColumn[ i ] ][ x ];
            
            // CASE 2: last tile (not pairable)
            // --> move tile to the topmost space
            if( i == 3 || PositionsInColumn[ i+1 ] < 0 )
            {
                NextBoard[ NextY ][ x ] = CurrentTileValue;
                TileTransition_SetFinalBoardY( CurrentTransition, NextY );
                
                NextY++;
                continue;
            }
            
            // read next tile value
            int NextTileValue = CurrentBoard[ PositionsInColumn[ i+1 ] ][ x ];
            
            // next moving tile
            TileTransition* NextTransition = &TransitionBoard[ PositionsInColumn[ i+1 ] ][ x ];
            
            // CASE 3: a non matching pair
            // --> move tile to the topmost space
            if( CurrentTileValue != NextTileValue )
            {
                NextBoard[ NextY ][ x ] = CurrentTileValue;
                TileTransition_SetFinalBoardY( CurrentTransition, NextY );
                
                NextY++;
            }
            
            // CASE 4: pair to be merged
            // --> merge both tiles into the next tile value
            else
            {
                // determine next board state
                NextBoard[ NextY ][ x ] = CurrentTileValue + 1;
                
                // prepare transitions
                TileTransition_SetFinalBoardY( CurrentTransition, NextY );
                CurrentTransition->IsMergeBottom = true;
                
                TileTransition_SetFinalBoardY( NextTransition, NextY );
                NextTransition->IsMergeTop = true;
                
                // signal any new merges
                MergedTiles[ NextY ][ x ] = CurrentTileValue + 1;
                
                // skip next tile, we already processed 2
                NextY++;
                i++;
            }
        }
    }
}

// ---------------------------------------------------------

void ExecuteMoveDown()
{
    // every column is independent (movement is in 1D)
    for( int x = 0; x < 4; x++ )
    {
        // first, create a list with all active tiles in the column, in order
        int[ 4 ] PositionsInColumn;
        memset( PositionsInColumn, -1, sizeof(PositionsInColumn) );  // -1 signals an empty tile
        int* PositionInColumn = &PositionsInColumn[ 3 ];
        
        // iterate from bottom to top
        for( int y = 3; y >= 0; y-- )
          if( CurrentBoard[ y ][ x ] > 0 )
            *(PositionInColumn--) = y;
        
        // we will move all tiles to the bottom, in order
        int NextY = 3;
        
        // iterate from bottom to top
        for( int i = 3; i >= 0; i-- )
        {
            // CASE 1: empty tile
            // --> column has ended
            if( PositionsInColumn[ i ] < 0 )
              break;
            
            // read current tile value
            int CurrentTileValue = CurrentBoard[ PositionsInColumn[ i ] ][ x ];
            
            // current moving tile
            TileTransition* CurrentTransition = &TransitionBoard[ PositionsInColumn[ i ] ][ x ];
            
            // CASE 2: last tile (not pairable)
            // --> move it to the bottommost space
            if( i == 0 || PositionsInColumn[ i-1 ] < 0 )
            {
                NextBoard[ NextY ][ x ] = CurrentTileValue;
                TileTransition_SetFinalBoardY( CurrentTransition, NextY );
                
                NextY--;
                continue;
            }
            
            // read next tile value
            int NextTileValue = CurrentBoard[ PositionsInColumn[ i-1 ] ][ x ];
            
            // next moving tile
            TileTransition* NextTransition = &TransitionBoard[ PositionsInColumn[ i-1 ] ][ x ];
            
            // CASE 3: a non matching pair
            // --> move tile to the bottommost space
            if( CurrentTileValue != NextTileValue )
            {
                NextBoard[ NextY ][ x ] = CurrentTileValue;
                TileTransition_SetFinalBoardY( CurrentTransition, NextY );
                
                NextY--;
            }
            
            // CASE 4: pair to be merged
            // --> merge both tiles into the next tile value
            else
            {
                // determine next board state
                NextBoard[ NextY ][ x ] = CurrentTileValue + 1;
                
                // prepare transitions
                TileTransition_SetFinalBoardY( CurrentTransition, NextY );
                CurrentTransition->IsMergeBottom = true;
                
                TileTransition_SetFinalBoardY( NextTransition, NextY );
                NextTransition->IsMergeTop = true;
                
                // signal any new merges
                MergedTiles[ NextY ][ x ] = CurrentTileValue + 1;
                
                // skip next tile, we already processed 2
                NextY--;
                i--;
            }
        }
    }
}

// ---------------------------------------------------------

// this is the general move function, that will delegate its
// work to the specific direction move and do general processing
void ExecuteMove()
{
    // first, clear the next board state
    memset( &NextBoard[0][0], 0, sizeof(NextBoard) );
    memset( &MergedTiles[0][0], 0, sizeof(MergedTiles) );
    
    // initialize all tile transitions
    for( int y = 0; y < 4; y++ )
      for( int x = 0; x < 4; x++ )
        TileTransition_Initialize( x, y );
    
    // depending on the move direction, iterate in
    // a different direction to determine next state
    if( MoveX < 0 )
      ExecuteMoveLeft();
    
    else if( MoveX > 0 )
      ExecuteMoveRight();
    
    else if( MoveY < 0 )
      ExecuteMoveUp();
    
    else // MoveY > 0
      ExecuteMoveDown();
    
    // now, compare the initial and final board
    // to see if the move actually moved any tile
    // (otherwise, it won't count as a move)
    LastMoveWasValid = false;
    int* CurrentTile = &CurrentBoard[ 0 ][ 0 ];
    int* NextTile = &NextBoard[ 0 ][ 0 ];
    
    for( int i = 0; i < 16; i++ )
      if( CurrentTile[ i ] != NextTile[ i ] )
        {
            LastMoveWasValid = true;
            break;
        }
    
    // increase move counter only when a move was valid
    // (if no tiles have moved, it is not considered a move)
    if( LastMoveWasValid )
      Moves++;
}
