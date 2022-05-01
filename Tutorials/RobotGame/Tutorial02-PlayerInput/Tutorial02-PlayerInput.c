// include Vircon libraries
#include "video.h"
#include "input.h"
#include "time.h"


// -----------------------------------------------
// DEFINITIONS
// -----------------------------------------------

// giving names to our texture regions
// makes them easier to define and use
#define RegionBackground  0
#define RegionRobotRight 10
#define RegionRobotLeft  11
#define RegionRobotDown  12
#define RegionRobotUp    13

// movement speed of our robot
#define RobotSpeed 2


// -----------------------------------------------
// MAIN FUNCTION
// -----------------------------------------------

void main( void )
{
    // -----------------------------------------------
    // PART 1: DEFINE OUR TEXTURE REGIONS
    
    // ROM texture numbers begin from 0, so select
    // our texture (the only one present in our ROM)
    select_texture( 0 );
    
    // within that texture, define our fixed background
    // region that will be drawn always at pixel (0,0)
    select_region( RegionBackground );
    define_region_topleft( 0,0,  639,359 );
    
    // we also have a 4x1 matrix of robot regions
    // that we will define using consecutive IDs
    // and with hotspot near the bottom center
    define_region_matrix( RegionRobotRight,  1,361,  66,441,  33,417,  4,1,  1 );
    
    // -----------------------------------------------
    // PART 2: INITIALIZATIONS
    
    // our robot starts at the screen center
    int RobotX = screen_width / 2;
    int RobotY = screen_height / 2;
    
    // our robot starts looking down
    int RobotImage = RegionRobotDown;
    
    // we will control the robot with player 1
    select_gamepad( 0 );
    
    // -----------------------------------------------
    // PART 3: MAIN LOOP
    
    // our game never ends: keep repeating
    // our game logic for every frame (60 fps)
    while( true )
    {
        // draw our background to fill the screen
        select_region( RegionBackground );
        draw_region_at( 0, 0 );
        
        // we want to know the direction being
        // pressed by the player; the function
        // used for that takes as parameters
        // our variables and uses them to store
        // is outputs; that way it is able to
        // return 2 values
        int DirectionX, DirectionY;
        gamepad_direction( &DirectionX, &DirectionY );
        
        // now make the robot move in that direction
        RobotX += RobotSpeed * DirectionX;
        RobotY += RobotSpeed * DirectionY;
        
        // our robot can move in 8 directions, but
        // we have only drawn the robot facing in 4
        // directions (and not the diagonals). So
        // for those cases we draw the robot facing
        // either left or right
        if( DirectionX > 0 )
          RobotImage = RegionRobotRight;
      
        else if( DirectionX < 0 )
          RobotImage = RegionRobotLeft;
        
        else if( DirectionY < 0 )
          RobotImage = RegionRobotUp;
          
        else if( DirectionY > 0 )
          RobotImage = RegionRobotDown;
        
        // our robot has now been updated so
        // just draw it in its current state
        select_region( RobotImage );
        draw_region_at( RobotX, RobotY );
        
        // we have ended this frame, so wait for
        // the next in order to control game speed
        end_frame();
    }
}