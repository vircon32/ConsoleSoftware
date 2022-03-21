// ---------------------------------------------------------
//   GAME OVER: STATE MANAGEMENT
// ---------------------------------------------------------


int GameOver_ElapsedFrames;

// ---------------------------------------------------------

void GameOver_ChangeState( int NewState )
{
    GameState = NewState;
    GameOver_ElapsedFrames = 0;
}


// ---------------------------------------------------------
//   GAME OVER: DRAW FUNCTIONS
// ---------------------------------------------------------


void GameOver_DrawScene( bool DrawText, int WhitenessLevel )
{
    // solid background
    clear_screen( color_black );
    set_multiply_color( color_white );
    
    // draw character and make it blink
    // (use global timer for smooth transitions)
    if( (get_frame_counter() % 20) < 15 )
    {
        select_texture( TextureGameplay );
        select_region( RegionPlayerDeath );
        draw_region_at( screen_width/2+5,screen_height/2-30 );
    }
    
    // when requested, draw text below the character
    if( DrawText )
      FontLarge_PrintTextCentered( screen_width/2, screen_height/2 + 40, "GAME OVER" );
    
    // darken the drawn scene
    if( WhitenessLevel < 255 )
      clear_screen( make_color_rgba( 0,0,0,255-max(0,WhitenessLevel) ) );
}


// ---------------------------------------------------------
//    GAME OVER: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void GameOver_RunState_Initialize()
{
    // stop music and play game over sound
    stop_channel( ChannelMusic );
    play_sound( SoundDeath );
    
    // transition immediately
    GameOver_ChangeState( GameOver_FadeIn );
}

// ---------------------------------------------------------

void GameOver_RunState_FadeIn()
{
    // draw scene with a fade-in from black
    int WhitenessLevel = max( 0, 255 * GameOver_ElapsedFrames / 60 );
    GameOver_DrawScene( false, WhitenessLevel );
    
    // transition when finished
    if( GameOver_ElapsedFrames >= 60 )
      GameOver_ChangeState( GameOver_TextAppears );
}

// ---------------------------------------------------------

void GameOver_RunState_TextAppears()
{
    // draw scene
    GameOver_DrawScene( true, 255 );
    
    // transition when finished
    if( GameOver_ElapsedFrames >= 140 )
      GameOver_ChangeState( GameOver_FadeOut );
}

// ---------------------------------------------------------

void GameOver_RunState_FadeOut()
{
    // draw scene with a fade-in to black
    int BlacknessLevel = max( 0, 255 * GameOver_ElapsedFrames / 90 );
    GameOver_DrawScene( true, 255 - BlacknessLevel );
    
    // when done, go back to title
    if( GameOver_ElapsedFrames >= 90 )
    {
        GameScene = Scene_Title;
        Title_ChangeState( Title_Initialize );
    }
}


// ---------------------------------------------------------
//   GAME OVER: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void GameOver_RunStateMachine()
{
    if( GameState == GameOver_Initialize )
      GameOver_RunState_Initialize();
    
    else if( GameState == GameOver_FadeIn )
      GameOver_RunState_FadeIn();
    
    else if( GameState == GameOver_TextAppears )
      GameOver_RunState_TextAppears();
    
    else if( GameState == GameOver_FadeOut )
      GameOver_RunState_FadeOut();
    
    // count the frames in current state
    GameOver_ElapsedFrames++;
}
