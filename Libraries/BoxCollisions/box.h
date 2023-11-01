/* *****************************************************************************
*  Vircon32 library: "box.h"                        File version: 2023/11/01   *
*  --------------------------------------------------------------------------- *
*  This header defines 2D axis-aligned boxes meant to be placed in game world  *
*  coordinates, have movement and be affected by collisions and events. This   *
*  library, together with the ones for tile maps and collisions, provide a     *
*  basic collision "framework" for rectangular axis-aligned shapes.            *
*  --------------------------------------------------------------------------- *
*  HOW TO USE THIS LIBRARY:                                                    *
*  You will first need to define some boxes with their shape, position and     *
*  velocity. Then, for each frame you will simulate their movement. All        *
*  collision functions assume that initial positions of objects are correct    *
*  (there is no penetration). Each frame should follow these steps:            *
*    1) begin a new frame, with box_begin_frame()                              *
*    2) simulate unrestricted box motion for this frame with box_simulate()    *
*    3) restrict that motion applying all needed collision functions           *
*    4) call box_move() to actually apply the restricted box motion            *
*  Keep in mind: collisions will only correct box positions, but not their     *
*  speed. We can optionally correct speeds using box_apply_bounce() after box  *
*  movement is applied. With a coefficient of 0, collisions will just stop a   *
*  box. If we use 1 we will instead get perfect bounces (speed is preserved).  *
***************************************************************************** */

// *****************************************************************************
    // start include guard
    #ifndef BOX_H
    #define BOX_H
    
    // include standard libraries
    #include "misc.h"
    
    // ensure dependencies are present
    #ifndef VECTOR2D_H
      #error "library vector2d.h needs to be included before this one"
    #endif
// *****************************************************************************


// =============================================================================
//   BOUNDARY INFORMATION FOR BOXES
// =============================================================================


// this structure determines which of the 4 box sides
// have suffered a collision on this frame
struct box_contacts
{
    bool left, right, top, bottom;
};


// =============================================================================
//   AXIS-ALIGNED BOXES FOR COLLISIONS AND EVENT DETECTION
// =============================================================================


// the concept of box "hotspot" is similar to texture region
// hotspots: that point will be taken as the box position;
// the hotspot vector stored here are the coordinates of the
// hotspot relative to the box's top-left point
struct box
{
    // box shape definition
    vector2d position;
    vector2d hotspot;
    float width, height;
    
    // motion dynamics
    vector2d velocity;
    vector2d acceleration;
    
    // tentative movement in current frame
    vector2d delta_position;
    vector2d delta_velocity;
    
    // currently active contacts
    box_contacts contacts;
    box_contacts previous_contacts;
};


// =============================================================================
//   OBTAIN CURRENT BOX BOUNDARIES
// =============================================================================


float box_left( box* b )
{
    return b->position.x - b->hotspot.x;
}

// ---------------------------------------------------------

float box_right( box* b )
{
    return b->position.x - b->hotspot.x + b->width;
}
// ---------------------------------------------------------

float box_top( box* b )
{
    return b->position.y - b->hotspot.y;
}

// ---------------------------------------------------------

float box_bottom( box* b )
{
    return b->position.y - b->hotspot.y + b->height;
}


// =============================================================================
//   BOX MOVEMENT SIMULATION
// =============================================================================


void box_full_stop( box* b )
{
    memset( &b->contacts, 0, sizeof(box_contacts) );
    memset( &b->previous_contacts, 0, sizeof(box_contacts) );
    
    b->velocity.x      = b->velocity.y      = 0;
    b->acceleration.x  = b->acceleration.y  = 0;
    b->delta_position.x = b->delta_position.y = 0;
    b->delta_velocity.x = b->delta_velocity.y = 0;
}

// ---------------------------------------------------------

void box_begin_frame( box* b )
{
    b->previous_contacts = b->contacts;
    memset( &b->contacts, 0, sizeof(box_contacts) );
    
    b->acceleration.x   = b->acceleration.y   = 0;
    b->delta_position.x = b->delta_position.y = 0;
    b->delta_velocity.x = b->delta_velocity.y = 0;
}

// ---------------------------------------------------------

void box_simulate( box* b )
{
    b->delta_position.x = frame_time * b->velocity.x;
    b->delta_position.y = frame_time * b->velocity.y;
    b->delta_velocity.x = frame_time * b->acceleration.x;
    b->delta_velocity.y = frame_time * b->acceleration.y;
}

// ---------------------------------------------------------

void box_move( box* b )
{
    b->position.x += b->delta_position.x;
    b->position.y += b->delta_position.y;
    b->velocity.x += b->delta_velocity.x;
    b->velocity.y += b->delta_velocity.y;
}

// ---------------------------------------------------------

// a bounce coefficient of 0 means it will just stop
// moving in that direction when hitting an obstacle
void box_apply_bounce( box* b, float bounce_coefficient )
{
    if( !b->previous_contacts.top && b->contacts.top )
      b->velocity.y *= -bounce_coefficient;
    
    if( !b->previous_contacts.bottom && b->contacts.bottom )
      b->velocity.y *= -bounce_coefficient;
    
    if( !b->previous_contacts.left && b->contacts.left )
      b->velocity.x *= -bounce_coefficient;
    
    if( !b->previous_contacts.right && b->contacts.right )
      b->velocity.x *= -bounce_coefficient;
}


// =============================================================================
//   DETECTION OF BOX OVERLAPS
// =============================================================================


// we don't consider just touching (i.e. boundary equality)
// to be overlapping, or else unwanted effects will happen
// on tile maps (neighboring tiles touch, but don't overlap)
bool boxes_overlap( box* b1, box* b2 )
{
    bool overlap_x = (box_right( b1 ) > box_left( b2 ) && box_right( b2 ) > box_left( b1 ));
    bool overlap_y = (box_bottom( b1 ) > box_top( b2 ) && box_bottom( b2 ) > box_top( b1 ));
    return overlap_x && overlap_y;
}

// ---------------------------------------------------------

// same as before: points right on boundary do not overlap
bool box_overlaps_point( box* b, vector2d* point )
{
    bool overlap_x = (box_left( b ) < point->x && box_right( b ) > point->x);
    bool overlap_y = (box_top( b ) < point->y && box_bottom( b ) > point->y);
    return overlap_x && overlap_y;
}


// =============================================================================
//   COLLIDING BOX AGAINST WORLD BOUNDARIES
// =============================================================================


void box_collide_with_top_boundary( box* b, float boundary_y )
{
    // determine if boundary penetration will happen next frame
    float box_next_top = box_top( b ) + b->delta_position.y;
    float penetration = boundary_y - box_next_top;
    if( penetration <= 0 ) return;
    
    // adjust box Y position in next frame
    b->delta_position.y = boundary_y - box_top( b ) + 0.1;  // to compensate rounding errors
    
    // update box contacts
    b->contacts.top = true;
}

// -----------------------------------------------------------------------------

void box_collide_with_bottom_boundary( box* b, float boundary_y )
{
    // determine if boundary penetration will happen next frame
    float box_next_bottom = box_bottom( b ) + b->delta_position.y;
    float penetration = box_next_bottom - boundary_y;
    if( penetration <= 0 ) return;
    
    // adjust box Y position in next frame
    b->delta_position.y = boundary_y - box_bottom( b ) - 0.1;  // to compensate rounding errors
    
    // update box contacts
    b->contacts.bottom = true;
}

// -----------------------------------------------------------------------------

void box_collide_with_left_boundary( box* b, float boundary_x )
{
    // determine if boundary penetration will happen next frame
    float box_next_left = box_left( b ) + b->delta_position.x;
    float penetration = boundary_x - box_next_left;
    if( penetration <= 0 ) return;
    
    // adjust box X position in next frame
    b->delta_position.x = boundary_x - box_left( b ) + 0.1;  // to compensate rounding errors
    
    // update box contacts
    b->contacts.left = true;
}

// -----------------------------------------------------------------------------

void box_collide_with_right_boundary( box* b, float boundary_x )
{
    // determine if boundary penetration will happen next frame
    float box_next_right = box_right( b ) + b->delta_position.x;
    float penetration = box_next_right - boundary_x;
    if( penetration <= 0 ) return;
    
    // adjust box X position in next frame
    b->delta_position.x = boundary_x - box_right( b ) - 0.1;  // to compensate rounding errors
    
    // update box contacts
    b->contacts.right = true;
}


// =============================================================================
//   BOX VS BOX: PARTIAL COLLISION FUNCTIONS
// =============================================================================


void boxes_collide_from_ref_left( box* reference, box* moved )
{
    float relative_delta_y = moved->delta_position.y - reference->delta_position.y;
    
    // evaluate extended contact surface across Y
    float reference_top = box_top( reference );
    float reference_bottom = box_bottom( reference );
    
    float moved_top_extended    = box_top( moved )    + fmin( 0.0, relative_delta_y );
    float moved_bottom_extended = box_bottom( moved ) + fmax( 0.0, relative_delta_y );
    
    // discard collision if there is no overlap in Y
    if( moved_top_extended >= reference_bottom || reference_top >= moved_bottom_extended )
      return;
    
    // we want to make sure we never move the box backwards, so
    // if the initial position was already colliding leave it be
    float moved_right = box_right( moved );
    float reference_left = box_left( reference );
    
    if( moved_right > (reference_left + 0.1) )   // to compensate rounding errors
      return;
    
    // detect if a collision will actually happen in next frame
    float excess_x = (moved_right + moved->delta_position.x)
                   - (reference_left + reference->delta_position.x);
    
    if( excess_x < 0 ) return;
    
    // adjust moved box's X position in the next frame
    float limit_x = reference_left + reference->delta_position.x;
    moved->delta_position.x = limit_x - moved_right;
    
    // update contacts in both boxes
    reference->contacts.left = true;
    moved->contacts.right = true;
}

// -----------------------------------------------------------------------------

void boxes_collide_from_ref_right( box* reference, box* moved )
{
    float relative_delta_y = moved->delta_position.y - reference->delta_position.y;
    
    // evaluate extended contact surface across Y
    float reference_top = box_top( reference );
    float reference_bottom = box_bottom( reference );
    
    float moved_top_extended    = box_top( moved )    + fmin( 0.0, relative_delta_y );
    float moved_bottom_extended = box_bottom( moved ) + fmax( 0.0, relative_delta_y );
    
    // discard collision if there is no overlap in Y
    if( moved_top_extended >= reference_bottom || reference_top >= moved_bottom_extended )
      return;
    
    // we want to make sure we never move the box backwards, so
    // if the initial position was already colliding leave it be
    float moved_left = box_left( moved );
    float reference_right = box_right( reference );
    
    if( moved_left < (reference_right - 0.1) )   // to compensate rounding errors
      return;
    
    // detect if a collision will actually happen in next frame
    float excess_x = (reference_right + reference->delta_position.x)
                   - (moved_left + moved->delta_position.x);
    
    if( excess_x < 0 ) return;
    
    // adjust moved box's X position in the next frame
    float limit_x = reference_right + reference->delta_position.x;
    moved->delta_position.x = limit_x - moved_left;
    
    // update contacts in both boxes
    reference->contacts.right = true;
    moved->contacts.left = true;
}

// -----------------------------------------------------------------------------

void boxes_collide_from_ref_top( box* reference, box* moved )
{
    float relative_delta_x = moved->delta_position.x - reference->delta_position.x;
    
    // evaluate extended contact surface across X
    float reference_left  = box_left( reference );
    float reference_right = box_right( reference );
    
    float moved_left_extended  = box_left( moved )  + fmin( 0.0, relative_delta_x );
    float moved_right_extended = box_right( moved ) + fmax( 0.0, relative_delta_x );
    
    // discard collision if there is no overlap in X
    // (careful! we should be including equality)
    if( moved_left_extended > reference_right || reference_left > moved_right_extended )
      return;
    
    // this corrects problems with corner-corner contacts
    // (applied only in VERTICAL collisions)
    if( box_left( moved ) == reference_right && moved->velocity.x >= reference->velocity.x )
      return;
    
    if( box_right( moved ) == reference_left && moved->velocity.x <= reference->velocity.x )
      return;
    
    // we want to make sure we never move the box backwards, so
    // if the initial position was already colliding leave it be
    float moved_bottom = box_bottom( moved );
    float reference_top = box_top( reference );
    
    if( moved_bottom > (reference_top + 0.1) )   // to compensate rounding errors
      return;  
    
    // detect if a collision will actually happen in next frame
    float excess_y = (moved_bottom + moved->delta_position.y )
                   - (reference_top + reference->delta_position.y);
    
    if( excess_y < 0 ) return;
    
    // adjust moved box's Y position in the next frame
    float limit_y = reference_top + reference->delta_position.y;
    moved->delta_position.y = limit_y - moved_bottom;
    
    // update contacts in both boxes
    reference->contacts.top = true;
    moved->contacts.bottom = true;
}

// -----------------------------------------------------------------------------

void boxes_collide_from_ref_bottom( box* reference, box* moved )
{
    float relative_delta_x = moved->delta_position.x - reference->delta_position.x;
    
    // evaluate extended contact surface across X
    float reference_left  = box_left( reference );
    float reference_right = box_right( reference );
    
    float moved_left_extended  = box_left( moved )  + fmin( 0.0, relative_delta_x );
    float moved_right_extended = box_right( moved ) + fmax( 0.0, relative_delta_x );
    
    // discard collision if there is no overlap in X
    // (careful! we should be including equality)
    if( moved_left_extended > reference_right || reference_left > moved_right_extended )
      return;
    
    // this corrects problems with corner-corner contacts
    // (applied only in VERTICAL collisions)
    if( box_left( moved ) == reference_right && moved->velocity.x >= reference->velocity.x )
      return;
    
    if( box_right( moved ) == reference_left && moved->velocity.x <= reference->velocity.x )
      return;
    
    // we want to make sure we never move the box backwards, so
    // if the initial position was already colliding leave it be
    float moved_top = box_top( moved );
    float reference_bottom = box_bottom( reference );
    
    if( moved_top < (reference_bottom - 0.1) )   // to compensate rounding errors
      return;  
    
    // detect if a collision will actually happen in next frame
    float excess_y = (reference_bottom + reference->delta_position.y)
                   - (moved_top + moved->delta_position.y);
                  
    if( excess_y < 0 ) return;
    
    // adjust moved box's Y position in the next frame
    float limit_y = reference_bottom + reference->delta_position.y;
    moved->delta_position.y = limit_y - moved_top;
    
    // update contacts in both boxes
    reference->contacts.bottom = true;
    moved->contacts.top = true;
}


// =============================================================================
//   BOX VS BOX: FULL COLLISION FUNCTION
// =============================================================================


// collision between 2 boxes is always done taking one of them
// (reference) as "fixed", i.e. it won't be affected when the
// boxes collide; the other box (moved) will be the only one
// which position and velocity is adjusted to avoid penetration;
// this function assumes boxes do not overlap in current frame
void boxes_collide( box* reference, box* moved )
{
    // determine relative movement
    float relative_speed_x = moved->velocity.x - reference->velocity.x;
    float relative_speed_y = moved->velocity.y - reference->velocity.y;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // PART 1: COLLIDE ON Y AXIS
    
    // [1-A] moved box comes from the top
    if( relative_speed_y >= 0 )
      if( box_bottom( moved ) < box_bottom( reference ) )
        boxes_collide_from_ref_top( reference, moved );
    
    // [1-B] moved box comes from the bottom
    if( relative_speed_y <= 0 )
      if( box_top( moved ) > box_top( reference ) )
        boxes_collide_from_ref_bottom( reference, moved );
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // PART 2: COLLIDE ON X AXIS
    
    // [2-A] moved box comes from the left
    if( relative_speed_x >= 0 )
      if( box_right( moved ) < box_right( reference ) )
        boxes_collide_from_ref_left( reference, moved );
    
    // [2-B] moved box comes from the right
    if( relative_speed_x <= 0 )
      if( box_left( moved ) > box_left( reference ) )
        boxes_collide_from_ref_right( reference, moved );
}


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
