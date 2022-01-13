// =============================================================================
//   STRING HANDLING FUNCTIONS
// =============================================================================


void strcpy( int* dest_text, int* src_text )
{
    while( *src_text )
    {
        *dest_text = *src_text;
        ++dest_text;
        ++src_text;
    }
    
    // add null termination
    *dest_text = 0;
}

// -----------------------------------------------------------------------------

void strncpy( int* dest_text, int* src_text, int max_characters )
{
    // check for the weird cases
    if( max_characters < 1 )
      return;
    
    // now actually copy
    while( *src_text )
    {
        *dest_text = *src_text;
        ++dest_text;
        ++src_text;
        
        // exit early if max characters are reached
        --max_characters;
        
        if( max_characters <= 0 )
          break;
    }
    
    // add null termination
    *dest_text = 0;
}

// -----------------------------------------------------------------------------


// initial text will be modified
// careful! no control of buffer length is done
void strcat( int* initial_text, int* added_text )
{
    // first, find the null termination
    // for the original text
    while( *initial_text )
      ++initial_text;
    
    // now operate just as strcpy
    while( *added_text )
    {
        *initial_text = *added_text;
        ++initial_text;
        ++added_text;
    }
    
    // add null termination
    *initial_text = 0;
}

// -----------------------------------------------------------------------------

void strncat( int* initial_text, int* added_text, int max_characters )
{
    // check for the weird cases
    if( max_characters < 1 )
      return;
    
    // first, find the null termination
    // for the original text
    while( *initial_text )
      ++initial_text;
    
    // now operate just as in strcpy
    while( *added_text )
    {
        *initial_text = *added_text;
        ++initial_text;
        ++added_text;
        
        // exit early if max characters are reached
        --max_characters;
        
        if( max_characters <= 0 )
          break;
    }
    
    // add null termination
    *initial_text = 0;
}


// =============================================================================
//   CONVERSION OF NUMBERS TO STRING
// =============================================================================


// careful! no control of buffer length is done
void itoa16( int value, int* result_text )
{
    int* digits = "0123456789ABCDEF";
    int[ 9 ] hex_string;
    
    // add 8 digits starting from the right
    for( int i = 7; i >= 0; --i )
    {
        hex_string[ i ] = digits[ value & 15 ];
        value >>= 4;
    }
    
    // now place the digits into result
    hex_string[ 8 ] = 0;
    strcpy( result_text, hex_string );
}


// =============================================================================
//   TEXT PRINTING FUNCTIONS
// =============================================================================


// properties of the bios font
#define character_width  10
#define character_height 20

// -----------------------------------------------------------------------------

// assumes that the text string is terminated with a 0;
// text is drawn with reference on its top-left corner;
// and any '\n' character will result in a new line
void print( int drawing_x, int drawing_y, int* text )
{
    int initial_drawing_x = drawing_x;
    
    while( *text )
    {
        
        // print this character
        select_region( *text );
        draw_region_at( drawing_x, drawing_y );
        
        // advance in x
        drawing_x += character_width;
        
        // execute line breaks
        if( *text == '\n' )
        {
            // y advances, x is reset
            drawing_x = initial_drawing_x;
            drawing_y += character_height;
        }
        
        // go to next character
        ++text;
    }
}
