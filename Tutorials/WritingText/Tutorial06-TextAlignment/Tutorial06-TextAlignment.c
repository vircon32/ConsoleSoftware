/* -----------------------------------------------
   Here we take the same variable-width font as
   in the previous step of the tutorial, and
   show how to control text alignment so that it
   becomes possible to write text not just from
   left to right, but also right to left and
   centered. 
   
   For simplicity this tutorial does not cover
   vertical alignment: in most cases, a game
   will not need multiline strings anyway. But
   it should be easy to expand this code with
   the ability to determine text height and its
   vertical placement.
   
   Also, an improvement over the previous step
   is that print functions will re-select the
   texture that was active before printing,
   just like in the standard print functions
   (not needed here, but a good practice).
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
// TEXT MEASURING FUNCTIONS
// -----------------------------------------------
// We only measure text width since we are not
// covering vertical alignment in this tutorial
// -----------------------------------------------


// this function remains the same as previously
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

// this measures the total length of a text line
// in order for our print functions to align it
int GetLineWidth( int* Text )
{
    int LineWidth = 0;
    
    // measure until end of string or line
    while( (*Text) && (*Text != '\n') )
    {
        // add separation only between characters
        // (and not before the first character)
        if( LineWidth != 0 )
          LineWidth += CharacterSeparationX;
        
        // measure character
        LineWidth += GetCharacterWidth( *Text );
        Text++;
    }
    
    return LineWidth;
}


// -----------------------------------------------
// TEXT PRINTING FUNCTIONS
// -----------------------------------------------
// Printing text works the same way as in the
// previous tutorial step, but now we add 2 new
// versions of our text printing function that
// write text 
// -----------------------------------------------


// This function only prints until the end of a line.
// This function is not to be used on its own! Instead
// it will be called by our multi-line print functions.
// That's why it doesn't bother selecting the texture.
// Returns the lentgh in characters of the printed line
// so that those functions can skip to the next line.
int PrintLineFromLeft( int LeftX, int BaselineY, int* Text )
{
    select_texture( TextureFont22x32 );
    int DrawingX = LeftX;
    int LineLength = 0;
    
    // continue until end of string or line
    while( (*Text) && (*Text != '\n') )
    {
        // print this character
        select_region( *Text );
        draw_region_at( DrawingX, BaselineY );
        
        // advance in x
        DrawingX += GetCharacterWidth( *Text ) + CharacterSeparationX;
        Text++;
        
        // count the characters
        LineLength++;
    }
    
    return LineLength;
}

// -----------------------------------------------

// general function to print multi-line text aligned left
void PrintTextFromLeft( int LeftX, int BaselineY, int* Text )
{
    // before selecting the font texture, save the
    // previous texture to restore it when done
    int PreviousTexture = get_selected_texture();
    select_texture( TextureFont22x32 );
    
    // keep printing lines until the string ends
    while( *Text )
    {
        // print this line
        int LineLength = PrintLineFromLeft( LeftX, BaselineY, Text );
        
        // advance to next line
        Text += LineLength;
        BaselineY += CharacterHeight + CharacterSeparationY;
        
        // then detect and skip the '\n' character
        // since it is not counted as part of the line
        if( *Text == '\n' )
          Text++;
    }
    
    // restore the previously selected texture
    select_texture( PreviousTexture );
}

// -----------------------------------------------

// To align right, this function breaks the text into
// lines and calculates the width for each one. Then
// it just calls PrintTextFromLeft with the left X.
void PrintTextFromRight( int RightX, int BaselineY, int* Text )
{
    // before selecting the font texture, save the
    // previous texture to restore it when done
    int PreviousTexture = get_selected_texture();
    select_texture( TextureFont22x32 );
    
    // keep printing lines until the string ends
    while( *Text )
    {
        // calculate this line's position
        int LineWidth = GetLineWidth( Text );
        int LeftX = RightX - LineWidth;
        
        // print this line
        int LineLength = PrintLineFromLeft( LeftX, BaselineY, Text );
        
        // advance to next line
        Text += LineLength;
        BaselineY += CharacterHeight + CharacterSeparationY;
        
        // then detect and skip the '\n' character
        // since it is not counted as part of the line
        if( *Text == '\n' )
          Text++;
    }
    
    // restore the previously selected texture
    select_texture( PreviousTexture );
}

// -----------------------------------------------

// this works like PrintTextFromRight, but instead it
// will center each line at the given X coordinate
void PrintTextCentered( int CenterX, int BaselineY, int* Text )
{
    // before selecting the font texture, save the
    // previous texture to restore it when done
    int PreviousTexture = get_selected_texture();
    select_texture( TextureFont22x32 );
    
    // keep printing lines until the string ends
    while( *Text )
    {
        // calculate this line's position
        int LineWidth = GetLineWidth( Text );
        int LeftX = CenterX - LineWidth/2;
        
        // print this line
        int LineLength = PrintLineFromLeft( LeftX, BaselineY, Text );
        
        // advance to next line
        Text += LineLength;
        BaselineY += CharacterHeight + CharacterSeparationY;
        
        // then detect and skip the '\n' character
        // since it is not counted as part of the line
        if( *Text == '\n' )
          Text++;
    }
    
    // restore the previously selected texture
    select_texture( PreviousTexture );
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

    // First, we define our font as if it was constant width
    define_region_matrix( FirstRegionTextFont,  0,0,  21,31,  0,31,  16,8,  0 );
    
    // We now redefine some characters with their
    // different-width versions as a second stage
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
    
    // just some multi-line sample text, including
    // 3 lines of different widths to test alignment
    int* TestText = "L.1\n" "Line 2\n" "!!Line 3!!";
    
    // print our text at different alignments
    PrintTextFromLeft( 10, 50, TestText );
    PrintTextCentered( screen_width/2, 50, TestText );
    PrintTextFromRight( screen_width-10, 50, TestText );
}
