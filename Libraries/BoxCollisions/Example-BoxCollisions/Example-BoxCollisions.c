// *****************************************************************************
    // include standard Vircon headers
    #include "math.h"
    #include "time.h"
    #include "input.h"
    #include "video.h"
    #include "audio.h"
    
    // include additional headers
    #include "vector2d.h"
    #include "tilemap.h"
    #include "../box.h"
    #include "../mapcollisions.h"
// *****************************************************************************


// ---------------------------------------------------------
//   DEFINITIONS AND CONSTANTS
// ---------------------------------------------------------


// names for textures
#define TextureCollisions 0

// names for texture regions
#define FirstRegionTileSet 0
#define FirstRegionBall 2

// dimensions of our map, in tiles
#define MapTilesX 8
#define MapTilesY 5

// define the moving balls
#define NumberOfBalls 4
#define BallSize 42     // in pixels
#define BallSpeed 200   // in pixels per second


// ---------------------------------------------------------
//   GLOBAL VARIABLES
// ---------------------------------------------------------


// this 2D array is our tile map; each tile ID; it
// with 80x72 pixel tiles, it is the same size of the
// screen; tiles have only 2 values: 0 = empty, 1 = block
int[ MapTilesY ][ MapTilesX ] MapArray = 
{
    { 1, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 1, 0 },
    { 0, 0, 0, 1, 0, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 0, 0, 0 },
};

// objects for our tile map; the same map and tiles are
// used for both drawing and determining box collisions
tileset Tiles;
tilemap Map;

// our moving balls are approximated as boxes
box[ NumberOfBalls ] Balls;

// this flag controls if ball hitboxes are shown
bool ShowBallHitbox = false;


// ---------------------------------------------------------
//   AUXILIARY FUNCTIONS
// ---------------------------------------------------------


void DrawBall( box* Ball )
{
    int BallScreenX = Ball->position.x;
    int BallScreenY = Ball->position.y;
    tilemap_convert_position_to_screen( &Map, &BallScreenX, &BallScreenY );
    
    // now draw the character at the converted position
    select_region( FirstRegionBall + ShowBallHitbox );
    draw_region_at( BallScreenX, BallScreenY );
}


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------
// Note that the structure of the main loop follows the same
// simulation steps described in box.h, but also adds box VS
// map collisions to ensure that no ball penetrates the map
// or exits it.
// 
// Also, be aware that when more than one box is colliding
// it might be needed to perform more than one iteration of
// collisions to ensure correctness, because of multiple
// collisions affecting a box in the same frame.
// ---------------------------------------------------------


void main( void )
{
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    select_texture( TextureCollisions );
    
    // regions for tileset
    define_region_matrix( FirstRegionTileSet,  1,1,  80,72,  1,1,  2,1,  0 );
    
    // regions for ball
    define_region_matrix( FirstRegionBall,  1,76,  50,125,  25,100,  2,1,  1 );
    
    // ------------------------------------
    // PART 2: DEFINE MAP AND TILES
    // ------------------------------------
    
    // - - - - - - - - - - - - - - - - - - -
    // 2.1: define our tiles
    
    // define dimensions for our tiles
    Tiles.width  = 80;
    Tiles.height = 72;
    
    // adjacent tiles have neither space nor overlap
    Tiles.gap_x = 0;
    Tiles.gap_y = 0;
    
    // define texture and regions for our tiles
    Tiles.texture_id = TextureCollisions;
    Tiles.tile_zero_region_id = FirstRegionTileSet;
    
    // this particular tile set does not use its
    // first tile (it's discarded as transparent)
    Tiles.draw_tile_zero = false;
    
    // define the range of solid tiles
    Tiles.first_solid_tile = 1;
    Tiles.last_solid_tile = 1;
    
    // - - - - - - - - - - - - - - - - - - -
    // 2.2: define our map
    
    // assign our tiles to the map
    Map.tiles = &Tiles;
    
    // define the map array
    Map.map = &MapArray[ 0 ][ 0 ];
    Map.map_width  = MapTilesX;
    Map.map_height = MapTilesY;
    
    // our camera will be fixed at map center
    Map.camera_position.x = screen_width/2;
    Map.camera_position.y = screen_height/2;
    
    // ------------------------------------
    // PART 3: DEFINE MOVING BALLS
    // ------------------------------------
    
    for( int i = 0; i < NumberOfBalls; ++i )
    {
        // define the balls shape (a square) with its
        // hotspot ot reference point at its center;
        // this point is taken as the box position
        Balls[ i ].width  = BallSize;
        Balls[ i ].height = BallSize;
        Balls[ i ].hotspot.x = BallSize/2;
        Balls[ i ].hotspot.y = BallSize/2;
        
        // distribute positions along X at screen center
        Balls[ i ].position.x = screen_width/2 + 60 * (-NumberOfBalls/2 + i);
        Balls[ i ].position.y = screen_height/2 - 40;
        
        // same X speed, but alternate Y speeds
        box_full_stop( &Balls[ i ] );
        Balls[ i ].velocity.x = BallSpeed * cos( pi/3 );
        Balls[ i ].velocity.y = BallSpeed * sin( pi/3 );
        if( i % 2 ) Balls[ i ].velocity.y *= -1;
    }
    
    // ------------------------------------
    // PART 4: MAIN LOOP
    // ------------------------------------
    
    while( true )
    {
        // - - - - - - - - - - - - - - - - - - - - - - - -
        // SIMULATION STEP 1
        // -> determine unrestricted motion for current frame
        for( int i = 0; i < NumberOfBalls; ++i )
        {
            // reset ball contacts and other data for this frame
            box_begin_frame( &Balls[ i ] );
            
            // simulate ball physics to determine their movement
            // (but don't apply it yet, since it's unrestricted)
            box_simulate( &Balls[ i ] );
        }
        
        // - - - - - - - - - - - - - - - - - - - - - - - -
        // SIMULATION STEP 2
        // -> restrict motion with ball-ball collisions
        for( int i = 0; i < NumberOfBalls; ++i )
          for( int j = i+1; j < NumberOfBalls; ++j )
            boxes_collide( &Balls[ i ], &Balls[ j ] );
        
        // - - - - - - - - - - - - - - - - - - - - - - - -
        // SIMULATION STEP 3
        // -> further restrict motion with ball-map collisions
        for( int i = 0; i < NumberOfBalls; ++i )
        {
            // collide box with map tiles, and also with map limits
            collide_box_with_tilemap( &Balls[ i ], &Map );
            collide_box_with_tilemap_limits( &Balls[ i ], &Map );
        }
        
        // - - - - - - - - - - - - - - - - - - - - - - - -
        // SIMULATION STEP 4
        // -> apply restricted motion and apply ball bounces
        for( int i = 0; i < NumberOfBalls; ++i )
        {
            // now we can safely apply the restricted motion
            // and there should be no object penetration
            box_move( &Balls[ i ] );
            
            // adapt ball speed to any collisions that happened
            // (bounce coeff. = 1 means speeds are conserved)
            box_apply_bounce( &Balls[ i ], 1 );
        }
        
        // - - - - - - - - - - - - - - - - - - - - - - - -
        // DRAWING THE SCENE
        
        // allow button A to toggle showing ball hitboxes
        select_gamepad( 0 );
        
        if( gamepad_button_a() == 1 )
          ShowBallHitbox = !ShowBallHitbox;
        
        // render tile map with a solid background
        clear_screen( color_blue );
        tilemap_draw_from_camera( &Map );
        
        // to draw elements in the map, like the balls, we first
        // need to convert their map coordinates to screen coordinates
        for( int i = 0; i < NumberOfBalls; ++i )
          DrawBall( &Balls[ i ] );
        
        // write "instructions" text
        print_at( 90, 10, "Press A to toggle ball hitboxes" );
        
        // wait until next frame
        end_frame();
    }
}
