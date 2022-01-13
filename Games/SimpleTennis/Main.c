// include Vircon libraries
#include "video.h"
#include "audio.h"
#include "input.h"
#include "time.h"
#include "string.h"
#include "math.h"
#include "misc.h"

// include this project files
#include "Definitions.h"
#include "Text.h"
#include "TextureRegions.h"
#include "GameLogic.h"
#include "SceneGameplay.h"
#include "SceneTitle.h"

// -----------------------------------------------
// MAIN FUNCTION
// -----------------------------------------------

void main( void )
{  
	// ------------------------------------
  // PART 1: DEFINE ALL TEXTURE REGIONS
  // ------------------------------------
    
  DefineTextureRegions();
    
  // -----------------------------------------------
  // PART 2: INITIALIZATIONS
  // ------------------------------------
  
	// start randomizer for initial angle
	srand( get_time() ); 

	// set initial game state
  GameScene = Scene_Title;
	GameState = Title_Start;
   
  // -----------------------------------------------
  // PART 3: MAIN LOOP
    
  // our game never ends: keep repeating
  // our game logic for every frame (60 fps)
  while( true )
  {
		if( GameScene == Scene_GamePlay )
      Gameplay_RunStateMachine();
    else if( GameScene == Scene_Title )
      Title_RunStateMachine();
    end_frame();
  }
}



