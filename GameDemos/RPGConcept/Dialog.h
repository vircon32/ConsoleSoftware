// ---------------------------------------------------------
//   DEFINITIONS AND CONSTANTS
// ---------------------------------------------------------


// our dialogs can be in these states
enum DialogState
{
    Dialog_FadeIn,
    Dialog_Filling,
    Dialog_Waiting,
    Dialog_FadeOut,
    Dialog_Finished
};

// ---------------------------------------------------------

// definition of our dialog
struct Dialog
{
    Character* Speaker;
    int[ 100 ] Text;
    
    // current state, when being displayed
    DialogState State;
    int ShownCharacters;
    int ElapsedFrames;
};


// ---------------------------------------------------------
//   DIALOG HANDLING FUNCTIONS
// ---------------------------------------------------------


void Dialog_Initialize( Dialog* D, Character* C, int* Text )
{
    strcpy( D->Text, Text );
    D->Speaker = C;
    D->State = Dialog_FadeIn;
    D->ElapsedFrames = 0;
    D->ShownCharacters = 0;
}

// ---------------------------------------------------------

void Dialog_Update( Dialog* D )
{
    D->ElapsedFrames++;
    
    switch( D->State )
    {
        case Dialog_FadeIn:
        {
            if( D->ElapsedFrames > 30 )
            {
                D->State = Dialog_Filling;
                D->ElapsedFrames = 0;
            }
            
            break;
        }
        
        case Dialog_FadeOut:
        {
            if( D->ElapsedFrames > 30 )
            {
                D->State = Dialog_Finished;
                D->ElapsedFrames = 0;
            }
            
            break;
        }
        
        case Dialog_Filling:
        {
            // add another letter at regular intervals
            if( D->ElapsedFrames >= 3 )
            {
                D->ShownCharacters++;
                D->ElapsedFrames = 0;
                
                // check if we are finished
                if( D->ShownCharacters >= strlen( D->Text ) )
                {
                    D->State = Dialog_Waiting;
                    D->ElapsedFrames = 0;
                }
            }
            
            break;
        }
        
        case Dialog_Waiting:
        {
            // fade out if player presses button
            if( gamepad_button_a() == 1 )
            {
                D->State = Dialog_FadeOut;
                D->ElapsedFrames = 0;
            }
            
            break;
        }
        
        default: break;
    }
    
}

// ---------------------------------------------------------

// we use this to determine when the portrait
// image should be displayed with mouth open
bool Dialog_LastCharacterIsVowel( Dialog* D )
{
    if( D->ShownCharacters <= 0 )
      return false;
    
    int lc = D->Text[ D->ShownCharacters - 1 ];
    lc = tolower( lc );
    
    return (lc == 'a') || (lc == 'e') || (lc == 'i')
        || (lc == 'o') || (lc == 'u') || (lc == 'y');
}

// ---------------------------------------------------------

// auxiliary function used by the main dialog draw,
// because the window frames need to be drawn the
// same way for multiple states but the rest of the
// dialog elements can change along dialog lifetime
void Dialog_DrawWindowFrames( Dialog* D )
{
    // draw the portrait frame, with no portrait
    select_texture( TextureRPG );
    select_region( RegionFramePortrait );
    draw_region_at( 3, 254 );
    
    // draw name frame center adapting it
    // to the character's name text width
    int NameWidth = textfont_get_line_width( &FontDialog, D->Speaker->Name );
    select_region( RegionFrameNameCenter );
    set_drawing_scale( NameWidth, 1 );
    draw_region_zoomed_at( 122, 254 );
    
    // draw name frame sides
    select_region( RegionFrameNameLeft );
    set_drawing_scale( -1, 1 );
    draw_region_at( 109, 254 );
    draw_region_zoomed_at( 122 + 12 + NameWidth, 254 );
    
    // draw text frame center
    select_region( RegionFrameTextCenter );
    set_drawing_scale( 502, 1 );
    draw_region_zoomed_at( 122, 289 );
    
    // draw text frame sides
    select_region( RegionFrameTextLeft );
    set_drawing_scale( -1, 1 );
    draw_region_at( 109, 289 );
    draw_region_zoomed_at( 636, 289 );
}

// ---------------------------------------------------------

void Dialog_Draw( Dialog* D )
{
    switch( D->State )
    {
        case Dialog_FadeIn:
        {
            // apply fade effect
            float OpacityLevel = D->ElapsedFrames / 30.0;
            set_multiply_color( make_color_rgba( 255, 255, 255, 255 * OpacityLevel ) );
            
            // draw dialog with static portrait
            Dialog_DrawWindowFrames( D );
            select_region( D->Speaker->FirstRegionPortrait );
            draw_region_at( 15, 266 );
            
            // write name text in yellow
            set_multiply_color( make_color_rgba( 255, 255, 0, 255 * OpacityLevel ) );
            textfont_print_from_left( &FontDialog, 121, 260, D->Speaker->Name );
            
            // restore multiply color
            set_multiply_color( color_white );
            break;
        }
        
        case Dialog_FadeOut:
        {
            // apply fade effect
            float OpacityLevel = 1.0 - D->ElapsedFrames / 30.0;
            set_multiply_color( make_color_rgba( 255, 255, 255, 255 * OpacityLevel ) );
            
            // draw dialog with static portrait
            Dialog_DrawWindowFrames( D );
            select_region( D->Speaker->FirstRegionPortrait );
            draw_region_at( 15, 266 );
            
            // write name text in yellow
            set_multiply_color( make_color_rgba( 255, 255, 0, 255 * OpacityLevel ) );
            textfont_print_from_left( &FontDialog, 121, 260, D->Speaker->Name );
            
            // restore multiply color
            set_multiply_color( color_white );
            break;
        }
        
        case Dialog_Filling:
        {
            // draw dialog frame
            Dialog_DrawWindowFrames( D );
            
            // draw the portrait animation
            // depending on the current character
            // (mouth opens only at vowels + y)
            bool MouthIsOpen = Dialog_LastCharacterIsVowel( D );
            select_region( D->Speaker->FirstRegionPortrait + MouthIsOpen );
            draw_region_at( 15, 266 );
            
            // write name text in yellow
            set_multiply_color( color_yellow );
            textfont_print_from_left( &FontDialog, 121, 260, D->Speaker->Name );
            
            // write current shown text in white
            int[ 100 ] ShownText = "";
            strncpy( ShownText, D->Text, D->ShownCharacters );
          
            set_multiply_color( color_white );
            textfont_print_from_left( &FontDialog, 121, 299, ShownText );
            break;
        }
        
        case Dialog_Waiting:
        {
            // draw dialog frame
            Dialog_DrawWindowFrames( D );
            
            // draw the portrait animation
            // depending on the current character
            // (mouth opens only at vowels + y)
            bool MouthIsOpen = Dialog_LastCharacterIsVowel( D );
            select_region( D->Speaker->FirstRegionPortrait + MouthIsOpen );
            draw_region_at( 15, 266 );
            
            // write name text in yellow
            set_multiply_color( color_yellow );
            textfont_print_from_left( &FontDialog, 121, 260, D->Speaker->Name );
            
            // draw the full text in white
            set_multiply_color( color_white );
            textfont_print_from_left( &FontDialog, 121, 299, D->Text );
            
            // draw cursor animation
            select_texture( TextureRPG );
            int CursorFrame = (D->ElapsedFrames / 4) % 8;
            select_region( FirstRegionCursorDown + CursorFrame );
            draw_region_at( 597, 326 );
            
            break;
        }
        
        default: break;
    }
}
