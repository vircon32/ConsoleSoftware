// *****************************************************************************
    // start include guard
    #ifndef SCENE_GAMEPLAY_H
    #define SCENE_GAMEPLAY_H
    
    // include project headers
    #include "Globals.h"
    #include "GameLogic.h"
    #include "DrawFunctions.h"
    #include "Physics.h"
// *****************************************************************************


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
    if( NewState == Gameplay_Countdown )
    {
        // load the room (careful: numbering starts at 1)
        LoadLevel( &GameLevel );
        ResetLevel();
        
        // position camera over the player
        LevelMap_PositionCamera( &CurrentLevelMap );
    }
    
    else if( NewState == Gameplay_Level )
      play_sound_in_channel( MusicLevel, ChannelMusic );
}


// ---------------------------------------------------------
//   GAMEPLAY: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Gameplay_RunState_Initialize()
{
    // transition immediately
    Gameplay_ChangeState( Gameplay_Countdown );
}

// ---------------------------------------------------------

void Gameplay_RunState_Countdown()
{
    if( Gameplay_ElapsedFrames < 60 )
      clear_screen( color_black );
  
    // fade-in effect after a while
    else
    {
        DrawCurrentLevel();
        DrawGUI();
        
        int BlackLevel = max( 0, 255 - 255 * (Gameplay_ElapsedFrames-60) / 60 );
        clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    }
    
    // do the countdown
    int RemainingSeconds = ((180 - Gameplay_ElapsedFrames) / 60) + 1;
    set_drawing_scale( 4, 4 );
    select_region( FirstRegionNumbers + RemainingSeconds );
    draw_region_zoomed_at( screen_width/2, screen_height/2 );
    
    // countdown sound
    if( (180 - Gameplay_ElapsedFrames) % 60 == 59 )
      play_sound( SoundCountdown );
    
    // when finished start the level
    if( Gameplay_ElapsedFrames >= 180 )
      Gameplay_ChangeState( Gameplay_Level );
}

// ---------------------------------------------------------

void Gameplay_RunState_Level()
{
    for( int i = 0; i < 4; ++i )
    {
        // (1) determine player movement, but don't apply it
        Player_DetermineMovement( &Players[ i ] );
        
        // (2) restrict calculated movements by colliding with level
        CollidePlayerWithLevel( &Players[ i ], &CurrentLevelMap );
        EnforceLevelBoundaries( &Players[ i ], &CurrentLevelMap );
        
        // (3) further restrict movement colliding players
        //
    }
    
    // (4) apply the restricted movements, that ensure no penetration
    for( int i = 0; i < 4; ++i )
      Player_ApplyMovement( &Players[ i ] );
    
    // ------------------------
    
    // apply gameplay logic
    UpdateLevel();
    
    // move camera to focus on the player
    LevelMap_PositionCamera( &CurrentLevelMap );
    
    // draw the scene
    DrawCurrentLevel();
    DrawGUI();
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
        
        case Gameplay_Countdown:
          Gameplay_RunState_Countdown();
          break;
        
        case Gameplay_Level:
          Gameplay_RunState_Level();
          break;
    }
    
    // count the frames in current state
    Gameplay_ElapsedFrames++;
}


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
