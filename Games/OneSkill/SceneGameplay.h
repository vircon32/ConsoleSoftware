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
        if( OldState != Gameplay_Death )
        {
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
//#include "GameLogic.h"
//#include "DrawFunctions.h"


// ---------------------------------------------------------
//   GAMEPLAY: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Gameplay_RunState_Initialize()
{
    // load the level
    //
    
    // transition immediately
    Gameplay_ChangeState( Gameplay_LevelIntro );
}

// ---------------------------------------------------------

void Gameplay_RunState_LevelIntro()
{
}

// ---------------------------------------------------------

void Gameplay_RunState_Level()
{
}

// ---------------------------------------------------------

void Gameplay_RunState_Pause()
{
}

// ---------------------------------------------------------

void Gameplay_RunState_Death()
{
}

// ---------------------------------------------------------

void Gameplay_RunState_RoomClear()
{
}

// ---------------------------------------------------------

void Gameplay_RunState_LevelClear()
{
}

// ---------------------------------------------------------

void Gameplay_RunState_FadeOut()
{
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
        
        case Gameplay_Pause:
          Gameplay_RunState_Pause();
          break;
        
        case Gameplay_Death:
          Gameplay_RunState_Death();
          break;
        
        case Gameplay_RoomClear:
          Gameplay_RunState_RoomClear();
          break;
        
        case Gameplay_LevelClear:
          Gameplay_RunState_LevelClear();
          break;
        
        case Gameplay_FadeOut:
          Gameplay_RunState_FadeOut();
          break;
    }
    
    // count the frames in current state
    Gameplay_ElapsedFrames++;
}
