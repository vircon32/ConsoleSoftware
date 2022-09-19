// *****************************************************************************
    // include standard Vircon headers
    #include "time.h"
    
    // include additional headers; print_numbers must
    // be included before, so that vector2d can detect
    // it and vector printing functions are enabled
    #include "print_numbers.h"
    #include "../vector2d.h"
// *****************************************************************************


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // define a couple of vectors
    vector2d UpLeft = { 1, -1 };
    vector2d Up = { 0, -1 };
    int[ 80 ] Text;
    
    // print both vectors
    strcpy( Text, "Vector UpLeft = " );
    add_vector2d_to_string( Text, &UpLeft );
    print_at( 20, 50, Text );
    
    strcpy( Text, "Vector Up = " );
    add_vector2d_to_string( Text, &Up );
    print_at( 20, 80, Text );
    
    // calculate their angle
    strcpy( Text, "Angle between them (in degrees) = " );
    int AngleInDegrees = round( (180.0 / pi) * v2_angle_between( &Up, &UpLeft ) );
    add_int_to_string( Text, AngleInDegrees );
    print_at( 20, 110, Text );
    
    // operate with them
    vector2d Result = UpLeft;
    vector2d Temporary;
    v2_self_multiply( &Result, 5 );
    v2_multiply( &Up, 2, &Temporary );
    v2_self_add( &Result, &Temporary );
    
    strcpy( Text, "5 UpLeft + 2 Up = " );
    add_vector2d_to_string( Text, &Result );
    
    // report the result's length too
    strcat( Text, ", which has a length of " );
    add_float_to_string( Text, v2_modulus( &Result ) );
    print_at( 20, 140, Text );
    
    // our program has ended; the screen
    // will keep showing everything we drew
}
