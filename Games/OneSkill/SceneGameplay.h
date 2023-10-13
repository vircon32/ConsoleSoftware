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
    
    if( OldState == Gameplay_RoomIntro
    &&  NewState == Gameplay_Level )
      play_sound_in_channel( MusicGameplay, ChannelMusic );
    
    if( OldState == Gameplay_Pause
    &&  NewState == Gameplay_Level )
      play_channel( ChannelMusic );
    
    if( NewState == Gameplay_RoomClear )
    {
        stop_channel( ChannelMusic );
        play_sound( SoundMenuAccept );
        
        Player1.ShapeBox.Position.x = RoomExit.EventBox.Position.x + 20;
        Player1.ShapeBox.Position.y = RoomExit.EventBox.Position.y + 40;
        Player1.Skill = Skill_None;
        
        RoomExit.ShowingSign = false;
    }
    
    else if( NewState == Gameplay_LevelClear )
    {
        stop_channel( ChannelMusic );
        play_sound( SoundLevelClear );
        
        Player1.ShapeBox.Position.x = RoomExit.EventBox.Position.x + 20;
        Player1.ShapeBox.Position.y = RoomExit.EventBox.Position.y + 40;
        Player1.Skill = Skill_None;
        
        RoomExit.ShowingSign = false;
    }
    
    else if( NewState == Gameplay_LevelIntro || NewState == Gameplay_RoomIntro )
    {
        // load the room (careful: numbering starts at 1)
        LoadRoom( &GameLevels[ LevelNumber - 1 ].Rooms[ RoomNumber - 1 ] );
        ResetRoom();
        
        // position camera over the player
        RoomMap_PositionCamera( &CurrentRoomMap );
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
    // transition immediately
    Gameplay_ChangeState( Gameplay_LevelIntro );
}

// ---------------------------------------------------------

void Gameplay_RunState_LevelIntro()
{
    if( Gameplay_ElapsedFrames < 60 )
      clear_screen( color_black );
  
    // fade-in effect after a while
    else
    {
        DrawCurrentRoom();
        DrawGUI();
        
        int BlackLevel = max( 0, 255 - 255 * (Gameplay_ElapsedFrames-60) / 60 );
        clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    }
    
    // write current level and room
    select_texture( TextureGameplay );
    int TextY = screen_height/2-15;
    
    select_region( RegionTextLevel );
    draw_region_at( 240, TextY );
    
    select_region( FirstRegionDigits + LevelNumber );
    draw_region_at( 370, TextY );
    
    if( Gameplay_ElapsedFrames >= 120 )
      Gameplay_ChangeState( Gameplay_Level );
}

// ---------------------------------------------------------

void Gameplay_RunState_RoomIntro()
{
    // draw the scene
    DrawCurrentRoom();
    DrawGUI();
    
    // fade-in effect
    int BlackLevel = max( 0, 255 - 255 * Gameplay_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    
    // when finished go back to level
    if( Gameplay_ElapsedFrames >= 60 )
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
    
    // ------------------------
    
    // apply gameplay logic
    UpdateRoom();
    
    // move camera to focus on the player
    RoomMap_PositionCamera( &CurrentRoomMap );
    
    // detect level and room clear
    if( RoomExit.WasEntered )
    {
        if( RoomExit.IsLevelDoor )
          Gameplay_ChangeState( Gameplay_LevelClear );
        else
          Gameplay_ChangeState( Gameplay_RoomClear );
    }
    
    // detect player death by fall
    else if( Box_Top( &Player1.ShapeBox ) > CurrentRoomMap.TilesInY * TileHeight )
      Gameplay_ChangeState( Gameplay_Death );
    
    // detect player death caused by other objects
    else if( Player1.IsDead )
      Gameplay_ChangeState( Gameplay_Death );
    
    // now that all elements are updated, animate
    // the player according to the situation
    Player_UpdateAnimation( &Player1 );
    
    // button start pauses the game
    if( gamepad_button_start() == 1 )
    {
        // prevent pause if any other event happened
        if( GameState == Gameplay_Level )
          Gameplay_ChangeState( Gameplay_Pause );
    }
    
    // ------------------------
    
    // draw the scene
    DrawCurrentRoom();
    DrawGUI();
}

// ---------------------------------------------------------

void Gameplay_RunState_Pause()
{
    // draw the scene normally
    DrawCurrentRoom();
    DrawGUI();
    
    // darken the screen and draw the pause text
    clear_screen( make_color_rgba(0,0,0,92) );
    select_region( RegionTextPause );
    draw_region_at( screen_width/2, screen_height/2 );
    
    // avoid pausing & resuming too quickly
    // in order to prevent unforeseen bugs
    if( Gameplay_ElapsedFrames < 5 )
      return;
    
    if( gamepad_button_start() == 1 )
      Gameplay_ChangeState( Gameplay_Level );
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
    DrawCurrentRoom();
    DrawGUI();
    
    // clear screen with transparency for dark red effect
    clear_screen( make_color_rgba(75,0,0,160) );
    
    // redraw player over the scene to highlight it
    Player_Draw( &Player1, &CurrentRoomMap.TopLeftOnScreen );
    
    // restart the level from room 1
    if( Gameplay_ElapsedFrames >= 120 )
    {
        RoomNumber = 1;
        Gameplay_ChangeState( Gameplay_LevelIntro );
    }
}

// ---------------------------------------------------------

void Gameplay_RunState_RoomClear()
{
    // update player animation
    Player_UpdateAnimation( &Player1 );
    
    // draw the scene
    DrawCurrentRoom();
    DrawGUI();
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * Gameplay_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
    
    // when finished, go to next room
    if( Gameplay_ElapsedFrames >= 60 )
    {
        RoomNumber++;
        Gameplay_ChangeState( Gameplay_RoomIntro );
    }
}

// ---------------------------------------------------------

void Gameplay_RunState_LevelClear()
{
    // update player animation
    Player_UpdateAnimation( &Player1 );
    
    // draw the scene
    DrawCurrentRoom();
    DrawGUI();
    
    // write level clear text
    select_region( RegionTextLevel );
    draw_region_at( 200, screen_height/2 );
    
    select_region( RegionTextClear );
    draw_region_at( 330, screen_height/2 );
    
    // fade out after a while
    if( Gameplay_ElapsedFrames >= 220 )
    {
        int AlphaLevel = min( 255, 255 * (Gameplay_ElapsedFrames-220) / 60 );
        clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
    }
    
    // when finished, go to next level
    if( Gameplay_ElapsedFrames >= 280 )
    {
        LevelNumber++;
        
        // continue the game
        if( LevelNumber <= NumberOfLevels )
        {
            RoomNumber = 1;
            Gameplay_ChangeState( Gameplay_Initialize );
        }
        
        // the whole game was cleared
        else
        {
            GameScene = Scene_Ending;
            GameState = Story_Initialize;
        }
    }
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
        
        case Gameplay_RoomIntro:
          Gameplay_RunState_RoomIntro();
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
