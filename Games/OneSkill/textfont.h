/* *****************************************************************************
*  Vircon32 library: "textfont.h"                   File version: 2022/11/03   *
*  --------------------------------------------------------------------------- *
*  This header allows users to define a custom text font and use it to print   *
*  text. These fonts only use ASCII characters, and they may define either     *
*  just the 128 standard ASCII characters, or the full 256 to also include     *
*  extended ASCII.                                                             *
*                                                                              *
*  These fonts provide support for multi-line texts as well as left, center    *
*  and right alignment. Printing int and float numbers is also supported.      *
***************************************************************************** */

// *****************************************************************************
    // start include guard
    #ifndef TEXTFONT_H
    #define TEXTFONT_H
    
    // include standard Vircon headers
    #include "string.h"
    #include "math.h"
// *****************************************************************************


// =============================================================================
//   DEFINITION OF A FULL CUSTOM TEXT FONT
// =============================================================================


struct textfont
{
    // texture regions for this font's characters are taken
    // as consecutive region IDs within a same texture; the
    // first of those regions corresponds to character zero
    int texture_id;
    int character_zero_region_id;
    
    // fixed character dimensions, in pixels
    // (these are global, and apply to all characters)
    int character_width, character_height;
    
    // when this flag is false, the font will behave as
    // constant-width and all characters will use the
    // fixed "character_width" field; when true, the
    // font will be considered "variable-width", and
    // each character will be looked into the array
    // "variable_character_widths" for their width
    bool use_variable_width;
    int[ 256 ] variable_character_widths;
    
    // horizontal separation in pixels between adjacent
    // characters (it can be negative for overlap)
    int character_separation;
    
    // vertical separation in pixels between consecutive
    // lines (it can be negative for overlap)
    int line_separation;
};


// =============================================================================
//   TEXT FONT: AUXILIARY FUNCTIONS
// =============================================================================


// for the given font, returns a character's width in pixels
int textfont_get_character_width( textfont* tf, int character )
{
    if( tf->use_variable_width )
      return tf->variable_character_widths[ character ];
    
    return tf->character_width;
}

// -----------------------------------------------

// for the given font, measures the total length of a text
// line in pixels so that our print functions can align it
int textfont_get_line_width( textfont* tf, int* text )
{
    int line_width = 0;
    
    // measure until end of string or line
    while( (*text) && (*text != '\n') )
    {
        // add separation only between characters
        // (and not before the first character)
        if( line_width != 0 )
          line_width += tf->character_separation;
        
        // measure character
        line_width += textfont_get_character_width( tf, *text );
        text++;
    }
    
    return line_width;
}

// -----------------------------------------------

// this function only prints until the end of a line; this function
// is not to be used on its own! instead it will be called by our
// multi-line print functions; that's why it doesn't bother selecting
// the texture; returns the lentgh in characters of the printed line
// so that those functions can skip to the next line
int textfont_print_line_from_left( textfont* tf, int left_x, int y, int* text )
{
    int line_length = 0;
    
    // continue until end of string or line
    while( (*text) && (*text != '\n') )
    {
        // print this character
        select_region( tf->character_zero_region_id + *text );
        draw_region_at( left_x, y );
        
        // advance in x
        left_x += textfont_get_character_width( tf, *text ) + tf->character_separation;
        text++;
        
        // count the characters
        line_length++;
    }
    
    return line_length;
}


// =============================================================================
//   TEXT FONT: END-USER MULTI-LINE FUNCTIONS
// =============================================================================


// general function to print multi-line text, left-aligned
void textfont_print_from_left( textfont* tf, int left_x, int y, int* text )
{
    select_texture( tf->texture_id );
    
    // keep printing lines until the string ends
    while( *text )
    {
        // print this line
        int line_length = textfont_print_line_from_left( tf, left_x, y, text );
        
        // advance to next line
        text += line_length;
        y += tf->character_height + tf->line_separation;
        
        // then detect and skip the '\n' character
        // since it is not counted as part of the line
        if( *text == '\n' )
          text++;
    }
}

// -----------------------------------------------

// general function to print multi-line text, right-aligned
void textfont_print_from_right( textfont* tf, int right_x, int y, int* text )
{
    select_texture( tf->texture_id );
    
    // keep printing lines until the string ends
    while( *text )
    {
        // calculate this line's position
        int line_width = textfont_get_line_width( tf, text );
        int left_x = right_x - line_width + 1;
        
        // print this line
        int line_length = textfont_print_line_from_left( tf, left_x, y, text );
        
        // advance to next line
        text += line_length;
        y += tf->character_height + tf->line_separation;
        
        // then detect and skip the '\n' character
        // since it is not counted as part of the line
        if( *text == '\n' )
          text++;
    }
}

// -----------------------------------------------

// general function to print multi-line text, centered
void textfont_print_centered( textfont* tf, int center_x, int y, int* text )
{
    select_texture( tf->texture_id );
    
    // keep printing lines until the string ends
    while( *text )
    {
        // calculate this line's position
        int line_width = textfont_get_line_width( tf, text );
        int left_x = center_x - line_width / 2;
        
        // print this line
        int line_length = textfont_print_line_from_left( tf, left_x, y, text );
        
        // advance to next line
        text += line_length;
        y += tf->character_height + tf->line_separation;
        
        // then detect and skip the '\n' character
        // since it is not counted as part of the line
        if( *text == '\n' )
          text++;
    }
}


// =============================================================================
//   TEXT FONT: OTHER USEFUL FUNCTIONS
// =============================================================================


// this function measures width for all characters texture
// regions and assigns all font character widths to match those
void textfont_read_region_widths( textfont* tf )
{
    select_texture( tf->texture_id );
    
    for( int i = 0; i < 256; ++i )
    {
        select_region( tf->character_zero_region_id + i );
        int region_left, region_right;
        
        asm
        {
            "in R0, GPU_RegionMinX"
            "mov {region_left}, R0"
            "in R0, GPU_RegionMaxX"
            "mov {region_right}, R0"
        }
        
        tf->variable_character_widths[ i ] = abs( region_right - region_left + 1 );
    }
}

// -----------------------------------------------

// prints an integer number, left-aligned
void textfont_print_int_from_left( textfont* tf, int left_x, int y, int value )
{
    // convert integer to string
    int[ 20 ] value_string;
    itoa( value, value_string, 10 );
    
    // print the integer as a string
    textfont_print_line_from_left( tf, left_x, y, value_string );
}

// -----------------------------------------------

// prints a float number, left-aligned
void textfont_print_float_from_left( textfont* tf, int left_x, int y, float value )
{
    // convert float to string
    int[ 30 ] value_string;
    ftoa( value, value_string );
    
    // print the float as a string
    textfont_print_line_from_left( tf, left_x, y, value_string );
}


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
