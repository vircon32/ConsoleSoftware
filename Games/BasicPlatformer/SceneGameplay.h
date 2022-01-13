// ---------------------------------------------------------
//   GAMEPLAY: STATE MANAGEMENT
// ---------------------------------------------------------


int Gameplay_ElapsedFrames;

// ---------------------------------------------------------

void Gameplay_ChangeState( int NewState )
{
    int OldState = GameState;
    GameState = NewState;
    Gameplay_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( OldState == Gameplay_LevelIntro
    &&  NewState == Gameplay_Level )
      play_sound_in_channel( MusicWorld1 + WorldNumber - 1, ChannelMusic );
    
    if( OldState == Gameplay_Pause
    &&  NewState == Gameplay_Level )
      play_channel( ChannelMusic );
    
    else if( NewState == Gameplay_Goal )
    {
        stop_channel( ChannelMusic );
        play_sound( SoundGoal );
    }
    
    else if( NewState == Gameplay_Death )
    {
        // we must prevent multiple death events!!
        // (for instance touching 2 spikes on the same frame)
        // this would make the player lose more than 1 life
        if( OldState != Gameplay_Death )
        {
            PlayerLives--;
            stop_channel( ChannelMusic );
            play_sound( SoundDeath );
        }
    }
    
    else if( NewState == Gameplay_Pause )
    {
        pause_channel( ChannelMusic );
        play_sound( SoundPause );
    }
}

// ---------------------------------------------------------

// only now include general game headers,
// since they will need to change gameplay state;
// this needs to be done this way because the
// compiler does not yet support declarations
// without a full definition (as in true header files)
#include "GameLogic.h"
#include "DrawFunctions.h"


// ---------------------------------------------------------
//   GAMEPLAY: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Gameplay_RunState_Initialize()
{
    // load the level
    int LevelArrayPosition = LevelsPerWorld * (WorldNumber-1) + (LevelNumber-1);
    LoadLevel( &GameLevels[ LevelArrayPosition ] );
    ResetLevel();
    
    // transition immediately
    Gameplay_ChangeState( Gameplay_LevelIntro );
}

// ---------------------------------------------------------

void Gameplay_RunState_LevelIntro()
{
    clear_screen( color_black );
    
    int[ 12 ] TextString;
    int[ 3 ] NumberString;
    strcpy( TextString, "LEVEL " );
    itoa( WorldNumber, NumberString, 10 );
    strcat( TextString, NumberString );
    strcat( TextString, "-" );
    itoa( LevelNumber, NumberString, 10 );
    strcat( TextString, NumberString );
    
    DrawTextCentered( screen_width/2, screen_height/2+15, TextString );
    
    if( Gameplay_ElapsedFrames >= 120 )
      Gameplay_ChangeState( Gameplay_Level );
}

// ---------------------------------------------------------

void Gameplay_RunState_Level()
{
    // (1) determine player movement, but don't apply it
    Player_DetermineMovement( &Player1 );
    
    // (2) restrict calculated movements by colliding with level
    CollidePlayerWithLevel( &Player1, &CurrentLevelMap );
    EnforceLevelBoundaries( &Player1, &CurrentLevelMap );
    
    // (3) apply the restricted movements, that ensure no penetration
    Player_ApplyMovement( &Player1 );
    
    // FOR DEBUG ONLY: INSTANT WIN BUTTON
    // if( gamepad_button_b() == 1 )
    //   Gameplay_ChangeState( Gameplay_Goal );
    
    // ------------------------
    
    // apply gameplay logic
    UpdateLevel();
    
    // move camera to focus on the player
    LevelMap_PositionCamera( &CurrentLevelMap, &Player1.ShapeBox.Position );
    
    // detect player death by fall
    if( Box_Top( &Player1.ShapeBox ) > CurrentLevelMap.TilesInY * TileHeight )
      Gameplay_ChangeState( Gameplay_Death );
    
    // decrease timer, and detect death by time
    RemainingTimeFrames--;
    
    if( RemainingTimeFrames <= 0 )
      Gameplay_ChangeState( Gameplay_Death );
    
    // warn when little time is remaining
    if( RemainingTimeFrames == (20*frames_per_second) )
      play_sound( SoundHurry );
    
    // now that all elements are updated, animate
    // the player according to the situation
    Player_UpdateAnimation( &Player1 );
    
    // button start pauses the game
    if( gamepad_button_start() == 1 )
      Gameplay_ChangeState( Gameplay_Pause );
    
    // ------------------------
    
    // draw the scene
    DrawLevel();
    DrawGUI();
}

// ---------------------------------------------------------

void Gameplay_RunState_Death()
{
    // give player an initial impulse upwards
    if( Gameplay_ElapsedFrames == 1 )
    {
        Player1.ShapeBox.Velocity.y = -JumpImpulse * 0.75;
        Player1.ShapeBox.Velocity.x = 0;
        
        Vector2D HighlightPosition = Player1.ShapeBox.Position;
        HighlightPosition.y -= 20;
        CreateHighlight( &HighlightPosition );
    }
    
    // apply gravity over player, with no collisions
    Box_BeginNewFrame( &Player1.ShapeBox );
    Player1.ShapeBox.Acceleration.y = Gravity;
    Box_Simulate( &Player1.ShapeBox );
    Box_Move( &Player1.ShapeBox );
    
    // update highlights
    for( int i = 0; i < 5; i++ )
      Highlight_Update( &Highlights[ i ] );
    
    // draw scene
    DrawLevel();
    DrawGUI();
    
    // clear screen with transparency for dark red  effect
    clear_screen( make_color_rgba(75,0,0,160) );
    
    // if death happened because of time, report to player
    if( RemainingTimeFrames <= 0 )
      if( !(Gameplay_ElapsedFrames & 16) || Gameplay_ElapsedFrames > 64 )
        {
            DrawTextCentered( screen_width/2, screen_height/2+15, "TIME'S UP!" );
            select_texture( TextureGame );
        }
    
    // redraw player over the scene to highlight it
    Player_Draw( &Player1, &CurrentLevelMap.TopLeftOnScreen );
    
    // change context depending on the situation
    if( Gameplay_ElapsedFrames >= 120 )
    {
        // when player has more lives, restart the level
        if( PlayerLives > 0 )
          Gameplay_ChangeState( Gameplay_Initialize );
        
        // otherwise trigger a game over
        else
        {
            GameScene = Scene_GameOver;
            GameState = GameOver_Initialize;
        }
    }
}

// ---------------------------------------------------------

void Gameplay_RunState_Goal()
{
    // animate player
    Player_UpdateAnimation( &Player1 );
    
    // update highlights
    for( int i = 0; i < 5; i++ )
      Highlight_Update( &Highlights[ i ] );
    
    // draw scene
    DrawLevel();
    DrawGUI();
    
    // show clear text and make it blink
    if( !(Gameplay_ElapsedFrames & 16) || Gameplay_ElapsedFrames > 64 )
    {
        DrawTextCentered( screen_width/2, screen_height/2+50, "CLEAR" );
        select_texture( TextureGame );
    }
    
    // when done, change scene
    if( Gameplay_ElapsedFrames >= 120 )
      Gameplay_ChangeState( Gameplay_FadeOut );
}

// ---------------------------------------------------------

void Gameplay_RunState_FadeOut()
{
    // draw scene with clear text
    DrawLevel();
    DrawGUI();
    DrawTextCentered( screen_width/2, screen_height/2+50, "CLEAR" );
    
    // fade out to black
    int BlacknessLevel = max( 0, 255 * Gameplay_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,min(255,BlacknessLevel) ));
    
    // when done, change scene depending on the cleared level
    if( Gameplay_ElapsedFrames >= 60 )
    {
        // if there are no more levels, go to world clear scene
        if( LevelNumber >= LevelsPerWorld )
        {
            GameScene = Scene_WorldClear;
            GameState = WorldClear_Initialize;
        }
        
        // otherwise begin gameplay on next level
        else
        {
            LevelNumber++;
            Gameplay_ChangeState( Gameplay_Initialize );
        }
    }    
}

// ---------------------------------------------------------

void Gameplay_RunState_Pause()
{
    // draw the scene normally
    DrawLevel();
    DrawGUI();
    
    // darken the screen and write a pause text
    clear_screen( make_color_rgba(0,0,0,92) );
    DrawTextCentered( screen_width/2, screen_height/2+20, "PAUSE" );
    
    // avoid pausing & resuming too quickly
    // in order to prevent unforeseen bugs
    if( Gameplay_ElapsedFrames < 5 )
      return;
    
    if( gamepad_button_start() == 1 )
      Gameplay_ChangeState( Gameplay_Level );
}


// ---------------------------------------------------------
//   GAMEPLAY: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Gameplay_RunStateMachine()
{
    if( GameState == Gameplay_Initialize )
      Gameplay_RunState_Initialize();
    
    else if( GameState == Gameplay_LevelIntro )
      Gameplay_RunState_LevelIntro();
    
    else if( GameState == Gameplay_Level )
      Gameplay_RunState_Level();
    
    else if( GameState == Gameplay_Death )
      Gameplay_RunState_Death();
    
    else if( GameState == Gameplay_Goal )
      Gameplay_RunState_Goal();
    
    else if( GameState == Gameplay_FadeOut )
      Gameplay_RunState_FadeOut();
    
    else if( GameState == Gameplay_Pause )
      Gameplay_RunState_Pause();
    
    // count the frames in current state
    Gameplay_ElapsedFrames++;
}
