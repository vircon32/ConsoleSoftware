/* -----------------------------------------------
   This tutorial step is similar to the previous
   one, but instead of having the same width for
   all characters we allow some of them to be a
   different width to improve the overall look
   of text. This implies that we now need to
   determine width and separation independently
   for every character, and that just knowing
   a text length does not determine its width.
   
   Note that this particular font does not
   distinguish uppercase and lowercase letters,
   since both use the same uppercase design. 
/  -------------------------------------------- */


// include Vircon libraries
#include "video.h"
#include "string.h"

// give names to our textures
// (always consecutive values from 0)
#define TextureFont22x32  0

// Name the first region for our font characters.
// We will use them as consecutive indices.
#define FirstRegionTextFont  0

// Default size of our characters in pixels.
// Width however may vary per character.
#define CharacterWidth  22
#define CharacterHeight 32

// we will apply separation along X (between
// characters) and Y (between lines)
#define CharacterSeparationX  1
#define CharacterSeparationY  15


// -----------------------------------------------
// TEXT PRINTING FUNCTIONS
// -----------------------------------------------
// The function printing text works the same way
// as in the example for constant width font. But
// now we need to have a function that determines
// the width of every given character.
// -----------------------------------------------


// the width of some characters has been tuned
// by hand to make the overall text look better
int GetCharacterWidth( int c )
{
    // our font does not distinguish
    // case, so take advantage of that
    c = tolower( c );
    
    // these characters are wider
    if( c == 'm' || c == 'w' )
      return 25;
    
    // this character is shorter
    if( c == 'i' )
      return 12;
    
    // spaces look better if shorter
    if( c == ' ' )
      return 16;
    
    // these punctuation characters are narrow
    if( c == '!' || c == ':' || c == ';' || c == ',' || c == '.' || c == '\''|| c == '|' )
      return 14;
    
    // other characters use the default width
    return CharacterWidth;
}

// -----------------------------------------------

void PrintText( int LeftX, int BaselineY, int* Text )
{
    select_texture( TextureFont22x32 );
    int DrawingX = LeftX;
    
    // strings in C are terminated with a null
    // character, so keep printing until we find it
    while( *Text )
    {
        // print this character
        select_region( *Text );
        draw_region_at( DrawingX, BaselineY );
        
        // advance in x
        DrawingX += GetCharacterWidth( *Text ) + CharacterSeparationX;
        
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
    select_texture( TextureFont22x32 );

    // First, we define our font as if it was constant
    // width, since most characters will fit in that
    // space. This means we will define 128 consecutive
    // regions with the same size and hotspot position.
    define_region_matrix
    (
        FirstRegionTextFont,    // ID of the first defined region
        0,0,                    // top-left pixel of the first character
        21,31,                  // bottom-right pixel of the first character
        0,31,                   // hotspot pixel of the first character
        16,8,                   // dimensions of the region matrix (16 in X, 8 in Y)
        0                       // separation between regions, in pixels
    );
    
    // We could leave things like this (this font
    // is also usable as constant width). However
    // we now redefine some characters with their
    // different-width versions as a second stage.
    // Note how we can use characters as region IDs
    // because we take advantage of the font layout.
    // Again, uppercase and lowercase are the same.
    select_region( 'M' );
    define_region( 22,0,  46,31,  22,31 );
    select_region( 'm' );
    define_region( 22,0,  46,31,  22,31 );
    
    select_region( 'W' );
    define_region( 66,0,  90,31,  66,31 );
    select_region( 'w' );
    define_region( 66,0,  90,31,  66,31 );
    
    select_region( 'I' );
    define_region( 110,0,  121,31,  110,31 );
    select_region( 'i' );
    define_region( 110,0,  121,31,  110,31 );
    
    // ------------------------------------
    // PART 2: DRAW ON SCREEN
    // ------------------------------------
    
    // clear the screen in some visible color
    clear_screen( color_blue );
    
    // print text using our font; for clarity
    // we split the string literal along its
    // 3 lines, though that is not needed.
    PrintText( 20, 50, "This is a MULTI-LINE text!\n"
                       "We can now test our CUSTOM\n"
                       "text font in Vircon32..." );
}
