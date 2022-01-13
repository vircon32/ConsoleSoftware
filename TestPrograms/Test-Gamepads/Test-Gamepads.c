// include Vircon headers
#include "input.h"
#include "video.h"
#include "time.h"


// ---------------------------------------------------------
//   AUXILIARY FUNCTIONS
// ---------------------------------------------------------


void DrawHorizontalLine( int y )
{
    select_region( 40 );
    set_drawing_point( 0, y );
    set_drawing_scale( 320, 1 );
    draw_region_zoomed();
    /*
    asm
    {
        "out GPU_SelectedRegion, 40"
        "mov R0, {y}"
        "out GPU_DrawingPointY, R0"
        "out GPU_DrawingPointX, 0"
        "out GPU_DrawingScaleY, 1.0"
        "out GPU_DrawingScaleX, 320.0"
        "out GPU_Command, GPUCommand_DrawRegionZoomed"
    }
    */
}

// ---------------------------------------------------------

void DrawVerticalLine( int x )
{
    select_region( 40 );
    set_drawing_point( x, 0 );
    set_drawing_scale( 1, 180 );
    draw_region_zoomed();
    /*
    asm
    {
        "out GPU_SelectedRegion, 40"
        "mov R0, {x}"
        "out GPU_DrawingPointX, R0"
        "out GPU_DrawingPointY, 0"
        "out GPU_DrawingScaleX, 1.0"
        "out GPU_DrawingScaleY, 180.0"
        "out GPU_Command, GPUCommand_DrawRegionZoomed"
    }
    */
}


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    select_texture( 0 );
    
    // gamepad
    select_region( 0 );
    define_region_topleft( 1,1,  227,112 );
    
    // texts P1,P2,P3,P4
    define_region_matrix( 10,  37,137,  80,164,  37,137,  4,1,  1 );
    
    // arrows Left,Right,Up,Down
    define_region_matrix( 20,  1,134,  16,149,  1,134,  2,2,  1 );
    
    // buttons A,B,X,Y
    define_region_matrix( 30,  121,114,  140,133,  121,114,  4,1,  1 );
    
    // buttons L,R
    define_region_matrix( 34,  1,114,  59,132,  1,114,  2,1,  1 );
    
    // button Start
    select_region( 36 );
    define_region_topleft( 205,114,  225,128 );
    
    // black rectangle (to draw lines)
    select_region( 40 );
    define_region_topleft( 205,130,  206,131 );
    
    // ------------------------------------
    // PART 2: MAIN LOOP
    // ------------------------------------
    
    while( true )
    {
        // draw background
        set_multiply_color( color_white );
        clear_screen( 0xFFDACA9D );
        DrawHorizontalLine( 179 );
        DrawVerticalLine( 319 );
        
        // represent all 4 gamepads
        for( int GamepadID = 0; GamepadID < 4; ++GamepadID )
        {
            select_gamepad( GamepadID );
            
            // position this gamepad in the grid
            int GamepadX = 46;
            int GamepadY = 46;
            
            if( GamepadID & 1 ) GamepadX = 367;
            if( GamepadID & 2 ) GamepadY = 227;
            
            // draw player text
            select_region( 10 + GamepadID );
            draw_region_at( GamepadX-38, GamepadY-38 );
            
            // non connected gamepads are just shown in shade
            // (we don't need to process their inputs)
            if( !gamepad_is_connected() )
            {
                set_multiply_color( 0x60606060 );
                select_region( 0 );
                draw_region_at( GamepadX, GamepadY );
                set_multiply_color( color_white );
                
                // advance to next gamepad
                continue;
            }
            
            // draw the connected gamepad
            select_region( 0 );
            draw_region_at( GamepadX, GamepadY );
            
            // draw all arrows
            if( gamepad_left() > 0 )
            {
                select_region( 20 );
                draw_region_at( GamepadX+26, GamepadY+56 );
            }
            
            if( gamepad_right() > 0 )
            {
                select_region( 21 );
                draw_region_at( GamepadX+54, GamepadY+56 );
            }
            
            if( gamepad_up() > 0 )
            {
                select_region( 22 );
                draw_region_at( GamepadX+40, GamepadY+42 );
            }
            
            if( gamepad_down() > 0 )
            {
                select_region( 23 );
                draw_region_at( GamepadX+40, GamepadY+70 );
            }
            
            // draw main front buttons
            if( gamepad_button_a() > 0 )
            {
                select_region( 30 );
                draw_region_at( GamepadX+188, GamepadY+54 );
            }
            
            if( gamepad_button_b() > 0 )
            {
                select_region( 31 );
                draw_region_at( GamepadX+169, GamepadY+73 );
            }
            
            if( gamepad_button_x() > 0 )
            {
                select_region( 32 );
                draw_region_at( GamepadX+169, GamepadY+35 );
            }
            
            if( gamepad_button_y() > 0 )
            {
                select_region( 33 );
                draw_region_at( GamepadX+150, GamepadY+54 );    
            }
            
            // draw shoulder buttons
            if( gamepad_button_l() > 0 )
            {
                select_region( 34 );
                draw_region_at( GamepadX+4, GamepadY+5 );
            }
            
            if( gamepad_button_r() > 0 )
            {
                select_region( 35 );
                draw_region_at( GamepadX+164, GamepadY+5 );
            }
            
            // draw start button
            if( gamepad_button_start() > 0 )
            {
                select_region( 36 );
                draw_region_at( GamepadX+103, GamepadY+82 );
            }
        }
        
        // wait until next frame
        end_frame();
    }
}
