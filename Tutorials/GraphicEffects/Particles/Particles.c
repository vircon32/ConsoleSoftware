// *****************************************************************************
    // include standard Vircon headers
    #include "time.h"
    #include "video.h"
    #include "misc.h"
    
    // include additional libraries
    #include "print_numbers.h"
    #include "vector2d.h"
    
    // include project headers
    #include "WaterDrops.h"
// *****************************************************************************


// ---------------------------------------------------------
//   DEFINITIONS
// ---------------------------------------------------------


// textures
#define TextureParticles 0

// texture regions
enum TextureRegions
{
    RegionTap,
    RegionFloor,
    RegionDropLarge,
    RegionDropSmall
};


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    
    // -----------------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // -----------------------------------------------
    
    select_texture( TextureParticles );
    
    select_region( RegionTap );
    define_region( 1,1,  143,76,  143,1 );
    
    select_region( RegionFloor );
    define_region( 147,1,  147,52,  147,52 );
    
    select_region( RegionDropLarge );
    define_region_center( 151,1,  175,25 );
    
    select_region( RegionDropSmall );
    define_region_center( 151,29,  160,38 );
    
    // -----------------------------------------------
    // PART 2: INITIALIZATIONS
    // -----------------------------------------------
    
    // initialize both lists
    List_Initialize( &LargeDrops );
    List_Initialize( &SmallDrops );
    
    // make dynamic memory range very small, to
    // force reusing blocks after a few seconds
    int Size1Drop = sizeof( malloc_block ) + sizeof( WaterDrop );
    int DropsOnMemory = 250; 
    malloc_end_address = (int*)malloc_start_address + DropsOnMemory * Size1Drop;
    
    // -----------------------------------------------
    // PART 3: MAIN LOOP
    // -----------------------------------------------
    
    while( true )
    {
        // simulate previous drops
        UpdateLargeDrops();
        UpdateSmallDrops();
        
        // create a new drop every frame
        WaterDrop* NewDrop = List_Add( &LargeDrops );
        
        // randomize its position
        float Displacement = RandomFloatBetween( -3, 3 );
        NewDrop->Position.x = 530 + Displacement;
        NewDrop->Position.y = 72 + Displacement;
        
        // randomize its speed
        float SpeedFactor = RandomFloatBetween( 0.8, 1.2 );
        NewDrop->Velocity.x = -200 * SpeedFactor;
        NewDrop->Velocity.y = 80 * SpeedFactor;
        
        // draw background
        clear_screen( make_gray( 130 ) );
        
        select_region( RegionFloor );
        set_drawing_scale( screen_width, 1 );
        draw_region_zoomed_at( 0, screen_height-1 );
        
        // draw all the drops
        List_Draw( &LargeDrops, RegionDropLarge );
        List_Draw( &SmallDrops, RegionDropSmall );
        
        // draw the tap
        select_region( RegionTap );
        draw_region_at( screen_width-1, 23 );
        
        // report total number of drops
        print_at( 5, 5, "Total drops on screen:" );
        print_int_at( 5, 25, LargeDrops.NumberOfDrops + SmallDrops.NumberOfDrops );
        
        // wait until next frame
        end_frame();
    }
}
