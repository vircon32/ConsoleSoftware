// include Vircon libraries
#include "video.h"
#include "input.h"
#include "time.h"
#include "misc.h"


// -----------------------------------------------
// DEFINITIONS FOR TEXTURES
// -----------------------------------------------


// give names to each texture in our cartridge
#define TexturePaletteSwap  0

// names for single texture regions
#define RegionConstants  0
#define RegionSkin       1
#define RegionDress      2
#define RegionBelt       3
#define RegionSideband   4
#define RegionGUI        5
#define RegionMarker     6


// -----------------------------------------------
// DEFINITIONS FOR GAMEPLAY
// -----------------------------------------------


// helper structure to represent the
// available colors for each object
struct ColorSet
{
    int[ 4 ] Colors;
    int SelectedIndex;
};

// the 4 selectable colors in our character
ColorSet ColorsSkin, ColorsDress, ColorsBelt, ColorsSideband;


// -----------------------------------------------
// MAIN FUNCTION
// -----------------------------------------------


void main( void )
{
    // -----------------------------------------------
    // PART 1: DEFINE OUR TEXTURE REGIONS
    
    select_texture( TexturePaletteSwap );
    
    // define character regions
    select_region( RegionSkin );
    define_region_topleft( 1,1,  177,346 );
    
    select_region( RegionConstants );
    define_region_topleft( 181,1,  391,196 );
    
    select_region( RegionDress );
    define_region_topleft( 395,1,  518,103 );
    
    select_region( RegionBelt );
    define_region_topleft( 407,107,  477,231 );
    
    select_region( RegionSideband );
    define_region_topleft( 481,107,  524,211 );
    
    // define GUI regions
    select_region( RegionGUI );
    define_region_topleft( 181,200,  403,387 );
    
    select_region( RegionMarker );
    define_region( 407,235,  440,270,  410,239 );
    
    // -----------------------------------------------
    // PART 2: DEMO INITIALIZATIONS
    
    // fill in the available skin colors
    ColorsSkin.Colors[ 0 ] = make_color_rgb( 200, 145, 120 );      // lighter skin
    ColorsSkin.Colors[ 1 ] = make_color_rgb( 125,  90,  60 );      // darker skin
    ColorsSkin.Colors[ 2 ] = make_color_rgb( 100, 120, 100 );      // greyish green
    ColorsSkin.Colors[ 3 ] = make_color_rgb( 150, 100, 180 );      // purple
    
    // colors for the 3 clothing parts are common
    ColorsDress.Colors[ 0 ] = make_color_rgb(  80, 155, 220 );     // blue
    ColorsDress.Colors[ 1 ] = make_color_rgb( 110, 170,  90 );     // green
    ColorsDress.Colors[ 2 ] = make_color_rgb( 215, 215,  40 );     // red
    ColorsDress.Colors[ 3 ] = make_color_rgb( 240,  45,  45 );     // yellow
    
    ColorsBelt = ColorsDress;
    ColorsSideband = ColorsDress;
    
    // set initial color selections
    ColorsSkin.SelectedIndex = 0;
    ColorsDress.SelectedIndex = 1;
    ColorsBelt.SelectedIndex = 2;
    ColorsSideband.SelectedIndex = 3;
    
    // we will control colors with gamepad 1
    select_gamepad( 0 );
    
    // -----------------------------------------------
    // PART 3: MAIN LOOP
    
    // our demo never ends: keep repeating
    // our logic for every frame (60 fps)
    while( true )
    {
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 1: Simulate the scene
        
        // button A cycles skin color
        if( gamepad_button_a() == 1 )
        {
            ColorsSkin.SelectedIndex++;
            ColorsSkin.SelectedIndex %= 4;
        }
        
        // button B cycles dress color
        if( gamepad_button_b() == 1 )
        {
            ColorsDress.SelectedIndex++;
            ColorsDress.SelectedIndex %= 4;
        }
        
        // button X cycles belt color
        if( gamepad_button_x() == 1 )
        {
            ColorsBelt.SelectedIndex++;
            ColorsBelt.SelectedIndex %= 4;
        }
        
        // button Y cycles sideband color
        if( gamepad_button_y() == 1 )
        {
            ColorsSideband.SelectedIndex++;
            ColorsSideband.SelectedIndex %= 4;
        }
        
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 2: Draw the scene on screen
        
        // draw a light background
        clear_screen( make_color_rgb( 140,180,220 ) );
        
        // each characters part is drawn using
        // the selected color as multiply color
        set_multiply_color( ColorsSkin.Colors[ ColorsSkin.SelectedIndex ] );
        select_region( RegionSkin );
        draw_region_at( 76,14 );
        
        set_multiply_color( ColorsDress.Colors[ ColorsDress.SelectedIndex ] );
        select_region( RegionDress );
        draw_region_at( 121,183 );
        
        set_multiply_color( ColorsBelt.Colors[ ColorsBelt.SelectedIndex ] );
        select_region( RegionBelt );
        draw_region_at( 144,170 );
        
        set_multiply_color( ColorsSideband.Colors[ ColorsSideband.SelectedIndex ] );
        select_region( RegionSideband );
        draw_region_at( 145,72 );
        
        // some character elements never change color
        set_multiply_color( color_white );
        select_region( RegionConstants );
        draw_region_at( 65,6 );
        
        // draw GUI and show current color selections
        select_region( RegionGUI );
        draw_region_at( 333,86 );
        
        select_region( RegionMarker );
        draw_region_at( 424 + 32*ColorsSkin.SelectedIndex, 94 );
        draw_region_at( 424 + 32*ColorsDress.SelectedIndex, 142 );
        draw_region_at( 424 + 32*ColorsBelt.SelectedIndex, 190 );
        draw_region_at( 424 + 32*ColorsSideband.SelectedIndex, 238 );
        
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 3: Wait for next frame
        end_frame();
    }
}
