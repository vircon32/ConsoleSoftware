// ---------------------------------------------------------
//   GAME: GAME LOGIC FUNCTIONS
// ---------------------------------------------------------


PieceTypes CreateRandomPiece()
{
    // we use the seed stored in the game 
    // session to ensure a continued sequence
    srand( Game.CurrentSeed );
    PieceTypes Piece = (PieceTypes)(1 + rand() % 7);
    
    // now save the next seed for next time
    Game.CurrentSeed = rand();
    return Piece;
}

// ---------------------------------------------------------

void ResetGame()
{
    // clear the board
    memset( Board, 0, sizeof( Board ) );
    
    // set initial state
    Game.State = Game_Falling;
    Game.ElapsedFrames = 0;
    
    // set initial score
    Game.CurrentLevel = 0;
    Game.ClearedLines = 0;
    Game.Score = 0;
    
    // reset all blink counters
    Game.LevelBlinkCounter = 0;
    Game.LinesBlinkCounter = 0;
    Game.ScoreBlinkCounter = 0;
    
    // generate and place current piece
    Game.CurrentSeed = get_time();
    Game.CurrentPiece = CreateRandomPiece();
    Game.PieceRotation = 0;
    Game.PieceX = 3;
    Game.PieceY = 1;
    
    // generate next piece
    Game.NextPiece = CreateRandomPiece();
}

// ---------------------------------------------------------

bool IsLegalPosition( PieceTypes PieceType, int PieceRotation, int PieceX, int PieceY )
{
    PieceRotation &= 3;
    PieceShape* Piece = &(*PieceModels[ PieceType ])[ PieceRotation ];
    
    for( int y = 0; y < 4; y++ )
    {
        int BoardY = PieceY + y;
        
        for( int x = 0; x < 4; x++ )
        {
            if( !(*Piece)[ y ][ x ] )
              continue;
            
            int BoardX = PieceX + x;
            
            // check if X is beyond board limits
            if( BoardX < 0 ) return false;
            if( BoardX >= BoardWidth ) return false;
            
            // check if Y is beyond board limits
            if( BoardY < 0 ) return false;
            if( BoardY >= BoardHeight ) return false;
            
            // check if it collides with board tiles
            if( Board[ BoardY ][ BoardX ] ) return false;
        }
    }
    
    // if we reach here, no problems were found
    return true;
}

// ---------------------------------------------------------

// returns the highest row that contains
// at least 1 cell (so, gives us the current
// top height of the board);
// for an empty board it returns board height
int GetHighestNonEmptyRow()
{
    for( int Row = 0; Row < BoardHeight; Row++ )
      for( int x = 0; x < BoardWidth; x++ )
        if( Board[ Row ][ x ] )
          return Row;
    
    return BoardHeight;
}

// ---------------------------------------------------------

bool IsRowFull( int Row )
{
    for( int x = 0; x < BoardWidth; x++ )
      if( !Board[ Row ][ x ] )
        return false;
    
    return true;
}

// ---------------------------------------------------------

bool IsRowEmpty( int Row )
{
    for( int x = 0; x < BoardWidth; x++ )
      if( Board[ Row ][ x ] )
        return false;
    
    return true;
}

// ---------------------------------------------------------

// returns the number of full rows
int CountFullRows()
{
    int FullRows = 0;
    
    for( int Row = 0; Row < BoardHeight; Row++ )
      if( IsRowFull( Row ) )
        FullRows++;
    
    return FullRows;
}

// ---------------------------------------------------------

void CopyBoardForLineHighlight()
{
    // clear it first
    memset( &BoardCopy[ 0 ][ 0 ], 0, sizeof( BoardCopy ) );
    
    for( int Row = 0; Row < BoardHeight; Row++ )
      if( !IsRowFull( Row ) )
        memcpy( BoardCopy[ Row ], Board[ Row ], BoardWidth );
}

// ---------------------------------------------------------

void CopyBoardForLineClear()
{
    // first, clear the copy board
    memset( &BoardCopy[ 0 ][ 0 ], 0, sizeof( BoardCopy ) );
    
    // copy line by line, excluding full lines
    int CopiedRow = BoardHeight - 1;
    
    for( int SourceRow = BoardHeight - 1; SourceRow >= 0; SourceRow-- )
    {
        if( CopiedRow < 0 )
          break;
        
        if( IsRowFull( SourceRow ) )
          continue;
        
        memcpy( BoardCopy[ CopiedRow ], Board[ SourceRow ], BoardWidth );
        CopiedRow--;
    }
}

// ---------------------------------------------------------

bool DetermineRowFallDistances()
{
    int CurrentFallDistance = 0;
    bool ThereAreFalls = false;
    
    for( int Row = BoardHeight - 1; Row >= 0; Row-- )
    {
        if( IsRowEmpty( Row ) )
          RowFallDistances[ Row ] = CurrentFallDistance;
        
        else if( IsRowFull( Row ) )
        {
            RowFallDistances[ Row ] = CurrentFallDistance;
            CurrentFallDistance++;
        }
        
        // only partially full rows can fall
        else
        {
            RowFallDistances[ Row ] = CurrentFallDistance;
            
            if( CurrentFallDistance > 0 )
              ThereAreFalls = true;
        }
    }
    
    return ThereAreFalls;
}


// ---------------------------------------------------------
//   GAME: SCORE MANAGEMENT
// ---------------------------------------------------------


void Game_AwardPiece()
{
    Game.Score += (Game.CurrentLevel + 1) * LineClearBaseScores[ 0 ];
    Game.ScoreBlinkCounter = 8;
}

// ---------------------------------------------------------

void Game_AwardLines( int ClearedLines )
{
    // should never happen, but just in case
    if( ClearedLines > 4 )
      ClearedLines = 4;
    
    Game.ClearedLines += ClearedLines;
    Game.LevelLines += ClearedLines;
    Game.Score += (Game.CurrentLevel + 1) * LineClearBaseScores[ ClearedLines ];
    
    Game.ScoreBlinkCounter = 8 * ClearedLines;
    Game.LinesBlinkCounter = 16;
    
    // play the sound depending on lines cleared
    play_sound( SoundClear1Line + ClearedLines - 1 );
}

// ---------------------------------------------------------

void Game_IncreaseLevel()
{
    // 29 is the maximum level
    if( Game.CurrentLevel >= 29 )
      return;
    
    Game.CurrentLevel += 1;
    Game.LevelLines = 0;
    Game.LevelBlinkCounter = 32;
    
    // play the sound
    play_sound( SoundLevelAdvance );
}


// ---------------------------------------------------------
//   GAME: STATE MANAGEMENT
// ---------------------------------------------------------


void Game_ChangeState( GameStates NewState )
{
    Game.State = NewState;
    Game.ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( NewState == Game_Locking )
    {
        play_sound( SoundPieceLock );
        Game_AwardPiece();
    }
    
    if( NewState == Game_LinesHighlight )
    {
        Game_AwardLines( CountFullRows() );
    }
    
    if( NewState == Game_Falling )
    {
        // check if level has to increase
        if( Game.LevelLines >= 10 )
          Game_IncreaseLevel();
    }
    
    if( NewState == Game_LoseHighlight )
    {
        // stop music
        stop_channel( ChannelMusic );
        
        // turn all board gray
        for( int y = 0; y < BoardHeight; y++ )
          for( int x = 0; x < BoardWidth; x++ )
            Board[ y ][ x ] &= 15;
    }
    
    if( NewState == Game_LinesFall )
    {
        play_sound( SoundBoardFall );
    }
}


// ---------------------------------------------------------
//   GAME: DRAWING FUNCTIONS
// ---------------------------------------------------------


void Game_DrawCurrentPiece( bool Highlight )
{
    if( Highlight )
      set_blending_mode( blending_add );
    
    PieceShape* Piece = &(*PieceModels[ Game.CurrentPiece ])[ Game.PieceRotation & 3 ];
    
    for( int y = 0; y < 4; y++ )
      for( int x = 0; x < 4; x++ )
        if( (*Piece)[ y ][ x ] )
        {
            // don't draw above visible board area
            if( Game.PieceY + y < 2 )
              break;
            
            // for highlight use gray tiles
            // (has better contrast with all colors)
            int Tile = (*Piece)[ y ][ x ];
            
            if( Highlight )
              select_region( FirstRegionPieceTiles + (Tile & 15) );
            else
              select_region( FirstRegionPieceTiles + Tile );
            
            draw_region_at( 240 + 16 * (Game.PieceX + x), -10 + 16 * (Game.PieceY + y - 2) );
        }
    
    set_blending_mode( blending_alpha );
}

// ---------------------------------------------------------

void Game_DrawBoard()
{
    switch( Game.State )
    {
        case Game_Falling:
        {
            // draw board
            tilemap_draw( &MapBoard );
            
            // draw current piece
            Game_DrawCurrentPiece( false );
            break;
        }
        
        case Game_Locking:
        {
            // draw board
            tilemap_draw( &MapBoard );
            
            // draw current piece and highlight it
            Game_DrawCurrentPiece( false );
            Game_DrawCurrentPiece( true );
            break;
        }
        
        case Game_LinesHighlight:
        {
            // make all cleared lines blink
            if( (Game.ElapsedFrames & 8) == 0 )
              tilemap_draw( &MapBoard );
            else
              tilemap_draw( &MapBoardCopy );
            
            break;
        }
        
        case Game_LinesFall:
        {
            // animate the fall per line
            float Proportion = Game.ElapsedFrames / 6.0;
            
            for( int y = 2; y < BoardHeight; y++ )
            {
                int DrawY = -10 + 16 * (y - 2 + Proportion * RowFallDistances[ y ]);
                
                for( int x = 0; x < BoardWidth; x++ )
                {
                    select_region( FirstRegionPieceTiles + Board[ y ][ x ] );
                    draw_region_at( 240 + 16 * x, DrawY );
                }
            }
            break;
        }
        
        case Game_LoseHighlight:
        {
            // make the whole board blink
            if( (Game.ElapsedFrames & 8) == 0 )
              tilemap_draw( &MapBoard );
            
            break;
        }
        
        case Game_Ended:
        {
            tilemap_draw( &MapBoard );
            break;
        }
    }
}


// ---------------------------------------------------------
//   GAME: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Game_RunState_Falling()
{
    // process piece movement
    if( gamepad_left() > 0 )
    {
        bool MoveTrigger = false;
        
        if( gamepad_left() == 1 )
          MoveTrigger = true;
        
        if( gamepad_left() >= DASInitialDelay )
          if( (gamepad_left() - DASInitialDelay) % DASRepeatSpeed == 0 )
            MoveTrigger = true;
        
        if( MoveTrigger )
        {
            if( IsLegalPosition( Game.CurrentPiece, Game.PieceRotation, Game.PieceX - 1, Game.PieceY ) )
              Game.PieceX--;
            
            play_sound( SoundPieceMove );
        }
    }
    
    else if( gamepad_right() > 0 )
    {
        bool MoveTrigger = false;
        
        if( gamepad_right() == 1 )
          MoveTrigger = true;
        
        if( gamepad_right() >= DASInitialDelay )
          if( (gamepad_right() - DASInitialDelay) % DASRepeatSpeed == 0 )
            MoveTrigger = true;
        
        if( MoveTrigger )
        {
            if( IsLegalPosition( Game.CurrentPiece, Game.PieceRotation, Game.PieceX + 1, Game.PieceY ) )
              Game.PieceX++;
            
            play_sound( SoundPieceMove );
        }
    }
    
    // process piece rotation
    if( gamepad_button_b() == 1 )
    {
        play_sound( SoundPieceRotate );
        int TriedRotation = (Game.PieceRotation - 1) & 3;
        
        if( IsLegalPosition( Game.CurrentPiece, TriedRotation, Game.PieceX, Game.PieceY ) )
          Game.PieceRotation = TriedRotation;
    }
    
    else if( gamepad_button_a() == 1 )
    {
        play_sound( SoundPieceRotate );
        int TriedRotation = (Game.PieceRotation + 1) & 3;
        
        if( IsLegalPosition( Game.CurrentPiece, TriedRotation, Game.PieceX, Game.PieceY ) )
          Game.PieceRotation = TriedRotation;
    }
    
    // process piece drop
    int ApplicableGravity = LevelGravitySpeeds[ Game.CurrentLevel ];
    
    if( gamepad_down() > 0 )
    {
        ApplicableGravity = 2;
        
        if( gamepad_down() == 1 )
          play_sound( SoundPieceDrop );
    }
    
    // process piece fall
    if( Game.ElapsedFrames % ApplicableGravity == 0 )
    {
        // pieces keep falling while they can
        if( IsLegalPosition( Game.CurrentPiece, Game.PieceRotation, Game.PieceX, Game.PieceY + 1 ) )
          Game.PieceY++;
        
        // when they hit something the piece is locked
        else Game_ChangeState( Game_Locking );
    }
}

// ---------------------------------------------------------

void Game_RunState_Locking()
{
    if( Game.ElapsedFrames > 10 )
    {
        // add current piece to the board
        PieceShape* Piece = &(*PieceModels[ Game.CurrentPiece ])[ Game.PieceRotation & 3 ];
        
        for( int y = 0; y < 4; y++ )
          for( int x = 0; x < 4; x++ )
            if( (*Piece)[ y ][ x ] )
              Board[ Game.PieceY + y ][ Game.PieceX + x ] = (*Piece)[ y ][ x ];
        
        // make the next piece current
        Game.CurrentPiece = Game.NextPiece;
        
        // place it on start position
        Game.PieceX = 3;
        Game.PieceY = 1;
        Game.PieceRotation = 0;
        
        // generate a new next piece
        Game.NextPiece = CreateRandomPiece();
        
        // check if some lines are cleared
        if( CountFullRows() > 0 )
        {
            // transition to lines highlight
            CopyBoardForLineHighlight();
            Game_ChangeState( Game_LinesHighlight );
        }
        
        // check if we lost the game
        else if( GetHighestNonEmptyRow() < 4 )
          Game_ChangeState( Game_LoseHighlight );
        
        // otherwise go back to falling
        else Game_ChangeState( Game_Falling );
    }
}

// ---------------------------------------------------------

void Game_RunState_LinesHighlight()
{
    if( Game.ElapsedFrames > 30 )
    {
        // add top borders to lines below cleared lines
        for( int Row = 1; Row < BoardHeight; Row++ )
        {
            if( IsRowFull( Row ) )
              continue;
            
            if( IsRowFull( Row - 1 ) )
            {
                for( int x = 0; x < BoardWidth; x++ )
                  if( Board[ Row ][ x ] )
                    Board[ Row ][ x ] |= 4;
            }
        }
        
        // add bottom borders to lines above cleared lines
        for( int Row = 0; Row < (BoardHeight-1); Row++ )
        {
            if( IsRowFull( Row ) )
              continue;
            
            if( IsRowFull( Row + 1 ) )
            {
                for( int x = 0; x < BoardWidth; x++ )
                  if( Board[ Row ][ x ] )
                    Board[ Row ][ x ] |= 8;
            }
        }
        
        // determine if any lines will fall, and how much
        bool LinesFall = DetermineRowFallDistances();
        
        // create a copy of the board with those
        // lines already fallen and cleared lines excluded
        CopyBoardForLineClear();
        
        // now we can actually remove cleared lines
        for( int Row = 0; Row < BoardHeight; Row++ )
        {
            if( IsRowFull( Row ) )
              for( int x = 0; x < BoardWidth; x++ )
                Board[ Row ][ x ] = 0;
        }
        
        // if there are lines that should fall, animate them
        if( LinesFall )
          Game_ChangeState( Game_LinesFall );
        
        // otherwise continue with the game
        else
        {
            // first check if we lost the game
            if( GetHighestNonEmptyRow() < 4 )
              Game_ChangeState( Game_LoseHighlight );
            
            // otherwise continue with next piece
            else Game_ChangeState( Game_Falling );
        }
    }
}

// ---------------------------------------------------------

void Game_RunState_LinesFall()
{
    // after the animation, continue with the game
    if( Game.ElapsedFrames >= 6 )
    {
        // first make lines actually fall
        memcpy( &Board[ 0 ][ 0 ], &BoardCopy[ 0 ][ 0 ], sizeof( Board ) );
        
        // now check if we lost the game
        if( GetHighestNonEmptyRow() < 4 )
          Game_ChangeState( Game_LoseHighlight );
        
        // otherwise continue with next piece
        else Game_ChangeState( Game_Falling );
    }
}

// ---------------------------------------------------------

void Game_RunState_LoseHighlight()
{
    // just let the game end after some frames
    if( Game.ElapsedFrames > 60 )
      Game_ChangeState( Game_Ended );
}


// ---------------------------------------------------------
//   GAME: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Game_RunStateMachine()
{
    switch( Game.State )
    {
        case Game_Falling:
          Game_RunState_Falling();
          break;
        case Game_Locking:
          Game_RunState_Locking();
          break;
        case Game_LinesHighlight:
          Game_RunState_LinesHighlight();
          break;
        case Game_LinesFall:
          Game_RunState_LinesFall();
          break;
        case Game_LoseHighlight:
          Game_RunState_LoseHighlight();
          break;
        default:
          break;
    }
    
    // count the frames in current state
    Game.ElapsedFrames++;
    
    // auto-decrease all blinking counters
    if( Game.LevelBlinkCounter > 0 )  Game.LevelBlinkCounter--;
    if( Game.LinesBlinkCounter > 0 )  Game.LinesBlinkCounter--;
    if( Game.ScoreBlinkCounter > 0 )  Game.ScoreBlinkCounter--;
}
