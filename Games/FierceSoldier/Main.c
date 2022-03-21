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
#include "Levels.h"

// include all game scenes
#include "SceneGameplay.h"
#include "SceneIntro.h"
#include "SceneTitle.h"
#include "SceneEnding.h"
#include "SceneGameOver.h"

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
    select_sound( MusicTitle );
    set_sound_loop( true );
    
    select_sound( MusicStory );
    set_sound_loop( true );
    
    select_sound( MusicGameplay );
    set_sound_loop( true );
    
    // configure channel volumes
    for( int i = 0; i < 15; i++ )
    {
        select_channel( i );
        set_channel_volume( 0.275 );
    }
    
    select_channel( ChannelMusic );
    set_channel_volume( 0.32 );
    
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
    while( true )
    {
        switch( GameScene )
        {
            case Scene_Title:
              Title_RunStateMachine();
              break;
            
            case Scene_Intro:
              Intro_RunStateMachine();
              break;
            
            case Scene_Gameplay:
              Gameplay_RunStateMachine();
              break;
            
            case Scene_GameOver:
              GameOver_RunStateMachine();
              break;
            
            case Scene_Ending:
              Ending_RunStateMachine();
              break;
        }
        
        // wait until next frame
        end_frame();
    }
}
