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
#include "SceneTitle.h"
#include "SceneIntro.h"
#include "SceneGameplay.h"
#include "SceneChangeSkill.h"
#include "SceneTutorial.h"
#include "SceneEnding.h"


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
    
    // define texture and regions for our characters
    Font11x16.texture_id = TextureFont11x16;
    Font11x16.character_zero_region_id = FirstRegionFont11x16;
    
    // define dimensions for our characters
    Font11x16.character_height = 16;
    Font11x16.use_variable_width = true;
    textfont_read_region_widths( &Font11x16 );
    
    // no separation between characters, 10 pixels between lines
    Font11x16.character_separation = 0;
    Font11x16.line_separation = 10;
    
    // ------------------------------------
    // PART 4: CREATE TILE SETS
    // ------------------------------------
    
    // tileset for levels
    TilesLevel.width  = 40;
    TilesLevel.height = 40;
    TilesLevel.gap_x = 0;
    TilesLevel.gap_y = 0;
    TilesLevel.texture_id = TextureGameplay;
    TilesLevel.tile_zero_region_id = FirstRegionTileset;
    TilesLevel.draw_tile_zero = false;
    
    // tileset for mountains background
    TilesMountains.width = 160;
    TilesMountains.height = 720;   // not true, but will be useful
    TilesMountains.gap_x = 0;
    TilesMountains.gap_y = 0;
    TilesMountains.texture_id = TextureGameplay;
    TilesMountains.tile_zero_region_id = FirstRegionMountains;
    TilesMountains.draw_tile_zero = true;
    
    // tileset for mountains background
    TilesClouds.width = 160;
    TilesClouds.height = 720;   // not true, but will be useful
    TilesClouds.gap_x = 0;
    TilesClouds.gap_y = 0;
    TilesClouds.texture_id = TextureGameplay;
    TilesClouds.tile_zero_region_id = FirstRegionClouds;
    TilesClouds.draw_tile_zero = true;
    
    // ------------------------------------
    // PART 5: CREATE TILE MAPS
    // ------------------------------------
    /*
    // map for levels
    MapLevel.tiles = &TilesLevel;
    MapLevel.map = &SomeMap[ 0 ][ 0 ]; ??????
    MapLevel.map_width = ???;
    MapLevel.map_height = ???;
    */
    // map for mountains
    MapMountains.tiles = &TilesMountains;
    MapMountains.map = &MapBackgrounds[ 0 ][ 0 ];
    MapMountains.map_width = 16;
    MapMountains.map_height = 1;
    
    // map for clouds
    MapClouds.tiles = &TilesClouds;
    MapClouds.map = &MapBackgrounds[ 0 ][ 0 ];
    MapClouds.map_width = 16;
    MapClouds.map_height = 1;
    
    // ------------------------------------
    // PART 6: GAME INITIALIZATIONS
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
    // PART 7: MAIN LOOP
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
            
            case Scene_Tutorial:
              Tutorial_RunStateMachine();
              break;
            
            case Scene_Gameplay:
              Gameplay_RunStateMachine();
              break;
            
            case Scene_ChangeSkill:
              ChangeSkill_RunStateMachine();
              break;
            
            case Scene_Ending:
              Ending_RunStateMachine();
              break;
        }
        
        // wait until next frame
        end_frame();
    }
}
