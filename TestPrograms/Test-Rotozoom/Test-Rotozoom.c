// include Vircon libraries
#include "video.h"
#include "time.h"
#include "input.h"
#include "math.h"
#include "string.h"


// ---------------------------------------------------------
//   DEFINITIONS AND CONSTANTS
// ---------------------------------------------------------


// names for texture regions
#define TextureBackground  0
#define TextureWheel       1

#define RegionBackground  10
#define RegionWheel       20

// other values
#define CenterX  200
#define CenterY  120


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

void clamp_float( float* value, float minimum, float maximum )
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
    
    // fixed background guide
    select_texture( TextureBackground );
    select_region( RegionBackground );
    define_region_topleft( 0,0,  639,359 );
    
    // rotating wheel
    select_texture( TextureWheel );
    select_region( RegionWheel );
    define_region( 0,0,  205,164,  57,82 );
    
    // ------------------------------------
    // PART 2: INITIAL PREPARATIONS
    // ------------------------------------
    
    // initial values
    int RotationDegrees = 0;
    float ZoomFactor = 1;
    
    // we always use gamepad 1
    select_gamepad( 0 );
    
    // ------------------------------------
    // PART 3: MAIN LOOP
    // ------------------------------------
    
    while( true )
    {
        // handle inputs
        if( gamepad_left() > 0 )
          RotationDegrees -= 1;
        
        if( gamepad_right() > 0 )
          RotationDegrees += 1;
        
        if( gamepad_up() > 0 )
          ZoomFactor += 0.025;
        
        if( gamepad_down() > 0 )
          ZoomFactor -= 0.025;
        
        // make parameters stay within limits
        clamp_int( &RotationDegrees, -360, 360 );
        clamp_float( &ZoomFactor, -2.5, 2.5 );
        
        // actually apply zoom and scale
        set_drawing_scale( ZoomFactor, ZoomFactor );
        set_drawing_angle( RotationDegrees * (2*pi/360) );
        
        // draw background
        set_multiply_color( color_white );
        select_texture( TextureBackground );
        select_region( RegionBackground );
        draw_region_at( 0, 0 );
        
        // draw wheel
        set_multiply_color( 0x80FFFFFF );
        select_texture( TextureWheel );
        select_region( RegionWheel );
        draw_region_rotozoomed_at( CenterX, CenterY );
        
        // report current angle
        int[ 40 ] Message;
        int[ 20 ] NumberString;
        strcpy( Message, "Angle: " );
        itoa( RotationDegrees, NumberString, 10 );
        strcat( Message, NumberString );
        strcat( Message, " degrees" );
        
        set_multiply_color( color_white );
        print_at( 450, 310, Message );
        
        // report current zoom
        strcpy( Message, "Zoom: X" );
        ftoa( ZoomFactor, NumberString );
        strcat( Message, NumberString );
        print_at( 450, 330, Message );
        
        end_frame();
    }
}