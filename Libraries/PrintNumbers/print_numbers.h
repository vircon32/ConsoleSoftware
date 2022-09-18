/* *****************************************************************************
*  Vircon32 library: "print_numbers.h"            File version: 2022/09/18     *
*  --------------------------------------------------------------------------- *
*  This header includes functions that make it easier to convert numbers to    *
*  string and printing them with the default BIOS text font.                   *
*                                                                              *
*  Keep in mind that:                                                          *
*   - same as print(), print number functions will modify the selected region. *
*   - append functions will asume your array to be long enough, so be careful. *
***************************************************************************** */

// *****************************************************************************
    // start include guard
    #ifndef PRINT_NUMBERS_H
    #define PRINT_NUMBERS_H
    
    // include standard Vircon headers
    #include "video.h"
    #include "string.h"
// *****************************************************************************


// =============================================================================
//   PRINTING NUMBERS ON SCREEN
// =============================================================================


// prints a bool as true/false
void print_bool_at( int x, int y, bool value )
{
    if( value )  print_at( x, y, "true" );
    else         print_at( x, y, "false" );
}

// ---------------------------------------------------------

// prints an integer in decimal format
void print_int_at( int x, int y, int value )
{
    int[ 20 ] value_string;
    itoa( value, value_string, 10 );
    print_at( x, y, value_string );
}

// ---------------------------------------------------------

// Prints an integer in hexadecimal format. Letters
// A-F will be in upper case. No prefix or suffix
// will be added, just the digits themselves, so
// be carefut not to confuse it with a decimal value
void print_hex_at( int x, int y, int value )
{
    int[ 20 ] value_string;
    itoa( value, value_string, 16 );
    print_at( x, y, value_string );
}

// ---------------------------------------------------------

// prints a float in decimal notation; exponential
// notation is not supported, so very large or
// small values may not be printed correctly
void print_float_at( int x, int y, float value )
{
    int[ 20 ] value_string;
    ftoa( value, value_string );
    print_at( x, y, value_string );
}


// =============================================================================
//   APPENDING NUMBERS TO STRINGS
// =============================================================================


// takes a string and appends to it a bool as true/false.
void add_bool_to_string( int* string, bool value )
{
    if( value )  strcat( string, "true" );
    else         strcat( string, "false" );
}

// ---------------------------------------------------------

// takes a string and appends to it an integer in decimal format
void add_int_to_string( int* string, int value )
{
    // convert number to string
    int[ 20 ] value_string;
    itoa( value, value_string, 10 );
    
    // concatenate the converted number to the original string
    strcat( string, value_string );
}

// ---------------------------------------------------------

// takes a string and appends to it an integer in hexadecimal format
void add_hex_to_string( int* string, int value )
{
    // convert number to string
    int[ 20 ] value_string;
    itoa( value, value_string, 16 );
    
    // concatenate the converted number to the original string
    strcat( string, value_string );
}

// ---------------------------------------------------------

// takes a string and appends to it a float number in decimal notation
void add_float_to_string( int* string, float value )
{
    // convert number to string
    int[ 30 ] value_string;
    ftoa( value, value_string );
    
    // concatenate the converted number to the original string
    strcat( string, value_string );
}


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
