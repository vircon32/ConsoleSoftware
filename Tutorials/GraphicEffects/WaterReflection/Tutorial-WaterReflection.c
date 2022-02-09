// include Vircon libraries
#include "video.h"
#include "audio.h"
#include "time.h"
#include "math.h"
#include "input.h"


// -----------------------------------------------
// DEFINITIONS
// -----------------------------------------------


// give names to our textures
#define TextureLakeMountains  0

// give names to our regions
#define RegionMountains   0
#define RegionSand        1
#define RegionLightLine   2
#define RegionShadowLine  3

// We will also create single-line regions that
// decompose the mountains into scanlines. For
// easier use we will number the regions using
// their target Y coordinate on screen. Note
// that this will turn the image upside down,
// as expected from a water reflection
#define MinScanlineY 192
#define MaxScanlineY 359

// Because of the ripple effect we will offset
// the line we choose at each screen scanline.
// To prevent falling out of range we will add
// a few more lines at both ends
#define FirstRegionScanlines  (MinScanlineY - 4)   // ripples are shorter at the horizon
#define LastRegionScanlines   (MaxScanlineY + 10)  // ripples are taller near the observer

// give names to our sounds
#define SoundDripping  0


// -----------------------------------------------
// MAIN FUNCTION
// -----------------------------------------------


// our program starts at the main function
void main( void )
{   
    // -----------------------------------------------
    // PART 1: DEFINE OUR TEXTURE REGIONS
    
    // select our only texture
    select_texture( TextureLakeMountains );
    
    // define our main background regions
    select_region( RegionMountains );
    define_region_topleft( 0,0,  639,197 );
    
    select_region( RegionSand );
    define_region( 0,199,  639,378,  0,378 );
    
    // these are only helper lines for the lighting
    select_region( RegionLightLine );
    define_region_topleft( 0,380,  639,380 );
    
    select_region( RegionShadowLine );
    define_region_topleft( 0,382,  639,382 );
    
    // now define the set of scanline regions for the reflection
    for( int Region = FirstRegionScanlines; Region <= LastRegionScanlines; Region++ )
    {
        int y = 192 - (Region - FirstRegionScanlines);
        select_region( Region );
        define_region_topleft( 0,y,  639,y );
    }
    
    // -----------------------------------------------
    // PART 2: INITIALIZATIONS
    
    // set our ambient sound to play looped
    select_sound( SoundDripping );
    set_sound_loop( true );
    play_sound( SoundDripping );
    
    // this will control the timing of the ripples
    int ElapsedFrames = 0;
    
    // we will allow to toggle each effect
    bool RipplesEnabled = true;
    bool LightingEnabled = true;
    
    // we will use the first gamepad
    select_gamepad( 0 );
    
    // -----------------------------------------------
    // PART 3: MAIN LOOP
    
    // our demo never ends: keep repeating
    // our logic for every frame (60 fps)
    while( true )
    {
        // button A toggles ripple effect
        if( gamepad_button_a() == 1 )
          RipplesEnabled = !RipplesEnabled;
        
        // button B toggles lighting
        if( gamepad_button_b() == 1 )
          LightingEnabled = !LightingEnabled;
        
        // draw our background to fill the screen
        select_region( RegionSand );
        draw_region_at( 0,359 );
        
        // draw the reflections one scanline at a time
        for( int y = MinScanlineY; y <= MaxScanlineY; y++ )
        {
            // first, estimate a distance from the far shore
            // based on the on-screen vertical distance to the horizon
            int YUnderHorizon = y - MinScanlineY;
            float EstimatedDistance = sqrt( YUnderHorizon * 40 );
            
            // calculate our ripples as a function of distance and time
            float RippleAltitude = 0.05 * 0.5 * EstimatedDistance * sin( EstimatedDistance - (2*pi/90)*(ElapsedFrames % 90) );
            
            // for the slope, use the derivative (cosine), but to get a
            // true slope we should not factor in the distance. We also
            // include a factor of 10 to increase the range
            float RippleSlope = 10 * cos( EstimatedDistance - (2*pi/90)*(ElapsedFrames % 90) );
            
            // PART 1: Draw reflection with ripple effect
            if( !RipplesEnabled )
              RippleAltitude = 0;
            
            int Opacity = min( 300 - YUnderHorizon, 255 );
            set_multiply_color( make_color_rgba( 128,210,255,Opacity ) );
            
            select_region( y + RippleAltitude );
            draw_region_at( 0,y );
            
            // PART 2: Draw lighting effect on top of the reflection
            if( LightingEnabled )
            {
                int LightingOpacity = 30 + YUnderHorizon;
                
                // for ripple areas sloped away from the camera
                // we apply a light using additive blending
                if( RippleSlope > 1 )
                {
                    set_blending_mode( blending_add );
                    
                    float SlopeProportion = (RippleSlope-1) / 9;
                    set_multiply_color( make_color_rgba( 255,255,255, LightingOpacity*SlopeProportion/12 ) );
                    select_region( RegionLightLine );
                    draw_region();
                    
                    set_blending_mode( blending_alpha );
                }
                
                // for ripple areas sloped towards the camera
                // apply a shadow with the multiply color
                else if( RippleSlope < -1 )
                {
                    float SlopeProportion = (1-RippleSlope) / 9;
                    set_multiply_color( make_color_rgba( 255,255,255, LightingOpacity*SlopeProportion/6 ) );
                    select_region( RegionShadowLine );
                    draw_region();
                }
            }
        }
        
        // now draw the mountains on top, unmodified
        set_blending_mode( blending_alpha );
        set_multiply_color( color_white );
        select_region( RegionMountains );
        draw_region_at( 0,0 );
        
        // we have ended this frame, so wait for
        // the next in order to control speed
        end_frame();
        ElapsedFrames++;
    }
}
