// ---------------------------------------------------------
//   ARCADE TRIPLE: STATE MANAGEMENT
// ---------------------------------------------------------


// general state for arcade triple scene
int ArcadeTriple_ElapsedFrames;
int ArcadeTriple_PauseOption;

// game session progress
int ArcadeTriple_Lives;
int ArcadeTriple_Score;
int ArcadeTriple_MaxCombo;
int ArcadeTriple_CurrentWorld;
TripleLevel* ArcadeTriple_CurrentLevel;

float ArcadeTriple_BackgroundPositionFar;
float ArcadeTriple_BackgroundPositionMiddle;
float ArcadeTriple_BackgroundPositionNear;
float ArcadeTriple_BackgroundSpeed;
float ArcadeTriple_GearAngle;


// ---------------------------------------------------------

// general functions for arcade triple scene
void ArcadeTriple_ChangeState( int NewState )
{
    int OldState = GameState;
    GameState = NewState;
    ArcadeTriple_ElapsedFrames = 0;
    
    // reset pause menu
    if( NewState == Arcade_PauseMenu )
      ArcadeTriple_PauseOption = 0;
    
    // some transitions trigger events
    if( OldState == Arcade_LevelIntro 
    &&  NewState == Arcade_Gameplay )
    {
        Player1.ActionsEnabled = true;
        play_sound_in_channel( MusicWorld1 - 1 + ArcadeTriple_CurrentWorld, ChannelMusic );
        
        // we need to restart player shoot countdown
        Player1.RemainingShootFrames = 600;
    }
    
    if( NewState == Arcade_LevelIntro )
      play_sound( SoundReady );
    
    if( NewState == Arcade_PauseMenu )
    {
        play_sound( SoundPause );
        pause_channel( ChannelMusic );
    }
    
    if( OldState == Arcade_PauseMenu
    &&  NewState == Arcade_Gameplay )
      play_channel( ChannelMusic );
        
    if( NewState == Arcade_Quit )
    {
        play_sound( SoundMenuCancel );
        stop_channel( ChannelMusic );
    }
    
    if( NewState == Arcade_Death )
    {
        stop_channel( ChannelMusic );
        play_sound( SoundDeath );
        ArcadeTriple_Lives--;
        
        // after a death, aim guide is reenabled
        CenterLauncher.AimTrajectoryEnabled = true;
    }
    
    if( NewState == Arcade_LevelClear )
    {
        stop_channel( ChannelMusic );
        play_sound( SoundLevelClear );
        
        // after a clear, aim guide is disabled
        CenterLauncher.AimTrajectoryEnabled = false;
    }
}


// ---------------------------------------------------------
//   ARCADE TRIPLE: SPECIFIC GAME PROCESSING
// ---------------------------------------------------------


void ArcadeTriple_SimulateBackground()
{
    // advance all layers
    ArcadeTriple_BackgroundPositionFar -= ArcadeTriple_BackgroundSpeed;
    ArcadeTriple_BackgroundPositionMiddle -= ArcadeTriple_BackgroundSpeed * 2;
    ArcadeTriple_BackgroundPositionNear -= ArcadeTriple_BackgroundSpeed * 3;
    
    // loop all layers
    if( ArcadeTriple_BackgroundPositionFar <= -640 )
      ArcadeTriple_BackgroundPositionFar += 640;
    
    if( ArcadeTriple_BackgroundPositionMiddle <= -640 )
      ArcadeTriple_BackgroundPositionMiddle += 640;
    
    if( ArcadeTriple_BackgroundPositionNear <= -640 )
      ArcadeTriple_BackgroundPositionNear += 640;
}

// ---------------------------------------------------------

void ArcadeTriple_DrawBackground()
{
    select_texture( TextureWorld1 - 1 + ArcadeTriple_CurrentWorld );
    
    select_region( RegionBackgroundFar );
    draw_region_at( ArcadeTriple_BackgroundPositionFar, 0 );
    draw_region_at( ArcadeTriple_BackgroundPositionFar+640, 0 );
    
    select_region( RegionBackgroundMid );
    draw_region_at( ArcadeTriple_BackgroundPositionMiddle, 359 );
    draw_region_at( ArcadeTriple_BackgroundPositionMiddle+640, 359 );
    
    select_region( RegionBackgroundNear );
    draw_region_at( ArcadeTriple_BackgroundPositionNear, 359 );
    draw_region_at( ArcadeTriple_BackgroundPositionNear+640, 359 );
}

// ---------------------------------------------------------

void ArcadeTriple_DrawScoreboard()
{
    select_texture( TextureGameplay );
    
    // write lives
    select_region( RegionFallZero + ArcadeTriple_Lives % 10 );
    draw_region_at( 251,3 );
    
    // write current world
    int AdvancedLevels = ArcadeTriple_CurrentLevel - &TripleLevel_1_1;
    int World = (AdvancedLevels / 10) + 1;
    select_region( RegionFallZero + World );
    draw_region_at( 278,3 );
    
    // write current level
    int Level = (AdvancedLevels % 10) + 1;
    select_region( RegionFallZero + Level );
    draw_region_at( 300,3 );
    
    // write score (7 digits)
    int Base = 1000000;
    int DrawX = 327;
    
    while( Base > 0 )
    {
        int Digit = (ArcadeTriple_Score/Base) % 10;
        bool GrayOut = (ArcadeTriple_Score < Base && Digit == 0);
        
        if( GrayOut )
          set_multiply_color( color_gray );
        else
          set_multiply_color( color_white );
        
        select_region( RegionFallZero + Digit );
        draw_region_at( DrawX,3 );
        
        Base /= 10;
        DrawX += 13;
    }
    
    set_multiply_color( color_white );
}

// ---------------------------------------------------------

void ArcadeTriple_DrawScenery()
{
    select_texture( TextureGameplay );
    
    // draw gears
    select_region( RegionCeilingGear );
    set_drawing_angle( ArcadeTriple_GearAngle );
    draw_region_rotated_at( -12, 36 );
    draw_region_rotated_at( 683, 92 );
    
    set_drawing_angle( -ArcadeTriple_GearAngle );
    draw_region_rotated_at( 616, -10 );
    
    // if applicable, draw ceiling pistons
    int CeilingDescent = CenterField.CeilingY - CenterField.MinY;
    
    if( CeilingDescent > 0 )
    {
        select_region( RegionDropCylinderBody );
        set_drawing_scale( 1, CeilingDescent );
        draw_region_zoomed_at( 140, 26 );
        draw_region_zoomed_at( 309, 26 );
        draw_region_zoomed_at( 478, 26 );
        
        select_region( RegionDropCylinderTop );
        draw_region_at( 140, 26 );
        draw_region_at( 309, 26 );
        draw_region_at( 478, 26 );
        
        select_region( RegionDropCylinderBottom );
        draw_region_at( 140, CenterField.CeilingY - 15 );
        draw_region_at( 309, CenterField.CeilingY - 15 );
        draw_region_at( 478, CenterField.CeilingY - 15 );
    }
    
    // draw scenery
    select_region( RegionTripleScoreboardWorld1 - 1 + ArcadeTriple_CurrentWorld );
    draw_region_at( 59,0 );
    
    select_region( RegionTripleCeilingWorld1 - 1 + ArcadeTriple_CurrentWorld );
    draw_region_at( 59,26 + (CenterField.CeilingY - CenterField.MinY) );
    
    select_region( RegionTripleWallWorld1 - 1 + ArcadeTriple_CurrentWorld );
    draw_region_at( 44,0 );
    draw_region_at( 580,0 );
    
    // populate the scoreboard
    ArcadeTriple_DrawScoreboard();
}

// ---------------------------------------------------------

void ArcadeTriple_DrawLauncherSeparators()
{
    select_texture( TextureGameplay );
    select_region( RegionLauncherSeparator );
    draw_region_at( 207,359 );
    draw_region_at( 428,359 );
}


// ---------------------------------------------------------
//   ARCADE TRIPLE: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void ArcadeTriple_RunState_Initialize()
{
    // reset game to initial level
    ArcadeTriple_CurrentWorld = 2;
    ArcadeTriple_CurrentLevel = &TripleLevel_1_1;
    
    // reset scoreboard
    ArcadeTriple_Lives = 3;
    ArcadeTriple_Score = 0;
    ArcadeTriple_MaxCombo = 0;
    
    // configure playfield
    Playfield_Create( &CenterField, 59, 359, 20, 12 );
    CenterField.ShotsPerDescent = 8;
    
    // configure center launcher
    Launcher_Create( &CenterLauncher, &CenterField, 0 );
    CenterLauncher.MinAngle = -pi/2 - 3*pi/10;
    CenterLauncher.MaxAngle = -pi/2 + 3*pi/10;
    CenterLauncher.InitialAngle = -pi/2;
    CenterLauncher.AimTrajectoryEnabled = true;
    CenterLauncher.HasCharacter = false;
    
    // configure left launcher
    Launcher_Create( &LeftLauncher, &CenterField, -221 );
    LeftLauncher.MinAngle = -pi/2 - 3*pi/10;
    LeftLauncher.MaxAngle = -pi/2 + 3*pi/10;
    LeftLauncher.InitialAngle = -pi/2 + 1*pi/10;
    LeftLauncher.AimTrajectoryEnabled = true;
    LeftLauncher.HasCharacter = false;
    
    // configure right launcher
    Launcher_Create( &RightLauncher, &CenterField, +221 );
    RightLauncher.MinAngle = -pi/2 - 3*pi/10;
    RightLauncher.MaxAngle = -pi/2 + 3*pi/10;
    RightLauncher.InitialAngle = -pi/2 - 1*pi/10;
    RightLauncher.AimTrajectoryEnabled = true;
    RightLauncher.HasCharacter = false;
    
    // configure character
    Character_Create( &Player1, 0, &CenterLauncher, &LeftLauncher, &RightLauncher, &PointsP1 );
    
    // transition immediately
    ArcadeTriple_ChangeState( Arcade_InitLevel );
}

// ---------------------------------------------------------

void ArcadeTriple_RunState_InitLevel()
{
    // reset background scroll
    ArcadeTriple_BackgroundPositionFar = 0;
    ArcadeTriple_BackgroundPositionMiddle = 0;
    ArcadeTriple_BackgroundPositionNear = 0;
    ArcadeTriple_BackgroundSpeed = 0;
    
    // reset gears
    ArcadeTriple_GearAngle = 0;
    
    // reset game objects
    Playfield_Reset( &CenterField );
    Launcher_Reset( &CenterLauncher );
    Launcher_Reset( &LeftLauncher );
    Launcher_Reset( &RightLauncher );
    Character_Reset( &Player1 );
    
    // reset combo count
    ArcadeTriple_MaxCombo = 0;
    
    // reset character position
    CenterLauncher.HasCharacter = false;
    LeftLauncher.HasCharacter = false;
    RightLauncher.HasCharacter = false;
    Character_Create( &Player1, 0, &CenterLauncher, &LeftLauncher, &RightLauncher, &PointsP1 );
    
    // let objects initialize
    Playfield_RunStateMachine( &CenterField );
    Launcher_RunStateMachine( &CenterLauncher );
    Launcher_RunStateMachine( &LeftLauncher );
    Launcher_RunStateMachine( &RightLauncher );
    Character_RunStateMachine( &Player1 );
    
    // let only the launcher with a character have bubbles
    LeftLauncher.CurrentBubble.Type = 0;
    LeftLauncher.NextBubble.Type = 0;
    RightLauncher.CurrentBubble.Type = 0;
    RightLauncher.NextBubble.Type = 0;
    
    // transition immediately
    ArcadeTriple_ChangeState( Arcade_LevelIntro );
}

// ---------------------------------------------------------

void ArcadeTriple_RunState_LevelIntro()
{
    // progressively add new rows of bubbles
    if( (ArcadeTriple_ElapsedFrames%8) == 1 )
    {
        int Row = ArcadeTriple_ElapsedFrames / 8;
        bool RowHasBubbles = false;
        
        for( int x = 0; x < CenterField.BubblesInX; ++x )
        {
            int ThisBubble = (*ArcadeTriple_CurrentLevel)[ Row ][ x ];
            CenterField.Bubbles[ Row ][ x ] = ThisBubble;
            
            if( ThisBubble > 0 )
              RowHasBubbles = true;
        }
        
        if( RowHasBubbles )
          play_sound( SoundMenuMove );
    }
    
    // update only the player character
    Character_RunStateMachine( &Player1 );
    
    // simulate background with acceleration
    ArcadeTriple_BackgroundSpeed = 0.00625 * ArcadeTriple_ElapsedFrames;
    ArcadeTriple_SimulateBackground();
    
    // when finished, init gameplay
    if( ArcadeTriple_ElapsedFrames >= 73 )
    {
        ArcadeTriple_BackgroundSpeed = 0.5;
        ArcadeTriple_ChangeState( Arcade_Gameplay );
    }
    
    // draw the scene
    ArcadeTriple_DrawBackground();
    ArcadeTriple_DrawScenery();
    Playfield_Draw( &CenterField );
    ArcadeTriple_DrawLauncherSeparators();
    Launcher_Draw( &CenterLauncher );
    Launcher_Draw( &LeftLauncher );
    Launcher_Draw( &RightLauncher );
    Character_Draw( &Player1 );
    
    // draw the ready text
    if( !((ArcadeTriple_ElapsedFrames/10) % 2) )
    {
        select_texture( TextureGameplay );
        select_region( RegionTextReady );
        draw_region_at( 320,180 );
    }
}

// ---------------------------------------------------------

void ArcadeTriple_RunState_Gameplay()
{
    // let objects update
    Playfield_RunStateMachine( &CenterField );
    Launcher_RunStateMachine( Player1.CurrentLauncher );  // CenterLauncher
    Character_RunStateMachine( &Player1 );
    Points_Update( &PointsP1 );
    
    // update gears
    float CeilingDescendedRows = CenterField.CeilingY - CenterField.MinY;
    CeilingDescendedRows /= BubbleDistanceY;
    ArcadeTriple_GearAngle = CeilingDescendedRows * (pi/2);
    
    // launch pause menu when requested
    select_gamepad( 0 );
    
    if( gamepad_button_start() == 1 )
      ArcadeTriple_ChangeState( Arcade_PauseMenu );
    
    // animate bubbles
    if( (ArcadeTriple_ElapsedFrames%12) < 6 )
      FirstRegionBubbles = FirstRegionBubbles1;
    else
      FirstRegionBubbles = FirstRegionBubbles2;
    
    // animate background
    ArcadeTriple_SimulateBackground();
    
    // detect points
    if( PointsP1.ElapsedFrames == 1 )
    {
        ArcadeTriple_Score += PointsP1.BurstPoints;
        ArcadeTriple_Score += PointsP1.FallPoints;
        
        // update max combo
        if( PointsP1.ComboCounter > ArcadeTriple_MaxCombo )
          ArcadeTriple_MaxCombo = PointsP1.ComboCounter;
    }
    
    // detect end of gameplay
    if( CenterField.State == Playfield_LevelClear )
      ArcadeTriple_ChangeState( Arcade_LevelClear );
    
    if( CenterField.State == Playfield_Death )
      ArcadeTriple_ChangeState( Arcade_Death );
    
    // draw scene
    select_texture( TextureWorld1 - 1 + ArcadeTriple_CurrentWorld );
    ArcadeTriple_DrawBackground();    
    ArcadeTriple_DrawScenery();
    
    Playfield_Draw( &CenterField );
    ArcadeTriple_DrawLauncherSeparators();
    
    select_texture( TextureCharacter1 );
    select_region( RegionPlayerDelimiter );
    draw_region_at( Player1.CurrentLauncher->LaunchPosition.x -110,359 );
    select_texture( TextureGameplay );
    
    Launcher_Draw( &CenterLauncher );
    Launcher_Draw( &LeftLauncher );
    Launcher_Draw( &RightLauncher );
    Character_Draw( &Player1 );
    
    Playfield_DrawFallingBubbles( &CenterField );
    Points_Draw( &PointsP1 );
}

// ---------------------------------------------------------

void ArcadeTriple_RunState_LevelClear()
{
    // simulate background with deceleration
    if( ArcadeTriple_BackgroundSpeed > 0 )
      ArcadeTriple_BackgroundSpeed -= 0.00625;
    
    ArcadeTriple_SimulateBackground();
    
    // let objects update
    Character_RunStateMachine( &Player1 );
    Playfield_RunStateMachine( &CenterField );
    Points_Update( &PointsP1 );
    
    // at some point, play clear voice
    if( ArcadeTriple_ElapsedFrames == 70 )
      play_sound( SoundClear );
    
    // when finished, show score
    if( ArcadeTriple_ElapsedFrames >= 180 )
      ArcadeTriple_ChangeState( Arcade_Bonus );
    
    // draw scene
    ArcadeTriple_DrawBackground();    
    ArcadeTriple_DrawScenery();
    Playfield_Draw( &CenterField );
    ArcadeTriple_DrawLauncherSeparators();
    Launcher_Draw( &CenterLauncher );
    Launcher_Draw( &LeftLauncher );
    Launcher_Draw( &RightLauncher );
    Character_Draw( &Player1 );
    Playfield_DrawFallingBubbles( &CenterField );
    Points_Draw( &PointsP1 );
    
    // lighten the screen
    clear_screen( make_color_rgba( 255,255,255,40 ) );
    
    // from some point, draw clear text
    if( ArcadeTriple_ElapsedFrames >= 70 )
    {
        select_texture( TextureGameplay );
        select_region( RegionTextClear );
        draw_region_at( 320,180 );
    }
}

// ---------------------------------------------------------

void ArcadeTriple_RunState_Death()
{
    // let objects update
    Character_RunStateMachine( &Player1 );
    Playfield_RunStateMachine( &CenterField );
    Points_Update( &PointsP1 );
    
    // at some point, play failed voice
    if( ArcadeTriple_ElapsedFrames == 70 )
      play_sound( SoundFailed );
    
    // remove bubbles from the field
    if( ArcadeTriple_ElapsedFrames % 10 == 1 )
    {
        int Row = 11 - ArcadeTriple_ElapsedFrames/10;
        Row = max( Row, 0 );
        
        for( int Column = 0; Column < CenterField.BubblesInX; ++Column )
          if( CenterField.Bubbles[ Row ][ Column ] > 0 )
          {
              Playfield_CreateFallingBubble( &CenterField, Row, Column, CenterField.Bubbles[ Row ][ Column ] );
              CenterField.Bubbles[ Row ][ Column ] = 0;
          }
    }
    
    // when finished, do a fade out
    if( ArcadeTriple_ElapsedFrames >= 180 )
      ArcadeTriple_ChangeState( Arcade_FadeOut );
    
    // draw scene
    ArcadeTriple_DrawBackground();    
    ArcadeTriple_DrawScenery();
    Playfield_Draw( &CenterField );
    ArcadeTriple_DrawLauncherSeparators();
    Launcher_Draw( &CenterLauncher );
    Launcher_Draw( &LeftLauncher );
    Launcher_Draw( &RightLauncher );
    Character_Draw( &Player1 );
    Playfield_DrawFallingBubbles( &CenterField );
    Points_Draw( &PointsP1 );
    
    // darken the screen
    clear_screen( make_color_rgba( 0,0,0,92 ) );
    
    // from some point, draw failed text
    if( ArcadeTriple_ElapsedFrames >= 70 )
    {
        select_texture( TextureGameplay );
        select_region( RegionTextFailed );
        draw_region_at( 320,180 );
    }
}

// ---------------------------------------------------------

void ArcadeTriple_RunState_Bonus()
{
    // do not update game objects!
    //
    
    // do timed events
    if( ArcadeTriple_ElapsedFrames == 40 )
      play_sound( SoundMenuAccept );
    
    if( ArcadeTriple_ElapsedFrames == 100 )
    {
        // only combos from X3 earn bonus
        if( ArcadeTriple_MaxCombo >= 3 )
        {
            play_sound( SoundTitleStart );
            int ScoredPoints = ArcadeTriple_MaxCombo * 1000;
            
            // actually score points
            ArcadeTriple_Score += ScoredPoints;
        }
        
        // otherwise play a sad sound
        else
          play_sound( SoundMenuCancel );
    }
    
    // when finished, do a fade out
    if( ArcadeTriple_ElapsedFrames >= 200 )
      ArcadeTriple_ChangeState( Arcade_FadeOut );
    
    // draw scene, with no launcher or character
    ArcadeTriple_DrawBackground();    
    ArcadeTriple_DrawScenery();
    Playfield_Draw( &CenterField );
    
    // darken the screen
    clear_screen( make_color_rgba( 0,128,128,128 ) );
    
    // draw fixed bonus elements
    select_region( RegionBonusBar );
    draw_region_at( 203,71 );
    draw_region_at( 203,175 );
    draw_region_at( 203,282 );
    
    select_region( RegionBonusMaxCombo );
    draw_region_at( 215,91 );
    
    select_region( RegionBonusBonus );
    draw_region_at( 215,197 );
    
    // write max combo
    if( ArcadeTriple_ElapsedFrames >= 40 )
    {
        set_multiply_color( color_cyan );
        select_region( RegionBonusX );
        draw_region_at( 255,133 );
        DrawBonusNumber( ArcadeTriple_MaxCombo, 277, 133 );
    }
    
    if( ArcadeTriple_ElapsedFrames >= 100 )
    {
        set_multiply_color( color_green );
        select_region( RegionBonusPlus );
        draw_region_at( 255,240 );
        
        int ScoredPoints = 0;
        
        if( ArcadeTriple_MaxCombo >= 3 )
          ScoredPoints = ArcadeTriple_MaxCombo * 1000;
            
        DrawBonusNumber( ScoredPoints, 277, 240 );
    }
    
    set_multiply_color( color_white );
}

// ---------------------------------------------------------

void ArcadeTriple_RunState_FadeOut()
{
    // do not update game objects!
    //
    
    // after finishing, there are multiple options
    if( ArcadeTriple_ElapsedFrames >= 60 )
    {
        // options after level cleared
        if( CenterField.State == Playfield_LevelClear )
        {
            // case 1: advance to next level
            if( ArcadeTriple_CurrentLevel < &TripleLevel_1_4 )
            {
                ArcadeTriple_CurrentLevel++;
                ArcadeTriple_ChangeState( Arcade_InitLevel );
            }
            
            // case 2: end of game
            else
            {
                GameScene = Scene_Ending;
                GameState = Ending_Initialize;
            }
        }
        
        // options for death
        else
        {
            // case 3: lose a life and restart
            if( ArcadeTriple_Lives > 0 )
              ArcadeTriple_ChangeState( Arcade_InitLevel );
            
            // case 4: game over
            else
            {
                GameScene = Scene_GameOver;
                GameState = GameOver_Initialize;
            }
            
        }
    }
    
    // draw scene, with no launcher or character
    ArcadeTriple_DrawBackground();    
    ArcadeTriple_DrawScenery();
    Playfield_Draw( &CenterField );
    
    // fade out to black, but start from a certain shadow
    // (otherwise it will not blend well with previous states)
    int AlphaLevel = min( 255, 92 + 163 * ArcadeTriple_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
}

// ---------------------------------------------------------

void ArcadeTriple_RunState_PauseMenu()
{
    // do not update game objects!
    //
    
    // handle menu inputs
    if( gamepad_up() == 1 )
    if( ArcadeTriple_PauseOption > 0 )
    {
        play_sound( SoundMenuMove );
        ArcadeTriple_PauseOption--;
    }
    
    if( gamepad_down() == 1 )
    if( ArcadeTriple_PauseOption < 1 )
    {
        play_sound( SoundMenuMove );
        ArcadeTriple_PauseOption++;
    }
    
    if( gamepad_button_start() == 1 )
    {
        if( ArcadeTriple_PauseOption == 0 )
          ArcadeTriple_ChangeState( Arcade_Gameplay );
          
        else if( ArcadeTriple_PauseOption == 1 )
          ArcadeTriple_ChangeState( Arcade_Quit );
    }
    
    // draw the background only, and darken it
    ArcadeTriple_DrawBackground();
    clear_screen( make_color_rgba( 0,0,0,92 ) );
    
    // draw window
    select_texture( TextureGameplay );
    select_region( RegionPauseWindow );
    draw_region_at( 194,101 );
    
    // draw texts
    for( int i = 0; i < 2; ++i )
    {
        select_region( RegionPauseContinue + i );
        draw_region_at( 243, 140 + 49*i );
    }
    
    // draw selection highlight
    int AnimationFrame = (ArcadeTriple_ElapsedFrames/4) % 8;
    select_region( FirstRegionPauseCursor + AnimationFrame  );
    draw_region_at( 234, 140 + 49*ArcadeTriple_PauseOption );
}

// ---------------------------------------------------------

void ArcadeTriple_RunState_Quit()
{
    // do not update game objects!
    //
    
    // draw the background
    ArcadeTriple_DrawBackground();
    
    // draw window
    select_texture( TextureGameplay );
    select_region( RegionPauseWindow );
    draw_region_at( 194,101 );
    
    // blink selection color
    if( !((ArcadeTriple_ElapsedFrames/4) % 2) )
      set_multiply_color( color_red );
    else
      set_multiply_color( color_white );
    
    // draw only selected option
    select_region( RegionPauseContinue + ArcadeTriple_PauseOption );
    draw_region_at( 243, 140 + 49*ArcadeTriple_PauseOption );
    set_multiply_color( color_white );
    
    // fade-out effect
    int BlackLevel = min( 255, 255 * ArcadeTriple_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    
    // exit
    if( ArcadeTriple_ElapsedFrames >= 60 )
    {
        GameScene = Scene_Title;
        Title_ChangeState( Title_Initialize );
    }
}


// ---------------------------------------------------------
//   ARCADE TRIPLE: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void ArcadeTriple_RunStateMachine()
{
    /*
    // button B changes background
    // (just for testing)
    select_gamepad( 0 );
    
    if( gamepad_button_b() == 1 )
    {
        ArcadeTriple_CurrentWorld++;
        
        if( ArcadeTriple_CurrentWorld > 5 )
          ArcadeTriple_CurrentWorld = 1;
    }
    */
    
    // optimize by checking for common states first
    if( GameState == Arcade_Gameplay )
      ArcadeTriple_RunState_Gameplay();
    
    else if( GameState == Arcade_LevelIntro )
      ArcadeTriple_RunState_LevelIntro();
    
    else if( GameState == Arcade_LevelClear )
      ArcadeTriple_RunState_LevelClear();
    
    else if( GameState == Arcade_Death )
      ArcadeTriple_RunState_Death();
    
    else if( GameState == Arcade_Bonus )
      ArcadeTriple_RunState_Bonus();
    
    else if( GameState == Arcade_FadeOut )
      ArcadeTriple_RunState_FadeOut();
      
    else if( GameState == Arcade_PauseMenu )
      ArcadeTriple_RunState_PauseMenu();
    
    else if( GameState == Arcade_Quit )
      ArcadeTriple_RunState_Quit();
    
    else if( GameState == Arcade_InitLevel )
      ArcadeTriple_RunState_InitLevel();
      
    else if( GameState == Arcade_Initialize )
      ArcadeTriple_RunState_Initialize();
    
    // count time in present state
    ArcadeTriple_ElapsedFrames++;
}
