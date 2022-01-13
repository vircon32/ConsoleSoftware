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
#include "GameLogic.h"
#include "DrawFunctions.h"
#include "TextureRegions.h"
#include "SceneTitle.h"
#include "SceneGameplay.h"
#include "SceneQuitGame.h"
#include "SceneLose.h"
#include "SceneWin2048.h"
#include "SceneWin65536.h"


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
    
    select_sound( MusicGameplay );
    set_sound_loop( true );
    
    // configure channel volumes
    for( int i = 0; i < 15; i++ )
    {
        select_channel( i );
        set_channel_volume( 0.4 );
    }
    
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
    // choice is optimized by checking for common scenes first
    while( true )
    {
        if( GameScene == Scene_Gameplay )
          Gameplay_RunStateMachine();
        
        else if( GameScene == Scene_Title )
          Title_RunStateMachine();
        
        else if( GameScene == Scene_QuitGame )
          QuitGame_RunStateMachine();
        
        else if( GameScene == Scene_Lose )
          Lose_RunStateMachine();
        
        else if( GameScene == Scene_Win2048 )
          Win2048_RunStateMachine();
        
        else if( GameScene == Scene_Win65536 )
          Win65536_RunStateMachine();
        
        // wait until next frame
        end_frame();
    }
}
