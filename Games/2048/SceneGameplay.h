// ---------------------------------------------------------
//   GAMEPLAY: STATE MANAGEMENT
// ---------------------------------------------------------


int Gameplay_ElapsedFrames;

// ---------------------------------------------------------

void Gameplay_ChangeState( int NewState )
{
    int OldState = GameState;
    GameState = NewState;
    Gameplay_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( OldState == Gameplay_Initialize )
      play_sound_in_channel( MusicGameplay, ChannelMusic );
    
    else if( NewState == Gameplay_AddTile )
      play_sound( SoundTileAdded );
    
    else if( NewState == Gameplay_Waiting )
    {
        // check lose condition
        if( !AreThereValidMoves() )
        {
            GameScene = Scene_Lose;
            GameState = Dialog_Initialize;
        }
    }
    
    else if( NewState == Gameplay_NewHighest )
      play_sound( SoundNewHighest );
}


// ---------------------------------------------------------
//   GAMEPLAY: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Gameplay_RunState_Initialize()
{
    // create a new game
    ResetGameState();
    
    // transition immediately
    Gameplay_ChangeState( Gameplay_Waiting );
}

// ---------------------------------------------------------

void Gameplay_RunState_Waiting()
{
    // check for a player move
    bool PlayerMoved = false;
    
    if( gamepad_left() == 1 )
    {
        PlayerMoved = true;
        MoveX = -1;
        MoveY = 0;
    }
    
    else if( gamepad_right() == 1 )
    {
        PlayerMoved = true;
        MoveX = 1;
        MoveY = 0;
    }
    
    else if( gamepad_up() == 1 )
    {
        PlayerMoved = true;
        MoveX = 0;
        MoveY = -1;
    }
    
    else if( gamepad_down() == 1 )
    {
        PlayerMoved = true;
        MoveX = 0;
        MoveY = 1;
    }
    
    if( PlayerMoved )
    {
        ExecuteMove();
        play_sound( SoundMoveBoard );
        DirectionHighlightFrames = 15;
        
        if( LastMoveWasValid )
          Gameplay_ChangeState( Gameplay_Transition );
    }
    
    // check if game was aborted
    else if( gamepad_button_a() == 1 )
    {
        GameScene = Scene_QuitGame;
        GameState = Dialog_Initialize;
    }
    
    // draw scene
    DrawBackground();
    DrawGUI();
    DrawBoard();
}

// ---------------------------------------------------------

void Gameplay_RunState_Transition()
{
    // for all transitions, simulate and determine
    // if any of them have just finished moving
    bool TilesAreMoving = false;
    bool TilesJustStopped = false;
    
    for( int y = 0; y < 4; y++ )
      for( int x = 0; x < 4; x++ )
      {
          if( TileTransition_Simulate( &TransitionBoard[ y ][ x ] ) )
            TilesJustStopped = true;
          
          if( TransitionBoard[ y ][ x ].IsMoving )
            TilesAreMoving = true;
      }
    
    // play tile move sound only once per frame!
    if( TilesJustStopped )
      play_sound( SoundTileCollision );
    
    // end transition when all tiles finished moving
    if( !TilesAreMoving )
    {
        // apply the next board state
        memcpy( &CurrentBoard[0][0], &NextBoard[0][0], sizeof(CurrentBoard) );
        CreateNewTile();
        
        // new tiles that were merged need to be
        // highlighted and counted as points
        TileHighlightFrames = 10;
        DetermineScoreIncrease();
        
        if( LastScoreIncrease > 0 )
          ScoreHighlightFrames = 30;
        
        // update highest tile
        if( UpdateHighestTileEver() )
          Gameplay_ChangeState( Gameplay_NewHighest );
        else
          Gameplay_ChangeState( Gameplay_AddTile );
    }
    
    // draw scene
    DrawBackground();
    DrawGUI();
    DrawTransitionBoard();
}

// ---------------------------------------------------------

void Gameplay_RunState_AddTile()
{
    // when finished, transition and
    // actually create the new tile
    if( Gameplay_ElapsedFrames >= 6 )
    {
        CurrentBoard[ NextTileRow ][ NextTileColumn ] = NextTileValue;
        Gameplay_ChangeState( Gameplay_Waiting );
    }
    
    // draw scene
    DrawBackground();
    DrawGUI();
    DrawBoard();
    
    // new tile appears with a zoom effect
    float Scale = Gameplay_ElapsedFrames / 7.0;
    set_drawing_scale( Scale, Scale );
    
    // draw the new tile
    select_region( RegionTile2 - 1 + NextTileValue );
    draw_region_zoomed_at( 102 + 74*NextTileColumn, 69 + 74*NextTileRow );
}

// ---------------------------------------------------------

void Gameplay_RunState_NewHighest()
{
    // draw scene
    DrawBackground();
    DrawGUI();
    DrawBoard();
    
    // draw highlights on all highest tiles
    if( Gameplay_ElapsedFrames < 24 )
    {
        select_region( FirstRegionMerge + (Gameplay_ElapsedFrames/4) );
        
        for( int y = 0; y < 4; y++ )
          for( int x = 0; x < 4; x++ )
            if( CurrentBoard[ y ][ x ] == HighestTileEver )
              draw_region_at( 100 + 75*x, 67 + 75*y );
    }
    
    // when finished, transition
    // and check for win conditions
    if( Gameplay_ElapsedFrames >= 30 )
    {
        // check for tile 2048 = 2 ^ 11
        if( HighestTileEver == 11 )
        {
            GameScene = Scene_Win2048;
            GameState = Dialog_Initialize;
        }
        
        // check for tile 65536 = 2 ^ 16
        else if( HighestTileEver == 16 )
        {
            GameScene = Scene_Win65536;
            GameState = Dialog_Initialize;
        }            
        
        // otherwise continue normal gameplay
        else
          Gameplay_ChangeState( Gameplay_AddTile );
    }
}


// ---------------------------------------------------------
//   GAMEPLAY: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Gameplay_RunStateMachine()
{
    if( GameState == Gameplay_Initialize )
      Gameplay_RunState_Initialize();
    
    else if( GameState == Gameplay_Waiting )
      Gameplay_RunState_Waiting();
    
    else if( GameState == Gameplay_Transition )
      Gameplay_RunState_Transition();
    
    else if( GameState == Gameplay_AddTile )
      Gameplay_RunState_AddTile();
    
    else if( GameState == Gameplay_NewHighest )
      Gameplay_RunState_NewHighest();
    
    // count the frames in current state
    Gameplay_ElapsedFrames++;
    
    // decrease remaining frames for highlight effects
    DirectionHighlightFrames--;
    TileHighlightFrames--;
    ScoreHighlightFrames--;
    
    // keep count of the played time
    // (which includes all gameplay states)
    PlayedFrames++;
}
