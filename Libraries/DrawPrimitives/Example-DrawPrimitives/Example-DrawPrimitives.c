// *****************************************************************************
    // include standard Vircon headers
    #include "time.h"
    
    // include additional headers
    #include "../draw_primitives.h"
// *****************************************************************************


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // we will use this angle to animate the scene
    float Angle = 0;
    
    // main loop
    while( true )
    {
        // clear any previous image
        clear_screen( color_black );
        
        // draw some static primitives in different colors
        set_multiply_color( color_white );
        draw_pixel( 600, 25 );
        draw_filled_rectangle( 20, 20, 120, 70 );
        
        set_multiply_color( color_cyan );
        draw_rectangle( 10, 10, screen_width-1-10, screen_height-1-10 );
        
        set_multiply_color( color_magenta );
        draw_arc( 50, 20, 100, 0, pi/2 );
        
        // draw several centered circles with a growing radius
        set_multiply_color( color_green );
        draw_circle( screen_width/2, screen_height/2, 5 );
        draw_circle( screen_width/2, screen_height/2, 25 );
        draw_circle( screen_width/2, screen_height/2, 50 );
        draw_circle( screen_width/2, screen_height/2, 100 );
        draw_circle( screen_width/2, screen_height/2, 200 );
        
        // this line will rotate with the angle
        set_multiply_color( color_yellow );
        draw_line( screen_width/2, screen_height/2, screen_width/2 + 100 * cos( Angle ), screen_height/2 + 100 * sin( Angle ) );
        
        // this filled circle will grow and reset with the angle
        set_multiply_color( color_yellow );
        draw_filled_circle( screen_width*0.75, screen_height*0.75, Angle * 10 );        
        
        // animate angle in a cycle
        Angle += pi / 60;
        
        if( Angle >= 2*pi )
          Angle -= 2*pi;
        
        // wait until next frame
        end_frame();
    }
}
