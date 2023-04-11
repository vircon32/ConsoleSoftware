// *****************************************************************************
    // start include guard
    #ifndef TILEMAP_H
    #define TILEMAP_H
    
    // include standard Vircon headers
    #include "math.h"
    #include "video.h"
// *****************************************************************************


/* *****************************************************************************
*  This header is a simplified version of the library "tilemap.h" to make it   *
*  easier to define and draw tile maps that are always on screen and don't     *
*  need to use a camera position and just use screen coordinates.              *
***************************************************************************** */


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
    
    // regular tilemap uses a camera and determines map's top-left
    // coordinates from it; this simplified version uses those
    // screen coordinates directly
    int topleft_x, topleft_y;
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

// draws the received tile map on screen, following its
// current camera position and using its tile set regions
void tilemap_draw( tilemap* tm )
{
    select_texture( tm->tiles->texture_id );
    
    // determine the range of tiles visible on screen
    // (needed to optimize the loop in large maps)
    int tile_step_x = tm->tiles->width  + tm->tiles->gap_x;
    int tile_step_y = tm->tiles->height + tm->tiles->gap_y;
    
    int min_tile_x = max( -tm->topleft_x / tile_step_x, 0 );
    int min_tile_y = max( -tm->topleft_y / tile_step_y, 0 );
    int max_tile_x = min( (-tm->topleft_x + screen_width ) / tile_step_x, tm->map_width -1 );
    int max_tile_y = min( (-tm->topleft_y + screen_height) / tile_step_y, tm->map_height-1 );
    
    // iterate to draw every tile in the visible range
    int draw_y = tm->topleft_y + min_tile_y * tile_step_y;
    
    for( int tile_y = min_tile_y; tile_y <= max_tile_y; ++tile_y )
    {
        int draw_x = tm->topleft_x + min_tile_x * tile_step_x;
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


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
