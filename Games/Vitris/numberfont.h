/* *****************************************************************************
*  Vircon32 library: "numberfont.h"                 File version: 2022/11/03   *
*  --------------------------------------------------------------------------- *
*  This header allows users to define a simplified text font specialized in    *
*  printing only numbers. Here you will only need to provide texture regions   *
*  for the digits 0-9 and the symbol characters '-' (for negative numbers)     *
*  and '.' (for floats).                                                       *
***************************************************************************** */

// *****************************************************************************
    // start include guard
    #ifndef NUMBERFONT_H
    #define NUMBERFONT_H
    
    // include standard Vircon headers
    #include "string.h"
    #include "math.h"
// *****************************************************************************


// =============================================================================
//   DEFINITION OF A CUSTOM NUMBER-ONLY FONT
// =============================================================================


struct numberfont
{
    // texture regions for this font's characters '0' to '9' are 
    // taken as 10 consecutive region IDs within a same texture
    int texture_id;
    int region_id_zero;
    
    // texture regions for our non-digit symbols
    int region_id_minus;
    int region_id_dot;
    
    // fixed character dimensions, in pixels
    // (these are global, and apply to all characters)
    int character_width, character_height;
    
    // horizontal separation in pixels between adjacent
    // characters (it can be negative for overlap)
    int character_separation;
};


// =============================================================================
//   NUMBER FONT: AUXILIARY FUNCTIONS
// =============================================================================


// this function just prints a string left-aligned, but it will take
// into account that only certain characters exist in a number font;
// this function is not to be used on its own! instead it will be
// called by our end-user number print functions.
void numberfont_print_string_from_left( numberfont* nf, int left_x, int y, int* text )
{
    select_texture( nf->texture_id );
    int step_x = nf->character_width + nf->character_separation;
    
    while( *text )
    {
        int character = *text;
        
        // print this character only if it is one of the
        // digits or symbols provided by a number font
        if( isdigit( character ) )
        {
            select_region( nf->region_id_zero + (character - '0') );
            draw_region_at( left_x, y );
        }
        else if( character == '-' )
        {
            select_region( nf->region_id_minus );
            draw_region_at( left_x, y );
        }
        else if( character == '.' )
        {
            select_region( nf->region_id_dot );
            draw_region_at( left_x, y );
        }          
        
        // advance in x
        left_x += step_x;
        text++;
    }
}


// =============================================================================
//   NUMBER FONT: END-USER FUNCTIONS
// =============================================================================


// this function prints an integer number in a fixed amount of digits,
// using only digits 0-9, as when printed in some digital displays;
// negative numbers are not supported, and out of range numbers will
// be clamped to the valid range; for instance, using for 4 digits 57
// would be printed as '0057', and numbers would be clamped to range
// [0000-9999]; the number is printed left-aligned
void numberfont_print_int_fixed( numberfont* nf, int left_x, int y, int number, int digits )
{
    // sanity check
    if( digits < 1 ) digits = 1;
    
    // determine maximum number we can represent
    int number_range = pow( 10, digits );
    int max_number = number_range - 1;
    
    // clamp number to the range we can represent
    if( number < 0 ) number = 0;
    if( number >= max_number ) number = max_number;
    
    // convert number to a string but adding a leading 1
    int[ 20 ] number_string;
    itoa( number_range + number, number_string, 10 );
    
    // print number as a string but begin from the second character
    // so that 1 is removed but any needed leading zeroes remain
    numberfont_print_string_from_left( nf, left_x, y, &number_string[ 1 ] );
}

// -----------------------------------------------

// prints an integer, left-aligned
void numberfont_print_int_from_left( numberfont* nf, int left_x, int y, int number )
{
    // convert integer to string
    int[ 20 ] number_string;
    itoa( number, number_string, 10 );
    
    // print integer as a string
    numberfont_print_string_from_left( nf, left_x, y, number_string );
}

// -----------------------------------------------

// prints an integer, right-aligned
void numberfont_print_int_from_right( numberfont* nf, int right_x, int y, int number )
{
    // first convert the number to a string
    int[ 20 ] number_string;
    itoa( number, number_string, 10 );
    
    // determine text width in pixels
    int string_length = strlen( number_string );
    int width = string_length * nf->character_width;
    width += (string_length-1) * nf->character_separation;
    
    // now we can print from the left as usual
    numberfont_print_string_from_left( nf, right_x - width + 1, y, number_string );
}

// -----------------------------------------------

// prints an integer, centered
void numberfont_print_int_centered( numberfont* nf, int center_x, int y, int number )
{
    // first convert the number to a string
    int[ 20 ] number_string;
    itoa( number, number_string, 10 );
    
    // determine text width in pixels
    int string_length = strlen( number_string );
    int width = string_length * nf->character_width;
    width += (string_length-1) * nf->character_separation;
    
    // now we can print from the left as usual
    numberfont_print_string_from_left( nf, center_x - width/2, y, number_string );
}

// -----------------------------------------------

// prints a float, left-aligned
void numberfont_print_float_from_left( numberfont* nf, int left_x, int y, float number )
{
    // convert float to string
    int[ 30 ] number_string;
    ftoa( number, number_string );
    
    // print integer as a string
    numberfont_print_string_from_left( nf, left_x, y, number_string );
}

// -----------------------------------------------

// prints a float, right-aligned
void numberfont_print_float_from_right( numberfont* nf, int right_x, int y, float number )
{
    // convert float to string
    int[ 30 ] number_string;
    ftoa( number, number_string );
    
    // determine text width in pixels
    int string_length = strlen( number_string );
    int width = string_length * nf->character_width;
    width += (string_length-1) * nf->character_separation;
    
    // now we can print from the left as usual
    numberfont_print_string_from_left( nf, right_x - width + 1, y, number_string );
}

// -----------------------------------------------

// prints a float, centered
void numberfont_print_float_centered( numberfont* nf, int center_x, int y, float number )
{
    // convert float to string
    int[ 30 ] number_string;
    ftoa( number, number_string );
    
    // determine text width in pixels
    int string_length = strlen( number_string );
    int width = string_length * nf->character_width;
    width += (string_length-1) * nf->character_separation;
    
    // now we can print from the left as usual
    numberfont_print_string_from_left( nf, center_x - width/2, y, number_string );
}


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
