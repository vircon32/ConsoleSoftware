// include Vircon libraries
#include "video.h"
#include "input.h"
#include "time.h"
#include "math.h"
#include "misc.h"


// -----------------------------------------------
// DEFINITIONS FOR TEXTURES
// -----------------------------------------------


// give a name to our only texture
#define TextureScrollParallax  0

// names for our regions in the texture; use
// 2 for each scroll plane (left and right)
// because to draw it in diffetent X positions
// while making it loop we will need to split
// each plane in 2 vertically, depending on X
enum Regions
{
    RegionSkyLeft,
    RegionSkyRight,
    RegionMountainsLeft,
    RegionMountainsRight,
    RegionPalmTreesLeft,
    RegionPalmTreesRight,
    RegionGrassLeft,
    RegionGrassRight
};


// -----------------------------------------------
// DEFINITIONS FOR THE SCROLL LAYERS
// -----------------------------------------------


// these will be our layers, in order from
// farthest (drawn first) to closest (last)
enum LayerNames
{
    LayerSky,
    LayerMountains,
    LayerPalmTrees,
    LayerGrass
};

// we will handle all our layers the same way;
// save their state and config in this structure
struct ScrollLayer
{
    int PositionX;
    int Speed;
    int RegionMinY, RegionMaxY;
    int RegionIDLeft, RegionIDRight;
};

// scroll layer speeds, in pixels/frame
// (closer layers will be faster)
#define SpeedSky        1
#define SpeedMountains  2
#define SpeedPalmTrees  4
#define SpeedGrass      8

// global variables
int ScrollX;
ScrollLayer[ 4 ] ScrollLayers;


// -----------------------------------------------
// MAIN FUNCTION
// -----------------------------------------------


void main( void )
{
    // -----------------------------------------------
    // PART 1: INITIALIZATIONS
    
    // global initial state
    select_texture( TextureScrollParallax );
    ScrollX = 0;
    
    // define each of our scroll layers
    ScrollLayers[ LayerSky ].Speed = SpeedSky;
    ScrollLayers[ LayerSky ].RegionMinY = 0;
    ScrollLayers[ LayerSky ].RegionMaxY = 359;
    ScrollLayers[ LayerSky ].RegionIDLeft = RegionSkyLeft;
    ScrollLayers[ LayerSky ].RegionIDRight = RegionSkyRight;
    
    ScrollLayers[ LayerMountains ].Speed = SpeedMountains;
    ScrollLayers[ LayerMountains ].RegionMinY = 361;
    ScrollLayers[ LayerMountains ].RegionMaxY = 604;
    ScrollLayers[ LayerMountains ].RegionIDLeft = RegionMountainsLeft;
    ScrollLayers[ LayerMountains ].RegionIDRight = RegionMountainsRight;
    
    ScrollLayers[ LayerPalmTrees ].Speed = SpeedPalmTrees;
    ScrollLayers[ LayerPalmTrees ].RegionMinY = 606;
    ScrollLayers[ LayerPalmTrees ].RegionMaxY = 796;
    ScrollLayers[ LayerPalmTrees ].RegionIDLeft = RegionPalmTreesLeft;
    ScrollLayers[ LayerPalmTrees ].RegionIDRight = RegionPalmTreesRight;
    
    ScrollLayers[ LayerGrass ].Speed = SpeedGrass;
    ScrollLayers[ LayerGrass ].RegionMinY = 798;
    ScrollLayers[ LayerGrass ].RegionMaxY = 844;
    ScrollLayers[ LayerGrass ].RegionIDLeft = RegionGrassLeft;
    ScrollLayers[ LayerGrass ].RegionIDRight = RegionGrassRight;
    
    // -----------------------------------------------
    // PART 2: MAIN LOOP
    
    // our game never ends: keep repeating
    // our game logic for every frame (60 fps)
    while( true )
    {
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 1: Avance the scroll
        
        // this is the global parameter controlling scroll
        ScrollX++;
        
        // wrap-around for an endless loop
        if( ScrollX >= screen_width )
          ScrollX -= screen_width;
        
        // adjust the position and state of each layer to global scroll X
        for( int Layer = LayerSky; Layer <= LayerGrass; Layer++ )
        {
            // determine layer position in X from its relative speed
            ScrollLayer* CurrentLayer = &ScrollLayers[ Layer ];
            CurrentLayer->PositionX = (ScrollX * CurrentLayer->Speed) % screen_width;
            
            // in the texture, the image for each scroll plane is the same
            // width as the screen but it loops horizontally; to draw it
            // with a given X offset we will redefine its left and right
            // parts each frame, so that the 2 of them always fill the whole
            // screen while making them match seamlessly 
            
            // the region at screen left will start at current X
            // position and end at the right end of the image
            select_region( CurrentLayer->RegionIDLeft );
            
            define_region
            (
                CurrentLayer->PositionX, CurrentLayer->RegionMinY,  // top-left pixel
                screen_width-1, CurrentLayer->RegionMaxY,           // bottom-right pixel
                CurrentLayer->PositionX, CurrentLayer->RegionMaxY   // hotspot at bottom-left
            );
            
            select_region( CurrentLayer->RegionIDRight );
            
            // the region at screen right will start at X = 0
            // and end the pixel before current X position
            define_region
            (
                0, CurrentLayer->RegionMinY,                            // top-left pixel
                CurrentLayer->PositionX + 1, CurrentLayer->RegionMaxY,  // bottom-right pixel
                0, CurrentLayer->RegionMaxY                             // hotspot at bottom-left
            );
        }
        
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 2: Draw the scene on screen
        
        // not needed since the sky plane covers the
        // whole screen, but useful to detect errors
        // if we split or position the plane wrong
        clear_screen( color_black );
        
        // draw each layer in order (from farthest to nearest)
        for( int Layer = LayerSky; Layer <= LayerGrass; Layer++ )
        {
            ScrollLayer* CurrentLayer = &ScrollLayers[ Layer ];
            
            // draw left region always at screen bottom-left
            select_region( CurrentLayer->RegionIDLeft );
            draw_region_at( 0, screen_height-1 );
            
            // draw right region right next to the end of left region
            select_region( CurrentLayer->RegionIDRight );
            draw_region_at( screen_width - CurrentLayer->PositionX, screen_height-1 );
        }
        
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 3: Wait for next frame
        end_frame();
    }
}
