// *****************************************************************************
    // include standard Vircon headers
    #include "time.h"
    #include "input.h"
    #include "video.h"
    #include "string.h"
    
    // include additional headers
    #include "vector2d.h"
    #include "interval.h"
    #include "../rectangle.h"
// *****************************************************************************


// ---------------------------------------------------------
//   DEFINITIONS AND CONSTANTS
// ---------------------------------------------------------


// names for textures
#define TextureRectangles 0

// names for texture regions
#define RegionRectanglePlayer  0
#define RegionRectangleLarge   1
#define RegionRectangleSmall   2

// define motion for our rectangle
#define MovementSpeed 100   // in pixels per second
#define RotationSpeed pi/2  // in radians per second


// ---------------------------------------------------------
//   GLOBAL VARIABLES
// ---------------------------------------------------------


// define our rectangles
rectangle RectPlayer =
{
    { 216, 105 },   // center position
    150/2, 80/2,    // half size
    0               // initial angle
};

// fixed large rectangle
rectangle RectLarge =
{
    { 420, 190 },   // center position
    150/2, 200/2,   // half size
    -pi/4           // angle
};

// fixed small rectangle
rectangle RectSmall =
{
    { 235, 240 },   // center position
    100/2, 50/2,    // half size
    pi/6            // angle
};

// these flags register which of the fixed
// rectangles were colliding on this frame
bool RectLargeColliding = false;
bool RectSmallColliding = false;


// ---------------------------------------------------------
//   AUXILIARY FUNCTIONS
// ---------------------------------------------------------


void DefineRegions()
{
    select_texture( TextureRectangles );
    select_region( RegionRectangleLarge );
    define_region_center( 0,0,  149,199 );
    select_region( RegionRectanglePlayer );
    define_region_center( 151,0,  300,79 );
    select_region( RegionRectangleSmall );
    define_region_center( 151,81,  250,130 );
}

// ---------------------------------------------------------

void DrawScene()
{
    // print instructions
    clear_screen( color_blue );
    print_at( 10, 10, "Move the green rectangle with the D-Pad" );
    print_at( 10, 30, "and rotate it with buttons A and B" );
    
    // draw player rectangle
    select_region( RegionRectanglePlayer );
    set_drawing_angle( RectPlayer.length_angle );
    draw_region_rotated_at( RectPlayer.center.x, RectPlayer.center.y );
    
    // draw small rectangle
    if( RectSmallColliding )
      set_multiply_color( color_gray );
    else
      set_multiply_color( color_white );
    
    select_region( RegionRectangleSmall );
    set_drawing_angle( RectSmall.length_angle );
    draw_region_rotated_at( RectSmall.center.x, RectSmall.center.y );
    
    // draw large rectangle
    if( RectLargeColliding )
      set_multiply_color( color_gray );
    else
      set_multiply_color( color_white );
    
    select_region( RegionRectangleLarge );
    set_drawing_angle( RectLarge.length_angle );
    draw_region_rotated_at( RectLarge.center.x, RectLarge.center.y );
    
    // restore neutral multiply color
    set_multiply_color( color_white );
}


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // ------------------------------------
    // PART 1: INITIALIZATIONS
    // ------------------------------------
    
    // define all texture regions
    DefineRegions();
    
    // initialize fixed rectangles
    rect_precalculate( &RectLarge );
    rect_precalculate( &RectSmall );
    
    // ------------------------------------
    // PART 2: MAIN LOOP
    // ------------------------------------
    
    while( true )
    {
        // read player inputs
        int DirectionX, DirectionY;
        gamepad_direction( &DirectionX, &DirectionY );
        
        // the d-pad moves our rectangle
        RectPlayer.center.x += DirectionX * (MovementSpeed / frames_per_second);
        RectPlayer.center.y += DirectionY * (MovementSpeed / frames_per_second);
        
        // buttons A and B rotate our rectangle
        if( gamepad_button_b() > 0 )
          RectPlayer.length_angle -= RotationSpeed / frames_per_second;
        
        if( gamepad_button_a() > 0 )
          RectPlayer.length_angle += RotationSpeed / frames_per_second;
        
        // check for collisions and solve them if any
        rect_precalculate( &RectPlayer );
        vector2d CenterCorrection;
        
        if( !rects_can_discard_collision( &RectPlayer, &RectLarge ) )
          RectLargeColliding = rects_solve_collision( &RectPlayer, &RectLarge, &CenterCorrection );
        
        if( !rects_can_discard_collision( &RectPlayer, &RectSmall ) )
          RectSmallColliding = rects_solve_collision( &RectPlayer, &RectSmall, &CenterCorrection );
        
        // - - - - - - - - - - - - - - - - - - - - - - - -
        // DRAWING THE SCENE
        
        DrawScene();
        
        // wait until next frame
        end_frame();
    }
}
