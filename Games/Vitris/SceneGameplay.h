// ---------------------------------------------------------
//   GAMEPLAY: STATE MANAGEMENT
// ---------------------------------------------------------


int Gameplay_ElapsedFrames;

// ---------------------------------------------------------

void Gameplay_ChangeState( int NewState )
{
    GameState = NewState;
    Gameplay_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( NewState == Gameplay_FadeIn )
      play_sound_in_channel( MusicGameplay, ChannelMusic );
}


// ---------------------------------------------------------
//   GAMEPLAY: DRAWING FUNCTIONS
// ---------------------------------------------------------


void Gameplay_DrawScene()
{
    // draw background
    select_texture( TextureGameplay );
    select_region( RegionBackground );
    draw_region_at( 0, 0 );
    
    // write current level (with blinking)
    int Alpha = 255 - 128 * ((Game.LevelBlinkCounter & 4) != 0);
    set_multiply_color( make_color_rgba( 255,255,130,Alpha ) );
    numberfont_print_int_fixed( &FontLevel, 509,  89, Game.CurrentLevel, 2 );
    
    // write cleared lines (with blinking)
    Alpha = 255 - 128 * ((Game.LinesBlinkCounter & 4) != 0);
    set_multiply_color( make_color_rgba( 80,255,80,Alpha ) );
    numberfont_print_int_from_right( &FontLines, 558, 248, Game.ClearedLines );
    
    // write score (with blinking)
    Alpha = 255 - 128 * ((Game.ScoreBlinkCounter & 4) != 0);
    set_multiply_color( make_color_rgba( 195,195,195,Alpha ) );
    numberfont_print_int_from_right( &FontLines, 164, 330, Game.Score );
    
    // show next piece
    set_multiply_color( color_white );
    select_region( FirstRegionPieces + Game.NextPiece );
    draw_region_at( 72, 154 );
    
    // draw top line
    select_region( RegionTopLine );
    draw_region_at( 224, 16 );
    
    // draw board, but ommit doing it
    // during pause to prevent cheating!
    if( GameScene != Scene_QuitGame )
      Game_DrawBoard();
}


// ---------------------------------------------------------
//   GAMEPLAY: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Gameplay_RunState_Initialize()
{
    // initialize the game
    ResetGame();
    Game.CurrentLevel = 5 * ChooseLevel_ChosenOption;
    
    // transition immediately
    Gameplay_ChangeState( Gameplay_FadeIn );
}

// ---------------------------------------------------------

void Gameplay_RunState_FadeIn()
{
    // draw scene
    Gameplay_DrawScene();
    
    // fade-in effect
    int BlackLevel = max( 0, 255 - 255 * Gameplay_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    
    // change state when finished
    if( Gameplay_ElapsedFrames >= 60 )
      Gameplay_ChangeState( Gameplay_Playing );
}

// ---------------------------------------------------------

void Gameplay_RunState_Playing()
{
    // draw scene
    Gameplay_DrawScene();
    
    // only in this state, run the game
    Game_RunStateMachine();
    
    // check if the game has ended
    if( Game.State == Game_Ended )
    {
        GameScene = Scene_GameOver;
        GameState = GameOver_Initialize;
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

void Gameplay_RunState_FadeOut()
{
    // darken the scene
    Gameplay_DrawScene();
    //
    
    // change scene when done
    //
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
        case Gameplay_Playing:
          Gameplay_RunState_Playing();
          break;
        case Gameplay_FadeOut:
          Gameplay_RunState_FadeOut();
          break;
    }
    
    // count the frames in current state
    Gameplay_ElapsedFrames++;
}
