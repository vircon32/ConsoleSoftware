// *****************************************************************************
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
    #include "TextureGameplay.h"
    #include "Levels.h"
    
    // include all game scenes
    #include "SceneTitle.h"
    #include "SceneGameplay.h"
    
    // include all C files
    // (this compiler has no linker)
    #include "Box.c"
    #include "DrawFunctions.c"
    #include "GameLogic.c"
    #include "Levels.c"
    #include "Physics.c"
    #include "Player.c"
    #include "LevelMap.c"
    #include "TileProperties.c"
// *****************************************************************************


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    DefineRegions_TextureGameplay();
    
    // ---------------------------------------
    // PART 2: CONFIGURE SOUNDS AND CHANNELS
    // ---------------------------------------
    
    // configure music
    select_sound( MusicLevel );
    set_sound_loop( true );
    
    // configure channel volumes
    for( int i = 0; i < 15; i++ )
    {
        select_channel( i );
        set_channel_volume( 0.3 );
    }
    
    select_channel( ChannelMusic );
    set_channel_volume( 0.4 );
    
    // ------------------------------------
    // PART 3: CREATE TEXT FONT
    // ------------------------------------
    
    // define texture and regions for our characters
    FontNumbers.texture_id = TextureGameplay;
    FontNumbers.region_id_zero = FirstRegionNumbers;
    
    // define dimensions for our characters
    FontNumbers.character_width = 13;
    FontNumbers.character_height = 16;
    
    // 1 pixel separation between characters
    FontNumbers.character_separation = 1;
    
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
    
    
    // ------------------------------------
    // PART 5: CREATE TILE MAPS
    // ------------------------------------
    
    // map for levels
    MapLevel.tiles = &TilesLevel;
    MapLevel.map = &CurrentLevelMap.Tiles[ 0 ][ 0 ];
    
    // ------------------------------------
    // PART 6: GAME INITIALIZATIONS
    // ------------------------------------
    
    // configure the console
    srand( get_time() );
    select_gamepad( 0 );
    
    // set initial game state
    CreateGameLevel();
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
            
            case Scene_Gameplay:
              Gameplay_RunStateMachine();
              break;
        }
        
        // wait until next frame
        end_frame();
    }
}
