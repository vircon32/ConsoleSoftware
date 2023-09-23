// *****************************************************************************
    // include standard Vircon headers
    // (none needed here)
    
    // include additional headers
    #include "draw_primitives.h"
    #include "interval.h"
    #include "vector2d.h"
    #include "../bbox.h"
// *****************************************************************************


// ---------------------------------------------------------
//   BOUNDING BOX DRAWING FUNCTIONS
// ---------------------------------------------------------


// both functions draw assuming origin is at screen pixel (0,0)
void draw_bbox( bbox* b )
{
    draw_rectangle( b->range_x.minimum, b->range_y.minimum,
                    b->range_x.maximum, b->range_y.maximum );
}

// ---------------------------------------------------------

void draw_filled_bbox( bbox* b )
{
    draw_filled_rectangle( b->range_x.minimum, b->range_y.minimum,
                           b->range_x.maximum, b->range_y.maximum );
}


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // define a couple of bounding boxes
    bbox BoxCyan = { {300, 400}, {100, 200} };
    bbox BoxRed  = { {350, 500}, {150, 250} };
    
    // draw both boxes
    set_multiply_color( color_cyan );
    draw_bbox( &BoxCyan );
    set_multiply_color( color_red );
    draw_bbox( &BoxRed );
    
    // determine the union and intersection of both
    bbox BoxUnion;
    bboxes_get_union( &BoxCyan, &BoxRed, &BoxUnion );
    
    bbox BoxIntersection;
    bboxes_get_intersection( &BoxCyan, &BoxRed, &BoxIntersection );
    
    // draw union in green, slightly expanded to be visible
    bbox_extend( &BoxUnion, 2 );
    set_multiply_color( color_green );
    draw_bbox( &BoxUnion );
    
    // draw intersection in yellow, slightly contracted to be visible
    bbox_extend( &BoxIntersection, -2 );
    set_multiply_color( color_yellow );
    draw_filled_bbox( &BoxIntersection );
    
    // print indications for each bounding box
    set_multiply_color( color_cyan );
    print_at( 20, 20, "Bounding Box A" );
    set_multiply_color( color_red );
    print_at( 20, 50, "Bounding Box B" );
    set_multiply_color( color_green );
    print_at( 20, 80, "Union of A and B\n" "(expanded 2 pixels)" );
    set_multiply_color( color_yellow );
    print_at( 20, 130, "Intersection of A and B\n" "(contracted 2 pixels)" );
    
    // our program has ended; the screen
    // will keep showing everything we drew
}
