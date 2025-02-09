/* *****************************************************************************
*  Vircon32 library: "tilemapzoomed.h"              File version: 2025/02/08   *
*  --------------------------------------------------------------------------- *
*  This header is a variant of tilemap.h so that the map can change zoom when  *
*  drawn. This can be used to simulate camera height changes. Other than that  *
*  everything is mostly the same than tilemap.h so, for better performance,    *
*  use the regular tilemap library if you don't need the scaling effect.       *
***************************************************************************** */

// *****************************************************************************
    // start include guard
    #ifndef TILEMAPZOOMED_H
    #define TILEMAPZOOMED_H
    
    // include standard Vircon headers
    #include "math.h"
    #include "video.h"
    
    // ensure dependencies are present
    #ifndef TILEMAP_H
      #error "library tilemap.h needs to be included before this one"
    #endif
// *****************************************************************************


// =============================================================================
//   DATA STRUCTURES FOR ZOOMED TILE MAPS
// =============================================================================


// a tile map is a 2D array where each element represents a
// tile number within a given tile set; to automate its placement
// on screen, here we add to it information on camera position
struct tilemapzoomed
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
    vector2d camera_position;
    float camera_zoom;
};


// =============================================================================
//   FUNCTIONS TO HANDLE ZOOMED TILE MAPS
// =============================================================================


// returns the map's total width in pixels, accounting
// for the configured gap between adjacent tiles
int tilemapzoomed_total_width( tilemapzoomed* tmz )
{
    int width_tiles = tmz->map_width * tmz->tiles->width;
    int width_gaps = (tmz->map_width - 1) * tmz->tiles->gap_x;
    return width_tiles + width_gaps;
}

// ---------------------------------------------------------

// returns the map's total height in pixels, accounting
// for the configured gap between adjacent tiles
int tilemapzoomed_total_height( tilemapzoomed* tmz )
{
    int height_tiles = tmz->map_height * tmz->tiles->height;
    int height_gaps = (tmz->map_height - 1) * tmz->tiles->gap_y;
    return height_tiles + height_gaps;
}

// ---------------------------------------------------------

// we may want to restrict camera positions so that the screen
// view remains fully within map bounds; this function corrects
// the map's camera position if view falls outside map bounds
void tilemapzoomed_clip_camera_position( tilemapzoomed* tmz )
{
    // determine limits for camera position
    float camera_x_min = (screen_width  / 2) / tmz->camera_zoom;
    float camera_y_min = (screen_height / 2) / tmz->camera_zoom;
    float camera_x_max = tilemapzoomed_total_width( tmz )  - (screen_width  / 2) / tmz->camera_zoom;
    float camera_y_max = tilemapzoomed_total_height( tmz ) - (screen_height / 2) / tmz->camera_zoom;
    
    // apply those limits
    tmz->camera_position.x = fmax( tmz->camera_position.x, camera_x_min );
    tmz->camera_position.x = fmin( tmz->camera_position.x, camera_x_max );
    tmz->camera_position.y = fmax( tmz->camera_position.y, camera_y_min );
    tmz->camera_position.y = fmin( tmz->camera_position.y, camera_y_max );
}

// ---------------------------------------------------------

// draws the received tile map on screen, using its tile set
// regions and positioning the map from its top-left corner
void tilemapzoomed_draw_from_topleft( tilemapzoomed* tmz, float topleft_x, float topleft_y )
{
    select_texture( tmz->tiles->texture_id );
    set_drawing_scale( tmz->camera_zoom + 0.02, tmz->camera_zoom + 0.02 );
    
    // determine the range of tiles visible on screen
    // (needed to optimize the loop in large maps)
    float tile_step_x = tileset_get_step_x( tmz->tiles ) * tmz->camera_zoom;
    float tile_step_y = tileset_get_step_y( tmz->tiles ) * tmz->camera_zoom;
    
    int min_tile_x = max( -topleft_x / tile_step_x, 0 );
    int min_tile_y = max( -topleft_y / tile_step_y, 0 );
    int max_tile_x = min( (-topleft_x + screen_width ) / tile_step_x, tmz->map_width -1 );
    int max_tile_y = min( (-topleft_y + screen_height) / tile_step_y, tmz->map_height-1 );
    
    // iterate to draw every tile in the visible range
    float draw_y = topleft_y + min_tile_y * tile_step_y;
    
    for( int tile_y = min_tile_y; tile_y <= max_tile_y; ++tile_y )
    {
        float draw_x = topleft_x + min_tile_x * tile_step_x;
        int* current_tile = tmz->map + tmz->map_width * tile_y + min_tile_x;
        
        for( int tile_x = min_tile_x; tile_x <= max_tile_x; ++tile_x )
        {
            if( *current_tile || tmz->tiles->draw_tile_zero )
            {
                select_region( tmz->tiles->tile_zero_region_id + *current_tile );
                draw_region_zoomed_at( draw_x, draw_y );
            }
            
            draw_x += tile_step_x;
            ++current_tile;
        }
        
        draw_y += tile_step_y;
    }
}

// ---------------------------------------------------------

// same as previous draw function but map is positioned so that
// its current camera position is placed at screen center
void tilemapzoomed_draw_from_camera( tilemapzoomed* tmz )
{
    // calculate screen coordinates for map's top-left corner 
    // given from the camera position
    float topleft_x = -tmz->camera_position.x * tmz->camera_zoom + screen_width  / 2;
    float topleft_y = -tmz->camera_position.y * tmz->camera_zoom + screen_height / 2;
    
    // now call the main draw function
    tilemapzoomed_draw_from_topleft( tmz, topleft_x, topleft_y );
}

// ---------------------------------------------------------

// we will usually have objects placed on the map that are
// stored in map coordinates; this function receives x and y
// variables in map coordinates and transforms them to screen
// coordinates, allowing us to draw map objects on screen
void tilemapzoomed_convert_position_to_screen( tilemapzoomed* tmz, int* position_x, int* position_y )
{
    *position_x = (*position_x - tmz->camera_position.x) * tmz->camera_zoom + screen_width  / 2;
    *position_y = (*position_y - tmz->camera_position.y) * tmz->camera_zoom + screen_height / 2;
}


// =============================================================================
//   FUNCTIONS FOR TILE MAP COLLISIONS
// =============================================================================


// these 4 functions (one for each tile edge) are meant to
// ensure that objects only collide with exposed tile edges,
// and prevent unreal inner edges from affecting movements
bool tilemapzoomed_is_tile_left_exposed( tilemapzoomed* tmz, int tile_x, int tile_y )
{
    // at map limits, all tile sides are exposed
    if( tile_x == 0 ) return true;
    
    // otherwise, analyze the neighbour left tile
    int tile_at_left = tmz->map[ tile_y * tmz->map_width + (tile_x-1) ];
    return !tileset_is_tile_solid( tmz->tiles, tile_at_left );
}

// ---------------------------------------------------------

bool tilemapzoomed_is_tile_right_exposed( tilemapzoomed* tmz, int tile_x, int tile_y )
{
    // at map limits, all tile sides are exposed
    if( tile_x == (tmz->map_width-1) ) return true;
    
    // otherwise, analyze the neighbour right tile
    int tile_at_right = tmz->map[ tile_y * tmz->map_width + (tile_x+1) ];
    return !tileset_is_tile_solid( tmz->tiles, tile_at_right );
}

// ---------------------------------------------------------

bool tilemapzoomed_is_tile_top_exposed( tilemapzoomed* tmz, int tile_x, int tile_y )
{
    // at map limits, all tile sides are exposed
    if( tile_y == 0 ) return true;
    
    // otherwise, analyze the neighbour top tile
    int tile_at_top = tmz->map[ (tile_y-1) * tmz->map_width + tile_x ];
    return !tileset_is_tile_solid( tmz->tiles, tile_at_top );
}

// ---------------------------------------------------------

bool tilemapzoomed_is_tile_bottom_exposed( tilemapzoomed* tmz, int tile_x, int tile_y )
{
    // at map limits, all tile sides are exposed
    if( tile_y == (tmz->map_height-1) ) return true;
    
    // otherwise, analyze the neighbour bottom tile
    int tile_at_bottom = tmz->map[ (tile_y+1) * tmz->map_width + tile_x ];
    return !tileset_is_tile_solid( tmz->tiles, tile_at_bottom );
}

// ---------------------------------------------------------

int* tilemapzoomed_get_tile_at_point( tilemapzoomed* tmz, vector2d* point )
{
    // no tiles outside the map
    if( point->x < 0 || point->x > tilemapzoomed_total_width( tmz ) )
      return NULL;
    
    if( point->y < 0 || point->y > tilemapzoomed_total_height( tmz ) )
      return NULL;
    
    int tile_step_x = tileset_get_step_x( tmz->tiles );
    int tile_step_y = tileset_get_step_y( tmz->tiles );
    int tile_x = point->x / tile_step_x;
    int tile_y = point->y / tile_step_y;
    return &tmz->map[ tmz->map_width * tile_y + tile_x ];
}

// ---------------------------------------------------------

bool tilemapzoomed_overlaps_point( tilemapzoomed* tmz, vector2d* point )
{
    // get the target tile, if any
    int* tile_at_point = tilemapzoomed_get_tile_at_point( tmz, point );
    if( tile_at_point ) return false;
    
    return tileset_is_tile_solid( tmz->tiles, *tile_at_point );
}


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
