// Simple font, every character has the same size

// Para escribir con la fuente:
void DrawText( int LeftX, int BaselineY, int* Text )
{
    select_texture( TextureFont );
    int DrawingX = LeftX;
    
    while( *Text )
    {
        // print this character
        select_region( *Text );
        draw_region_at( DrawingX, BaselineY );
        
        // advance in x
        DrawingX += 11 + 1;
        
        // execute line breaks
        if( *Text == '\n' )
        {
            // y advances, x is reset
            BaselineY += 16 + 1;
            DrawingX = LeftX;
        }
        
        // go to next character
        ++Text;
    }
}