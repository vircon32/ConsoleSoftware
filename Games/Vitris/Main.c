// include Vircon libraries
#include "video.h"
#include "audio.h"
#include "time.h"
#include "input.h"
#include "misc.h"

// include additional libraries
#include "numberfont.h"
#include "TileMap.h"

// include this project files
#include "Definitions.h"
#include "Globals.h"
#include "TextureTitle.h"
#include "TextureGameplay.h"
#include "GameLogic.h"
#include "SceneTitle.h"
#include "SceneChooseLevel.h"
#include "SceneGameplay.h"
#include "SceneQuitGame.h"
#include "SceneGameOver.h"


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main()
{
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    DefineRegions_TextureTitle();
    DefineRegions_TextureGameplay();
    
    // ---------------------------------------
    // PART 2: CONFIGURE SOUNDS AND CHANNELS
    // ---------------------------------------
    
    // configure music
    select_sound( MusicTitle );
    set_sound_loop( true );
    
    select_sound( MusicGameplay );
    set_sound_loop( true );
    
    // configure channel volumes
    set_global_volume( 0.5 );
    
    select_channel( ChannelMusic );
    set_channel_volume( 0.55 );
    
    // ------------------------------------
    // PART 3: DEFINE FONTS AND TILE MAPS
    // ------------------------------------
    
    // number font to display current level
    FontLevel.character_width = 22;
    FontLevel.character_height = 28;
    FontLevel.character_separation = 5;
    FontLevel.texture_id = TextureGameplay;
    FontLevel.region_id_zero = FirstRegionDigitsLevel;
    
    // number font to display cleared lines
    FontLines.character_width = 15;
    FontLines.character_height = 21;
    FontLines.character_separation = 3;
    FontLines.texture_id = TextureGameplay;
    FontLines.region_id_zero = FirstRegionDigitsLines;
    
    // tile set for the pieces
    TilesPieces.width = 16;
    TilesPieces.height = 16;
    TilesPieces.gap_x = 0;
    TilesPieces.gap_y = 0;
    TilesPieces.texture_id = TextureGameplay;
    TilesPieces.tile_zero_region_id = FirstRegionPieceTiles;
    TilesPieces.draw_tile_zero = false;
    
    // tile map for the title
    MapTitle.tiles = &TilesPieces;
    MapTitle.map = &TitleMapEditable[ 0 ][ 0 ];
    MapTitle.map_width = TitleMapWidth;
    MapTitle.map_height = TitleMapHeight;
    MapTitle.topleft_x = 152;
    MapTitle.topleft_y = 95;
    
    // tile map for the game board
    // (hack it to skip drawing the top 2 extra rows!)
    MapBoard.tiles = &TilesPieces;
    MapBoard.map = &Board[ 2 ][ 0 ];
    MapBoard.map_width = BoardWidth;
    MapBoard.map_height = BoardHeight - 2;
    MapBoard.topleft_x = 240;
    MapBoard.topleft_y = -10;
    
    // this map is identical to the game board
    // but it uses a copy of the array for animations
    MapBoardCopy = MapBoard;
    MapBoardCopy.map = &BoardCopy[ 2 ][ 0 ];
    
    // ---------------------------------------
    // PART 4: GENERAL CONFIGURATIONS
    // ---------------------------------------
    
    // configure the console
    srand( get_time() );
    select_gamepad( 0 );
    
    // set initial game state
    GameScene = Scene_Title;
    GameState = Title_Initialize;
    
    // ------------------------------------
    // PART 5: MAIN LOOP
    // ------------------------------------
    
    // the game is structured in a series of scenes;
    while( true )
    {
        switch( GameScene )
        {
            case Scene_Title:
              Title_RunStateMachine();
              break;
            
            case Scene_Gameplay:
              Gameplay_RunStateMachine();
              break;
            
            case Scene_ChooseLevel:
              ChooseLevel_RunStateMachine();
              break;
            
            case Scene_QuitGame:
              QuitGame_RunStateMachine();
              break;
            
            case Scene_GameOver:
              GameOver_RunStateMachine();
              break;
        }
        
        // wait until next frame
        end_frame();
    }
}
