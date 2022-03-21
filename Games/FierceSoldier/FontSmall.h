// size of our characters in pixels
// (our drawing functions will need it)
#define FontSmall_CharacterWidth  11
#define FontSmall_CharacterHeight 16

// we will apply separation along X (between
// characters) and Y (between lines)
#define FontSmall_CharacterSeparationX  1
#define FontSmall_CharacterSeparationY  8


// -----------------------------------------------
//   INITIALIZATION
// -----------------------------------------------


void FontSmall_CreateFont()
{
    // select our font texture
    select_texture( TextureFont11x16 );
    
    // define our font as constant width
    define_region_matrix( 0,  0,0,  10,15,  0,15,  16,8,  0 );
}


// -----------------------------------------------
//   TEXT MEASUREMENT FUNCTIONS
// -----------------------------------------------


// this measures the total length of a text line
// in order for our print functions to align it
int FontSmall_GetLineWidth( int* Text )
{
    int LineWidth = 0;
    
    // measure until end of string or line
    while( (*Text) && (*Text != '\n') )
    {
        // add separation only between characters
        // (and not before the first character)
        if( LineWidth != 0 )
          LineWidth += FontSmall_CharacterSeparationX;
        
        // measure character
        LineWidth += FontSmall_CharacterWidth;
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
int FontSmall_PrintLineFromLeft( int LeftX, int BaselineY, int* Text )
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
        DrawingX += FontSmall_CharacterWidth + FontSmall_CharacterSeparationX;
        Text++;
        
        // count the characters
        LineLength++;
    }
    
    return LineLength;
}

// -----------------------------------------------

// general function to print multi-line text aligned left
void FontSmall_PrintTextFromLeft( int LeftX, int BaselineY, int* Text )
{
    // before selecting the font texture, save the
    // previous texture to restore it when done
    int PreviousTexture = get_selected_texture();
    select_texture( TextureFont11x16 );
    
    // keep printing lines until the string ends
    while( *Text )
    {
        // print this line
        int LineLength = FontSmall_PrintLineFromLeft( LeftX, BaselineY, Text );
        
        // advance to next line
        Text += LineLength;
        BaselineY += FontSmall_CharacterHeight + FontSmall_CharacterSeparationY;
        
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
void FontSmall_PrintTextFromRight( int RightX, int BaselineY, int* Text )
{
    // before selecting the font texture, save the
    // previous texture to restore it when done
    int PreviousTexture = get_selected_texture();
    select_texture( TextureFont11x16 );
    
    // keep printing lines until the string ends
    while( *Text )
    {
        // calculate this line's position
        int LineWidth = FontSmall_GetLineWidth( Text );
        int LeftX = RightX - LineWidth;
        
        // print this line
        int LineLength = FontSmall_PrintLineFromLeft( LeftX, BaselineY, Text );
        
        // advance to next line
        Text += LineLength;
        BaselineY += FontSmall_CharacterHeight + FontSmall_CharacterSeparationY;
        
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
void FontSmall_PrintTextCentered( int CenterX, int BaselineY, int* Text )
{
    // before selecting the font texture, save the
    // previous texture to restore it when done
    int PreviousTexture = get_selected_texture();
    select_texture( TextureFont11x16 );
    
    // keep printing lines until the string ends
    while( *Text )
    {
        // calculate this line's position
        int LineWidth = FontSmall_GetLineWidth( Text );
        int LeftX = CenterX - LineWidth/2;
        
        // print this line
        int LineLength = FontSmall_PrintLineFromLeft( LeftX, BaselineY, Text );
        
        // advance to next line
        Text += LineLength;
        BaselineY += FontSmall_CharacterHeight + FontSmall_CharacterSeparationY;
        
        // then detect and skip the '\n' character
        // since it is not counted as part of the line
        if( *Text == '\n' )
          Text++;
    }
    
    // restore the previously selected texture
    select_texture( PreviousTexture );
}
