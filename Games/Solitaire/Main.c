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
#include "TextureSolitaire.h"
#include "GameLogic.h"
#include "DrawFunctions.h"

// include all game scenes
#include "SceneTitle.h"
#include "SceneMenu.h"
#include "SceneGameplay.h"
#include "SceneQuitGame.h"
#include "SceneWin.h"


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    DefineRegions_TextureSolitaire();
    
    // ---------------------------------------
    // PART 2: CONFIGURE SOUNDS AND CHANNELS
    // ---------------------------------------
    
    // configure music
    select_sound( MusicTitle );
    set_sound_loop( true );
    
    select_sound( MusicGameplay );
    set_sound_loop( true );
    
    // configure channel volumes
    set_global_volume( 0.6 );
    
    select_channel( ChannelMusic );
    set_channel_volume( 0.45 );
    
    // ------------------------------------
    // PART 3: GAME INITIALIZATIONS
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
    
    // the game is structured in a series of scenes;
    while( true )
    {
        switch( GameScene )
        {
            case Scene_Title:
              Title_RunStateMachine();
              break;
            
            case Scene_Menu:
              Menu_RunStateMachine();
              break;
            
            case Scene_Gameplay:
              Gameplay_RunStateMachine();
              break;
            
            case Scene_QuitGame:
              QuitGame_RunStateMachine();
              break;
            
            case Scene_Win:
              Win_RunStateMachine();
              break;
        }
        
        // wait until next frame
        end_frame();
    }
}
