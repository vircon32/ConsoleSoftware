// include Vircon libraries
#include "time.h"
#include "video.h"
#include "audio.h"
#include "math.h"
#include "input.h"
#include "misc.h"
#include "string.h"

// include additional libraries
#include "textfont.h"
#include "tilemap.h"

// include this project files
#include "Definitions.h"
#include "TextureGameplay.h"
#include "TextureTitle.h"
#include "TextureCutscenes.h"
#include "TextureFont11x16.h"
#include "Levels.h"

// include all game scenes
/*
#include "SceneGameplay.h"
#include "SceneTitle.h"
#include "SceneTutorial.h"
#include "SceneIntro.h"
#include "SceneEnding.h"
*/


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    DefineRegions_TextureGameplay();
    DefineRegions_TextureTitle();
    DefineRegions_TextureCutscenes();
    DefineRegions_TextureFont11x16();
    
    // ---------------------------------------
    // PART 2: CONFIGURE SOUNDS AND CHANNELS
    // ---------------------------------------
    
    // configure music
    select_sound( MusicTitle );
    set_sound_loop( true );
    
    select_sound( MusicGameplay );
    set_sound_loop( true );
    
    select_sound( MusicCutscenes );
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
    // PART 3: CREATE TEXT FONT
    // ------------------------------------
    
    // ------------------------------------
    // PART 4: CREATE TILE MAPS
    // ------------------------------------
    
    // ------------------------------------
    // PART 5: GAME INITIALIZATIONS
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
            /*
            case Scene_Title:
              Title_RunStateMachine();
              break;
            
            case Scene_Intro:
              Intro_RunStateMachine();
              break;
            
            case Scene_Tutorial:
              Tutorial_RunStateMachine();
              break;
            
            case Scene_Gameplay:
              Gameplay_RunStateMachine();
              break;
            
            case Scene_Ending:
              Ending_RunStateMachine();
              break;
            */
        }
        
        // wait until next frame
        end_frame();
    }
}
