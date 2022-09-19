// *****************************************************************************
    // include standard Vircon headers
    #include "time.h"
    
    // include additional headers
    #include "../print_numbers.h"
// *****************************************************************************


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // set a blue background
    clear_screen( color_blue );
    
    // PART 1: At the left draw some numbers, in white
    set_multiply_color( color_white );
    print_bool_at( 100, 50, false );
    print_int_at( 100, 100, -25000 );
    print_hex_at( 100, 150, 0xABCDEF );
    print_float_at( 100, 200, 123.4567 );
    
    // PART 2: At the right print some strings with numbers, in yellow
    set_multiply_color( color_yellow );
    
    int[ 40 ] BoolString = "Boolean = ";
    add_bool_to_string( BoolString, true );
    print_at( 400, 50, BoolString );
    
    int[ 40 ] IntString = "Int = ";
    add_int_to_string( IntString, 999999 );
    print_at( 400, 100, IntString );
    
    int[ 40 ] HexString = "Hex = ";
    add_hex_to_string( HexString, 0x1234AB );
    strcat( HexString, " h" );  // add a hex indication
    print_at( 400, 150, HexString );
    
    int[ 40 ] FloatString = "Float = ";
    add_float_to_string( FloatString, -0.0015 );
    print_at( 400, 200, FloatString );
    
    // our program has ended; the screen
    // will keep showing everything we drew
}
