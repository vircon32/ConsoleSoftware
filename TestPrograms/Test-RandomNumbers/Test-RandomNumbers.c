// include Vircon libraries
#include "video.h"
#include "audio.h"
#include "time.h"
#include "input.h"
#include "math.h"
#include "string.h"
#include "misc.h"


// ---------------------------------------------------------
//   DEFINITIONS AND CONSTANTS
// ---------------------------------------------------------


// names for texture regions
#define RegionBackground     0
#define FirstRegionDice     10
#define FirstRegionBars     20

// names for sounds
#define SoundDice      0
#define SoundFinished  1

// other values
#define DiceSides    6
#define PageRows     7
#define PageColumns  7
#define PageRolls    (PageRows * PageColumns)


// ---------------------------------------------------------
//   SCENE DRAWING FUNCTIONS
// ---------------------------------------------------------


void DrawPercentageBar( int Number, int Percentage )
{
    select_region( FirstRegionBars + Number );
    set_drawing_point( 379 + 43*Number, 219 );
    set_drawing_scale( 1, -2*Percentage );
    draw_region_zoomed();
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
    
    // background image for statistics
    select_region( RegionBackground );
    define_region( 0,0,  285,359,  285,0 );
    
    // all sides for the dice
    define_region_matrix( FirstRegionDice,  287,0,  321,36,  287,0,  1,6,  1 );
    
    // all statistic bars
    define_region_matrix( FirstRegionBars,  287,228,  309,228,  287,228,  1,6,  1 );
    
    // ------------------------------------
    // PART 2: INITIAL PREPARATIONS
    // ------------------------------------
    
    // this stores the dice rolled in the current page
    int[ PageRows ][ PageColumns ] PageResults;
    
    // these are the global statistics counting all pages
    int TotalRollCount = 0;
    int[ DiceSides ] RollsByResult;
    int[ DiceSides ] Percentages;
    
    for( int i = 0; i < DiceSides; ++i )
      RollsByResult[ i ] = 0;
    
    // initial seed
    srand( get_time() );
    
    // gamepad 1 is used for control
    select_gamepad( 0 );
    
    // adjust sound volumes
    select_channel( 0 );
    set_channel_volume( 0.2 );
    
    select_channel( 1 );
    set_channel_volume( 0.8 );
    
    // ------------------------------------
    // PART 3: MAIN LOOP
    // ------------------------------------
    
    while( true )
    {
        // generate a new page
        set_multiply_color( color_white );
        clear_screen( 0xFF7C6A40 );
        
        for( int PageRollCount = 0; PageRollCount < PageRolls; ++PageRollCount )
        {
            // determine roll placement
            int Column = PageRollCount % PageColumns;
            int Row = PageRollCount / PageColumns;
            
            // roll a new dice
            int RollResult = rand() % 6;
            PageResults[ Row ][ Column ] = RollResult;
            play_sound( SoundDice );
            
            // recalculate percentages
            RollsByResult[ RollResult ] += 1;
            TotalRollCount += 1;
            
            for( int i = 0; i < DiceSides; ++i )
              Percentages[ i ] = (100 * RollsByResult[ i ]) / TotalRollCount;
            
            // draw the last dice
            select_region( FirstRegionDice + RollResult );
            draw_region_at( 32 + 43*Column, 36 + 42*Row );
            
            // draw percentages
            select_region( RegionBackground );
            draw_region_at( 639, 0 );
            
            for( int i = 0; i < DiceSides; ++i )
              DrawPercentageBar( i, Percentages[ i ] );
            
            // write the total number of rolls
            int[ 30 ] RollsMessage;
            strcpy( RollsMessage, "Total rolls: " );
            
            int[ 10 ] NumberString;
            itoa( TotalRollCount, NumberString, 10 );
            strcat( RollsMessage, NumberString );
            print_at( 370, 300, RollsMessage );
            
            // have a small delay between rolls
            sleep( 10 );
        }
        
        // signal the end of page
        play_sound( SoundFinished );
        set_multiply_color( color_red );
        print_at( 370, 320, "Press A to continue..." );
        
        // wait for a button press
        while( gamepad_button_a() <= 0 )
          end_frame();
    }
}