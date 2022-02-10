// include Vircon libraries
#include "video.h"
#include "input.h"
#include "time.h"
#include "misc.h"


// -----------------------------------------------
// DEFINITIONS FOR TEXTURES
// -----------------------------------------------


// give names to each texture in our cartridge
#define TextureSilhouette  0

// names for single texture regions
#define RegionGroundDay    1
#define RegionGroundNight  0
#define RegionShortTree    2
#define RegionTallTree     3
#define RegionGUI          4

// names for region groups
#define FirstRegionPeasant  10
#define FirstRegionSlime    20


// -----------------------------------------------
// DEFINITIONS FOR GAMEPLAY
// -----------------------------------------------


// movement speeds of the different objects
#define WalkSpeed  2
int WalkedPixels = 0;

// we will let the player toggle between either
// day (regular images) or night (silhouette mode)
bool NightModeEnabled = false;


// -----------------------------------------------
// AUXILIARY FUNCTIONS
// -----------------------------------------------


// this is the core function for this tutorial:
// it draws a region with a silhouette effect
void DrawWithSilhouette( int RegionID, int PositionX, int PositionY )
{
    select_region( RegionID );
    
    // to make the light outer contour we draw
    // the image 4 times at neighboring positions
    // with light effect (additive blending)
    set_blending_mode( blending_add );
    set_multiply_color( color_white );
    draw_region_at( PositionX-1, PositionY   );
    draw_region_at( PositionX+1, PositionY   );
    draw_region_at( PositionX  , PositionY-1 );
    draw_region_at( PositionX  , PositionY+1 );
    
    // then, on top of the previous, draw
    // the image in full black, centered
    set_blending_mode( blending_alpha );
    set_multiply_color( color_black );
    draw_region_at( PositionX, PositionY );
    
    // restore multiply color to normal
    set_multiply_color( color_white );
}


// -----------------------------------------------
// MAIN FUNCTION
// -----------------------------------------------


void main( void )
{
    // -----------------------------------------------
    // PART 1: DEFINE OUR TEXTURE REGIONS
    
    select_texture( TextureSilhouette );
    
    // define region sequences for animations
    define_region_matrix( FirstRegionPeasant,  1,1,  185,217,  102,207,  4,2,  1 );
    define_region_matrix( FirstRegionSlime,  1,439,  68,514,  34,514,  4,1,  1 );
    
    // define single regions
    select_region( RegionGroundDay );
    define_region( 279,439,  438,514,  279,514 );
    
    select_region( RegionGroundNight );
    define_region( 442,439,  601,514,  442,514 );
    
    select_region( RegionShortTree );
    define_region( 747,1,  970,192,  837,192 );
    
    select_region( RegionTallTree );
    define_region( 747,196,  972,435,  846,435 );
    
    select_region( RegionGUI );
    define_region_topleft( 605,439,  652,504 );
    
    // -----------------------------------------------
    // PART 2: DEMO INITIALIZATIONS
    
    // we will control day/night mode with gamepad 1
    select_gamepad( 0 );
    
    // -----------------------------------------------
    // PART 3: MAIN LOOP
    
    // our demo never ends: keep repeating
    // our logic for every frame (60 fps)
    while( true )
    {
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 1: Simulate the scene
        
        // button A toggles day/night mode
        if( gamepad_button_a() == 1 )
          NightModeEnabled = !NightModeEnabled;
        
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 2: Draw the scene on screen
        
        // our movement will stay within a 1-screen cycle
        WalkedPixels += WalkSpeed;
        WalkedPixels %= screen_width;
        
        int ShortTreeX = 112-WalkedPixels;
        int TallTreeX = 497-WalkedPixels;
        int GroundBlockX = -(WalkedPixels % 160);
        
        // determine animation frames
        int ElapsedFrames = WalkedPixels / WalkSpeed;
        int RegionPeasant = FirstRegionPeasant + (ElapsedFrames/8) % 8;
        int RegionSlime = FirstRegionSlime + (ElapsedFrames/10) % 4;
        
        // CASE A: draw the scene at night time
        if( NightModeEnabled )
        {
            // night sky
            clear_screen( make_color_rgb( 0,15,30 ) );
            
            // draw ground as a series of tiles
            // (done without silhouette so that
            // ground surface can be seen)
            select_region( RegionGroundNight );
            
            while( GroundBlockX < screen_width )
            {
                draw_region_at( GroundBlockX, 359 );
                GroundBlockX += 160;
            }
            
            // trees are drawn twice because they
            // may appear twice on the screen
            DrawWithSilhouette( RegionShortTree, ShortTreeX, 288 );
            DrawWithSilhouette( RegionTallTree, TallTreeX, 288 );
            
            DrawWithSilhouette( RegionShortTree, ShortTreeX + screen_width, 288 );
            DrawWithSilhouette( RegionTallTree, TallTreeX + screen_width, 288 );
            
            // characters do not move on screen X
            DrawWithSilhouette( RegionPeasant, 373, 300 );
            DrawWithSilhouette( RegionSlime, 240, 308 );
        }
        
        // CASE B: draw the scene at daytime
        else
        {
            // daylight sky
            clear_screen( make_color_rgb( 145,195,215 ) );
            
            // draw ground as a series of tiles
            // (done without silhouette so that
            // ground surface can be seen)
            select_region( RegionGroundDay );
            
            while( GroundBlockX < screen_width )
            {
                draw_region_at( GroundBlockX, 359 );
                GroundBlockX += 160;
            }
            
            // trees are drawn twice because they
            // may appear twice on the screen
            select_region( RegionShortTree );
            draw_region_at( ShortTreeX, 288 );
            draw_region_at( ShortTreeX + screen_width, 288 );
            
            select_region( RegionTallTree );
            draw_region_at( TallTreeX, 288 );
            draw_region_at( TallTreeX + screen_width, 288 );
            
            // characters do not move on screen X
            select_region( RegionPeasant );
            draw_region_at( 373, 300 );
            
            select_region( RegionSlime );
            draw_region_at( 240, 308 );
        }
        
        // GUI is always drawn without silhouette
        select_region( RegionGUI );
        draw_region_at( 5, 5 );
        
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 3: Wait for next frame
        end_frame();
    }
}
