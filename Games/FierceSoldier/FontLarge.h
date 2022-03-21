// Default size of our characters in pixels.
// Width however may vary per character.
#define FontLarge_CharacterWidth  22
#define FontLarge_CharacterHeight 32

// we will apply separation along X (between
// characters) and Y (between lines)
#define FontLarge_CharacterSeparationX  1
#define FontLarge_CharacterSeparationY  15


// -----------------------------------------------
//   INITIALIZATION
// -----------------------------------------------


void FontLarge_CreateFont()
{
    // select our font texture
    select_texture( TextureFont22x32 );
    
    // First, we define our font as if it was constant width
    define_region_matrix( 0,  0,0,  21,31,  0,31,  16,8,  0 );
    
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
}


// -----------------------------------------------
//   TEXT MEASUREMENT FUNCTIONS
// -----------------------------------------------


int FontLarge_GetCharacterWidth( int c )
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
    return FontLarge_CharacterWidth;
}

// -----------------------------------------------

// this measures the total length of a text line
// in order for our print functions to align it
int FontLarge_GetLineWidth( int* Text )
{
    int LineWidth = 0;
    
    // measure until end of string or line
    while( (*Text) && (*Text != '\n') )
    {
        // add separation only between characters
        // (and not before the first character)
        if( LineWidth != 0 )
          LineWidth += FontLarge_CharacterSeparationX;
        
        // measure character
        LineWidth += FontLarge_GetCharacterWidth( *Text );
        Text++;
    }
    
    return LineWidth;
}


// -----------------------------------------------
//   TEXT PRINTING FUNCTIONS
// -----------------------------------------------


// This function only prints until the end of a line.
// This function is not to be used on its own! Instead
// it will be called by our multi-line print functions.
// That's why it doesn't bother selecting the texture.
// Returns the length in characters of the printed line
// so that those functions can skip to the next line.
int FontLarge_PrintLineFromLeft( int LeftX, int BaselineY, int* Text )
{
    int DrawingX = LeftX;
    int LineLength = 0;
    
    // continue until end of string or line
    while( (*Text) && (*Text != '\n') )
    {
        // print this character
        select_region( *Text );
        draw_region_at( DrawingX, BaselineY );
        
        // advance in x
        DrawingX += FontLarge_GetCharacterWidth( *Text ) + FontLarge_CharacterSeparationX;
        Text++;
        
        // count the characters
        LineLength++;
    }
    
    return LineLength;
}

// -----------------------------------------------

// general function to print multi-line text aligned left
void FontLarge_PrintTextFromLeft( int LeftX, int BaselineY, int* Text )
{
    // before selecting the font texture, save the
    // previous texture to restore it when done
    int PreviousTexture = get_selected_texture();
    select_texture( TextureFont22x32 );
    
    // keep printing lines until the string ends
    while( *Text )
    {
        // print this line
        int LineLength = FontLarge_PrintLineFromLeft( LeftX, BaselineY, Text );
        
        // advance to next line
        Text += LineLength;
        BaselineY += FontLarge_CharacterHeight + FontLarge_CharacterSeparationY;
        
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
void FontLarge_PrintTextFromRight( int RightX, int BaselineY, int* Text )
{
    // before selecting the font texture, save the
    // previous texture to restore it when done
    int PreviousTexture = get_selected_texture();
    select_texture( TextureFont22x32 );
    
    // keep printing lines until the string ends
    while( *Text )
    {
        // calculate this line's position
        int LineWidth = FontLarge_GetLineWidth( Text );
        int LeftX = RightX - LineWidth;
        
        // print this line
        int LineLength = FontLarge_PrintLineFromLeft( LeftX, BaselineY, Text );
        
        // advance to next line
        Text += LineLength;
        BaselineY += FontLarge_CharacterHeight + FontLarge_CharacterSeparationY;
        
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
void FontLarge_PrintTextCentered( int CenterX, int BaselineY, int* Text )
{
    // before selecting the font texture, save the
    // previous texture to restore it when done
    int PreviousTexture = get_selected_texture();
    select_texture( TextureFont22x32 );
    
    // keep printing lines until the string ends
    while( *Text )
    {
        // calculate this line's position
        int LineWidth = FontLarge_GetLineWidth( Text );
        int LeftX = CenterX - LineWidth/2;
        
        // print this line
        int LineLength = FontLarge_PrintLineFromLeft( LeftX, BaselineY, Text );
        
        // advance to next line
        Text += LineLength;
        BaselineY += FontLarge_CharacterHeight + FontLarge_CharacterSeparationY;
        
        // then detect and skip the '\n' character
        // since it is not counted as part of the line
        if( *Text == '\n' )
          Text++;
    }
    
    // restore the previously selected texture
    select_texture( PreviousTexture );
}
