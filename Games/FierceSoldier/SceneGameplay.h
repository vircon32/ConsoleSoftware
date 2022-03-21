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
      play_sound_in_channel( MusicGameplay, ChannelMusic );
    
    if( OldState == Gameplay_Pause
    &&  NewState == Gameplay_Level )
      play_channel( ChannelMusic );
    
    else if( NewState == Gameplay_LevelClear )
    {
        stop_channel( ChannelMusic );
        play_sound( SoundLevelClear );
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
    LoadLevel( &GameLevels[ LevelNumber-1 ] );
    ResetLevel();
    
    // transition immediately
    Gameplay_ChangeState( Gameplay_LevelIntro );
}

// ---------------------------------------------------------

void Gameplay_RunState_LevelIntro()
{
    clear_screen( color_black );
    
    int[ 12 ] TextString = "LEVEL ";
    int[ 3 ] NumberString;
    itoa( LevelNumber, NumberString, 10 );
    strcat( TextString, NumberString );
    
    FontLarge_PrintTextCentered( screen_width/2, screen_height/2+15, TextString );
    
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
    
    // check if player is killed by enemy shots
    Player_DetectShotImpacts( &Player1 );
    
    // FOR DEBUG ONLY: INSTANT WIN BUTTON
    // if( gamepad_button_b() == 1 )
    //   Gameplay_ChangeState( Gameplay_LevelClear );
    
    // ------------------------
    
    // apply gameplay logic
    UpdateLevel();
    
    // move camera to focus on the player
    LevelMap_PositionCamera( &CurrentLevelMap, &Player1.ShapeBox.Position );
    
    // detect player death by fall
    if( Box_Top( &Player1.ShapeBox ) > CurrentLevelMap.TilesInY * TileHeight )
      Gameplay_ChangeState( Gameplay_Death );
    
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
        Vector2D HighlightPosition = Player1.ShapeBox.Position;
        Player1.ShapeBox.Velocity.x = 0;
        
        // impulse is greater when player fell off the
        // screen, so that we can see it
        int LevelHeight = TileHeight * CurrentLevelMap.TilesInY;
        
        if( Player1.ShapeBox.Position.y >= LevelHeight )
        {
            Player1.ShapeBox.Velocity.y = -JumpImpulse * 1;
            HighlightPosition.y -= 70;
        }
        
        else
        {
            Player1.ShapeBox.Velocity.y = -JumpImpulse * 0.5;
            HighlightPosition.y -= 40;
        }
        
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

void Gameplay_RunState_LevelClear()
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
        FontLarge_PrintTextCentered( screen_width/2, screen_height/2+20, "LEVEL CLEAR" );
        select_texture( TextureGameplay );
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
    FontLarge_PrintTextCentered( screen_width/2, screen_height/2+20, "LEVEL CLEAR" );
    
    // fade out to black
    int BlacknessLevel = max( 0, 255 * Gameplay_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,min(255,BlacknessLevel) ));
    
    // when done, change scene depending on the cleared level
    if( Gameplay_ElapsedFrames >= 60 )
    {
        // if there are no more levels, go to world clear scene
        if( LevelNumber >= NumberOfLevels )
        {
            GameScene = Scene_Ending;
            GameState = Story_Initialize;
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
    FontLarge_PrintTextCentered( screen_width/2, screen_height/2+20, "PAUSE" );
    
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
    switch( GameState )
    {
        case Gameplay_Initialize:
          Gameplay_RunState_Initialize();
          break;
          
        case Gameplay_LevelIntro:
          Gameplay_RunState_LevelIntro();
          break;
          
        case Gameplay_Level:
          Gameplay_RunState_Level();
          break;
          
        case Gameplay_Death:
          Gameplay_RunState_Death();
          break;
          
        case Gameplay_LevelClear:
          Gameplay_RunState_LevelClear();
          break;
        
        case Gameplay_Pause:
          Gameplay_RunState_Pause();
          break;
          
        case Gameplay_FadeOut:
          Gameplay_RunState_FadeOut();
          break;
    }
    
    // count the frames in current state
    Gameplay_ElapsedFrames++;
}
