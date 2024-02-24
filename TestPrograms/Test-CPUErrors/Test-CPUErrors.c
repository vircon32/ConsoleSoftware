// include Vircon32 headers
#include "video.h"
#include "time.h"
#include "string.h"
#include "input.h"


// ---------------------------------------------------------
//   DISPLAY FUNCTIONS
// ---------------------------------------------------------


#define BiosTexture -1
#define WhitePixelRegion 256

void DrawHorizontalLine( int y )
{
    select_texture( BiosTexture );
    select_region( WhitePixelRegion );
    set_drawing_scale( screen_width, 1 );
    draw_region_zoomed_at( 0, y );
}

// ---------------------------------------------------------

void DrawVerticalLine()
{
    select_texture( BiosTexture );
    select_region( WhitePixelRegion );
    set_drawing_scale( 1, screen_height-55 );
    draw_region_zoomed_at( 320, 55 );
}

// ---------------------------------------------------------

void WriteInstructions()
{
    clear_screen( color_black );
    
    // main instructions
    set_multiply_color( color_cyan );
    print_at( 20, 20, "Use the first gamepad to produce these errors:" );
    print_at( 20, 70, "With Start not pressed:" );
    print_at( 340, 70, "While pressing Start:" );
    
    // separator lines
    set_multiply_color( color_yellow );
    DrawHorizontalLine( 55 );
    DrawHorizontalLine( 105 );
    DrawVerticalLine();
    
    // left half options
    set_multiply_color( color_white );
    print_at( 20, 120, "A --> Invalid memory read" );
    print_at( 20, 160, "B --> Invalid memory write" );
    print_at( 20, 200, "X --> Invalid port read" );
    print_at( 20, 240, "Y --> Invalid port write" );
    print_at( 20, 280, "L --> Stack overflow" );
    print_at( 20, 320, "R --> Stack underflow" );
    
    // right half options
    print_at( 340, 120, "A --> Division error" );
    print_at( 340, 160, "B --> Arc cosine error" );
    print_at( 340, 200, "X --> Arc tangent 2 error" );
    print_at( 340, 240, "Y --> Logarithm error" );
    print_at( 340, 280, "L --> Power error" );
}


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main()
{
    // write on-screen error menu
    WriteInstructions();
    select_gamepad( 0 );
    
    // wait for a keypress
    while( true )
    {
        // check for button presses with start unpressed
        if( gamepad_button_start() < 0 )
        {
            // produce error: invalid memory read
            if( gamepad_button_a() == 1 )
            {
                asm{ "mov R0, [0x00400000]" };
            }
            
            // produce error: invalid memory write
            else if( gamepad_button_b() == 1 )
            {
                asm{ "mov [0x00400000], R0" };
            }
            
            // produce error: invalid port read
            else if( gamepad_button_x() == 1 )
            {
                asm{ "in R0, GPU_Command" };
            }
            
            // produce error: invalid port write
            else if( gamepad_button_y() == 1 )
            {
                asm{ "out TIM_FrameCounter, R0" };
            }
            
            // produce error: stack overflow
            else if( gamepad_button_l() == 1 )
            {
                asm
                {
                    "mov SP, 0x00000000"
                    "push R0"
                };
            }
            
            // produce error: stack underflow
            else if( gamepad_button_r() == 1 )
            {
                asm
                {
                    "mov SP, 0x003FFFFF"
                    "pop R0"
                };
            }
        }
        
        // check for button presses with start pressed
        else
        {
            // produce error: division error
            if( gamepad_button_a() == 1 )
            {
                asm
                {
                    "mov R0, 0"
                    "idiv R1, R0"
                };
            }
            
            // produce error: arc cosine error
            else if( gamepad_button_b() == 1 )
            {
                asm
                {
                    "mov R0, 3.0"
                    "acos R0"
                };
            }
            
            // produce error: arc tangent 2 error
            else if( gamepad_button_x() == 1 )
            {
                asm
                {
                    "mov R0, 0.0"
                    "mov R1, 0.0"
                    "atan2 R0, R1"
                };
            }
            
            // produce error: logarithm error
            else if( gamepad_button_y() == 1 )
            {
                asm
                {
                    "mov R0, -1.0"
                    "log R0"
                };
            }
            
            // produce error: power error
            else if( gamepad_button_l() == 1 )
            {
                asm
                {
                    "mov R0, -1.0"
                    "mov R1, 0.5"
                    "pow R0, R1"
                };
            }
        }
        
        end_frame();
    }
}