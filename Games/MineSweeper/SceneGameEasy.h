// ---------------------------------------------------------
//   GAME EASY: STATE MANAGEMENT
// ---------------------------------------------------------


int GameEasy_ElapsedFrames;

// stores movement direction during transition
int GameEasy_DeltaX;
int GameEasy_DeltaY;

// this keeps track of current reveal propatation steps
int GameEasy_CurrentRevealStep;
int GameEasy_MaxRevealStep;

// ---------------------------------------------------------

void GameEasy_ChangeState( int NewState )
{
    GameState = NewState;
    GameEasy_ElapsedFrames = 0;
    
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
//   GAME EASY: DRAWING FUNCTIONS
// ---------------------------------------------------------


// graphic definitions
#define EasyBoardX 161
#define EasyBoardY 37

// ---------------------------------------------------------

void GameEasy_DrawScene()
{
    DrawUpperBoard();
    
    select_region( RegionInstructionsLeftEasy );
    draw_region_at( 0, screen_height-1 );

    select_region( RegionInstructionsRightEasy );
    draw_region_at( screen_width-1, screen_height-1 );
    
    select_region( RegionFrameLeftEasy );
    draw_region_at( 157, 35 );

    select_region( RegionFrameRightEasy );
    draw_region_at( 478,35 );
    
    select_region( RegionFrameTopEasy );
    draw_region_at( 161,35 );

    select_region( RegionFrameBottomEasy );
    draw_region_at( 161,354 );
    
    Board_DrawLarge( &GameBoard, EasyBoardX, EasyBoardY );
}


// ---------------------------------------------------------
//   GAME EASY: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void GameEasy_RunState_Initialize()
{
    // initialize the game
    GameBoard.Width = EasyBoardWidth;
    GameBoard.Height = EasyBoardHeight;
    GameBoard.NumberOfMines = EasyBoardMines;
    Board_Clear( &GameBoard );
    GameMode = ModeEasy;
    PlayedFrames = 0;
    
    // play gameplay music
    play_sound_in_channel( MusicGameplay, ChannelMusic );
    
    // transition immediately
    GameEasy_ChangeState( Gameplay_FadeIn );
}

// ---------------------------------------------------------

void GameEasy_RunState_FadeIn()
{
    // draw scene
    GameEasy_DrawScene();
    
    // fade-in effect
    int BlackLevel = max( 0, 255 - 255 * GameEasy_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    
    // change state when finished
    if( GameEasy_ElapsedFrames >= 60 )
      GameEasy_ChangeState( Gameplay_Waiting );   
}

// ---------------------------------------------------------

void GameEasy_RunState_Waiting()
{
    // draw scene
    GameEasy_DrawScene();
    
    // draw cursor animation
    int CursorFrame = (GameEasy_ElapsedFrames/12) % 4;
    select_region( FirstRegionCursorLarge + CursorFrame );
    int CursorX = EasyBoardX + GameBoard.SelectionX * LargeTileStep;
    int CursorY = EasyBoardY + GameBoard.SelectionY * LargeTileStep;
    draw_region_at( CursorX, CursorY );
    
    // obtain the current movement direction
    GameEasy_DeltaX = 0;
    GameEasy_DeltaY = 0;
    
    if( gamepad_left() > 0 && GameBoard.SelectionX > 0 )  GameEasy_DeltaX = -1;
    if( gamepad_up()   > 0 && GameBoard.SelectionY > 0 )  GameEasy_DeltaY = -1;
    
    if( gamepad_right() > 0 && GameBoard.SelectionX < (GameBoard.Width -1) ) GameEasy_DeltaX = 1;
    if( gamepad_down()  > 0 && GameBoard.SelectionY < (GameBoard.Height-1) ) GameEasy_DeltaY = 1;
    
    // when requested, move the cursor
    if( GameEasy_DeltaX || GameEasy_DeltaY )
    {
        GameEasy_ChangeState( Gameplay_Moving );
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
              GameEasy_ChangeState( Gameplay_RemovingFlag );
            else
              GameEasy_ChangeState( Gameplay_AddingFlag );
        }
        
        return;            
    }
    
    // button A reveals the tile
    if( gamepad_button_a() == 1 )
    {
        if( !SelectedTile->Revealed )
          GameEasy_ChangeState( Gameplay_RevealTile );
        
        return;            
    }
}

// ---------------------------------------------------------

void GameEasy_RunState_Moving()
{
    // draw scene
    GameEasy_DrawScene();
    
    // draw cursor without animation
    select_region( FirstRegionCursorLarge );
    int CursorX = EasyBoardX + GameBoard.SelectionX * LargeTileStep + GameEasy_ElapsedFrames * LargeTileStep * GameEasy_DeltaX / 10;
    int CursorY = EasyBoardY + GameBoard.SelectionY * LargeTileStep + GameEasy_ElapsedFrames * LargeTileStep * GameEasy_DeltaY / 10;
    draw_region_at( CursorX, CursorY );
    
    // when finished, actually update selection
    if( GameEasy_ElapsedFrames >= 10 )
    {
        GameBoard.SelectionX += GameEasy_DeltaX;
        GameBoard.SelectionY += GameEasy_DeltaY;
        GameEasy_ChangeState( Gameplay_Waiting );
    }
}

// ---------------------------------------------------------

void GameEasy_RunState_AddingFlag()
{
    // draw scene
    GameEasy_DrawScene();
    
    // draw add flag animation
    int AnimationFrame = (GameEasy_ElapsedFrames / 8) % 3;
    select_region( FirstRegionFlagLarge + AnimationFrame );
    int TileX = EasyBoardX + GameBoard.SelectionX * LargeTileStep;
    int TileY = EasyBoardY + GameBoard.SelectionY * LargeTileStep;
    draw_region_at( TileX, TileY );
    
    // when finished, actually update tile
    if( GameEasy_ElapsedFrames >= 23 )
    {
        GameBoard.Tiles[ GameBoard.SelectionY ][ GameBoard.SelectionX ].Flagged = true;
        GameBoard.NumberOfFlags++;
        
        GameEasy_ChangeState( Gameplay_Waiting );
    }
}

// ---------------------------------------------------------

void GameEasy_RunState_RemovingFlag()
{
    // draw scene
    GameEasy_DrawScene();
    
    // draw add flag animation
    int AnimationFrame = (GameEasy_ElapsedFrames / 8) % 3;
    select_region( FirstRegionUnflagLarge + AnimationFrame );
    int TileX = EasyBoardX + GameBoard.SelectionX * LargeTileStep;
    int TileY = EasyBoardY + GameBoard.SelectionY * LargeTileStep;
    draw_region_at( TileX, TileY );
    
    // when finished, actually update tile
    if( GameEasy_ElapsedFrames >= 23 )
    {
        GameBoard.Tiles[ GameBoard.SelectionY ][ GameBoard.SelectionX ].Flagged = false;
        GameBoard.NumberOfFlags--;
        
        GameEasy_ChangeState( Gameplay_Waiting );
    }
}

// ---------------------------------------------------------

void GameEasy_RunState_RevealTile()
{
    // only on the first reveal, place all the mines
    // (done here to ensure that first pick is safe)
    if( !GameBoard.MinesArePlaced )
      Board_PlaceMines( &GameBoard, GameBoard.SelectionX, GameBoard.SelectionY );
    
    // draw scene
    GameEasy_DrawScene();
    
    // draw reveal animation
    int AnimationFrame = (GameEasy_ElapsedFrames / 4) % 4;
    select_region( FirstRegionRevealLarge + AnimationFrame );
    int TileX = EasyBoardX + GameBoard.SelectionX * LargeTileStep;
    int TileY = EasyBoardY + GameBoard.SelectionY * LargeTileStep;
    draw_region_at( TileX, TileY );
    
    // when finished, actually update tile
    if( GameEasy_ElapsedFrames >= 15 )
    {
        Tile* SelectedTile = &GameBoard.Tiles[ GameBoard.SelectionY ][ GameBoard.SelectionX ];
        Board_RevealTile( &GameBoard, SelectedTile );
        
        // CASE 1: if tile has a mine end game
        if( SelectedTile->MineState >= 9 )
        {
            GameBoard.MineExploded = true;
            stop_channel( ChannelMusic );
            GameEasy_ChangeState( Gameplay_Explosion );
            return;
        }
        
        // CASE 2: check if we have a reveal chain
        if( SelectedTile->MineState <= 0 )
        {
            GameEasy_CurrentRevealStep = 2;
            GameEasy_MaxRevealStep = Board_DetermineTilesToReveal( &GameBoard, GameBoard.SelectionX, GameBoard.SelectionY );
            GameEasy_ChangeState( Gameplay_PropagateReveal );
            return;
        }
        
        // CASE 3: on single reveal, check if the
        // board is already solved, otherwise just go back
        if( Board_IsSolved( &GameBoard ) )
          GameEasy_ChangeState( Gameplay_RevealMines );
        else
          GameEasy_ChangeState( Gameplay_Waiting );
    }
}

// ---------------------------------------------------------

void GameEasy_RunState_PropagateReveal()
{
    // draw scene
    GameEasy_DrawScene();
    
    // draw reveal animation for all tiles in current step
    // but it will play faster than the first reveal
    int AnimationFrame = (GameEasy_ElapsedFrames / 2) % 4;
    select_region( FirstRegionRevealLarge + AnimationFrame );
    
    for( int y = 0; y < GameBoard.Height; y++ )
      for( int x = 0; x < GameBoard.Width; x++ )
        if( GameBoard.Tiles[ y ][ x ].RevealDistance == GameEasy_CurrentRevealStep )
        {
            int TileX = EasyBoardX + x * LargeTileStep;
            int TileY = EasyBoardY + y * LargeTileStep;
            draw_region_at( TileX, TileY );
        }
    
    // check when this step ends
    if( GameEasy_ElapsedFrames >= 7 )
    {
        // actually reveal the tiles
        for( int y = 0; y < GameBoard.Height; y++ )
          for( int x = 0; x < GameBoard.Width; x++ )
            if( GameBoard.Tiles[ y ][ x ].RevealDistance == GameEasy_CurrentRevealStep )
              Board_RevealTile( &GameBoard, &GameBoard.Tiles[ y ][ x ] );
        
        // CASE 1: there are more reveal steps
        if( GameEasy_CurrentRevealStep < GameEasy_MaxRevealStep )
        {
            GameEasy_CurrentRevealStep++;
            GameEasy_ElapsedFrames = 0;
            play_sound( SoundReveal );
        }
        
        // CASE 2: we are finished
        else
        {
            // check if the board is already
            // solved, otherwise just go back
            if( Board_IsSolved( &GameBoard ) )
              GameEasy_ChangeState( Gameplay_RevealMines );
            else
              GameEasy_ChangeState( Gameplay_Waiting );
        }
    }
}

// ---------------------------------------------------------

void GameEasy_RunState_Explosion()
{
    // draw scene
    GameEasy_DrawScene();
    
    // PART 1: draw explosion animation
    if( GameEasy_ElapsedFrames <= 20 )
    {
        // draw explosion animation with a stretch effect
        int ExplosionX = EasyBoardX + LargeTileStep * (GameBoard.SelectionX + 0.5 );
        int ExplosionY = EasyBoardY + LargeTileStep * (GameBoard.SelectionY + 0.5 );
        float ExplosionScale = sin( pi * GameEasy_ElapsedFrames / 20.0 );
        select_region( RegionExplosionLarge );
        set_drawing_scale( 1, ExplosionScale );
        draw_region_zoomed_at( ExplosionX, ExplosionY );
        
        // to form the cross, draw the same region again but rotated 90 degrees 
        set_drawing_angle( pi/2 );
        draw_region_rotozoomed_at( ExplosionX, ExplosionY );
    }
    
    // PART 2: simply wait for a bit
    
    // when finished, reveal all mines
    if( GameEasy_ElapsedFrames >= 50 )
      GameEasy_ChangeState( Gameplay_RevealMines );
}

// ---------------------------------------------------------

void GameEasy_RunState_RevealMines()
{
    // draw scene
    GameEasy_DrawScene();
    
    // PART 1: draw reveal animation for all mines
    if( GameEasy_ElapsedFrames <= 15 )
    {
        int AnimationFrame = (GameEasy_ElapsedFrames / 4) % 4;
        select_region( FirstRegionRevealLarge + AnimationFrame );
        
        for( int y = 0; y < GameBoard.Height; y++ )
          for( int x = 0; x < GameBoard.Width; x++ )
            if( GameBoard.Tiles[ y ][ x ].MineState >= 9 )
            {
                // make an exception for the already exploded mine
                if( x == GameBoard.SelectionX && y == GameBoard.SelectionY )
                  continue;
                
                int TileX = EasyBoardX + x * LargeTileStep;
                int TileY = EasyBoardY + y * LargeTileStep;
                draw_region_at( TileX, TileY );
            }
    }
    
    // PART 2: reveal, and wait to let player see the mines
    if( GameEasy_ElapsedFrames == 16 )
    {
        for( int y = 0; y < GameBoard.Height; y++ )
          for( int x = 0; x < GameBoard.Width; x++ )
            if( GameBoard.Tiles[ y ][ x ].MineState >= 9 )
              GameBoard.Tiles[ y ][ x ].Revealed = true;      
    }
    
    // this scene is called in any situation where
    // the game finishes, so after this go to win
    // or lose scene, as applicable
    if( GameEasy_ElapsedFrames >= 60 )
    {
        if( GameBoard.MineExploded )
          GameScene = Scene_Lose;
        else
          GameScene = Scene_Win;
        
        GameState = Dialog_Initialize;
    }
}


// ---------------------------------------------------------
//   GAME EASY: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void GameEasy_RunStateMachine()
{
    switch( GameState )
    {
        case Gameplay_Initialize:
          GameEasy_RunState_Initialize();
          break;
        case Gameplay_FadeIn:
          GameEasy_RunState_FadeIn();
          break;
        case Gameplay_Waiting:
          GameEasy_RunState_Waiting();
          break;
        case Gameplay_Moving:
          GameEasy_RunState_Moving();
          break;
        case Gameplay_AddingFlag:
          GameEasy_RunState_AddingFlag();
          break;
        case Gameplay_RemovingFlag:
          GameEasy_RunState_RemovingFlag();
          break;
        case Gameplay_RevealTile:
          GameEasy_RunState_RevealTile();
          break;
        case Gameplay_PropagateReveal:
          GameEasy_RunState_PropagateReveal();
          break;
        case Gameplay_Explosion:
          GameEasy_RunState_Explosion();
          break;
        case Gameplay_RevealMines:
          GameEasy_RunState_RevealMines();
          break;
    }
    
    // count the frames in current state
    GameEasy_ElapsedFrames++;
    
    // count total played mode
    PlayedFrames++;
}
