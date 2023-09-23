/* *****************************************************************************
*  Vircon32 library: "bbox.h"                     File version: 2023/09/23     *
*  --------------------------------------------------------------------------- *
*  This header defines 2D axis-aligned bounding boxes as a 2D generalization   *
*  1D intervals. It also provides functions for general operations with them.  *
***************************************************************************** */

// *****************************************************************************
    // start include guard
    #ifndef BBOX_H
    #define BBOX_H
    
    // check for dependency: interval.h
    #ifndef INTERVAL_H
      #error "This header requires interval.h to be included before it"
    #endif
    
    // check for dependency: vector2d.h
    #ifndef VECTOR2D_H
      #error "This header requires vector2d.h to be included before it"
    #endif
// *****************************************************************************


// =============================================================================
//   DEFINITION OF A 2D AXIS-ALIGNED BOUNDING BOX
// =============================================================================


struct bbox
{
    interval range_x;
    interval range_y;
};


// =============================================================================
//   BOUNDING BOX PROPERTIES
// =============================================================================


bool bbox_is_empty( bbox* b )
{
    return interval_is_empty( &b->range_x )
        || interval_is_empty( &b->range_y );
}

// ---------------------------------------------------------

float bbox_area( bbox* b )
{
    if( bbox_is_empty( b ) )
      return -1;
    
    return interval_length( &b->range_x )
         * interval_length( &b->range_y );
}

// ---------------------------------------------------------

void bbox_get_center( bbox* b, vector2d* center )
{
    center->x = interval_center( &b->range_x );
    center->y = interval_center( &b->range_y );
}

// ---------------------------------------------------------

bool bbox_contains_point( bbox* b, float point_x, float point_y )
{
    return interval_contains_number( &b->range_x, point_x )
        && interval_contains_number( &b->range_y, point_y );
}


// =============================================================================
//   MODIFYING A BOUNDING BOX
// =============================================================================


void bbox_extend( bbox* b, float distance )
{
    interval_extend( &b->range_x, distance );
    interval_extend( &b->range_y, distance );
}

// ---------------------------------------------------------

void bbox_displace( bbox* b, float distance_x, float distance_y )
{
    interval_displace( &b->range_x, distance_x );
    interval_displace( &b->range_y, distance_y );
}


// =============================================================================
//   RELATIONS BETWEEN BOUNDING BOXES
// =============================================================================


// don't use intervals union function: union of 2 intervals
// is empty if they are disjoint; bounds, however, are meant
// to be expanded to cover the full range even if disjoint
void bboxes_get_union( bbox* b1, bbox* b2, bbox* result )
{
    result->range_x.minimum = fmin( b1->range_x.minimum, b2->range_x.minimum );
    result->range_x.maximum = fmax( b1->range_x.maximum, b2->range_x.maximum );
    result->range_y.minimum = fmin( b1->range_y.minimum, b2->range_y.minimum );
    result->range_y.maximum = fmax( b1->range_y.maximum, b2->range_y.maximum );
}

// ---------------------------------------------------------

void bboxes_get_intersection( bbox* b1, bbox* b2, bbox* result )
{
    intervals_get_intersection( &b1->range_x, &b2->range_x, &result->range_x );
    intervals_get_intersection( &b1->range_y, &b2->range_y, &result->range_y );
}

// ---------------------------------------------------------

bool bboxes_overlap( bbox* b1, bbox* b2 )
{
    return intervals_overlap( &b1->range_x, &b2->range_x )
        && intervals_overlap( &b1->range_y, &b2->range_y );
}

// ---------------------------------------------------------

bool bbox_contains_bbox( bbox* container, bbox* contained )
{
    return interval_contains_interval( &container->range_x, &contained->range_x )
        && interval_contains_interval( &container->range_y, &contained->range_y );
}


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
