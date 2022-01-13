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
#include "Text.h"
#include "Levels.h"

// include all game scenes
#include "SceneGameplay.h"
#include "SceneTitle.h"
#include "SceneGameOver.h"
#include "SceneCredits.h"
#include "SceneEnding.h"
#include "SceneWorldClear.h"


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
    
    // configure music
    select_sound( MusicWorld1 );
    set_sound_loop( true );
    set_sound_loop_start( 338730 );
    
    select_sound( MusicWorld2 );
    set_sound_loop( true );
    set_sound_loop_start( 192482 );
    
    select_sound( MusicWorld3 );
    set_sound_loop( true );
    set_sound_loop_start( 184253 );
    
    select_sound( MusicTitle );
    set_sound_loop( true );
    
    // configure channel volumes
    for( int i = 0; i < 15; i++ )
    {
        select_channel( i );
        set_channel_volume( 0.3 );
    }
    
    select_channel( ChannelMusic );
    set_channel_volume( 0.2 );
    
    // ------------------------------------
    // PART 3: GAME INITIALIZATIONS
    // ------------------------------------
    
    // configure the console
    srand( get_time() );
    select_gamepad( 0 );
    
    // set initial game state
    CreateGameLevels();
    ResetGameSession();
    GameScene = Scene_Title;
    GameState = Title_Initialize;
    
    // ------------------------------------
    // PART 4: MAIN LOOP
    // ------------------------------------
    
    // the game is structured in a series of scenes;
    // choice is optimized by checking for common scenes first
    while( true )
    {
        if( GameScene == Scene_Gameplay )
          Gameplay_RunStateMachine();
        
        else if( GameScene == Scene_Title )
          Title_RunStateMachine();
        
        else if( GameScene == Scene_WorldClear )
          WorldClear_RunStateMachine();
        
        else if( GameScene == Scene_GameOver )
          GameOver_RunStateMachine();
        
        else if( GameScene == Scene_Ending )
          Ending_RunStateMachine();
        
        else if( GameScene == Scene_Credits )
          Credits_RunStateMachine();
        
        // wait until next frame
        end_frame();
    }
}
