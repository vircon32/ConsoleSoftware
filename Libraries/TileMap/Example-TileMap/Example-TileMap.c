// *****************************************************************************
    // include standard Vircon headers
    #include "math.h"
    #include "time.h"
    #include "input.h"
    #include "video.h"
    
    // include additional headers
    #include "../tilemap.h"
// *****************************************************************************


// ---------------------------------------------------------
//   DEFINITIONS AND CONSTANTS
// ---------------------------------------------------------


// names for textures
#define TextureTiles 0

// names for texture regions
#define FirstRegionTileSet 0
#define RegionCharacter 100

// dimensions of our map, in tiles
#define MapTilesX 32
#define MapTilesY 18


// ---------------------------------------------------------
//   GLOBAL VARIABLES
// ---------------------------------------------------------


// our 2D array containing the map tile IDs
// is stored in ROM from an external file
embedded int[ MapTilesY ][ MapTilesX ] MapArray = "obj/LandMap.vmap";


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    select_texture( TextureTiles );
    
    // regions for tileset
    define_region_matrix( FirstRegionTileSet,  0,0,  39,39,  0,0,  8,6,  0 );
    
    // left arrow
    select_region( RegionCharacter );
    define_region_center( 1,243,  30,272 );
    
    // ------------------------------------
    // PART 2: DEFINE MAP AND TILES
    // ------------------------------------
    
    tileset LandTiles;
    tilemap LandMap;
    
    // - - - - - - - - - - - - - - - - - - -
    // 2.1: define our tiles
    
    // define dimensions for our tiles
    LandTiles.width  = 40;
    LandTiles.height = 40;
    
    // adjacent tiles have neither space nor overlap
    LandTiles.gap_x = 0;
    LandTiles.gap_y = 0;
    
    // define texture and regions for our tiles
    LandTiles.texture_id = TextureTiles;
    LandTiles.tile_zero_region_id = FirstRegionTileSet;
    
    // this particular tile set makes use of its
    // first tile (it's not discarded as transparent)
    LandTiles.draw_tile_zero = true;
    
    // - - - - - - - - - - - - - - - - - - -
    // 2.2: define our map
    
    // assign our tiles to the map
    LandMap.tiles = &LandTiles;
    
    // define the map array
    LandMap.map = &MapArray[ 0 ][ 0 ];
    LandMap.map_width  = MapTilesX;
    LandMap.map_height = MapTilesY;
    
    // - - - - - - - - - - - - - - - - - - -
    // 2.3: define our player character
    
    // initially place player at map center
    int PlayerX = tilemap_total_width( &LandMap ) / 2;
    int PlayerY = tilemap_total_height( &LandMap ) / 2;
    
    // ------------------------------------
    // PART 3: MAIN LOOP
    // ------------------------------------
    
    while( true )
    {
        // read inputs from the first gamepad
        select_gamepad( 0 );
        
        // move camera as indicated by player
        int DeltaX, DeltaY;
        gamepad_direction( &DeltaX, &DeltaY );
        
        PlayerX += 2 * DeltaX;
        PlayerY += 2 * DeltaY;
        
        // make camera follow the player
        LandMap.camera_x = PlayerX;
        LandMap.camera_y = PlayerY;
        
        // in this case, we want to restrict camera placement
        // so that, even when the player is near map bounds,
        // the screen view will never reach out of the map
        tilemap_clip_camera_position( &LandMap );
        
        // render tile map
        tilemap_draw( &LandMap );
        
        // to draw elements in the map, like our character, we first
        // need to convert its map coordinates to screen coordinates
        int PlayerScreenX = PlayerX;
        int PlayerScreenY = PlayerY;
        tilemap_convert_position_to_screen( &LandMap, &PlayerScreenX, &PlayerScreenY );
        
        // now draw the character at the converted position
        select_region( RegionCharacter );
        draw_region_at( PlayerScreenX, PlayerScreenY );
        
        // wait until next frame
        end_frame();
    }
}