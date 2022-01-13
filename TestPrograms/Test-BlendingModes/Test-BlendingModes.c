// include Vircon libraries
#include "video.h"
#include "audio.h"
#include "time.h"
#include "math.h"
#include "misc.h"


// ---------------------------------------------------------
//   DEFINITIONS AND CONSTANTS
// ---------------------------------------------------------


// names for texture regions
#define RegionBackground    0
#define RegionBall          1
#define RegionLamp          2
#define RegionLight        10
#define RegionShadow       20
#define RegionReflection   30

// names for sounds
#define SoundLampMoving  0

// scene cycle time
#define SceneFrames  (4 * 60)


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
    define_region_topleft( 0,0,  639,359 );
    
    // ball has hotspot on bottom center
    select_region( RegionBall );
    define_region( 202,361,  301,460,  251,460 );
    
    // lamp has hotspot on sphere center
    select_region( RegionLamp );
    define_region( 202,462,  272,592,  237,557 );
    
    // shadow has hotspot on ball bottom left 
    select_region( RegionShadow );
    define_region( 303,361,  434,413,  303,386 );
    
    // light has hotspot on lamp sphere center
    select_region( RegionLight );
    define_region( 1,361,  200,607,  101,414 );
    
    // ball reflection has hotspot on reflected lamp center
    select_region( RegionReflection );
    define_region( 303,415,  316,431,  309,425 );
    
    // ------------------------------------
    // PART 2: INITIAL PREPARATIONS
    // ------------------------------------
    
    // this defines the movement cycle
    int ElapsedFrames = 0;
    
    // define lamp sound properties
    select_sound( SoundLampMoving );
    set_sound_loop( true );
    
    // play looped sound
    play_sound_in_channel( SoundLampMoving, 0 );
    set_channel_volume( 0.4 );
    
    
    // ------------------------------------
    // PART 3: MAIN LOOP
    // ------------------------------------
    
    while( true )
    {
        // precalculate scene parameters
        float OscillationAngle = ((2*pi) * ElapsedFrames / SceneFrames ) + pi/6;
        int LampOffsetX = 40 * cos( OscillationAngle );
        float LampFlicker = 0.5 * (rand()%5) / 5;
        float ShadowScaleX = 1 + 0.2 * (1 - cos( OscillationAngle ));
        
        // draw the background
        set_blending_mode( blending_alpha );
        select_region( RegionBackground );
        draw_region_at( 0, 0 );
        
        // draw the ball shadow
        set_blending_mode( blending_subtract );
        set_multiply_color( make_gray( 130 + 20 * cos( OscillationAngle ) ) );
        set_drawing_scale( ShadowScaleX, 1 );
        select_region( RegionShadow );
        draw_region_zoomed_at( 432, 254 );
        
        // draw the ball
        set_blending_mode( blending_alpha );
        set_multiply_color( color_white );
        select_region( RegionBall );
        draw_region_at( 469, 254 );
        
        // draw the lamp
        select_region( RegionLamp );
        draw_region_at( 206+LampOffsetX, 95 );
        
        // draw the lamp light
        set_blending_mode( blending_add );
        set_multiply_color( make_gray( 255 * (1-LampFlicker) ) );
        select_region( RegionLight );
        draw_region_at( 206+LampOffsetX, 95 );
        
        // draw the ball reflection
        set_multiply_color( 0xAAFFFFFF );
        select_region( RegionReflection );
        draw_region_at( 438+LampOffsetX/10, 191-LampOffsetX/6 );
        
        // advance the movement cycle
        // and make it loop when needed
        ++ElapsedFrames;
        
        if( ElapsedFrames >= SceneFrames )
          ElapsedFrames = 0;
        
        // wait for next frame
        end_frame();
    }
}