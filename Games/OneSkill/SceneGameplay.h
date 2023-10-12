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
//   GAMEPLAY: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Gameplay_RunState_Initialize()
{
    // load the room (careful: numbering starts at 1)
    LoadRoom( &GameLevels[ LevelNumber - 1 ].Rooms[ RoomNumber - 1 ] );
    ResetRoom();
    
    // transition immediately
    Gameplay_ChangeState( Gameplay_LevelIntro );
}

// ---------------------------------------------------------

void Gameplay_RunState_LevelIntro()
{
    clear_screen( color_black );
    
    // write current level and room
    select_texture( TextureGameplay );
    int TextY = screen_height/2-15;
    
    select_region( RegionTextLevel );
    draw_region_at( 220, TextY );
    
    select_region( FirstRegionDigits + LevelNumber );
    draw_region_at( 350, TextY );
    
    select_region( RegionDashCharacter );
    draw_region_at( 374, TextY );
    
    select_region( FirstRegionDigits + RoomNumber );
    draw_region_at( 398, TextY );
    
    if( Gameplay_ElapsedFrames >= 120 )
      Gameplay_ChangeState( Gameplay_Level );
}

// ---------------------------------------------------------

void Gameplay_RunState_Level()
{
    // (1) determine player movement, but don't apply it
    Player_DetermineMovement( &Player1 );
    
    // (2) restrict calculated movements by colliding with room
    CollidePlayerWithRoom( &Player1, &CurrentRoomMap );
    EnforceRoomBoundaries( &Player1, &CurrentRoomMap );
    
    // (3) apply the restricted movements, that ensure no penetration
    Player_ApplyMovement( &Player1 );
    
    // FOR DEBUG ONLY: INSTANT WIN BUTTON
    // if( gamepad_button_b() == 1 )
    //   Gameplay_ChangeState( Gameplay_Goal );
    
    // ------------------------
    
    // apply gameplay logic
    UpdateRoom();
    
    // move camera to focus on the player
    RoomMap_PositionCamera( &CurrentRoomMap );
    
    // detect player death by fall
    if( Box_Top( &Player1.ShapeBox ) > CurrentRoomMap.TilesInY * TileHeight )
      Gameplay_ChangeState( Gameplay_Death );
    
    // now that all elements are updated, animate
    // the player according to the situation
    Player_UpdateAnimation( &Player1 );
    
    // button start pauses the game
    if( gamepad_button_start() == 1 )
      Gameplay_ChangeState( Gameplay_Pause );
    
    // ------------------------
    
    // draw the scene
    DrawCurrentRoom();
    DrawGUI();
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


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
