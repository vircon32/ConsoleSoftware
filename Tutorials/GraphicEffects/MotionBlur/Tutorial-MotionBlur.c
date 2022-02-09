// include Vircon libraries
#include "video.h"
#include "input.h"
#include "time.h"
#include "math.h"
#include "misc.h"


// -----------------------------------------------
// DEFINITIONS FOR TEXTURES
// -----------------------------------------------


// give names to each texture in our cartridge
#define TextureMotionBlur  0

// names for our texture regions
// within the background texture
#define RegionBackground  0

// names for our texture regions
// within the game objects texture
#define RegionBackground      0
#define RegionPlayerShip      1
#define RegionGUI             2


// -----------------------------------------------
// DEFINITIONS FOR GAMEPLAY
// -----------------------------------------------


// movement speeds of the different objects
#define PlayerSpeed  6
#define ScrollSpeed  3

// basic state for the ship object
struct GameObject
{
    int X, Y;           // position of hitbox center on screen
    int Width, Height;  // dimensions of hitbox, in pixels
};

// we will let the player toggle between either
// full-screen blur or blurring only the ship
bool FullScreenBlurEnabled = false;

// for ship-only blurring we will store the last
// ship positions to manually draw the blur
int[ 10 ] LastXPositions, LastYPositions;


// -----------------------------------------------
// MAIN FUNCTION
// -----------------------------------------------


void main( void )
{
    // -----------------------------------------------
    // PART 1: DEFINE OUR TEXTURE REGIONS
    
    select_texture( TextureMotionBlur );
    
    select_region( RegionBackground );
    define_region_topleft( 0,0,  639,359 );
    
    select_region( RegionPlayerShip );
    define_region( 643,1,  711,52,  676,30 );
    
    select_region( RegionGUI );
    define_region_topleft( 643,56,  694,129 );
    
    // -----------------------------------------------
    // PART 2: DEMO INITIALIZATIONS
    
    // we will control the ship with gamepad 1
    select_gamepad( 0 );
    
    // set the initial state of our ship
    GameObject Player;
    Player.Width = 55;
    Player.Height = 37;
    Player.X = screen_width / 2;
    Player.Y = screen_height / 2;
    
    // scroll control
    int ElapsedFrames = 0;
    
    // -----------------------------------------------
    // PART 3: MAIN LOOP
    
    // our game never ends: keep repeating
    // our game logic for every frame (60 fps)
    while( true )
    {
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 1: Simulate the scene
        
        // read the direction pressed on the D-Pad
        int DirectionX, DirectionY;
        gamepad_direction( &DirectionX, &DirectionY );        
        
        // button A toggles background blur 
        if( gamepad_button_a() == 1 )
          FullScreenBlurEnabled = !FullScreenBlurEnabled;
        
        // move the sequence of stored last positions 1 frame
        // towards the past, to make room for the last position
        memcpy( LastXPositions, &LastXPositions[1], 9 );
        memcpy( LastYPositions, &LastYPositions[1], 9 );
        
        // save previous position before changing it
        LastXPositions[ 9 ] = Player.X;
        LastYPositions[ 9 ] = Player.Y;
        
        // move the ship in the direction pressed
        Player.X += PlayerSpeed * DirectionX;
        Player.Y += PlayerSpeed * DirectionY;
            
        // prevent player from leaving the screen
        Player.X = max( Player.X, Player.Width/2 );
        Player.Y = max( Player.Y, Player.Height/2 );
        
        Player.X = min( Player.X, screen_width - Player.Width/2 );
        Player.Y = min( Player.Y, screen_height - Player.Height/2 );
        
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 2: Draw the scene on screen
        
        // make background scroll left
        int BackgroundX = -((ScrollSpeed*ElapsedFrames) % screen_width);
        
        // full-screen blur, if enabled, is done by making
        // current frame background not completely opaque
        // (so, the previous frames are slightly seen through)
        if( FullScreenBlurEnabled )
          set_multiply_color( make_color_rgba( 255,255,255,75 ) );
        
        // when disabled, the previous step is not taken
        // and background is just drawn at full opacity
        select_region( RegionBackground );
        draw_region_at( BackgroundX, 0 );
        draw_region_at( BackgroundX + screen_width, 0 );
        
        // when full-screen blur is disabled we will instead
        // manually draw a blur of the ship, by drawing it at
        // its last positions with progressively higher opacity
        if( !FullScreenBlurEnabled )
        {
            select_region( RegionPlayerShip );
            
            for( int i = 0; i < 10; i++ )
            {
                set_multiply_color( make_color_rgba( 255,255,255,10*i ) );
                draw_region_at( LastXPositions[ i ], LastYPositions[ i ] );
            }
        }
        
        // draw player ship at current position, at full opacity
        set_multiply_color( color_white );
        select_region( RegionPlayerShip );
        draw_region_at( Player.X, Player.Y );
        
        // draw GUI, always at full opacity
        // (GUI is never affeced by any blur)
        select_region( RegionGUI );
        draw_region_at( 5, 5 );        
        
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 3: Wait for next frame
        end_frame();
        ElapsedFrames++;
    }
}
