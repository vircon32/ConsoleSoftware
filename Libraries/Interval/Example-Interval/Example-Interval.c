// *****************************************************************************
    // include standard Vircon headers
    // (none needed here)
    
    // include additional headers; print_numbers must
    // be included before, so that interval can detect
    // it and interval printing functions are enabled
    #include "print_numbers.h"
    #include "../interval.h"
// *****************************************************************************


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // define a couple of intervals
    interval Reals = { minus_inf(), plus_inf() };
    interval ZeroOne = { 0, 1 };
    int[ 80 ] Text;
    
    // print both intervals
    strcpy( Text, "Interval Reals = " );
    add_interval_to_string( Text, &Reals );
    print_at( 20, 50, Text );
    
    strcpy( Text, "Interval ZeroOne = " );
    add_interval_to_string( Text, &ZeroOne );
    print_at( 20, 80, Text );
    
    // calculate their intersection
    interval Intersection;
    intervals_get_intersection( &Reals, &ZeroOne, &Intersection );
    strcpy( Text, "Intersection of both = " );
    add_interval_to_string( Text, &Intersection );
    print_at( 20, 110, Text );
    
    // determine a union
    interval OneThree = { 1, 3 };
    interval Union;
    intervals_get_union( &ZeroOne, &OneThree, &Union );
    strcpy( Text, "Union of [0,1] and [1,3] = " );
    add_interval_to_string( Text, &Union );
    print_at( 20, 140, Text );
    
    interval TwoThree = { 2, 3 };
    intervals_get_union( &ZeroOne, &TwoThree, &Union );
    strcpy( Text, "Union of [0,1] and [2,3] = " );
    add_interval_to_string( Text, &Union );
    print_at( 20, 170, Text );
    
    // contained intervals
    bool ContainsInterval = interval_contains_interval( &OneThree, &TwoThree );
    strcpy( Text, "[1,3] contains [2,3]? " );
    add_bool_to_string( Text, ContainsInterval );
    print_at( 20, 210, Text );
    
    // contained numbers
    bool ContainsNumber = interval_contains_number( &OneThree, 3.5 );
    strcpy( Text, "[1,3] contains 3.5? " );
    add_bool_to_string( Text, ContainsNumber );
    print_at( 20, 240, Text );
    
    // extend an interval
    interval Extended = ZeroOne;
    interval_extend( &Extended, 2 );
    strcpy( Text, "[0,1] extended by 2 = " );
    add_interval_to_string( Text, &Extended );
    print_at( 20, 270, Text );
    
    // our program has ended; the screen
    // will keep showing everything we drew
}
