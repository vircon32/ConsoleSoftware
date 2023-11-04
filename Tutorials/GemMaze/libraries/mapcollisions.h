/* *****************************************************************************
*  Vircon32 library: "mapcollisions.h"              File version: 2023/11/01   *
*  --------------------------------------------------------------------------- *
*  This header expands the box.h collisions framework to be able to detect     *
*  and process collisions between boxes and tile maps. This covers not only    *
*  collisions against any solid tiles, but also "internal" collisions against  *
*  map limits to keep game objects within the map when needed                  *
***************************************************************************** */

// *****************************************************************************
    // start include guard
    #ifndef MAPCOLLISIONS_H
    #define MAPCOLLISIONS_H
    
    // ensure dependencies are present
    #ifndef BOX_H
      #error "library box.h needs to be included before this one"
    #endif
    
    #ifndef TILEMAP_H
      #error "library tilemap.h needs to be included before this one"
    #endif
// *****************************************************************************


// =============================================================================
//      BOX VS TILEMAP: DETECTING OVERLAPS
// =============================================================================


bool box_overlaps_tilemap( box* b, tilemap* tm )
{
    // determine the range of tiles overlapping box extents
    int tile_step_x = tm->tiles->width  + tm->tiles->gap_x;
    int tile_step_y = tm->tiles->height + tm->tiles->gap_y;
    
    // account for box extents not on the map
    int min_tile_x = max( box_left( b ) / tile_step_x, 0 );
    int min_tile_y = max( box_top( b )  / tile_step_y, 0 );
    int max_tile_x = min( box_right( b )  / tile_step_x, tm->map_width -1 );
    int max_tile_y = min( box_bottom( b ) / tile_step_y, tm->map_height-1 );
    
    for( int tile_y = min_tile_y; tile_y <= max_tile_y; ++tile_y )
    for( int tile_x = min_tile_x; tile_x <= max_tile_x; ++tile_x )
    {
        int tile_value = tm->map[ tm->map_width * tile_y + tile_x ];
        
        if( tileset_is_tile_solid( tm->tiles, tile_value ) )
           return true;
    }
    
    return false;
}


// =============================================================================
//      BOX VS TILEMAP: INTERNAL COLLISION VS. TILE MAP LIMITS
// =============================================================================


// this performs an inner collision with map limits,
// disregarding the contents of the tiles themselves
void collide_box_with_tilemap_limits( box* b, tilemap* tm )
{
    // collisions along Y axis
    if( b->delta_position.y <= 0 )
      box_collide_with_top_boundary( b, 0 );
    
    if( b->delta_position.y >= 0 )
      box_collide_with_bottom_boundary( b, tilemap_total_height( tm ) );
    
    // collisions along X axis
    if( b->delta_position.x <= 0 )
      box_collide_with_left_boundary( b, 0 );
    
    if( b->delta_position.x >= 0 )
      box_collide_with_right_boundary( b, tilemap_total_width( tm ) );
}


// =============================================================================
//      BOX VS TILEMAP: PARTIAL COLLISION FUNCTIONS
// =============================================================================


void collide_box_with_tile_left( box* b, tilemap* tm, int tile_x, int tile_y )
{
    // applicable only if box moves right
    if( b->delta_position.x <= 0 )
      return;
    
    // evaluate extended contact surface across Y
    float box_expanded_top    = box_top( b )    + fmin( b->delta_position.y, 0.0 );
    float box_expanded_bottom = box_bottom( b ) + fmax( b->delta_position.y, 0.0 );
    
    int tile_top    = tileset_get_step_y( tm->tiles ) * tile_y;
    int tile_bottom = tileset_get_step_y( tm->tiles ) * (tile_y + 1);
    
    // discard collision if there is no overlap in Y
    if( box_expanded_bottom <= tile_top || box_expanded_top >= tile_bottom )
      return;
    
    // obtain tile left and box right (current and next)
    int tile_left = tileset_get_step_x( tm->tiles ) * tile_x;
    float box_current_right = box_right( b );
    float box_next_right = box_current_right + b->delta_position.x;
    
    // demand that the boundary will be trespassed in next frame
    if( box_current_right <= tile_left && box_next_right > tile_left )
    {
        // correct box X position in next frame
        b->delta_position.x = (tile_left - 0.1) - box_current_right;   // 0.1 to avoid errors from rounding
        
        // update box contacts
        b->contacts.right = true;
    }
}

// ---------------------------------------------------------

void collide_box_with_tile_right( box* b, tilemap* tm, int tile_x, int tile_y )
{
    // applicable only if box moves left
    if( b->delta_position.x >= 0 )
      return;
    
    // evaluate extended contact surface across Y
    float box_expanded_top    = box_top( b )    + fmin( b->delta_position.y, 0.0 );
    float box_expanded_bottom = box_bottom( b ) + fmax( b->delta_position.y, 0.0 );
    
    int tile_top    = tileset_get_step_y( tm->tiles ) * tile_y;
    int tile_bottom = tileset_get_step_y( tm->tiles ) * (tile_y + 1);
    
    // discard collision if there is no overlap in Y
    if( box_expanded_bottom <= tile_top || box_expanded_top >= tile_bottom )
      return;
    
    // obtain tile right and box left (current and next)
    int tile_right = tileset_get_step_x( tm->tiles ) * (tile_x + 1);
    float box_current_left = box_left( b );
    float box_next_left = box_current_left + b->delta_position.x;
    
    // demand that the boundary is trespassed in this frame
    if( box_current_left >= tile_right && box_next_left < tile_right )
    {
        // correct box X position in next frame
        b->delta_position.x = (tile_right + 0.1) - box_current_left;   // 0.1 to avoid errors from rounding
        
        // update box contacts
        b->contacts.left = true;
    }
}

// ---------------------------------------------------------

void collide_box_with_tile_top( box* b, tilemap* tm, int tile_x, int tile_y )
{
    // applicable only if box moves down
    if( b->delta_position.y <= 0 )
      return;
    
    // evaluate extended contact surface across X
    float box_expanded_left  = box_left( b )  + fmin( b->delta_position.x, 0.0 );
    float box_expanded_right = box_right( b ) + fmax( b->delta_position.x, 0.0 );
    
    int tile_left  = tileset_get_step_x( tm->tiles ) * tile_x;
    int tile_right = tileset_get_step_x( tm->tiles ) * (tile_x + 1);
    
    // discard collision if there is no overlap in X
    if( box_expanded_right <= tile_left || box_expanded_left >= tile_right )
      return;
    
    // obtain tile top and box bottom (current and next)
    int tile_top = tileset_get_step_y( tm->tiles ) * tile_y;
    float box_current_bottom = box_bottom( b );
    float box_next_bottom = box_current_bottom + b->delta_position.y;
    
    // demand that the boundary is trespassed in this frame
    if( box_current_bottom <= tile_top && box_next_bottom > tile_top )
    {
        // correct box Y position in next frame
        b->delta_position.y = (tile_top - 0.1) - box_current_bottom;   // 0.1 to avoid errors from rounding
        
        // update box contacts
        b->contacts.bottom = true;
    }
}

// ---------------------------------------------------------

void collide_box_with_tile_bottom( box* b, tilemap* tm, int tile_x, int tile_y )
{
    // applicable only if box moves up
    if( b->delta_position.y >= 0 )
      return;
    
    // evaluate extended contact surface across X
    float box_expanded_left  = box_left( b )  + fmin( b->delta_position.x, 0.0 );
    float box_expanded_right = box_right( b ) + fmax( b->delta_position.x, 0.0 );
    
    int tile_left  = tileset_get_step_x( tm->tiles ) * tile_x;
    int tile_right = tileset_get_step_x( tm->tiles ) * (tile_x + 1);
    
    // discard collision if there is no overlap in X
    if( box_expanded_right <= tile_left || box_expanded_left >= tile_right )
      return;
    
    // obtain tile bottom and box top (current and next)
    int tile_bottom = tileset_get_step_y( tm->tiles ) * (tile_y + 1);
    float box_current_top = box_top( b );
    float box_next_top = box_current_top + b->delta_position.y;
    
    // demand that the boundary is trespassed in this frame
    if( box_current_top >= tile_bottom && box_next_top < tile_bottom )
    {
        // correct box Y position in next frame
        b->delta_position.y = (tile_bottom + 0.1) - box_current_top;   // 0.1 to avoid errors from rounding
        
        // update box contacts
        b->contacts.top = true;
    }
}


// =============================================================================
//      BOX VS TILEMAP: FULL COLLISION FUNCTION
// =============================================================================


void collide_box_with_tile( box* b, tilemap* tm, int tile_x, int tile_y )
{
    // do nothing for tile coordinates outside the map
    if( tile_x < 0 || tile_x >= tm->map_width )
      return;
    
    if( tile_y < 0 || tile_y >= tm->map_height )
      return;
    
    // nothing to collide with if this tile is empty
    int current_tile = tm->map[ tile_y * tm->map_width + tile_x ];
    
    if( !tileset_is_tile_solid( tm->tiles, current_tile ) )
      return;
    
    // collide box's top against tile's bottom
    if( tilemap_is_tile_bottom_exposed( tm, tile_x, tile_y ) )
      collide_box_with_tile_bottom( b, tm, tile_x, tile_y );
    
    // collide box's bottom against tile's top
    if( tilemap_is_tile_top_exposed( tm, tile_x, tile_y ) )
      collide_box_with_tile_top( b, tm, tile_x, tile_y );
    
    // collide box's left against tile's right
    if( tilemap_is_tile_right_exposed( tm, tile_x, tile_y ) )
      collide_box_with_tile_right( b, tm, tile_x, tile_y );
    
    // collide box's right against tile's left
    if( tilemap_is_tile_left_exposed( tm, tile_x, tile_y ) )
      collide_box_with_tile_left( b, tm, tile_x, tile_y );
}

// ---------------------------------------------------------

void collide_box_with_tilemap( box* b, tilemap* tm )
{
    // obtain tile limits to apply collisions; expand it with
    // current frame movement and add 1 more pixel to avoid
    // possible problems with rounding errors
    int min_tile_x = (box_left  ( b ) + fmin( b->delta_position.x, 0.0 ) - 1) / tileset_get_step_x( tm->tiles );
    int max_tile_x = (box_right ( b ) + fmax( b->delta_position.x, 0.0 ) + 1) / tileset_get_step_x( tm->tiles );
    int min_tile_y = (box_top   ( b ) + fmin( b->delta_position.y, 0.0 ) - 1) / tileset_get_step_y( tm->tiles );
    int max_tile_y = (box_bottom( b ) + fmax( b->delta_position.y, 0.0 ) + 1) / tileset_get_step_y( tm->tiles );
    
    // collide with each of the tiles
    for( int tile_y = min_tile_y; tile_y <= max_tile_y; ++tile_y )
      for( int tile_x = min_tile_x; tile_x <= max_tile_x; ++tile_x )
        collide_box_with_tile( b, tm, tile_x, tile_y );
}


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
