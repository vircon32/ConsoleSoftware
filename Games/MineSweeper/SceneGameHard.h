// ---------------------------------------------------------
//   GAME HARD: STATE MANAGEMENT
// ---------------------------------------------------------


int GameHard_ElapsedFrames;

// stores movement direction during transition
int GameHard_DeltaX;
int GameHard_DeltaY;

// this keeps track of current reveal propatation steps
int GameHard_CurrentRevealStep;
int GameHard_MaxRevealStep;

// ---------------------------------------------------------

void GameHard_ChangeState( int NewState )
{
    GameState = NewState;
    GameHard_ElapsedFrames = 0;
    
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
//   GAME HARD: DRAWING FUNCTIONS
// ---------------------------------------------------------


// graphic definitions
#define HardBoardX 35
#define HardBoardY 35

// ---------------------------------------------------------

void GameHard_DrawScene()
{
    DrawUpperBoard();
    
    select_region( RegionInstructionsLeftHard );
    draw_region_at( 0, screen_height-1 );
    
    select_region( RegionFrameRightHard );
    draw_region_at( screen_width-1, screen_height-1 );
    
    select_region( RegionFrameBottomHard );
    draw_region_at( 35,356 );
    
    Board_DrawSmall( &GameBoard, HardBoardX, HardBoardY );
}


// ---------------------------------------------------------
//   GAME HARD: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void GameHard_RunState_Initialize()
{
    // initialize the game
    GameBoard.Width = HardBoardWidth;
    GameBoard.Height = HardBoardHeight;
    GameBoard.NumberOfMines = HardBoardMines;
    Board_Clear( &GameBoard );
    GameMode = ModeHard;
    PlayedFrames = 0;
    
    // play gameplay music
    play_sound_in_channel( MusicGameplay, ChannelMusic );
    
    // transition immediately
    GameHard_ChangeState( Gameplay_FadeIn );
}

// ---------------------------------------------------------

void GameHard_RunState_FadeIn()
{
    // draw scene
    GameHard_DrawScene();
    
    // fade-in effect
    int BlackLevel = max( 0, 255 - 255 * GameHard_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    
    // change state when finished
    if( GameHard_ElapsedFrames >= 60 )
      GameHard_ChangeState( Gameplay_Waiting );   
}

// ---------------------------------------------------------

void GameHard_RunState_Waiting()
{
    // draw scene
    GameHard_DrawScene();
    
    // draw cursor animation
    int CursorFrame = (GameHard_ElapsedFrames/12) % 4;
    select_region( FirstRegionCursorSmall + CursorFrame );
    int CursorX = HardBoardX + GameBoard.SelectionX * SmallTileStep;
    int CursorY = HardBoardY + GameBoard.SelectionY * SmallTileStep;
    draw_region_at( CursorX, CursorY );
    
    // obtain the current movement direction
    GameHard_DeltaX = 0;
    GameHard_DeltaY = 0;
    
    if( gamepad_left() > 0 && GameBoard.SelectionX > 0 )  GameHard_DeltaX = -1;
    if( gamepad_up()   > 0 && GameBoard.SelectionY > 0 )  GameHard_DeltaY = -1;
    
    if( gamepad_right() > 0 && GameBoard.SelectionX < (GameBoard.Width -1) ) GameHard_DeltaX = 1;
    if( gamepad_down()  > 0 && GameBoard.SelectionY < (GameBoard.Height-1) ) GameHard_DeltaY = 1;
    
    // when requested, move the cursor
    if( GameHard_DeltaX || GameHard_DeltaY )
    {
        GameHard_ChangeState( Gameplay_Moving );
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
              GameHard_ChangeState( Gameplay_RemovingFlag );
            else
              GameHard_ChangeState( Gameplay_AddingFlag );
        }
        
        return;            
    }
    
    // button A reveals the tile
    if( gamepad_button_a() == 1 )
    {
        if( !SelectedTile->Revealed )
          GameHard_ChangeState( Gameplay_RevealTile );
        
        return;            
    }
}

// ---------------------------------------------------------

void GameHard_RunState_Moving()
{
    // draw scene
    GameHard_DrawScene();
    
    // draw cursor without animation
    select_region( FirstRegionCursorSmall );
    int CursorX = HardBoardX + GameBoard.SelectionX * SmallTileStep + GameHard_ElapsedFrames * SmallTileStep * GameHard_DeltaX / 10;
    int CursorY = HardBoardY + GameBoard.SelectionY * SmallTileStep + GameHard_ElapsedFrames * SmallTileStep * GameHard_DeltaY / 10;
    draw_region_at( CursorX, CursorY );
    
    // when finished, actually update selection
    if( GameHard_ElapsedFrames >= 10 )
    {
        GameBoard.SelectionX += GameHard_DeltaX;
        GameBoard.SelectionY += GameHard_DeltaY;
        GameHard_ChangeState( Gameplay_Waiting );
    }
}

// ---------------------------------------------------------

void GameHard_RunState_AddingFlag()
{
    // draw scene
    GameHard_DrawScene();
    
    // draw add flag animation
    int AnimationFrame = (GameHard_ElapsedFrames / 8) % 3;
    select_region( FirstRegionFlagSmall + AnimationFrame );
    int TileX = HardBoardX + GameBoard.SelectionX * SmallTileStep;
    int TileY = HardBoardY + GameBoard.SelectionY * SmallTileStep;
    draw_region_at( TileX, TileY );
    
    // when finished, actually update tile
    if( GameHard_ElapsedFrames >= 23 )
    {
        GameBoard.Tiles[ GameBoard.SelectionY ][ GameBoard.SelectionX ].Flagged = true;
        GameBoard.NumberOfFlags++;
        
        GameHard_ChangeState( Gameplay_Waiting );
    }
}

// ---------------------------------------------------------

void GameHard_RunState_RemovingFlag()
{
    // draw scene
    GameHard_DrawScene();
    
    // draw add flag animation
    int AnimationFrame = (GameHard_ElapsedFrames / 8) % 3;
    select_region( FirstRegionUnflagSmall + AnimationFrame );
    int TileX = HardBoardX + GameBoard.SelectionX * SmallTileStep;
    int TileY = HardBoardY + GameBoard.SelectionY * SmallTileStep;
    draw_region_at( TileX, TileY );
    
    // when finished, actually update tile
    if( GameHard_ElapsedFrames >= 23 )
    {
        GameBoard.Tiles[ GameBoard.SelectionY ][ GameBoard.SelectionX ].Flagged = false;
        GameBoard.NumberOfFlags--;
        
        GameHard_ChangeState( Gameplay_Waiting );
    }
}

// ---------------------------------------------------------

void GameHard_RunState_RevealTile()
{
    // only on the first reveal, place all the mines
    // (done here to ensure that first pick is safe)
    if( !GameBoard.MinesArePlaced )
      Board_PlaceMines( &GameBoard, GameBoard.SelectionX, GameBoard.SelectionY );
    
    // draw scene
    GameHard_DrawScene();
    
    // draw reveal animation
    int AnimationFrame = (GameHard_ElapsedFrames / 4) % 4;
    select_region( FirstRegionRevealSmall + AnimationFrame );
    int TileX = HardBoardX + GameBoard.SelectionX * SmallTileStep;
    int TileY = HardBoardY + GameBoard.SelectionY * SmallTileStep;
    draw_region_at( TileX, TileY );
    
    // when finished, actually update tile
    if( GameHard_ElapsedFrames >= 15 )
    {
        Tile* SelectedTile = &GameBoard.Tiles[ GameBoard.SelectionY ][ GameBoard.SelectionX ];
        Board_RevealTile( &GameBoard, SelectedTile );
        
        // CASE 1: if tile has a mine end game
        if( SelectedTile->MineState >= 9 )
        {
            GameBoard.MineExploded = true;
            stop_channel( ChannelMusic );
            GameHard_ChangeState( Gameplay_Explosion );
            return;
        }
        
        // CASE 2: check if we have a reveal chain
        if( SelectedTile->MineState <= 0 )
        {
            GameHard_CurrentRevealStep = 2;
            GameHard_MaxRevealStep = Board_DetermineTilesToReveal( &GameBoard, GameBoard.SelectionX, GameBoard.SelectionY );
            GameHard_ChangeState( Gameplay_PropagateReveal );
            return;
        }
        
        // CASE 3: on single reveal, check if the
        // board is already solved, otherwise just go back
        if( Board_IsSolved( &GameBoard ) )
          GameHard_ChangeState( Gameplay_RevealMines );
        else
          GameHard_ChangeState( Gameplay_Waiting );
    }
}

// ---------------------------------------------------------

void GameHard_RunState_PropagateReveal()
{
    // draw scene
    GameHard_DrawScene();
    
    // draw reveal animation for all tiles in current step
    // but it will play faster than the first reveal
    int AnimationFrame = (GameHard_ElapsedFrames / 2) % 4;
    select_region( FirstRegionRevealSmall + AnimationFrame );
    
    for( int y = 0; y < GameBoard.Height; y++ )
      for( int x = 0; x < GameBoard.Width; x++ )
        if( GameBoard.Tiles[ y ][ x ].RevealDistance == GameHard_CurrentRevealStep )
        {
            int TileX = HardBoardX + x * SmallTileStep;
            int TileY = HardBoardY + y * SmallTileStep;
            draw_region_at( TileX, TileY );
        }
    
    // check when this step ends
    if( GameHard_ElapsedFrames >= 7 )
    {
        // actually reveal the tiles
        for( int y = 0; y < GameBoard.Height; y++ )
          for( int x = 0; x < GameBoard.Width; x++ )
            if( GameBoard.Tiles[ y ][ x ].RevealDistance == GameHard_CurrentRevealStep )
              Board_RevealTile( &GameBoard, &GameBoard.Tiles[ y ][ x ] );
        
        // CASE 1: there are more reveal steps
        if( GameHard_CurrentRevealStep < GameHard_MaxRevealStep )
        {
            GameHard_CurrentRevealStep++;
            GameHard_ElapsedFrames = 0;
            play_sound( SoundReveal );
        }
        
        // CASE 2: we are finished
        else
        {
            // check if the board is already
            // solved, otherwise just go back
            if( Board_IsSolved( &GameBoard ) )
              GameHard_ChangeState( Gameplay_RevealMines );
            else
              GameHard_ChangeState( Gameplay_Waiting );
        }
    }
}

// ---------------------------------------------------------

void GameHard_RunState_Explosion()
{
    // draw scene
    GameHard_DrawScene();
    
    // PART 1: draw explosion animation
    if( GameHard_ElapsedFrames <= 20 )
    {
        // draw explosion animation with a stretch effect
        int ExplosionX = HardBoardX + SmallTileStep * (GameBoard.SelectionX + 0.5 );
        int ExplosionY = HardBoardY + SmallTileStep * (GameBoard.SelectionY + 0.5 );
        float ExplosionScale = sin( pi * GameHard_ElapsedFrames / 20.0 );
        select_region( RegionExplosionSmall );
        set_drawing_scale( 1, ExplosionScale );
        draw_region_zoomed_at( ExplosionX, ExplosionY );
        
        // to form the cross, draw the same region again but rotated 90 degrees 
        set_drawing_angle( pi/2 );
        draw_region_rotozoomed_at( ExplosionX, ExplosionY );
    }
    
    // PART 2: simply wait for a bit
    
    // when finished, reveal all mines
    if( GameHard_ElapsedFrames >= 50 )
      GameHard_ChangeState( Gameplay_RevealMines );
}

// ---------------------------------------------------------

void GameHard_RunState_RevealMines()
{
    // draw scene
    GameHard_DrawScene();
    
    // PART 1: draw reveal animation for all mines
    if( GameHard_ElapsedFrames <= 15 )
    {
        int AnimationFrame = (GameHard_ElapsedFrames / 4) % 4;
        select_region( FirstRegionRevealSmall + AnimationFrame );
        
        for( int y = 0; y < GameBoard.Height; y++ )
          for( int x = 0; x < GameBoard.Width; x++ )
            if( GameBoard.Tiles[ y ][ x ].MineState >= 9 )
            {
                // make an exception for the already exploded mine
                if( x == GameBoard.SelectionX && y == GameBoard.SelectionY )
                  continue;
                
                int TileX = HardBoardX + x * SmallTileStep;
                int TileY = HardBoardY + y * SmallTileStep;
                draw_region_at( TileX, TileY );
            }
    }
    
    // PART 2: reveal, and wait to let player see the mines
    if( GameHard_ElapsedFrames == 16 )
    {
        for( int y = 0; y < GameBoard.Height; y++ )
          for( int x = 0; x < GameBoard.Width; x++ )
            if( GameBoard.Tiles[ y ][ x ].MineState >= 9 )
              GameBoard.Tiles[ y ][ x ].Revealed = true;      
    }
    
    // this scene is called in any situation where
    // the game finishes, so after this go to win
    // or lose scene, as applicable
    if( GameHard_ElapsedFrames >= 60 )
    {
        if( GameBoard.MineExploded )
          GameScene = Scene_Lose;
        else
          GameScene = Scene_Win;
        
        GameState = Dialog_Initialize;
    }
}


// ---------------------------------------------------------
//   GAME HARD: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void GameHard_RunStateMachine()
{
    switch( GameState )
    {
        case Gameplay_Initialize:
          GameHard_RunState_Initialize();
          break;
        case Gameplay_FadeIn:
          GameHard_RunState_FadeIn();
          break;
        case Gameplay_Waiting:
          GameHard_RunState_Waiting();
          break;
        case Gameplay_Moving:
          GameHard_RunState_Moving();
          break;
        case Gameplay_AddingFlag:
          GameHard_RunState_AddingFlag();
          break;
        case Gameplay_RemovingFlag:
          GameHard_RunState_RemovingFlag();
          break;
        case Gameplay_RevealTile:
          GameHard_RunState_RevealTile();
          break;
        case Gameplay_PropagateReveal:
          GameHard_RunState_PropagateReveal();
          break;
        case Gameplay_Explosion:
          GameHard_RunState_Explosion();
          break;
        case Gameplay_RevealMines:
          GameHard_RunState_RevealMines();
          break;
    }
    
    // count the frames in current state
    GameHard_ElapsedFrames++;
    
    // count total played mode
    PlayedFrames++;
}
