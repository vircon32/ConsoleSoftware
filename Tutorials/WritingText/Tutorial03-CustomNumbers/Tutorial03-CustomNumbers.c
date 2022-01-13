/* -----------------------------------------------
   Many games don't actually write any text, or
   just need a couple of fixed words that can be
   stored as premade images.
   
   Still, it's common that a game needs to show
   numbers with custom digits (for example, to
   display a score).
   
   In this step of the tutorial we show how to
   draw numbers this way, without having to
   prepare a full custom text font.
/  -------------------------------------------- */


// include Vircon libraries
#include "video.h"
#include "string.h"

// give names to our textures
// (always consecutive values from 0)
#define TextureNumbers  0

// give names to our texture regions
// (any values we want in the range 0-4095)
#define RegionScoreFrame  1

// these 2 need to be consecutive since
// define_region_matrix works that way
#define RegionMinus  10
#define RegionZero   11

// width of our digits in pixels
// (our drawing functions will need it)
#define DigitWidth  22


// -----------------------------------------------
// NUMBER PRINTING FUNCTIONS
// -----------------------------------------------
// These functions will automate the process of
// using our custom digits to print numbers. Note
// that none of them use strings, but the numbers
// directly. If we tried to use strings it would
// cause problems since we don't have a full font
// with correct character ordering.
//
// To print floats, extended versions of these
// functions could be created. This is harder to
// do, and would also require to have a decimal
// separator character in our texture.
// -----------------------------------------------

// This function prints an integer using our custom digits.
// It supports negative numbers, and will only print as many
// digits as needed (no leading zeroes) from left to right.
void PrintInteger( int LeftX, int TopY, int Number )
{
    // first, draw the minus sign if needed
    if( Number < 0 )
    {
        select_region( RegionMinus );
        draw_region_at( LeftX, TopY );
        LeftX += DigitWidth;
        
        // we can now treat the number
        // as if it was positive
        Number = -Number;
    }
    
    // We want to write from left to right, while
    // also avoiding to write any unneeded zeroes.
    // To do that we need to determine the leftmost
    // digit to know when we should start writing.
    // We test starting from the largest possible
    // digit for a 32-bit integer. 
    int CurrentDigitBase = 1000000000;
    
    // in this first loop, stop at 1: if the number
    // is zero we want to at least write 1 digit
    while( CurrentDigitBase > 1 )
    {
        if( Number >= CurrentDigitBase )
          break;
        
        CurrentDigitBase /= 10;
    }
    
    // now just draw all digits, including zeroes
    while( CurrentDigitBase > 0 )
    {
        int Digit = (Number / CurrentDigitBase) % 10;
        select_region( RegionZero + Digit );
        draw_region_at( LeftX, TopY );
        
        // advance to the right for next digit
        CurrentDigitBase /= 10;
        LeftX += DigitWidth;
    }
}

// -----------------------------------------------

// This second version will always print the number as 6 digits,
// so if our number is small it will add leading zeroes. This is
// a common way to print game scores. Note it does not support
// numbers outside the 6-digit range, which will be clamped.
void PrintScore( int LeftX, int TopY, int Number )
{
    // adjust number to the printable range (6 digits)
    if( Number < 000000 ) Number = 000000;  // written this way for clarity
    if( Number > 999999 ) Number = 999999;
    
    // an easy way to get leading zeroes is
    // to add 1000000 and omit the first digit
    Number += 1000000;
    int CurrentDigitBase = 100000;
    
    // now just draw all digits, including zeroes
    while( CurrentDigitBase > 0 )
    {
        int Digit = (Number / CurrentDigitBase) % 10;
        select_region( RegionZero + Digit );
        draw_region_at( LeftX, TopY );
        
        // advance to the right for next digit
        CurrentDigitBase /= 10;
        LeftX += DigitWidth;
    }    
}


// -----------------------------------------------
// MAIN FUNCTION
// -----------------------------------------------


void main( void )
{   
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    // select our only texture
    select_texture( TextureNumbers );
    
    // define a region for the score frame image
    // with its hotspot placed at its top-left
    select_region( RegionScoreFrame );
    define_region_topleft
    (
        1,36,       // top-left pixel
        150,103     // bottom-right pixel
    );
    
    // define our custom digits as 10 consecutive
    // regions with the same size and hotspot position;
    // they become 11 by adding the minus sign too
    define_region_matrix
    (
        RegionMinus,    // ID of the first defined region
        1,1,            // top-left pixel of the first region (minus sign)
        22,32,          // bottom-right pixel of the first region (minus sign)
        1,1,            // hotspot pixel of the first region (minus sign)
        11,1,           // dimensions of the region matrix (11 in X, 1 in Y)
        1               // separation between regions, in pixels
    );
    
    // ------------------------------------
    // PART 2: DRAW ON SCREEN
    // ------------------------------------
    
    // clear the screen in some visible color
    clear_screen( color_cyan );
    
    // use the first function to print a negative number
    select_region( RegionScoreFrame );
    draw_region_at( 10,10 );
    PrintInteger( 19, 37, -208 );
    
    // Use the second function to print 6-digit score.
    // We will use yellow color to show that this can
    // also work with custom fonts if they are white
    select_region( RegionScoreFrame );
    draw_region_at( 210,10 );
    set_multiply_color( color_yellow );
    PrintScore( 219, 37, 208 );
}
