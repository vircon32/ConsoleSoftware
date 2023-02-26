/* *****************************************************************************
*  Vircon32 library: "tilemap.h"                    File version: 2022/10/30   *
*  --------------------------------------------------------------------------- *
*  This header defines tile sets and tile maps, and automates the process of   *
*  handling tile map based backgrounds. Included here are functions to point   *
*  camera view at map points and draw the map on screen following that view.   *
***************************************************************************** */

// *****************************************************************************
    // start include guard
    #ifndef TILEMAP_H
    #define TILEMAP_H
    
    // include standard Vircon headers
    #include "math.h"
    #include "video.h"
// *****************************************************************************


// =============================================================================
//   DATA STRUCTURES FOR TILE MAPS
// =============================================================================


// a tile set represents a sequence of tiles numbered from zero,
// along with the needed information for drawing them on screen
struct tileset
{
    // tile dimensions in pixels
    int width, height;
    
    // separation between adjacent tiles, in pixels
    // (they can be negative for overlap)
    int gap_x, gap_y;
    
    // it is common for some tile sets to reserve tile number
    // zero to indicate no tile or a fully transparent tile;
    // when this flag is false, tile number zero will never
    // be drawn (even if it has a non-transparent region)
    bool draw_tile_zero;
    
    // texture regions for this map's tiles are taken as
    // consecutive region IDs within a same texture; the
    // first of those regions corresponds to tile number zero
    int texture_id;
    int tile_zero_region_id;
};

// ---------------------------------------------------------

// a tile map is a 2D array where each element represents a
// tile number within a given tile set; to automate its placement
// on screen, here we add to it information on camera position
struct tilemap
{
    // pointer to the tile set that we will use to interpret
    // and draw the tile IDs stored in each map position
    tileset* tiles;
    
    // the map itself is an external 2D array that contains
    // tile IDs for each position; here we store a pointer to
    // its first element
    int* map;
    
    // these are the map dimensions in tiles; they will be the
    // same as the dimensions of the 2D array
    int map_width, map_height;
    
    // current camera position, relative to the map's top-left
    // corner (coordinates are in pixels); this will determine
    // how the tile map is rendered on screen
    int camera_x, camera_y;
};


// =============================================================================
//   FUNCTIONS TO HANDLE TILE MAPS
// =============================================================================


// returns the map's total width in pixels, accounting
// for the configured gap between adjacent tiles
int tilemap_total_width( tilemap* tm )
{
    int width_tiles = tm->map_width * tm->tiles->width;
    int width_gaps = (tm->map_width - 1) * tm->tiles->gap_x;
    return width_tiles + width_gaps;
}

// ---------------------------------------------------------

// returns the map's total height in pixels, accounting
// for the configured gap between adjacent tiles
int tilemap_total_height( tilemap* tm )
{
    int height_tiles = tm->map_height * tm->tiles->height;
    int height_gaps = (tm->map_height - 1) * tm->tiles->gap_y;
    return height_tiles + height_gaps;
}

// ---------------------------------------------------------

// we may want to restrict camera positions so that the screen
// view remains fully within map bounds; this function corrects
// the map's camera position if view falls outside map bounds
void tilemap_clip_camera_position( tilemap* tm )
{
    // determine limits for camera position
    int camera_x_min = screen_width  / 2;
    int camera_y_min = screen_height / 2;
    int camera_x_max = tilemap_total_width( tm )  - screen_width  / 2;
    int camera_y_max = tilemap_total_height( tm ) - screen_height / 2;
    
    // apply those limits
    tm->camera_x = fmax( tm->camera_x, camera_x_min );
    tm->camera_x = fmin( tm->camera_x, camera_x_max );
    tm->camera_y = fmax( tm->camera_y, camera_y_min );
    tm->camera_y = fmin( tm->camera_y, camera_y_max );
}

// ---------------------------------------------------------

// draws the received tile map on screen, following its
// current camera position and using its tile set regions
void tilemap_draw( tilemap* tm )
{
    select_texture( tm->tiles->texture_id );
    
    // calculate map position in screen coordinates
    int map_topleft_x = -tm->camera_x + screen_width  / 2;
    int map_topleft_y = -tm->camera_y + screen_height / 2;
    
    // determine the range of tiles visible on screen
    // (needed to optimize the loop in large maps)
    int tile_step_x = tm->tiles->width  + tm->tiles->gap_x;
    int tile_step_y = tm->tiles->height + tm->tiles->gap_y;
    
    int min_tile_x = max( -map_topleft_x / tile_step_x, 0 );
    int min_tile_y = max( -map_topleft_y / tile_step_y, 0 );
    int max_tile_x = min( (-map_topleft_x + screen_width ) / tile_step_x, tm->map_width -1 );
    int max_tile_y = min( (-map_topleft_y + screen_height) / tile_step_y, tm->map_height-1 );
    
    // iterate to draw every tile in the visible range
    int draw_y = map_topleft_y + min_tile_y * tile_step_y;
    
    for( int tile_y = min_tile_y; tile_y <= max_tile_y; ++tile_y )
    {
        int draw_x = map_topleft_x + min_tile_x * tile_step_x;
        int* current_tile = tm->map + tm->map_width * tile_y + min_tile_x;
        
        for( int tile_x = min_tile_x; tile_x <= max_tile_x; ++tile_x )
        {
            if( *current_tile || tm->tiles->draw_tile_zero )
            {
                select_region( tm->tiles->tile_zero_region_id + *current_tile );
                draw_region_at( draw_x, draw_y );
            }
            
            draw_x += tile_step_x;
            ++current_tile;
        }
        
        draw_y += tile_step_y;
    }
}

// ---------------------------------------------------------

// we will usually have objects placed on the map that are
// stored in map coordinates; this function receives x and y
// variables in map coordinates and transforms them to screen
// coordinates, allowing us to draw map objects on screen
void tilemap_convert_position_to_screen( tilemap* tm, int* position_x, int* position_y )
{
    *position_x += -tm->camera_x + screen_width  / 2;
    *position_y += -tm->camera_y + screen_height / 2;
}


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
