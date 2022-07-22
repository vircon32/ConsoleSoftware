// ---------------------------------------------------------
//   GAME MEDIUM: STATE MANAGEMENT
// ---------------------------------------------------------


int GameMedium_ElapsedFrames;

// stores movement direction during transition
int GameMedium_DeltaX;
int GameMedium_DeltaY;

// this keeps track of current reveal propatation steps
int GameMedium_CurrentRevealStep;
int GameMedium_MaxRevealStep;

// ---------------------------------------------------------

void GameMedium_ChangeState( int NewState )
{
    GameState = NewState;
    GameMedium_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( NewState == Gameplay_Moving )
      play_sound( SoundMenuMove );
  
    else if( NewState == Gameplay_RevealTile )
      play_sound( SoundReveal );
  
    else if( NewState == Gameplay_PropagateReveal )
      play_sound( SoundReveal );
  
    else if( NewState == Gameplay_AddingFlag )
      play_sound( SoundAddFlag );
  
    else if( NewState == Gameplay_RemovingFlag )
      play_sound( SoundRemoveFlag );
  
    else if( NewState == Gameplay_Explosion )
      play_sound( SoundExplosion );
}


// ---------------------------------------------------------
//   GAME MEDIUM: DRAWING FUNCTIONS
// ---------------------------------------------------------


// graphic definitions
#define MediumBoardX 159
#define MediumBoardY 35

// ---------------------------------------------------------

void GameMedium_DrawScene()
{
    DrawUpperBoard();
    
    select_region( RegionInstructionsLeftEasy );
    draw_region_at( 0, screen_height-1 );
    
    select_region( RegionInstructionsRightEasy );
    draw_region_at( screen_width-1, screen_height-1 );
    
    select_region( RegionFrameLeftMedium );
    draw_region_at( 157, 35 );

    select_region( RegionFrameRightMedium );
    draw_region_at( 480,35 );
    
    select_region( RegionFrameBottomMedium );
    draw_region_at( 159,356 );
    
    Board_DrawSmall( &GameBoard, MediumBoardX, MediumBoardY );
}


// ---------------------------------------------------------
//   GAME MEDIUM: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void GameMedium_RunState_Initialize()
{
    // initialize the game
    GameBoard.Width = MediumBoardWidth;
    GameBoard.Height = MediumBoardHeight;
    GameBoard.NumberOfMines = MediumBoardMines;
    Board_Clear( &GameBoard );
    GameMode = ModeMedium;
    PlayedFrames = 0;
    
    // play gameplay music
    play_sound_in_channel( MusicGameplay, ChannelMusic );
    
    // transition immediately
    GameMedium_ChangeState( Gameplay_FadeIn );
}

// ---------------------------------------------------------

void GameMedium_RunState_FadeIn()
{
    // draw scene
    GameMedium_DrawScene();
    
    // fade-in effect
    int BlackLevel = max( 0, 255 - 255 * GameMedium_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    
    // change state when finished
    if( GameMedium_ElapsedFrames >= 60 )
      GameMedium_ChangeState( Gameplay_Waiting );   
}

// ---------------------------------------------------------

void GameMedium_RunState_Waiting()
{
    // draw scene
    GameMedium_DrawScene();
    
    // draw cursor animation
    int CursorFrame = (GameMedium_ElapsedFrames/12) % 4;
    select_region( FirstRegionCursorSmall + CursorFrame );
    int CursorX = MediumBoardX + GameBoard.SelectionX * SmallTileStep;
    int CursorY = MediumBoardY + GameBoard.SelectionY * SmallTileStep;
    draw_region_at( CursorX, CursorY );
    
    // obtain the current movement direction
    GameMedium_DeltaX = 0;
    GameMedium_DeltaY = 0;
    
    if( gamepad_left() > 0 && GameBoard.SelectionX > 0 )  GameMedium_DeltaX = -1;
    if( gamepad_up()   > 0 && GameBoard.SelectionY > 0 )  GameMedium_DeltaY = -1;
    
    if( gamepad_right() > 0 && GameBoard.SelectionX < (GameBoard.Width -1) ) GameMedium_DeltaX = 1;
    if( gamepad_down()  > 0 && GameBoard.SelectionY < (GameBoard.Height-1) ) GameMedium_DeltaY = 1;
    
    // when requested, move the cursor
    if( GameMedium_DeltaX || GameMedium_DeltaY )
    {
        GameMedium_ChangeState( Gameplay_Moving );
        return;
    }
    
    // start button launches quit dialog
    if( gamepad_button_start() == 1 )
    {
        pause_channel( ChannelMusic );
        GameScene = Scene_QuitGame;
        GameState = Dialog_Initialize;
        return;
    }
    
    Tile* SelectedTile = &GameBoard.Tiles[ GameBoard.SelectionY ][ GameBoard.SelectionX ];
    
    // button B toggles flags
    if( gamepad_button_b() == 1 )
    {
        // flags cannot be toggled on already revealed cells
        if( !SelectedTile->Revealed )
        {
            if( SelectedTile->Flagged )
              GameMedium_ChangeState( Gameplay_RemovingFlag );
            else
              GameMedium_ChangeState( Gameplay_AddingFlag );
        }
        
        return;            
    }
    
    // button A reveals the tile
    if( gamepad_button_a() == 1 )
    {
        if( !SelectedTile->Revealed )
          GameMedium_ChangeState( Gameplay_RevealTile );
        
        return;            
    }
}

// ---------------------------------------------------------

void GameMedium_RunState_Moving()
{
    // draw scene
    GameMedium_DrawScene();
    
    // draw cursor without animation
    select_region( FirstRegionCursorSmall );
    int CursorX = MediumBoardX + GameBoard.SelectionX * SmallTileStep + GameMedium_ElapsedFrames * SmallTileStep * GameMedium_DeltaX / 10;
    int CursorY = MediumBoardY + GameBoard.SelectionY * SmallTileStep + GameMedium_ElapsedFrames * SmallTileStep * GameMedium_DeltaY / 10;
    draw_region_at( CursorX, CursorY );
    
    // when finished, actually update selection
    if( GameMedium_ElapsedFrames >= 10 )
    {
        GameBoard.SelectionX += GameMedium_DeltaX;
        GameBoard.SelectionY += GameMedium_DeltaY;
        GameMedium_ChangeState( Gameplay_Waiting );
    }
}

// ---------------------------------------------------------

void GameMedium_RunState_AddingFlag()
{
    // draw scene
    GameMedium_DrawScene();
    
    // draw add flag animation
    int AnimationFrame = (GameMedium_ElapsedFrames / 8) % 3;
    select_region( FirstRegionFlagSmall + AnimationFrame );
    int TileX = MediumBoardX + GameBoard.SelectionX * SmallTileStep;
    int TileY = MediumBoardY + GameBoard.SelectionY * SmallTileStep;
    draw_region_at( TileX, TileY );
    
    // when finished, actually update tile
    if( GameMedium_ElapsedFrames >= 23 )
    {
        GameBoard.Tiles[ GameBoard.SelectionY ][ GameBoard.SelectionX ].Flagged = true;
        GameBoard.NumberOfFlags++;
        
        GameMedium_ChangeState( Gameplay_Waiting );
    }
}

// ---------------------------------------------------------

void GameMedium_RunState_RemovingFlag()
{
    // draw scene
    GameMedium_DrawScene();
    
    // draw add flag animation
    int AnimationFrame = (GameMedium_ElapsedFrames / 8) % 3;
    select_region( FirstRegionUnflagSmall + AnimationFrame );
    int TileX = MediumBoardX + GameBoard.SelectionX * SmallTileStep;
    int TileY = MediumBoardY + GameBoard.SelectionY * SmallTileStep;
    draw_region_at( TileX, TileY );
    
    // when finished, actually update tile
    if( GameMedium_ElapsedFrames >= 23 )
    {
        GameBoard.Tiles[ GameBoard.SelectionY ][ GameBoard.SelectionX ].Flagged = false;
        GameBoard.NumberOfFlags--;
        
        GameMedium_ChangeState( Gameplay_Waiting );
    }
}

// ---------------------------------------------------------

void GameMedium_RunState_RevealTile()
{
    // only on the first reveal, place all the mines
    // (done here to ensure that first pick is safe)
    if( !GameBoard.MinesArePlaced )
      Board_PlaceMines( &GameBoard, GameBoard.SelectionX, GameBoard.SelectionY );
    
    // draw scene
    GameMedium_DrawScene();
    
    // draw reveal animation
    int AnimationFrame = (GameMedium_ElapsedFrames / 4) % 4;
    select_region( FirstRegionRevealSmall + AnimationFrame );
    int TileX = MediumBoardX + GameBoard.SelectionX * SmallTileStep;
    int TileY = MediumBoardY + GameBoard.SelectionY * SmallTileStep;
    draw_region_at( TileX, TileY );
    
    // when finished, actually update tile
    if( GameMedium_ElapsedFrames >= 15 )
    {
        Tile* SelectedTile = &GameBoard.Tiles[ GameBoard.SelectionY ][ GameBoard.SelectionX ];
        Board_RevealTile( &GameBoard, SelectedTile );
        
        // CASE 1: if tile has a mine end game
        if( SelectedTile->MineState >= 9 )
        {
            GameBoard.MineExploded = true;
            stop_channel( ChannelMusic );
            GameMedium_ChangeState( Gameplay_Explosion );
            return;
        }
        
        // CASE 2: check if we have a reveal chain
        if( SelectedTile->MineState <= 0 )
        {
            GameMedium_CurrentRevealStep = 2;
            GameMedium_MaxRevealStep = Board_DetermineTilesToReveal( &GameBoard, GameBoard.SelectionX, GameBoard.SelectionY );
            GameMedium_ChangeState( Gameplay_PropagateReveal );
            return;
        }
        
        // CASE 3: on single reveal, check if the
        // board is already solved, otherwise just go back
        if( Board_IsSolved( &GameBoard ) )
          GameMedium_ChangeState( Gameplay_RevealMines );
        else
          GameMedium_ChangeState( Gameplay_Waiting );
    }
}

// ---------------------------------------------------------

void GameMedium_RunState_PropagateReveal()
{
    // draw scene
    GameMedium_DrawScene();
    
    // draw reveal animation for all tiles in current step
    // but it will play faster than the first reveal
    int AnimationFrame = (GameMedium_ElapsedFrames / 2) % 4;
    select_region( FirstRegionRevealSmall + AnimationFrame );
    
    for( int y = 0; y < GameBoard.Height; y++ )
      for( int x = 0; x < GameBoard.Width; x++ )
        if( GameBoard.Tiles[ y ][ x ].RevealDistance == GameMedium_CurrentRevealStep )
        {
            int TileX = MediumBoardX + x * SmallTileStep;
            int TileY = MediumBoardY + y * SmallTileStep;
            draw_region_at( TileX, TileY );
        }
    
    // check when this step ends
    if( GameMedium_ElapsedFrames >= 7 )
    {
        // actually reveal the tiles
        for( int y = 0; y < GameBoard.Height; y++ )
          for( int x = 0; x < GameBoard.Width; x++ )
            if( GameBoard.Tiles[ y ][ x ].RevealDistance == GameMedium_CurrentRevealStep )
              Board_RevealTile( &GameBoard, &GameBoard.Tiles[ y ][ x ] );
        
        // CASE 1: there are more reveal steps
        if( GameMedium_CurrentRevealStep < GameMedium_MaxRevealStep )
        {
            GameMedium_CurrentRevealStep++;
            GameMedium_ElapsedFrames = 0;
            play_sound( SoundReveal );
        }
        
        // CASE 2: we are finished
        else
        {
            // check if the board is already
            // solved, otherwise just go back
            if( Board_IsSolved( &GameBoard ) )
              GameMedium_ChangeState( Gameplay_RevealMines );
            else
              GameMedium_ChangeState( Gameplay_Waiting );
        }
    }
}

// ---------------------------------------------------------

void GameMedium_RunState_Explosion()
{
    // draw scene
    GameMedium_DrawScene();
    
    // PART 1: draw explosion animation
    if( GameMedium_ElapsedFrames <= 20 )
    {
        // draw explosion animation with a stretch effect
        int ExplosionX = MediumBoardX + SmallTileStep * (GameBoard.SelectionX + 0.5 );
        int ExplosionY = MediumBoardY + SmallTileStep * (GameBoard.SelectionY + 0.5 );
        float ExplosionScale = sin( pi * GameMedium_ElapsedFrames / 20.0 );
        select_region( RegionExplosionSmall );
        set_drawing_scale( 1, ExplosionScale );
        draw_region_zoomed_at( ExplosionX, ExplosionY );
        
        // to form the cross, draw the same region again but rotated 90 degrees 
        set_drawing_angle( pi/2 );
        draw_region_rotozoomed_at( ExplosionX, ExplosionY );
    }
    
    // PART 2: simply wait for a bit
    
    // when finished, reveal all mines
    if( GameMedium_ElapsedFrames >= 50 )
      GameMedium_ChangeState( Gameplay_RevealMines );
}

// ---------------------------------------------------------

void GameMedium_RunState_RevealMines()
{
    // draw scene
    GameMedium_DrawScene();
    
    // PART 1: draw reveal animation for all mines
    if( GameMedium_ElapsedFrames <= 15 )
    {
        int AnimationFrame = (GameMedium_ElapsedFrames / 4) % 4;
        select_region( FirstRegionRevealSmall + AnimationFrame );
        
        for( int y = 0; y < GameBoard.Height; y++ )
          for( int x = 0; x < GameBoard.Width; x++ )
            if( GameBoard.Tiles[ y ][ x ].MineState >= 9 )
            {
                // make an exception for the already exploded mine
                if( x == GameBoard.SelectionX && y == GameBoard.SelectionY )
                  continue;
                
                int TileX = MediumBoardX + x * SmallTileStep;
                int TileY = MediumBoardY + y * SmallTileStep;
                draw_region_at( TileX, TileY );
            }
    }
    
    // PART 2: reveal, and wait to let player see the mines
    if( GameMedium_ElapsedFrames == 16 )
    {
        for( int y = 0; y < GameBoard.Height; y++ )
          for( int x = 0; x < GameBoard.Width; x++ )
            if( GameBoard.Tiles[ y ][ x ].MineState >= 9 )
              GameBoard.Tiles[ y ][ x ].Revealed = true;      
    }
    
    // this scene is called in any situation where
    // the game finishes, so after this go to win
    // or lose scene, as applicable
    if( GameMedium_ElapsedFrames >= 60 )
    {
        if( GameBoard.MineExploded )
          GameScene = Scene_Lose;
        else
          GameScene = Scene_Win;
        
        GameState = Dialog_Initialize;
    }
}


// ---------------------------------------------------------
//   GAME MEDIUM: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void GameMedium_RunStateMachine()
{
    switch( GameState )
    {
        case Gameplay_Initialize:
          GameMedium_RunState_Initialize();
          break;
        case Gameplay_FadeIn:
          GameMedium_RunState_FadeIn();
          break;
        case Gameplay_Waiting:
          GameMedium_RunState_Waiting();
          break;
        case Gameplay_Moving:
          GameMedium_RunState_Moving();
          break;
        case Gameplay_AddingFlag:
          GameMedium_RunState_AddingFlag();
          break;
        case Gameplay_RemovingFlag:
          GameMedium_RunState_RemovingFlag();
          break;
        case Gameplay_RevealTile:
          GameMedium_RunState_RevealTile();
          break;
        case Gameplay_PropagateReveal:
          GameMedium_RunState_PropagateReveal();
          break;
        case Gameplay_Explosion:
          GameMedium_RunState_Explosion();
          break;
        case Gameplay_RevealMines:
          GameMedium_RunState_RevealMines();
          break;
    }
    
    // count the frames in current state
    GameMedium_ElapsedFrames++;
    
    // count total played mode
    PlayedFrames++;
}
