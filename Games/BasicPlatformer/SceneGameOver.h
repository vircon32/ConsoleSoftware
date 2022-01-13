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


void GameOver_DrawScene( bool DrawTexts, int WhitenessLevel )
{
    // solid background
    clear_screen( make_color_rgb(47,30,75) );
    
    // draw character with a flicker effect
    if( (GameOver_ElapsedFrames % 4) == 0 )
      set_multiply_color( make_color_rgba(255,255,255,128) );
    
    // draw character with its shadow
    select_texture( TextureGame );
    select_region( RegionCharacterShadow );
    draw_region_at( screen_width/2,267 );
    
    select_texture( TextureGame );
    select_region( RegionCharacterLose );
    draw_region_at( screen_width/2,234 );
    
    set_multiply_color( color_white );
    
    // when requested, draw texts above and below the character
    if( DrawTexts )
    {
        DrawTextCentered( screen_width/2,70, "GAME OVER" );
        DrawTextCentered( screen_width/2,323, "PLEASE TRY AGAIN!" );
    }
    
    // darken the drawn scene
    if( WhitenessLevel < 255 )
      clear_screen( make_color_rgba( 0,0,0,255-max(0,WhitenessLevel) ) );
    
    // restore graphic options
    select_texture( TextureGame );
}


// ---------------------------------------------------------
//    GAME OVER: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void GameOver_RunState_Initialize()
{
    // stop music and play game over sound
    stop_channel( ChannelMusic );
    play_sound( MusicGameOver );
    
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
