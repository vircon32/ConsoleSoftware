// include Vircon headers
#include "time.h"
#include "video.h"
#include "audio.h"


// ---------------------------------------------------------
//   AUXILIARY FUNCTIONS
// ---------------------------------------------------------


void DrawTable()
{
    select_region( 1 );
    set_drawing_scale( 640, 1 );
    draw_region_zoomed_at( 0, 359 );
}

// ---------------------------------------------------------

void DrawLargeDigit( int Digit, int HotSpotX, int HotSpotY )
{
    select_region( 10 + (Digit % 10) );
    draw_region_at( HotSpotX, HotSpotY );
}

// ---------------------------------------------------------

void DrawSmallDigit( int Digit, int HotSpotX, int HotSpotY )
{
    select_region( 20 + (Digit % 10) );
    draw_region_at( HotSpotX, HotSpotY );
}

// ---------------------------------------------------------

void DrawDateDigit( int Digit, int HotSpotX, int HotSpotY )
{
    select_region( 30 + (Digit % 10) );
    draw_region_at( HotSpotX, HotSpotY );
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
    
    // clock
    select_region( 0 );
    define_region_topleft( 1,1,  362,253 );
    
    // table (hotspot at bottom)
    select_region( 1 );
    define_region( 364,1,  364,31,  364,31 );
    
    // seconds colon
    select_region( 2 );
    define_region_topleft( 211,315,  216,346 );
    
    // large digits
    define_region_matrix( 10,  1,255,  40,313,  1,255,  10,1,  1 );
    
    // small digits
    define_region_matrix( 20,  1,315,  20,346,  1,315,  10,1,  1 );
    
    // date digits
    define_region_matrix( 30,  1,348,  26,383,  1,348,  10,1,  1 );
    
    // month abbreviations
    define_region_matrix( 40,  1,385,  82,420,  1,385,  4,3,  1 );
    
    // ------------------------------------
    // PART 2: DEFINE ALL SOUNDS
    // ------------------------------------
    
    // channel 1 is "tic"
    assign_channel_sound( 1, 0 );
    set_channel_volume( 0.2 );
    set_channel_speed( 1.1892 );  // 3 semitones higher
    
    // channel 0 is "tac"
    assign_channel_sound( 0, 0 );
    set_channel_volume( 0.25 );
    
    // ------------------------------------
    // PART 3: MAIN LOOP
    // ------------------------------------
    
    while( true )
    {
        // draw background
        clear_screen( color_black );
        DrawTable();
        
        // draw clock base
        int ClockX = 138;
        int ClockY = 76;
        select_region( 0 );
        draw_region_at( ClockX, ClockY );
        
        // - - - - - - - - - - - - - - - - - - - - - -
        // PART 1: DISPLAY TIME OF DAY
        
        // read current time
        int VirconTime = get_time();
        
        // separate it in its components
        time_info HumanTime;
        translate_time( VirconTime, &HumanTime );
        
        // draw hours as 2 digits
        DrawLargeDigit( HumanTime.hours / 10, ClockX+57, ClockY+49 );
        DrawLargeDigit( HumanTime.hours % 10, ClockX+100, ClockY+49 );
        
        // draw minutes as 2 digits
        DrawLargeDigit( HumanTime.minutes / 10, ClockX+166, ClockY+49 );
        DrawLargeDigit( HumanTime.minutes % 10, ClockX+209, ClockY+49 );
        
        // draw hours as 2 digits
        DrawSmallDigit( HumanTime.seconds / 10, ClockX+267, ClockY+76 );
        DrawSmallDigit( HumanTime.seconds % 10, ClockX+289, ClockY+76 );
        
        // make colon before seconds blink
        int FrameCounter = get_frame_counter();
        
        if( (FrameCounter % 60) >= 30 )
        {
            select_region( 2 );
            draw_region_at( ClockX+255, ClockY+76 );
        }
        
        // - - - - - - - - - - - - - - - - - - - - - -
        // PART 2: DISPLAY FULL DATE
        
        // read current date
        int VirconDate = get_date();
        
        // separate it in its components
        date_info HumanDate;
        translate_date( VirconDate, &HumanDate );
        
        // draw year as 4 digits
        DrawDateDigit(  HumanDate.year / 1000     , ClockX+215, ClockY+153 );
        DrawDateDigit( (HumanDate.year / 100) % 10, ClockX+243, ClockY+153 );
        DrawDateDigit( (HumanDate.year /  10) % 10, ClockX+271, ClockY+153 );
        DrawDateDigit(         HumanDate.year % 10, ClockX+299, ClockY+153 );
        
        // draw month abbreviation
        select_region( 40 + (HumanDate.month-1) );
        draw_region_at( ClockX+112, ClockY+153 );
        
        // draw day as 2 digits
        DrawDateDigit( HumanDate.day / 10, ClockX+37, ClockY+153 );
        DrawDateDigit( HumanDate.day % 10, ClockX+65, ClockY+153 );
        
        // - - - - - - - - - - - - - - - - - - - - - -
        // PART 3: MAKE CLOCK SOUND
        
        // if a second has just passed, make a sound
        if( !(FrameCounter % 60) )
          play_channel( HumanTime.seconds % 2 );
        
        // wait until next frame
        end_frame();
    }
}
