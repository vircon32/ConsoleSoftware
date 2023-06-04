// include Vircon libraries
#include "video.h"
#include "time.h"
#include "input.h"
#include "math.h"
#include "string.h"

// include additional headers
#include "draw_primitives.h"


// ---------------------------------------------------------
//   DEFINITIONS AND CONSTANTS
// ---------------------------------------------------------


// names for textures
#define TextureCheckers  0

// names for texture regions
#define RegionCheckers  10


// ---------------------------------------------------------
//   AUXILIARY FUNCTIONS
// ---------------------------------------------------------


void clamp_int( int* value, int minimum, int maximum )
{
    if( *value < minimum )
      *value = minimum;
    
    if( *value > maximum )
      *value = maximum;
}


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    select_texture( TextureCheckers );
    select_region( RegionCheckers );
    define_region_topleft( 1,1,  16,9 );
    
    // ------------------------------------
    // PART 2: INITIAL PREPARATIONS
    // ------------------------------------
    
    // initial values
    int RotationDegrees = 0;
    int ZoomFactor = 20;
    int MirrorX = 1;
    int MirrorY = 1;
    
    // we always use gamepad 1
    select_gamepad( 0 );
    
    // ------------------------------------
    // PART 3: MAIN LOOP
    // ------------------------------------
    
    while( true )
    {
        // left and right control rotation
        if( gamepad_left() > 0 )
          RotationDegrees -= 1;
        
        if( gamepad_right() > 0 )
          RotationDegrees += 1;
        
        // up and down control zoom
        if( gamepad_up() == 1 )
          ZoomFactor += 1;
        
        if( gamepad_down() == 1 )
          ZoomFactor -= 1;
        
        if( gamepad_down() == 1 )
          ZoomFactor -= 1;
        
        // buttons A and B control mirroring
        if( gamepad_button_a() == 1 )
          MirrorX *= -1;
        
        if( gamepad_button_b() == 1 )
          MirrorY *= -1;
        
        // make parameters stay within limits
        clamp_int( &RotationDegrees, -360, 360 );
        clamp_int( &ZoomFactor, 5, 35 );
        
        // actually apply zoom and scale
        set_drawing_scale( ZoomFactor * MirrorX, ZoomFactor * MirrorY );
        set_drawing_angle( RotationDegrees * (2*pi/360) );
        
        // draw a black background
        clear_screen( color_black );
        
        // draw checkered region
        select_texture( TextureCheckers );
        select_region( RegionCheckers );
        draw_region_rotozoomed_at( 250, 140 );
        
        // add a mark at the drawing point
        select_texture( TextureCheckers );
        set_multiply_color( make_color_rgba( 255, 255, 0, 80 ) );
        draw_horizontal_line( 0, 140, screen_width-1 );
        draw_vertical_line( 250, 0, screen_height-1 );
        set_multiply_color( color_white );
        
        // report current angle
        int[ 40 ] Message;
        int[ 20 ] NumberString;
        strcpy( Message, "Angle: " );
        itoa( RotationDegrees, NumberString, 10 );
        strcat( Message, NumberString );
        strcat( Message, " degrees" );
        print_at( 10, 280, Message );
        
        // report current zoom
        strcpy( Message, "Zoom: X" );
        itoa( ZoomFactor, NumberString, 10 );
        strcat( Message, NumberString );
        print_at( 10, 300, Message );
        
        // report mirror on X
        if( MirrorX > 0 )
          print_at( 10, 320, "Mirror X: OFF" );
        else
          print_at( 10, 320, "Mirror X: ON" );
        
        // report mirror on Y
        if( MirrorY > 0 )
          print_at( 10, 340, "Mirror Y: OFF" );
        else
          print_at( 10, 340, "Mirror Y: ON" );
        
        end_frame();
    }
}