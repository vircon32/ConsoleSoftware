// since this is a minimal test and standard
// headers include many more things, we just
// make a small header with only what we use
#include "Test-MinimalTest.h"


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main()
{
    // define our texture region
    select_texture( 0 );
    select_region( 0 );
    define_region_center( 0,0,  139,99 );
    
    // clear the screen in some visible color
    clear_screen( color_purple );
    
    // draw our region at the center of the screen
    draw_region_at( 640/2, 360/2 );
    
    // play our sound
    play_sound_in_channel( 0, 0 );
    
    // we are done: after main, Vircon will halt
    // execution; our sound will finish playing
    // and the screen will keep showing our image
}