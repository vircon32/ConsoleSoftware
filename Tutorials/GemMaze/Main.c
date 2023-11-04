// *****************************************************************************
    // include standard Vircon32 libraries
    #include "math.h"
    #include "time.h"
    #include "input.h"
    #include "video.h"
    #include "audio.h"
    
    // include additional libraries
    #include "libraries/numberfont.h"
    #include "libraries/vector2d.h"
    #include "libraries/tilemap.h"
    #include "libraries/box.h"
    #include "libraries/mapcollisions.h"
    
    // include project headers
    #include "Definitions.h"
    #include "TextureGemMaze.h"   // file generated automatically by the Regions Editor tool
    #include "Player.h"
    #include "Maze.h"
    #include "GameLogic.h"
// *****************************************************************************


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    DefineRegions_TextureGemMaze();
    
    // ------------------------------------
    // PART 2: DEFINE MAZE TILESET
    // ------------------------------------
    
    // define dimensions for our tiles
    TilesMaze.width  = 50;
    TilesMaze.height = 50;
    
    // adjacent tiles have neither space nor overlap
    TilesMaze.gap_x = 0;
    TilesMaze.gap_y = 0;
    
    // define texture and regions for our tiles
    TilesMaze.texture_id = TextureGemMaze;
    TilesMaze.tile_zero_region_id = FirstRegionTileSet;
    
    // this particular tile set does use its first
    // tile (it's not discarded as transparent)
    TilesMaze.draw_tile_zero = true;
    
    // define the range of solid tiles (only the wall)
    TilesMaze.first_solid_tile = TileWall;
    TilesMaze.last_solid_tile = TileWall;
    
    // ------------------------------------
    // PART 3: DEFINE MAZE TILEMAP
    // ------------------------------------
    
    // assign our tiles to the map
    MapMaze.tiles = &TilesMaze;
    
    // define the map array
    MapMaze.map = &MazeArray[ 0 ][ 0 ];  // use the working copy, not the original!
    MapMaze.map_width  = MAZE_WIDTH;
    MapMaze.map_height = MAZE_HEIGHT;
    
    // ------------------------------------
    // PART 4: DEFINE TEXT FONT FOR NUMBERS
    // ------------------------------------
    
    // define dimensions for our characters
    FontNumbers.character_width = 16;
    FontNumbers.character_height = 22;
    
    // 1 pixel separation between characters
    FontNumbers.character_separation = 1;
    
    // define texture and regions for our characters
    FontNumbers.texture_id = TextureGemMaze;
    FontNumbers.region_id_zero = FirstRegionNumbers;
    
    // ------------------------------------
    // PART 5: INITIAL CONFIGURATIONS
    // ------------------------------------
    
    // inputs will always use the first gamepad
    select_gamepad( 0 );
    
    // initialize the game
    ResetGameSession();
    
    // ------------------------------------
    // PART 6: MAIN LOOP
    // ------------------------------------
    
    // the game is structured as a state machine
    while( true )
    {
        switch( GameState )
        {
            case StateStart:
              RunState_Start();
              break;
            
            case StatePlay:
              RunState_Play();
              break;
              
            case StateDeath:
              RunState_Death();
              break;
            
            case StateGoal:
              RunState_Goal();
              break;
        }
        
        // wait until next frame
        end_frame();
    }
}
