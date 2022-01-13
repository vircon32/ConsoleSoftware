// include Vircon headers
#include "input.h"
#include "video.h"
#include "math.h"
#include "time.h"


// ---------------------------------------------------------
//   DEFINITIONS
// ---------------------------------------------------------


// texture regions
#define RegionBackground  0
#define RegionTab         1
#define RegionCursor      2
#define RegionGraphDot    3

// function tabs
#define TabSin    0
#define TabCos    1
#define TabTan    2
#define TabArcSin 3
#define TabArcCos 4
#define TabExp    5
#define TabLog    6
#define TabCubic  7


// ---------------------------------------------------------
//   GLOBAL VARIABLES
// ---------------------------------------------------------


int SelectedTab = 0;


// ---------------------------------------------------------
//   AUXILIARY FUNCTIONS
// ---------------------------------------------------------

float cubic( float x )
{
    return pow(x,3) - 4*x;
}

// ---------------------------------------------------------

// careful when iterating: the domain of some
// represented funcions is less than [-4,+4]
void DrawFunction()
{
    select_region( RegionGraphDot );
    
    if( SelectedTab == TabSin )
    {
        for( float x = -4; x <= 4; x += 0.0625 )
        {
            set_drawing_point( 460 + 40*x, 180 - 40*sin( x ) );
            draw_region();
        }
        
        return;
    }
    
    if( SelectedTab == TabCos )
    {
        for( float x = -4; x <= 4; x += 0.0625 )
        {
            set_drawing_point( 460 + 40*x, 180 - 40*cos( x ) );
            draw_region();
        }
        
        return;
    }
    
    if( SelectedTab == TabTan )
    {
        for( float x = -4; x <= 4; x += 0.0625 )
        {
            set_drawing_point( 460 + 40*x, 180 - 40*tan( x ) );
            draw_region();
        }
        
        return;
    }
    
    if( SelectedTab == TabArcSin )
    {
        for( float x = -1; x <= 1; x += 0.0625 )
        {
            set_drawing_point( 460 + 40*x, 180 - 40*asin( x ) );
            draw_region();
        }
        
        return;
    }
    
    if( SelectedTab == TabArcCos )
    {
        for( float x = -1; x <= 1; x += 0.0625 )
        {
            set_drawing_point( 460 + 40*x, 180 - 40*acos( x ) );
            draw_region();
        }
        
        return;
    }
    
    if( SelectedTab == TabExp )
    {
        for( float x = -4; x <= 4; x += 0.0625 )
        {
            set_drawing_point( 460 + 40*x, 180 - 40*exp( x ) );
            draw_region();
        }
        
        return;
    }
    
    if( SelectedTab == TabLog )
    {
        for( float x = 4; x > 0; x -= 0.0625 )
        {
            set_drawing_point( 460 + 40*x, 180 - 40*log( x ) );
            draw_region();
        }
        
        return;
    }
    
    else
    {
        for( float x = -4; x <= 4; x += 0.0625 )
        {
            set_drawing_point( 460 + 40*x, 180 - 40*cubic( x ) );
            draw_region();
        }
        
        return;
    }
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
    
    // background
    select_region( RegionBackground );
    define_region_topleft( 0,0,  639,359 );
    
    // tabs
    select_region( RegionTab );
    define_region_topleft( 1,361,  207,397 );
    
    // cursor
    select_region( RegionCursor );
    define_region( 209,361,  233,385,  233,373 );
    
    // graph dot
    select_region( RegionGraphDot );
    define_region_center( 235,361,  239,365 );
    
    // ------------------------------------
    // PART 2: MAIN LOOP
    // ------------------------------------
    
    while( true )
    {
        // draw background
        set_multiply_color( color_white );
        select_region( RegionBackground );
        draw_region_at( 0, 0 );
        
        // highlight current tab
        select_region( RegionTab );
        draw_region_at( 59, 22 + 40*SelectedTab );
        
        // draw cursor
        select_region( RegionCursor );
        draw_region_at( 45, 40 + 40*SelectedTab );
        
        // plot current function
        DrawFunction();
        
        // wait for inputs
        while( true )
        {
            // wait until next frame
            end_frame();
            
            if( gamepad_up() == 1 )
            {
                SelectedTab = max( SelectedTab - 1, 0 );
                break;
            }
            
            else if( gamepad_down() == 1 )
            {
                SelectedTab = min( SelectedTab + 1, 7 );
                break;
            }
        }
    }
}
