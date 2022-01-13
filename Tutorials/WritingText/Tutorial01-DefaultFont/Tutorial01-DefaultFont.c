/* -----------------------------------------------
   This first step shows the most basic ways
   to print text on the screen using the
   default BIOS font, through the standard
   library functions.
/  -------------------------------------------- */


// include Vircon libraries
#include "video.h"
#include "string.h"


// our program will begin at the main function
void main( void )
{   
    // clear the screen in some visible color
    clear_screen( color_blue );
    
    // write some text directly
    print_at( 20,20, "Literal text" );
    
    // Store text in a string and use it to write
    // -----------------------------------------------
    // Note that this text array is different from
    // regular C, where you would instead use:
    // char TextString[ 20 ];
    // This is different here because:
    // (1) Vircon32 cannot handle bytes, so there
    //     can be no char type. We use int instead.
    // (2) This compiler simplifies declarations to
    //     <type> <name> (separately), so the array
    //     type here becomes int[20].
    int[ 20 ] TextString;
    strcpy( TextString, "Text in a string" );
    print_at( 20,50, TextString );
    
    // write text in another color; this works because
    // our text is white, and white x color = color
    set_multiply_color( color_green );
    print_at( 20,80, "Text in green color" );
    
    // we need to disable color modifications so
    // that our next texts are written in white again
    set_multiply_color( color_white );
    
    // we can also set the drawing position separately
    set_drawing_point( 250,20 );
    print( "Text at current drawing position" );
    
    // our text print functions support multiple
    // lines by detecting the newline character '\n'
    print_at( 250,50, "Line 1\nLine 2\nLine 3" );
    
    // we are done: after main, Vircon will halt
    // execution and our text will keep showing
}
