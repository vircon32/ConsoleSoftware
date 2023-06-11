// ---------------------------------------------------------
//   GAMEPLAY: STATE MANAGEMENT
// ---------------------------------------------------------


int Gameplay_ElapsedFrames;

// this is used to rotate the hint
// shown along all available pairs
int Gameplay_NumberOfHintsUsed;

// for dialog messages
AlertTitles Gameplay_AlertTitle;
AlertMessages Gameplay_AlertMessage;

// ---------------------------------------------------------

void Gameplay_ChangeState( int NewState )
{
    GameState = NewState;
    Gameplay_ElapsedFrames = 0;
    
    // at any state change, always update shadows and selection
    DetermineTileShadows();
    UpdatePointedTile();
    
    // some transitions have specific actions
    if( NewState == Gameplay_MovingCursor )
    {
        ClearActions();
        
        // re-check actions every time we go back to
        // moving (only state where actions are allowed)
        bool FirstTileIsPicked = (PickedTileIndex[ 0 ] >= 0);
        ActionsAllowed[ Action_PickTile ] = !FirstTileIsPicked;
        ActionsAllowed[ Action_MakePair ] = FirstTileIsPicked;
        ActionsAllowed[ Action_UndoPick ] = FirstTileIsPicked;
        ActionsAllowed[ Action_ShowHint ] = !FirstTileIsPicked;
        ActionsAllowed[ Action_QuitGame ] = true;
        
        // these actions also depend on the state of the undo stack
        bool UndoStackUsable = (!FirstTileIsPicked && UndoStackSize > 0);
        ActionsAllowed[ Action_UndoLast ] = (UndoStackUsable && UndoStackPosition >= 0);
        ActionsAllowed[ Action_RedoNext ] = (UndoStackUsable && UndoStackPosition < (UndoStackSize-1));
    }
    
    else if( NewState == Gameplay_ShowAlert )
      play_sound( SoundWrongMove );
    
    else if( NewState == Gameplay_Picking )
      play_sound( SoundSelectTile );
    
    else if( NewState == Gameplay_Unpicking )
      play_sound( SoundUndoSelect );
    
    else if( NewState == Gameplay_Undoing )
      play_sound( SoundUnmatchTiles );
    
    else if( NewState == Gameplay_Redoing )
      play_sound( SoundMatchTiles );
    
    else if( NewState == Gameplay_Pairing )
      play_sound( SoundMatchTiles );
}


// ---------------------------------------------------------
//   GAMEPLAY: DRAWING FUNCTIONS
// ---------------------------------------------------------


void Gameplay_DrawScene()
{
    // draw background
    select_region( RegionTableMat );
    draw_region_at( 0, 0 );
    
    // draw GUI with allowed actions
    // (left for each scene to configure)
    DrawGUI();
    DrawAllowedActions();
}


// ---------------------------------------------------------
//   GAMEPLAY: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Gameplay_RunState_Initialize()
{
    // initialize the board
    FindLayoutTiles();
    ShuffleLayoutTiles();
    MakeLayoutSolvable();
    
    // initialize game session
    ResetUndoStack();
    ClearActions();
    Gameplay_NumberOfHintsUsed = 0;
    
    // clear selection and picks
    PointedTileIndex = -1;
    PickedTileIndex[ 0 ] = -1;
    PickedTileIndex[ 1 ] = -1;
    
    // place cursor at center of play area
    CursorX = 375;
    CursorY = 162;
    
    // transition immediately
    Gameplay_ChangeState( Gameplay_FadeIn );
}

// ---------------------------------------------------------

void Gameplay_RunState_FadeIn()
{
    // draw scene
    Gameplay_DrawScene();
    
    // fade-in effect
    int BlackLevel = max( 0, 255 - 255 * Gameplay_ElapsedFrames / 40 );
    clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    
    // change state when finished
    if( Gameplay_ElapsedFrames >= 40 )
      Gameplay_ChangeState( Gameplay_PlacingLayers );   
}

// ---------------------------------------------------------

void Gameplay_RunState_PlacingLayers()
{
    // draw scene
    Gameplay_DrawScene();
    
    // keep stacking layers
    int LastLayer = Gameplay_ElapsedFrames / 25;
    int LayerFrames = Gameplay_ElapsedFrames % 25;
    
    for( int z = 0; z <= LastLayer; ++z )
    {
        // make the last layer fade in
        if( z == LastLayer )
        {
            int Opacity = min( 255, 255 * LayerFrames / 10 );
            set_multiply_color( make_color_rgba( 255,255,255,Opacity ) );
        }
        
        DrawLayerShadows( z );
        DrawLayerEdges( z );
        DrawLayerTiles( z );
        
        set_multiply_color( color_white );
    }
    
    // play higher notes with each
    if( LayerFrames == 1 )
    {
        // work on an identifiable channel so we can alter it
        play_sound_in_channel( SoundHighlight, ChannelMusic - 1 );
        set_channel_speed( pow( 1.06, 2 * LastLayer ) );
    }
    
    // change state when finished
    if( (LayerFrames == 0) && IsLayerEmpty( LastLayer ) )
    {
        // restore used channel to normal speed
        select_channel( ChannelMusic - 1 );
        set_channel_speed( 1.0 );
        
        // play gameplay music
        play_sound_in_channel( MusicGameplay, ChannelMusic );
        
        Gameplay_ChangeState( Gameplay_MovingCursor );   
    }
}

// ---------------------------------------------------------

void Gameplay_RunState_MovingCursor()
{
    // draw scene
    DrawnTileIndex[ 0 ] = PointedTileIndex;
    DrawnTileIndex[ 1 ] = PickedTileIndex[ 0 ];
    Gameplay_DrawScene();
    DrawBoard( DrawSelection, Gameplay_ElapsedFrames );
    
    // draw cursor animation
    DrawCursor( (Gameplay_ElapsedFrames / 8) % 2 );
    
    // obtain the current movement direction
    int CursorDeltaX = 0;
    int CursorDeltaY = 0;
    gamepad_direction( &CursorDeltaX, &CursorDeltaY );
    
    // dampen diagonal speed
    int Speed = 3.0;
    
    if( CursorDeltaX && CursorDeltaY )
      Speed = 2.0;
    
    // move cursor and limit it to gameplay area
    CursorX += Speed * CursorDeltaX;
    CursorY += Speed * CursorDeltaY;
    
    if( CursorX < 115 ) CursorX = 115;
    if( CursorX > screen_width - 17 ) CursorX = screen_width - 17;
    
    if( CursorY < 17 ) CursorY = 17;
    if( CursorY > screen_height - 17 ) CursorY = screen_height - 17;
    
    // update selection after position change
    UpdatePointedTile();
    
    // button A may either pick or pair
    if( gamepad_button_a() == 1 )
    {   
        // case 1: attempt to pick a first tile
        if( PickedTileIndex[ 0 ] < 0 )
        {
            // check that there is a tile to pick
            if( PointedTileIndex < 0 )
            {
                Gameplay_AlertTitle = AlertTitle_WrongMove;
                Gameplay_AlertMessage = AlertMessage_NoTile;
                Gameplay_ChangeState( Gameplay_ShowAlert );
            }
            
            // check that the tile is selectable
            else if( !IsTileFree( &LayoutTiles[ PointedTileIndex ] ) )
            {
                Gameplay_AlertTitle = AlertTitle_WrongMove;
                Gameplay_AlertMessage = AlertMessage_CannotPick;
                Gameplay_ChangeState( Gameplay_ShowAlert );
            }
                
            // now we can safely pick the pair's first tile
            else
            {
                PickedTileIndex[ 0 ] = PointedTileIndex;
                Gameplay_ChangeState( Gameplay_Picking );
            }
        }
        
        // case 2: attempt to match previous picked tile
        else
        {
            // check that there is a tile to pick
            if( PointedTileIndex < 0 )
            {
                Gameplay_AlertTitle = AlertTitle_WrongMove;
                Gameplay_AlertMessage = AlertMessage_NoTile;
                Gameplay_ChangeState( Gameplay_ShowAlert );
            }
            
            // check that the tile is selectable
            else if( !IsTileFree( &LayoutTiles[ PointedTileIndex ] ) )
            {
                Gameplay_AlertTitle = AlertTitle_WrongMove;
                Gameplay_AlertMessage = AlertMessage_CannotPick;
                Gameplay_ChangeState( Gameplay_ShowAlert );
            }
            
            // do not allow to pick the same tile twice
            else if( PointedTileIndex == PickedTileIndex[ 0 ] )
              play_sound( SoundMenuCancel );
            
            // check that the tiles do match
            else if( !DoTilesMatch( LayoutTiles[ PickedTileIndex[ 0 ] ].ID, LayoutTiles[ PointedTileIndex ].ID ) )
            {
                Gameplay_AlertTitle = AlertTitle_WrongMove;
                Gameplay_AlertMessage = AlertMessage_CannotPair;
                Gameplay_ChangeState( Gameplay_ShowAlert );
            }
            
            // now we can safely pair both tiles
            else
            {
                // pick second tile
                PickedTileIndex[ 1 ] = PointedTileIndex;
                
                // add pair to the undo stack
                AddUndoPair
                (
                    PickedTileIndex[ 0 ],
                    PickedTileIndex[ 1 ],
                    LayoutTiles[ PickedTileIndex[ 0 ] ].ID,
                    LayoutTiles[ PickedTileIndex[ 1 ] ].ID
                );
                
                // remove the pair before the animation
                SetLayoutTile( PickedTileIndex[ 0 ], 0 );
                SetLayoutTile( PickedTileIndex[ 1 ], 0 );
                Gameplay_ChangeState( Gameplay_Pairing );
            }
        }
        
        return;
    }
    
    // if a first pair tile is picked, button B will unpick it
    if( gamepad_button_b() == 1 && ActionsAllowed[ Action_UndoPick ] )
    {
        Gameplay_ChangeState( Gameplay_Unpicking );
        return;
    }
    
    // otherwise button B will undo the last match
    if( gamepad_button_b() == 1 && ActionsAllowed[ Action_UndoLast ] )
    {
        Gameplay_ChangeState( Gameplay_Undoing );
        return;
    }
    
    // button Y will redo the next match in the stack
    if( gamepad_button_y() == 1 && ActionsAllowed[ Action_RedoNext ] )
    {
        RedoNextPair();
        Gameplay_ChangeState( Gameplay_Redoing );
        return;
    }
    
    // button X will show a hint
    if( gamepad_button_x() == 1 && ActionsAllowed[ Action_ShowHint ] )
    {
        FindAllFreePairs();
        
        if( AllFreePairs.NumberOfPairs > 0 )
        {
            Gameplay_NumberOfHintsUsed++;
            Gameplay_ChangeState( Gameplay_ShowingHint );  
        }
        
        else
        {
            Gameplay_AlertTitle = AlertTitle_NoPairs;
            Gameplay_AlertMessage = AlertMessage_UndoOrRestart;
            Gameplay_ChangeState( Gameplay_ShowAlert );
        }
        
        return;
    }
    
    // button Start will show the quit game dialog
    if( gamepad_button_start() == 1 )
    {
        GameScene = Scene_QuitGame;
        GameState = QuitGame_Initialize;
        return;
    }
}

// ---------------------------------------------------------

void Gameplay_RunState_Picking()
{
    // draw scene
    Gameplay_DrawScene();
    DrawBoard( DrawBoardOnly, 0 );
    
    // change state when finished
    if( Gameplay_ElapsedFrames >= 3 )
      Gameplay_ChangeState( Gameplay_MovingCursor );       
}

// ---------------------------------------------------------

void Gameplay_RunState_Pairing()
{
    // draw scene
    DrawnTileIndex[ 0 ] = PickedTileIndex[ 0 ];
    DrawnTileIndex[ 1 ] = PickedTileIndex[ 1 ];
    Gameplay_DrawScene();
    DrawBoard( DrawPairing, Gameplay_ElapsedFrames );
    
    // change state when finished
    if( Gameplay_ElapsedFrames >= 30 )
    {
        // remove both tiles
        PickedTileIndex[ 0 ] = PickedTileIndex[ 1 ] = -1;
        
        // check if the game was won
        if( GameHasEnded() )
        {
            stop_channel( ChannelMusic );
            GameScene = Scene_Win;
            GameState = Win_Initialize;
        }
        
        // otherwise go back
        else Gameplay_ChangeState( Gameplay_MovingCursor );       
    }
}

// ---------------------------------------------------------

void Gameplay_RunState_Unpicking()
{
    // draw scene
    Gameplay_DrawScene();
    DrawBoard( DrawBoardOnly, 0 );
    
    // change state when finished
    if( Gameplay_ElapsedFrames >= 3 )
    {
        PickedTileIndex[ 0 ] = -1;
        Gameplay_ChangeState( Gameplay_MovingCursor );
    }
}

// ---------------------------------------------------------

void Gameplay_RunState_Undoing()
{
    // draw scene
    DrawnTileIndex[ 0 ] = UndoStack[ UndoStackPosition ].Index1;
    DrawnTileIndex[ 1 ] = UndoStack[ UndoStackPosition ].Index2;
    Gameplay_DrawScene();
    DrawBoard( DrawPairing, 30 - Gameplay_ElapsedFrames );
    
    // change state when finished
    if( Gameplay_ElapsedFrames >= 30 )
    {
        UndoLastPair();
        Gameplay_ChangeState( Gameplay_MovingCursor );
    }
}

// ---------------------------------------------------------

void Gameplay_RunState_Redoing()
{
    // draw scene
    DrawnTileIndex[ 0 ] = UndoStack[ UndoStackPosition ].Index1;
    DrawnTileIndex[ 1 ] = UndoStack[ UndoStackPosition ].Index2;
    Gameplay_DrawScene();
    DrawBoard( DrawPairing, Gameplay_ElapsedFrames );
    
    // change state when finished
    if( Gameplay_ElapsedFrames >= 30 )
      Gameplay_ChangeState( Gameplay_MovingCursor );
}

// ---------------------------------------------------------

void Gameplay_RunState_ShowingHint()
{
    
    // decide which pair to use
    FreePair* HintPair = &AllFreePairs.Pairs[ 0 ];
    
    if( AllFreePairs.NumberOfPairs > 1 )
      HintPair = &AllFreePairs.Pairs[ Gameplay_NumberOfHintsUsed % AllFreePairs.NumberOfPairs ];
    
    // draw
    DrawnTileIndex[ 0 ] = HintPair->Index1;
    DrawnTileIndex[ 1 ] = HintPair->Index2;
    Gameplay_DrawScene();
    DrawBoard( DrawHint, Gameplay_ElapsedFrames );
    
    // play sound
    if( ((Gameplay_ElapsedFrames - 10) % 20) == 0 )
      play_sound( SoundHighlight );
    
    // change state when finished
    if( Gameplay_ElapsedFrames >= 60 )
      Gameplay_ChangeState( Gameplay_MovingCursor );       
}

// ---------------------------------------------------------

void Gameplay_RunState_ShowAlert()
{
    // darken the scene
    Gameplay_DrawScene();
    DrawBoard( DrawBoardOnly, 0 );
    clear_screen( make_color_rgba( 0,0,0,128 ) );
    
    // show the correct dialog
    select_region( FirstRegionAlertTitles + Gameplay_AlertTitle );
    draw_region_at( 370, screen_height/2 );
    
    select_region( FirstRegionAlertMessages + Gameplay_AlertMessage );
    draw_region();
    
    // wait for user to press a button
    if( gamepad_button_a() == 1 || gamepad_button_b() == 1 )
      if( Gameplay_ElapsedFrames >= 20 )
      {
          play_sound( SoundMenuCancel );
          Gameplay_ChangeState( Gameplay_MovingCursor );
      }
}


// ---------------------------------------------------------
//   GAMEPLAY: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Gameplay_RunStateMachine()
{
    switch( GameState )
    {
        case Gameplay_Initialize:
          Gameplay_RunState_Initialize();
          break;
        case Gameplay_FadeIn:
          Gameplay_RunState_FadeIn();
          break;
        case Gameplay_PlacingLayers:
          Gameplay_RunState_PlacingLayers();
          break;
        case Gameplay_MovingCursor:
          Gameplay_RunState_MovingCursor();
          break;
        case Gameplay_Picking:
          Gameplay_RunState_Picking();
          break;
        case Gameplay_Pairing:
          Gameplay_RunState_Pairing();
          break;
        case Gameplay_Unpicking:
          Gameplay_RunState_Unpicking();
          break;
        case Gameplay_Undoing:
          Gameplay_RunState_Undoing();
          break;
        case Gameplay_Redoing:
          Gameplay_RunState_Redoing();
          break;
        case Gameplay_ShowingHint:
          Gameplay_RunState_ShowingHint();
          break;
        case Gameplay_ShowAlert:
          Gameplay_RunState_ShowAlert();
          break;
    }
    
    // count the frames in current state
    Gameplay_ElapsedFrames++;
}
