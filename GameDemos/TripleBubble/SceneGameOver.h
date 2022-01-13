int GameOver_ElapsedFrames = 0;


// ---------------------------------------------------------
//   GAME OVER: GENERAL FUNCTIONS
// ---------------------------------------------------------


void GameOver_DrawCharacter()
{
    int AnimationRegion = FirstRegionGameOverChar;
    
    if( GameOver_ElapsedFrames & 16 )
      AnimationRegion++;
    
    select_region( AnimationRegion );
    draw_region_at( 319,311 );
}

// ---------------------------------------------------------

void GameOver_DrawBackground()
{
    // draw the fixed elements
    select_texture( TextureCutscenes );
    clear_screen( make_color_rgb( 95, 105, 120 ) );
    
    select_region( RegionGameOverText );
    draw_region_at( 319,163 );
}


// ---------------------------------------------------------
//   GAME OVER: STATE MACHINE
// ---------------------------------------------------------


void GameOver_ChangeState( int NewState )
{
    GameState = NewState;
    GameOver_ElapsedFrames = 0;
}

// ---------------------------------------------------------

void GameOver_RunState_Initialize()
{
    // (nothing to initialize for now)
    //
    
    // transition immediately
    GameOver_ChangeState( GameOver_Burst );
}

// ---------------------------------------------------------

void GameOver_RunState_Burst()
{
    // draw the background
    GameOver_DrawBackground();
    int AnimationFrame = 0;
    
    // first, draw the bubbles
    if( GameOver_ElapsedFrames < 45 )
    {
        select_region( RegionGameOverBubble );
        
        for( int DrawX = 241; DrawX < 400; DrawX += 52 )
          draw_region_at( DrawX,164 );
    }
    
    // after some time, draw the burst animation
    else
    {
        select_texture( TextureGameplay );
        set_drawing_scale( 2, 2 );
        int AnimationFrame = min( 5, (GameOver_ElapsedFrames - 45) / 4 );
        select_region( FirstRegionBubbleBurst + AnimationFrame );
        
        for( int DrawX = 241; DrawX < 400; DrawX += 52 )
          draw_region_zoomed_at( DrawX,164 );
    }
    
    // at burst start, play burst sound
    if( GameOver_ElapsedFrames == 45 )
      play_sound( SoundBubblesBurst );
    
    // transition when finished
    if( GameOver_ElapsedFrames > 69 )
      GameOver_ChangeState( GameOver_Wait );
}

// ---------------------------------------------------------

void GameOver_RunState_Wait()
{
    // draw the background
    GameOver_DrawBackground();
    
    // character appears shortly after burst
    if( GameOver_ElapsedFrames > 30 )
      GameOver_DrawCharacter();
    
    // after some time, go to fade out
    if( GameOver_ElapsedFrames > 160 )
      GameOver_ChangeState( GameOver_FadeOut );
}

// ---------------------------------------------------------

void GameOver_RunState_FadeOut()
{
    // draw the scene
    GameOver_DrawBackground();
    GameOver_DrawCharacter();
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * GameOver_ElapsedFrames / 120 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
    
    // transition when finished
    if( GameOver_ElapsedFrames >= 120 )
    {
        GameScene = Scene_Title;
        GameState = Title_Initialize;
    }
}

// ---------------------------------------------------------

void GameOver_RunStateMachine()
{
    if( GameState == GameOver_Initialize )
      GameOver_RunState_Initialize();
    
    else if( GameState == GameOver_Burst )
      GameOver_RunState_Burst();
    
    else if( GameState == GameOver_Wait )
      GameOver_RunState_Wait();
    
    else if( GameState == GameOver_FadeOut )
      GameOver_RunState_FadeOut();
    
    // count the elapse time
    GameOver_ElapsedFrames++;
}
