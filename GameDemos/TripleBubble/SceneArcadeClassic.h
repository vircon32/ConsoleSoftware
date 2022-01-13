// ---------------------------------------------------------
//   ARCADE CLASSIC: STATE MANAGEMENT
// ---------------------------------------------------------


// general state for arcade classic scene
int ArcadeClassic_ElapsedFrames;
int ArcadeClassic_PauseOption;

// game session progress
int ArcadeClassic_Lives;
int ArcadeClassic_Score;
int ArcadeClassic_MaxCombo;
int ArcadeClassic_CurrentWorld;
ClassicLevel* ArcadeClassic_CurrentLevel;

float ArcadeClassic_BackgroundPositionFar;
float ArcadeClassic_BackgroundPositionMiddle;
float ArcadeClassic_BackgroundPositionNear;
float ArcadeClassic_BackgroundSpeed;
float ArcadeClassic_GearAngle;


// ---------------------------------------------------------

// general functions for arcade classic scene
void ArcadeClassic_ChangeState( int NewState )
{
    int OldState = GameState;
    GameState = NewState;
    ArcadeClassic_ElapsedFrames = 0;
    
    // reset pause menu
    if( NewState == Arcade_PauseMenu )
      ArcadeClassic_PauseOption = 0;
    
    // some transitions trigger events
    if( OldState == Arcade_LevelIntro 
    &&  NewState == Arcade_Gameplay )
    {
        Player1.ActionsEnabled = true;
        play_sound_in_channel( MusicWorld1 - 1 + ArcadeClassic_CurrentWorld, ChannelMusic );
        
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
        ArcadeClassic_Lives--;
        
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
//   ARCADE CLASSIC: SPECIFIC GAME PROCESSING
// ---------------------------------------------------------


void ArcadeClassic_SimulateBackground()
{
    // advance all layers
    ArcadeClassic_BackgroundPositionFar -= ArcadeClassic_BackgroundSpeed;
    ArcadeClassic_BackgroundPositionMiddle -= ArcadeClassic_BackgroundSpeed * 2;
    ArcadeClassic_BackgroundPositionNear -= ArcadeClassic_BackgroundSpeed * 3;
    
    // loop all layers
    if( ArcadeClassic_BackgroundPositionFar <= -640 )
      ArcadeClassic_BackgroundPositionFar += 640;
    
    if( ArcadeClassic_BackgroundPositionMiddle <= -640 )
      ArcadeClassic_BackgroundPositionMiddle += 640;
    
    if( ArcadeClassic_BackgroundPositionNear <= -640 )
      ArcadeClassic_BackgroundPositionNear += 640;
}

// ---------------------------------------------------------

void ArcadeClassic_DrawBackground()
{
    select_texture( TextureWorld1 - 1 + ArcadeClassic_CurrentWorld );
    
    select_region( RegionBackgroundFar );
    draw_region_at( ArcadeClassic_BackgroundPositionFar, 0 );
    draw_region_at( ArcadeClassic_BackgroundPositionFar+640, 0 );
    
    select_region( RegionBackgroundMid );
    draw_region_at( ArcadeClassic_BackgroundPositionMiddle, 359 );
    draw_region_at( ArcadeClassic_BackgroundPositionMiddle+640, 359 );
    
    select_region( RegionBackgroundNear );
    draw_region_at( ArcadeClassic_BackgroundPositionNear, 359 );
    draw_region_at( ArcadeClassic_BackgroundPositionNear+640, 359 );
}

// ---------------------------------------------------------

void ArcadeClassic_DrawScoreboard()
{
    select_texture( TextureGameplay );
    
    // write lives
    select_region( RegionFallZero + ArcadeClassic_Lives % 10 );
    draw_region_at( 251,3 );
    
    // write current world
    int AdvancedLevels = ArcadeClassic_CurrentLevel - &ClassicLevel_1_1;
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
        int Digit = (ArcadeClassic_Score/Base) % 10;
        bool GrayOut = (ArcadeClassic_Score < Base && Digit == 0);
        
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

void ArcadeClassic_DrawScenery()
{
    select_texture( TextureGameplay );
    
    // draw gears
    select_region( RegionCeilingGear );
    set_drawing_angle( ArcadeClassic_GearAngle );
    draw_region_rotated_at( 96, -3 );
    draw_region_rotated_at( 643, 39 );
    
    set_drawing_angle( -ArcadeClassic_GearAngle );
    draw_region_rotated_at( 542, -27 );
    
    // if applicable, draw ceiling pistons
    int CeilingDescent = CenterField.CeilingY - CenterField.MinY;
    
    if( CeilingDescent > 0 )
    {
        select_region( RegionDropCylinderBody );
        set_drawing_scale( 1, CeilingDescent );
        draw_region_zoomed_at( 218, 26 );
        draw_region_zoomed_at( 400, 26 );
        
        select_region( RegionDropCylinderTop );
        draw_region_at( 218, 26 );
        draw_region_at( 400, 26 );
        
        select_region( RegionDropCylinderBottom );
        draw_region_at( 218, CenterField.CeilingY - 15 );
        draw_region_at( 400, CenterField.CeilingY - 15 );
    }
    
    // draw scenery
    select_region( RegionClassicScoreboardWorld1 - 1 + ArcadeClassic_CurrentWorld );
    draw_region_at( 163,0 );
    
    select_region( RegionClassicCeilingWorld1 - 1 + ArcadeClassic_CurrentWorld );
    draw_region_at( 163,26 + (CenterField.CeilingY - CenterField.MinY) );
    
    select_region( RegionTripleWallWorld1 - 1 + ArcadeClassic_CurrentWorld );
    draw_region_at( 148,0 );
    draw_region_at( 476,0 );
    
    // populate the scoreboard
    ArcadeClassic_DrawScoreboard();
}

// ---------------------------------------------------------

void DrawBonusNumber( int Number, int DrawX, int DrawY )
{
    int[ 10 ] NumberString;
    itoa( Number, NumberString, 10 );
    int* Digit = NumberString;
    
    while( *Digit )
    {
        select_region( RegionBonusZero + *Digit - '0' );
        draw_region_at( DrawX, DrawY );
        DrawX += 22;
        Digit++;
    }
}


// ---------------------------------------------------------
//   ARCADE CLASSIC: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void ArcadeClassic_RunState_Initialize()
{
    // reset game to initial level
    ArcadeClassic_CurrentWorld = 1;
    ArcadeClassic_CurrentLevel = &ClassicLevel_1_1;
    
    // reset scoreboard
    ArcadeClassic_Lives = 3;
    ArcadeClassic_Score = 0;
    ArcadeClassic_MaxCombo = 0;
    
    // configure playfield
    Playfield_Create( &CenterField, 163, 359, 12, 12 );
    CenterField.ShotsPerDescent = 6;
    
    // configure launcher
    Launcher_Create( &CenterLauncher, &CenterField, 0 );
    CenterLauncher.MinAngle = -9*pi/10;
    CenterLauncher.MaxAngle = -pi/10;
    CenterLauncher.InitialAngle = -pi/2;
    CenterLauncher.AimTrajectoryEnabled = true;
    
    // configure character
    Character_Create( &Player1, 0, &CenterLauncher, NULL, NULL, &PointsP1 );
    
    // transition immediately
    ArcadeClassic_ChangeState( Arcade_InitLevel );
}

// ---------------------------------------------------------

void ArcadeClassic_RunState_InitLevel()
{
    // reset background scroll
    ArcadeClassic_BackgroundPositionFar = 0;
    ArcadeClassic_BackgroundPositionMiddle = 0;
    ArcadeClassic_BackgroundPositionNear = 0;
    ArcadeClassic_BackgroundSpeed = 0;
    
    // reset gears
    ArcadeClassic_GearAngle = 0;
    
    // reset game objects
    Playfield_Reset( &CenterField );
    Launcher_Reset( &CenterLauncher );
    Character_Reset( &Player1 );
    
    // reset combo count
    ArcadeClassic_MaxCombo = 0;
    
    // let objects initialize
    Playfield_RunStateMachine( &CenterField );
    Launcher_RunStateMachine( &CenterLauncher );
    Character_RunStateMachine( &Player1 );
    
    // transition immediately
    ArcadeClassic_ChangeState( Arcade_LevelIntro );
}

// ---------------------------------------------------------

void ArcadeClassic_RunState_LevelIntro()
{
    // progressively add new rows of bubbles
    if( (ArcadeClassic_ElapsedFrames%8) == 1 )
    {
        int Row = ArcadeClassic_ElapsedFrames / 8;
        bool RowHasBubbles = false;
        
        for( int x = 0; x < CenterField.BubblesInX; ++x )
        {
            int ThisBubble = (*ArcadeClassic_CurrentLevel)[ Row ][ x ];
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
    ArcadeClassic_BackgroundSpeed = 0.00625 * ArcadeClassic_ElapsedFrames;
    ArcadeClassic_SimulateBackground();
    
    // when finished, init gameplay
    if( ArcadeClassic_ElapsedFrames >= 73 )
    {
        ArcadeClassic_BackgroundSpeed = 0.5;
        ArcadeClassic_ChangeState( Arcade_Gameplay );
    }
    
    // draw the scene
    ArcadeClassic_DrawBackground();
    ArcadeClassic_DrawScenery();
    Playfield_Draw( &CenterField );
    Launcher_Draw( &CenterLauncher );
    Character_Draw( &Player1 );
    
    // draw the ready text
    if( !((ArcadeClassic_ElapsedFrames/10) % 2) )
    {
        select_texture( TextureGameplay );
        select_region( RegionTextReady );
        draw_region_at( 320,180 );
    }
}

// ---------------------------------------------------------

void ArcadeClassic_RunState_Gameplay()
{
    // let objects update
    Playfield_RunStateMachine( &CenterField );
    Launcher_RunStateMachine( &CenterLauncher );
    Character_RunStateMachine( &Player1 );
    Points_Update( &PointsP1 );
    
    // update gears
    float CeilingDescendedRows = CenterField.CeilingY - CenterField.MinY;
    CeilingDescendedRows /= BubbleDistanceY;
    ArcadeClassic_GearAngle = CeilingDescendedRows * (pi/2);
    
    // launch pause menu when requested
    select_gamepad( 0 );
    
    if( gamepad_button_start() == 1 )
      ArcadeClassic_ChangeState( Arcade_PauseMenu );
    
    // animate bubbles
    if( (ArcadeClassic_ElapsedFrames%12) < 6 )
      FirstRegionBubbles = FirstRegionBubbles1;
    else
      FirstRegionBubbles = FirstRegionBubbles2;
    
    // animate background
    ArcadeClassic_SimulateBackground();
    
    // detect points
    if( PointsP1.ElapsedFrames == 1 )
    {
        ArcadeClassic_Score += PointsP1.BurstPoints;
        ArcadeClassic_Score += PointsP1.FallPoints;
        
        // update max combo
        if( PointsP1.ComboCounter > ArcadeClassic_MaxCombo )
          ArcadeClassic_MaxCombo = PointsP1.ComboCounter;
    }
    
    // detect end of gameplay
    if( CenterField.State == Playfield_LevelClear )
      ArcadeClassic_ChangeState( Arcade_LevelClear );
    
    if( CenterField.State == Playfield_Death )
      ArcadeClassic_ChangeState( Arcade_Death );
    
    // draw scene
    ArcadeClassic_DrawBackground();    
    ArcadeClassic_DrawScenery();
    Playfield_Draw( &CenterField );
    Launcher_Draw( &CenterLauncher );
    Character_Draw( &Player1 );
    Playfield_DrawFallingBubbles( &CenterField );
    Points_Draw( &PointsP1 );
}

// ---------------------------------------------------------

void ArcadeClassic_RunState_LevelClear()
{
    // simulate background with deceleration
    if( ArcadeClassic_BackgroundSpeed > 0 )
      ArcadeClassic_BackgroundSpeed -= 0.00625;
    
    ArcadeClassic_SimulateBackground();
    
    // let objects update
    Character_RunStateMachine( &Player1 );
    Playfield_RunStateMachine( &CenterField );
    Points_Update( &PointsP1 );
    
    // at some point, play clear voice
    if( ArcadeClassic_ElapsedFrames == 70 )
      play_sound( SoundClear );
    
    // when finished, show score
    if( ArcadeClassic_ElapsedFrames >= 180 )
      ArcadeClassic_ChangeState( Arcade_Bonus );
    
    // draw scene
    ArcadeClassic_DrawBackground();    
    ArcadeClassic_DrawScenery();
    Playfield_Draw( &CenterField );
    Launcher_Draw( &CenterLauncher );
    Character_Draw( &Player1 );
    Playfield_DrawFallingBubbles( &CenterField );
    Points_Draw( &PointsP1 );
    
    // lighten the screen
    clear_screen( make_color_rgba( 255,255,255,40 ) );
    
    // from some point, draw clear text
    if( ArcadeClassic_ElapsedFrames >= 70 )
    {
        select_texture( TextureGameplay );
        select_region( RegionTextClear );
        draw_region_at( 320,180 );
    }
}

// ---------------------------------------------------------

void ArcadeClassic_RunState_Death()
{
    // let objects update
    Character_RunStateMachine( &Player1 );
    Playfield_RunStateMachine( &CenterField );
    Points_Update( &PointsP1 );
    
    // at some point, play failed voice
    if( ArcadeClassic_ElapsedFrames == 70 )
      play_sound( SoundFailed );
    
    // remove bubbles from the field
    if( ArcadeClassic_ElapsedFrames % 10 == 1 )
    {
        int Row = 11 - ArcadeClassic_ElapsedFrames/10;
        Row = max( Row, 0 );
        
        for( int Column = 0; Column < CenterField.BubblesInX; ++Column )
          if( CenterField.Bubbles[ Row ][ Column ] > 0 )
          {
              Playfield_CreateFallingBubble( &CenterField, Row, Column, CenterField.Bubbles[ Row ][ Column ] );
              CenterField.Bubbles[ Row ][ Column ] = 0;
          }
    }
    
    // when finished, do a fade out
    if( ArcadeClassic_ElapsedFrames >= 180 )
      ArcadeClassic_ChangeState( Arcade_FadeOut );
    
    // draw scene
    ArcadeClassic_DrawBackground();    
    ArcadeClassic_DrawScenery();
    Playfield_Draw( &CenterField );
    Launcher_Draw( &CenterLauncher );
    Character_Draw( &Player1 );
    Playfield_DrawFallingBubbles( &CenterField );
    Points_Draw( &PointsP1 );
    
    // darken the screen
    clear_screen( make_color_rgba( 0,0,0,92 ) );
    
    // from some point, draw failed text
    if( ArcadeClassic_ElapsedFrames >= 70 )
    {
        select_texture( TextureGameplay );
        select_region( RegionTextFailed );
        draw_region_at( 320,180 );
    }
}

// ---------------------------------------------------------

void ArcadeClassic_RunState_Bonus()
{
    // do not update game objects!
    //
    
    // do timed events
    if( ArcadeClassic_ElapsedFrames == 40 )
      play_sound( SoundMenuAccept );
    
    if( ArcadeClassic_ElapsedFrames == 100 )
    {
        // only combos from X3 earn bonus
        if( ArcadeClassic_MaxCombo >= 3 )
        {
            play_sound( SoundTitleStart );
            int ScoredPoints = ArcadeClassic_MaxCombo * 1000;
            
            // actually score points
            ArcadeClassic_Score += ScoredPoints;
        }
        
        // otherwise play a sad sound
        else
          play_sound( SoundMenuCancel );
    }
    
    // when finished, do a fade out
    if( ArcadeClassic_ElapsedFrames >= 200 )
      ArcadeClassic_ChangeState( Arcade_FadeOut );
    
    // draw scene, with no launcher or character
    ArcadeClassic_DrawBackground();    
    ArcadeClassic_DrawScenery();
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
    if( ArcadeClassic_ElapsedFrames >= 40 )
    {
        set_multiply_color( color_cyan );
        select_region( RegionBonusX );
        draw_region_at( 255,133 );
        DrawBonusNumber( ArcadeClassic_MaxCombo, 277, 133 );
    }
    
    if( ArcadeClassic_ElapsedFrames >= 100 )
    {
        set_multiply_color( color_green );
        select_region( RegionBonusPlus );
        draw_region_at( 255,240 );
        
        int ScoredPoints = 0;
        
        if( ArcadeClassic_MaxCombo >= 3 )
          ScoredPoints = ArcadeClassic_MaxCombo * 1000;
            
        DrawBonusNumber( ScoredPoints, 277, 240 );
    }
    
    set_multiply_color( color_white );
}

// ---------------------------------------------------------

void ArcadeClassic_RunState_FadeOut()
{
    // do not update game objects!
    //
    
    // after finishing, there are multiple options
    if( ArcadeClassic_ElapsedFrames >= 60 )
    {
        // options after level cleared
        if( CenterField.State == Playfield_LevelClear )
        {
            // case 1: advance to next level
            if( ArcadeClassic_CurrentLevel < &ClassicLevel_1_4 )
            {
                ArcadeClassic_CurrentLevel++;
                ArcadeClassic_ChangeState( Arcade_InitLevel );
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
            if( ArcadeClassic_Lives > 0 )
              ArcadeClassic_ChangeState( Arcade_InitLevel );
            
            // case 4: game over
            else
            {
                GameScene = Scene_GameOver;
                GameState = GameOver_Initialize;
            }
            
        }
    }
    
    // draw scene, with no launcher or character
    ArcadeClassic_DrawBackground();    
    ArcadeClassic_DrawScenery();
    Playfield_Draw( &CenterField );
    
    // fade out to black, but start from a certain shadow
    // (otherwise it will not blend well with previous states)
    int AlphaLevel = min( 255, 92 + 163 * ArcadeClassic_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
}

// ---------------------------------------------------------

void ArcadeClassic_RunState_PauseMenu()
{
    // do not update game objects!
    //
    
    // handle menu inputs
    if( gamepad_up() == 1 )
    if( ArcadeClassic_PauseOption > 0 )
    {
        play_sound( SoundMenuMove );
        ArcadeClassic_PauseOption--;
    }
    
    if( gamepad_down() == 1 )
    if( ArcadeClassic_PauseOption < 1 )
    {
        play_sound( SoundMenuMove );
        ArcadeClassic_PauseOption++;
    }
    
    if( gamepad_button_start() == 1 )
    {
        if( ArcadeClassic_PauseOption == 0 )
          ArcadeClassic_ChangeState( Arcade_Gameplay );
          
        else if( ArcadeClassic_PauseOption == 1 )
          ArcadeClassic_ChangeState( Arcade_Quit );
    }
    
    // draw the background only, and darken it
    ArcadeClassic_DrawBackground();
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
    int AnimationFrame = (ArcadeClassic_ElapsedFrames/4) % 8;
    select_region( FirstRegionPauseCursor + AnimationFrame  );
    draw_region_at( 234, 140 + 49*ArcadeClassic_PauseOption );
}

// ---------------------------------------------------------

void ArcadeClassic_RunState_Quit()
{
    // do not update game objects!
    //
    
    // draw the background
    ArcadeClassic_DrawBackground();
    
    // draw window
    select_texture( TextureGameplay );
    select_region( RegionPauseWindow );
    draw_region_at( 194,101 );
    
    // blink selection color
    if( !((ArcadeClassic_ElapsedFrames/4) % 2) )
      set_multiply_color( color_red );
    else
      set_multiply_color( color_white );
    
    // draw only selected option
    select_region( RegionPauseContinue + ArcadeClassic_PauseOption );
    draw_region_at( 243, 140 + 49*ArcadeClassic_PauseOption );
    set_multiply_color( color_white );
    
    // fade-out effect
    int BlackLevel = min( 255, 255 * ArcadeClassic_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    
    // exit
    if( ArcadeClassic_ElapsedFrames >= 60 )
    {
        GameScene = Scene_Title;
        Title_ChangeState( Title_Initialize );
    }
}


// ---------------------------------------------------------
//   ARCADE CLASSIC: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void ArcadeClassic_RunStateMachine()
{
    /*
    // button B changes background
    // (just for testing)
    select_gamepad( 0 );
    
    if( gamepad_button_b() == 1 )
    {
        ArcadeClassic_CurrentWorld++;
        
        if( ArcadeClassic_CurrentWorld > 5 )
          ArcadeClassic_CurrentWorld = 1;
    }
    */
    
    // optimize by checking for common states first
    if( GameState == Arcade_Gameplay )
      ArcadeClassic_RunState_Gameplay();
    
    else if( GameState == Arcade_LevelIntro )
      ArcadeClassic_RunState_LevelIntro();
    
    else if( GameState == Arcade_LevelClear )
      ArcadeClassic_RunState_LevelClear();
    
    else if( GameState == Arcade_Death )
      ArcadeClassic_RunState_Death();
    
    else if( GameState == Arcade_Bonus )
      ArcadeClassic_RunState_Bonus();
    
    else if( GameState == Arcade_FadeOut )
      ArcadeClassic_RunState_FadeOut();
      
    else if( GameState == Arcade_PauseMenu )
      ArcadeClassic_RunState_PauseMenu();
    
    else if( GameState == Arcade_Quit )
      ArcadeClassic_RunState_Quit();
    
    else if( GameState == Arcade_InitLevel )
      ArcadeClassic_RunState_InitLevel();
      
    else if( GameState == Arcade_Initialize )
      ArcadeClassic_RunState_Initialize();
    
    // count time in present state
    ArcadeClassic_ElapsedFrames++;
}
