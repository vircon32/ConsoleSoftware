// include Vircon libraries
#include "time.h"
#include "video.h"
#include "audio.h"
#include "math.h"
#include "input.h"
#include "misc.h"
#include "string.h"

// include this project files
#include "Definitions.h"
#include "TextureRegions.h"
#include "Bubble.h"
#include "Playfield.h"
#include "Points.h"
#include "Launcher.h"
#include "Character.h"
#include "Globals.h"
#include "SceneTitle.h"
#include "SceneArcadeClassic.h"
#include "SceneArcadeTriple.h"
#include "SceneMainMenu.h"
#include "SceneGameOver.h"
#include "SceneEnding.h"


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    DefineTextureRegions();
    
    // ---------------------------------------
    // PART 2: CONFIGURE SOUNDS AND CHANNELS
    // ---------------------------------------
    
    // configure gameplay music
    select_sound( MusicWorld1 );
    set_sound_loop_start( 165339 );
    set_sound_loop( true );
    
    // configure gameplay music
    select_sound( MusicWorld2 );
    set_sound_loop_start( 86553 );
    set_sound_loop( true );
    
    // configure launcher move sound
    select_sound( SoundLauncherMove );
    set_sound_loop( true );
    
    // configure channel volumes
    select_channel( ChannelMusic );
    set_channel_volume( 0.18 );
    
    select_channel( ChannelLauncherMove );
    set_channel_volume( 0.2 );
    
    for( int i = 0; i < ChannelLauncherMove; ++i )
    {
        select_channel( i );
        set_channel_volume( 0.3 );
    }
    
    // ------------------------------------
    // PART 3: PREPARE THE LEVEL
    // ------------------------------------
    
    // configure the console
    srand( get_time() );
    select_gamepad( 0 );
    
    // set initial game state
    GameScene = Scene_Title;
    GameState = Title_Initialize;
    
    // ------------------------------------
    // PART 4: MAIN LOOP
    // ------------------------------------
    
    while( true )
    {
        // optimize by checking for common scenes first
        if( GameScene == Scene_ArcadeClassic )
          ArcadeClassic_RunStateMachine();
        
        else if( GameScene == Scene_ArcadeTriple )
          ArcadeTriple_RunStateMachine();
        
        else if( GameScene == Scene_Title )
          Title_RunStateMachine();
        
        else if( GameScene == Scene_MainMenu )
          MainMenu_RunStateMachine();
        
        else if( GameScene == Scene_GameOver )
          GameOver_RunStateMachine();
        
        else if( GameScene == Scene_Ending )
          Ending_RunStateMachine();
        
        // wait until next frame
        GlobalElapsedFrames++;
        end_frame();
    }
}
