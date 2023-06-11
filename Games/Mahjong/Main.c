// include Vircon libraries
#include "time.h"
#include "video.h"
#include "audio.h"
#include "math.h"
#include "input.h"
#include "misc.h"
#include "string.h"

// include additional libraries
#include "numberfont.h"

// include this project files
#include "Definitions.h"
#include "TextureMahjong.h"
#include "GameLogic.h"
#include "ShuffleLayout.h"
#include "UndoStack.h"
#include "DrawFunctions.h"

// include all game scenes
#include "SceneTitle.h"
#include "SceneChooseLayout.h"
#include "SceneGameplay.h"
#include "SceneQuitGame.h"
#include "SceneWin.h"


// ---------------------------------------------------------
//   AUXILIARY FUNCTIONS
// ---------------------------------------------------------


// In the tile maps for our layouts, the grid splits
// tiles into 4 quarters, so that they can be placed in
// intermediate positions; However, only their top-left
// quarters are actually taken as true tile positions
// and drawn on screen. So, after all tiles are defined
// in a matrix as usual, we just resize the top-left
// regions to cover the whole tile. That way we avoid
// having to draw the 4 quarters individually every time.
void ResizeTopLeftTileRegions()
{
    for( int y = 0; y < 10; y += 2 )
    for( int x = 0; x < 18; x += 2 )
    {
        select_region( FirstRegionTileFronts + 18*y + x );
        
        asm
        {
            // increase region width
            "in R0, GPU_RegionMaxX"
            "iadd R0, 15"
            "out GPU_RegionMaxX, R0"
            
            // increase region width
            "in R0, GPU_RegionMaxY"
            "iadd R0, 20"
            "out GPU_RegionMaxY, R0"
        }
    }
}


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    DefineRegions_TextureMahjong();
    ResizeTopLeftTileRegions();
    
    // ---------------------------------------
    // PART 2: CONFIGURE SOUNDS AND CHANNELS
    // ---------------------------------------
    
    // configure music
    select_sound( MusicTitle );
    set_sound_loop( true );
    
    select_sound( MusicGameplay );
    set_sound_loop( true );
    
    // configure channel volumes
    set_global_volume( 0.65 );
    
    select_channel( ChannelMusic );
    set_channel_volume( 0.3 );
    
    // ------------------------------------
    // PART 3: DEFINE FONTS AND TILE MAPS
    // ------------------------------------
    
    // number font to display remaining tiles
    FontTiles.character_width = 11;
    FontTiles.character_height = 16;
    FontTiles.character_separation = 0;
    FontTiles.texture_id = TextureMahjong;
    FontTiles.region_id_zero = FirstRegionDigits;
    
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
            
            case Scene_ChooseLayout:
              ChooseLayout_RunStateMachine();
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
