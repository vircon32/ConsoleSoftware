/* -----------------------------------------------
   In this step of the tutorial we show how to
   write text with a custom font, instead of
   using the default BIOS font that print()
   functions use.
   
   This time we will cover the simplest case,
   in which all characters in our font have
   the exact same size.
/  -------------------------------------------- */


// include Vircon libraries
#include "video.h"
#include "string.h"

// give names to our textures
// (always consecutive values from 0)
#define TextureFont11x16  0

// Our font is a set is 128 characters, so
// it needs 128 regions. We will define them
// as consecutive to use them as an index.
// So we only need to name the first one.
#define FirstRegionTextFont  0

// size of our characters in pixels
// (our drawing functions will need it)
#define CharacterWidth  11
#define CharacterHeight 16

// we will apply separation along X (between
// characters) and Y (between lines)
#define CharacterSeparationX  1
#define CharacterSeparationY  8


// -----------------------------------------------
// TEXT PRINTING FUNCTION
// -----------------------------------------------
// This function works the same way as print_at()
// from the standard library, just that it uses
// our custom font. 
//
// For this to work, we rely on the characters
// following the same order as defined in the
// standard ASCII. That way we can treat each
// character code as an index within our set of
// character regions.
//
// Note that our font includes only the 128
// characters defined in standard ASCII. That
// means the function may print garbage if we use
// other characters (like non-english letters).
// -----------------------------------------------


void PrintText( int LeftX, int BaselineY, int* Text )
{
    select_texture( TextureFont11x16 );
    int DrawingX = LeftX;
    
    // strings in C are terminated with a null
    // character, so keep printing until we find it
    while( *Text )
    {
        // print this character
        select_region( *Text );
        draw_region_at( DrawingX, BaselineY );
        
        // advance in x
        DrawingX += CharacterWidth + CharacterSeparationX;
        
        // execute line breaks
        if( *Text == '\n' )
        {
            // y advances, x is reset
            BaselineY += CharacterHeight + CharacterSeparationY;
            DrawingX = LeftX;
        }
        
        // go to next character
        ++Text;
    }
    
    // Careful, after printing our text
    // the font texture remains selected!
    // Our code must take that into account.
}


// -----------------------------------------------
// MAIN FUNCTION
// -----------------------------------------------

void main( void )
{   
    // ------------------------------------
    // PART 1: DEFINE OUR FONT
    // ------------------------------------
    
    // select our only texture
    select_texture( TextureFont11x16 );

    // Define our custom font as 128 consecutive
    // regions with the same size and hotspot
    // position, arranged as a 16x8 matrix. Note
    // that for fonts it is usual to define the
    // hotspot at the base of the characters,
    // instead of at the top like we did when
    // we used only digits.
    define_region_matrix
    (
        FirstRegionTextFont,    // ID of the first defined region
        0,0,                    // top-left pixel of the first character
        10,15,                  // bottom-right pixel of the first character
        0,15,                   // hotspot pixel of the first character
        16,8,                   // dimensions of the region matrix (16 in X, 8 in Y)
        0                       // separation between regions, in pixels
    );
    
    // ------------------------------------
    // PART 2: DRAW ON SCREEN
    // ------------------------------------
    
    // clear the screen in some visible color
    clear_screen( color_blue );
    
    // print text using our font; for clarity
    // we split the string literal along its
    // 3 lines, though that is not needed.
    PrintText( 50, 50, "This is a MULTI-LINE text!\n"
                       "We can now test our CUSTOM\n"
                       "text font in Vircon32..." );
}
