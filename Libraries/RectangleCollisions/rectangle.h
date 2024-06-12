/* *****************************************************************************
*  Vircon32 library: "rectangle.h"                  File version: 2024/06/12   *
*  --------------------------------------------------------------------------- *
*  This header defines 2D non axis-aligned rectangles meant to be placed in    *
*  game world coordinates, have movement and be affected by collisions and     *
*  events. This library provides only basic static collision treatment for     *
*  general rectangular shapes. It does not include any predefined way to mix   *
*  rectangles with other shapes (like the collision boxes in box.h ).          *
***************************************************************************** */

// *****************************************************************************
    // start include guard
    #ifndef RECTANGLE_H
    #define RECTANGLE_H
    
    // include standard libraries
    #include "math.h"
    
    // ensure dependencies are included
    #ifndef VECTOR2D_H
      #error "library vector2d.h needs to be included before this one"
    #endif
    
    #ifndef INTERVAL_H
      #error "library interval.h needs to be included before this one"
    #endif
// *****************************************************************************


// =============================================================================
//   NON AXIS-ALIGNED RECTANGLES FOR COLLISIONS AND EVENT DETECTION
// =============================================================================


// unlike boxes, rectangle position is considered always at their center;
// also note they currently include no motion or contact information
struct rectangle
{
    // box shape definition
    vector2d center;                 // rectangle position on game world
    float half_length, half_width;   // full dimensions are twice these
    float length_angle;              // angle of the length axis respect to the world
    
    // precalculated variables;
    // these need to be calculated for collision functions to work.
    float half_diagonal;
    interval self_projection_length;
    interval self_projection_width;
};


// =============================================================================
//   PROJECTING RECTANGLES ALONG SPECIFIC AXES
// =============================================================================


// we will take advantage of the fact that all our shapes are rectangles;
// this will project at once onto 2 ortogonal directions: angle and angle+90?
void rect_project( rectangle* r, float angle, interval* projection_l, interval* projection_w )
{
    // project center on origin
    float center_projection_l = r->center.x *  cos( angle ) + r->center.y * sin( angle );
    float center_projection_w = r->center.x * -sin( angle ) + r->center.y * cos( angle );
    
    // obtain semiprojection of rectangle body
    float relative_angle = r->length_angle - angle;
    float rect_semiprojection_l = r->half_length * fabs( cos(relative_angle) ) + r->half_width * fabs( cos(relative_angle+pi/2) );
    projection_l->minimum = center_projection_l - rect_semiprojection_l;
    projection_l->maximum = center_projection_l + rect_semiprojection_l;
    
    // obtain semiprojection of rectangle body
    float rect_semiprojection_w = r->half_length * fabs( sin(relative_angle) ) + r->half_width * fabs( sin(relative_angle+pi/2) );
    projection_w->minimum = center_projection_w - rect_semiprojection_w;
    projection_w->maximum = center_projection_w + rect_semiprojection_w;
}

// ---------------------------------------------------------

// this will also project at once onto 2 ortogonal directions, but
// given by the local axes of the rectangle itself: length and width
void rect_self_project( rectangle* r )
{
    // rotate rect around origin
    float length_angle = r->length_angle;
    float center_projection_l = r->center.x *  cos( length_angle ) + r->center.y * sin( length_angle );
    float center_projection_w = r->center.x * -sin( length_angle ) + r->center.y * cos( length_angle );
    
    // this is a simplified case
    r->self_projection_length.minimum = center_projection_l - r->half_length;
    r->self_projection_length.maximum = center_projection_l + r->half_length;
    
    r->self_projection_width.minimum = center_projection_w - r->half_width;
    r->self_projection_width.maximum = center_projection_w + r->half_width;
}


// =============================================================================
//   PRECALCULATING RECTANGLE PROPERTIES
// =============================================================================


// for fixed rectangles we can precalculate just once after creation;
// on moving rectangles we need to be recalculate to keep them updated
void rect_precalculate( rectangle* r )
{
    r->half_diagonal = sqrt( r->half_length * r->half_length + r->half_width * r->half_width );
    rect_self_project( r );
}


// =============================================================================
//   COLLISION DETECTION
// =============================================================================


// this is intended for broad-phase collision detection:
// a fast, less precise function that can discard many collisions
bool rects_can_discard_collision( rectangle* r1, rectangle* r2 )
{
    float SeparationDistance = r1->half_diagonal + r2->half_diagonal;
    
    float distance_x = fabs( r1->center.x - r2->center.x );
    if( distance_x > SeparationDistance ) return true;
    
    float distance_y = fabs( r1->center.y - r2->center.y );
    return (distance_y > SeparationDistance);
}

// ---------------------------------------------------------

// this does precise collision detection, but no collision resolution
bool rects_are_colliding( rectangle* r1, rectangle* r2 )
{
    // project r1 along r2's angle
    interval r1_along_r2_length, r1_along_r2_width;
    rect_project( r1, r2->length_angle, &r1_along_r2_length, &r1_along_r2_width );
    
    // check intervals along r2's axes for separation
    if( !intervals_overlap( &r1_along_r2_length, &r2->self_projection_length ) )
      return false;

    if( !intervals_overlap( &r1_along_r2_width, &r2->self_projection_width ) )
      return false;
    
    // project r2 along r1's angle
    interval r2_along_r1_length, r2_along_r1_width;
    rect_project( r2, r1->length_angle, &r2_along_r1_length, &r2_along_r1_width );
    
    // check intervals along r2's axes for separation
    if( !intervals_overlap( &r2_along_r1_length, &r1->self_projection_length ) )
      return false;

    if( !intervals_overlap( &r2_along_r1_width, &r1->self_projection_width ) )
      return false;
    
    // no separations detected
    return true;
}


// =============================================================================
//   COLLISION RESPONSE
// =============================================================================


// this functions does both collision detection AND, when colliding, applies
// collsion resolution. It does so by moving rectangle r1 along the axis with
// minimum penetration. The other rectangle (r2) is fixed. It will return true
// when there was a collision and, in that case, it will also provide the
// position correction made to r1 to solve the collision
bool rects_solve_collision( rectangle* r1, rectangle* r2, vector2d* r1_correction )
{
    // project r1 along r2's angle
    interval r1_along_r2_length, r1_along_r2_width;
    rect_project( r1, r2->length_angle, &r1_along_r2_length, &r1_along_r2_width );
    
    // check intervals along r2's axes for separation
    if( !intervals_overlap( &r1_along_r2_length, &r2->self_projection_length ) )
      return false;
    
    if( !intervals_overlap( &r1_along_r2_width, &r2->self_projection_width ) )
      return false;
    
    // project r2 along r1's angle
    interval r2_along_r1_length, r2_along_r1_width;
    rect_project( r2, r1->length_angle, &r2_along_r1_length, &r2_along_r1_width );
    
    // check intervals along r2's axes for separation
    if( !intervals_overlap( &r2_along_r1_length, &r1->self_projection_length ) )
      return false;

    if( !intervals_overlap( &r2_along_r1_width, &r1->self_projection_width ) )
      return false;
    
    // check which axis has the minimal intersection
    interval IR1L, IR1W, IR2L, IR2W;
    intervals_get_intersection( &r2_along_r1_length, &r1->self_projection_length, &IR1L );
    intervals_get_intersection( &r2_along_r1_width,  &r1->self_projection_width,  &IR1W );
    intervals_get_intersection( &r1_along_r2_length, &r2->self_projection_length, &IR2L );
    intervals_get_intersection( &r1_along_r2_width,  &r2->self_projection_width,  &IR2W );
    
    float separation_angle = r1->length_angle;
    float separation_length = interval_length( &IR1L );
    interval* separation_interval_r1 = &r1->self_projection_length;
    interval* separation_interval_r2 = &r2_along_r1_length;
    
    if( interval_length( &IR1W ) < separation_length )
    {
        separation_interval_r1 = &r1->self_projection_width;
        separation_interval_r2 = &r2_along_r1_width;
        separation_angle = r1->length_angle + pi/2;
        separation_length = interval_length( &IR1W );
    }
    
    if( interval_length( &IR2L ) < separation_length )
    {
        separation_interval_r1 = &r1_along_r2_length;
        separation_interval_r2 = &r2->self_projection_length;
        separation_angle = r2->length_angle;
        separation_length = interval_length( &IR2L );
    }
    
    if( interval_length( &IR2W ) < separation_length )
    {
        separation_interval_r1 = &r1_along_r2_width;
        separation_interval_r2 = &r2->self_projection_width;
        separation_angle = r2->length_angle + pi/2;
        separation_length = interval_length( &IR2W );
    }
    
    // choose direction to separate in that axis
    if( interval_center( separation_interval_r1 ) < interval_center( separation_interval_r2 ) )
      separation_length = -separation_length;
    
    // solve the collision by moving rectangle r1,
    // and provide the applied motion vector
    r1_correction->x = separation_length * cos( separation_angle );
    r1_correction->y = separation_length * sin( separation_angle );
    v2_self_add( &r1->center, r1_correction );
    
    return true;
}


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
