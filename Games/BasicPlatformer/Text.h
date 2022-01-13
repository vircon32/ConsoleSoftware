int GetCharacterWidth( int c )
{
    c = tolower( c );
    
    if( c == 'm' || c == 'w' )
      return 25;
    
    if( c == 'i' )
      return 12;
    
    if( c == ' ' )
      return 16;
    
    if( c == '!' || c == ':' || c == ';' || c == ',' || c == '.' || c == '\''|| c == '|' )
      return 14;
    
    // default character width
    // (as given by the font)
    return 22;
}

// ---------------------------------------------------------

// simplified version, does not support multiline text
// (not needed since there are none in this game)
int GetTextWidth( int* Text )
{
    int Width = 0;
    
    while( *Text )
    {
        Width += GetCharacterWidth( *Text ) + 1;
        Text++;
    }
    
    return Width;
}

// ---------------------------------------------------------

void DrawTextFromLeft( int LeftX, int BaselineY, int* Text )
{
    select_texture( TextureTextFont );
    int DrawingX = LeftX;
    
    while( *Text )
    {
        // print this character
        select_region( *Text );
        draw_region_at( DrawingX, BaselineY );
        
        // advance in x
        DrawingX += GetCharacterWidth( *Text ) + 1;
        
        // execute line breaks
        if( *Text == '\n' )
        {
            // y advances, x is reset
            BaselineY += 32 + 1;
            DrawingX = LeftX;
        }
        
        // go to next character
        ++Text;
    }
}

// ---------------------------------------------------------

void DrawTextCentered( int CenterX, int BaselineY, int* Text )
{
    // find width and redirect to left writing version
    int LeftX = CenterX - GetTextWidth( Text ) / 2;
    DrawTextFromLeft( LeftX, BaselineY, Text );
}
