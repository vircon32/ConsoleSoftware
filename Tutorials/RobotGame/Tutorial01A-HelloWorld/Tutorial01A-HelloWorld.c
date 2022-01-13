// include Vircon libraries
#include "video.h"

// notice that main prototype is different
// in Vircon since there is no other program
void main( void )
{   
    // clear the screen in some visible color
    clear_screen( color_blue );
    
    // draw our text near the top-left of the screen
    set_drawing_point( 20, 40 );
    print( "Hello, World!" );
    
    // we are done: after main, Vircon will halt
    // execution and our text will keep showing
}
