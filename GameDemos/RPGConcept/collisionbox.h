// *****************************************************************************
    // start include guard
    #ifndef COLLISIONBOX_H
    #define COLLISIONBOX_H
// *****************************************************************************


// =============================================================================
//   DATA STRUCTURES FOR COLLISION BOXES
// =============================================================================


// This represents an axis-aligned 2D rectangle, placed
// within our scene or map. We will define a reference
// point (the hotspot) within the rectangle, which will
// be considered the object "position" for some entities
// like characters. For fixed collision boxes that
// hotspot is not actually needed.
struct collisionbox
{
    int position_x, position_y;   // position of hotspot in scene coordinates
    int hotspot_x, hotspot_y;     // coordinates of hotspot relative to box top-left
    int width, height;            // box dimensions, in pixels
    bool solid;                   // we may want to disable collision for some boxes
};


// =============================================================================
//   FUNCTIONS TO OBTAIN BOX LIMITS
// =============================================================================


int collisionbox_left( collisionbox* cb )
{
    return cb->position_x - cb->hotspot_x;
}

// ---------------------------------------------------------

int collisionbox_right( collisionbox* cb )
{
    return cb->position_x - cb->hotspot_x + cb->width;
}

// ---------------------------------------------------------

int collisionbox_top( collisionbox* cb )
{
    return cb->position_y - cb->hotspot_y;
}

// ---------------------------------------------------------

int collisionbox_bottom( collisionbox* cb )
{
    return cb->position_y - cb->hotspot_y + cb->height;
}


// =============================================================================
//   PARTIAL COLLISION FUNCTIONS (SINGLE BOUNDARY)
// -----------------------------------------------------------------------------
//   They constrain box displacement in a single direction
// =============================================================================


// performs collisions on X when box is moving left
void collisionbox_collide_left( collisionbox* cb, int min_left_x, int* delta_x )
{
    // non-solid boxes will not collide
    if( !cb->solid ) return;
    
    // does not apply if not moving left
    if( *delta_x >= 0 ) return;
    
    // does not apply if the boundary was already trespassed
    int box_left = cb->position_x - cb->hotspot_x;
    
    if( box_left < min_left_x )
      return;
    
    // when there is excess displacement, constrain it
    box_left += *delta_x;
    int excess = min_left_x - box_left;
    
    if( excess > 0 )
      *delta_x += excess;
}

// ---------------------------------------------------------

// performs collisions on X when box is moving right
void collisionbox_collide_right( collisionbox* cb, int max_right_x, int* delta_x )
{
    // non-solid boxes will not collide
    if( !cb->solid ) return;
    
    // does not apply if not moving right
    if( *delta_x <= 0 ) return;
    
    // does not apply if the boundary was already trespassed
    int box_right = cb->position_x - cb->hotspot_x + cb->width;
    
    if( box_right > max_right_x )
      return;
    
    // when there is excess displacement, constrain it
    box_right += *delta_x;
    int excess = box_right - max_right_x;
    
    if( excess > 0 )
      *delta_x -= excess;
}

// ---------------------------------------------------------

// performs collisions on Y when box is moving up
void collisionbox_collide_top( collisionbox* cb, int min_top_y, int* delta_y )
{
    // non-solid boxes will not collide
    if( !cb->solid ) return;
    
    // does not apply if not moving up
    if( *delta_y >= 0 ) return;
    
    // does not apply if the boundary was already trespassed
    int box_top = cb->position_y - cb->hotspot_y;
    
    if( box_top < min_top_y )
      return;
    
    // when there is excess displacement, constrain it
    box_top += *delta_y;
    int excess = min_top_y - box_top;
    
    if( excess > 0 )
      *delta_y += excess;
}

// ---------------------------------------------------------

// performs collisions on Y when box is moving down
void collisionbox_collide_bottom( collisionbox* cb, int max_bottom_y, int* delta_y )
{
    // non-solid boxes will not collide
    if( !cb->solid ) return;
    
    // does not apply if not moving down
    if( *delta_y <= 0 ) return;
    
    // does not apply if the boundary was already trespassed
    int box_bottom = cb->position_y - cb->hotspot_y + cb->height;
    
    if( box_bottom > max_bottom_y )
      return;
    
    // when there is excess displacement, constrain it
    box_bottom += *delta_y;
    int excess = box_bottom - max_bottom_y;
    
    if( excess > 0 )
      *delta_y -= excess;
}


// =============================================================================
//   FULL COLLISION FUNCTIONS
// =============================================================================


// detect box-box overlapping (touching is NOT considered overlap)
bool collisionboxes_overlap( collisionbox* cb1, collisionbox* cb2 )
{
    // check separation on X
    if( collisionbox_right( cb1 ) <= collisionbox_left( cb2 ) )  return false;
    if( collisionbox_right( cb2 ) <= collisionbox_left( cb1 ) )  return false;
    
    // check separation on Y
    if( collisionbox_bottom( cb1 ) <= collisionbox_top( cb2 ) )  return false;
    if( collisionbox_bottom( cb2 ) <= collisionbox_top( cb1 ) )  return false;
    
    // no separations <=> overlap
    return true;
}

// ---------------------------------------------------------

// This function performs collision resolution between 2 boxes.
// The first one is considered static and will never be moved.
// The second is attempting to move by a Delta vector, and this
// displacement will be adjusted as needed to preventg collision.
// ---------------------------------------------------------
// Careful! for now, this function is only valid for the
// case where movement is never diagonal, but restricted
// to 4 directions: at any given moment, boxes can only
// move either along X or along Y; to handle diagonals we
// would need to separate our collision detection and
// constraint for both axes
void collisionboxes_collide( collisionbox* fixedbox, collisionbox* movingbox, int* delta_x, int* delta_y )
{
    // non-solid boxes will not collide
    if( !fixedbox->solid  ) return;
    if( !movingbox->solid ) return;
    
    // collide along X axis
    // - - - - - - - - - - - - - - - - - -
    if( *delta_x != 0 )
    {
        // only applies if there is overlap along Y
        if( collisionbox_bottom( movingbox ) <= collisionbox_top( fixedbox  ) )  return;
        if( collisionbox_bottom( fixedbox  ) <= collisionbox_top( movingbox ) )  return;
        
        // collide on the direction on X movement
        if( *delta_x < 0 )
          collisionbox_collide_left( movingbox, collisionbox_right( fixedbox ), delta_x );
        else
          collisionbox_collide_right( movingbox, collisionbox_left( fixedbox ), delta_x );
    }
    
    // collide along Y axis
    // - - - - - - - - - - - - - - - - - -
    if( *delta_y != 0 )
    {
        // only applies if there is overlap along X
        if( collisionbox_right( movingbox ) <= collisionbox_left( fixedbox  ) )  return;
        if( collisionbox_right( fixedbox  ) <= collisionbox_left( movingbox ) )  return;
        
        // collide on the direction on Y movement
        if( *delta_y < 0 )
          collisionbox_collide_top( movingbox, collisionbox_bottom( fixedbox ), delta_y );
        else
          collisionbox_collide_bottom( movingbox, collisionbox_top( fixedbox ), delta_y );
    }
}


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
