// include Vircon libraries
#include "audio.h"
#include "video.h"
#include "input.h"
#include "time.h"
#include "math.h"
#include "misc.h"


// -----------------------------------------------
// DEFINITIONS FOR TEXTURES
// -----------------------------------------------


// give names to each texture in our cartridge
//


// give names to our regions
// within each texture
//


// -----------------------------------------------
// DEFINITIONS FOR SOUNDS
// -----------------------------------------------


// give names to each sound in our cartridge
//



// -----------------------------------------------
// DEFINITIONS FOR GAMEPLAY
// -----------------------------------------------


// define game objects
// set movement speeds


// -----------------------------------------------
// MAIN FUNCTION
// -----------------------------------------------


void main( void )
{
    // -----------------------------------------------
    // PART 1: DEFINE OUR TEXTURE REGIONS
    
    
    // (to do)
    
    
    // ---------------------------------------
    // PART 2: CONFIGURE SOUNDS AND CHANNELS
    
    
    // (to do)
    
    
    // -----------------------------------------------
    // PART 3: GAME INITIALIZATIONS
    
    // we will control the ship with gamepad 1
    select_gamepad( 0 );
    
    // make random numbers different each time
    srand( get_time() );
    
    // set the initial state of the game
    //
    
    // begin playing music
    //
    
    // -----------------------------------------------
    // PART 4: MAIN LOOP
    
    // our game never ends: keep repeating
    // our game logic for every frame (60 fps)
    while( true )
    {
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 1: Read player inputs
        
        // read the direction pressed on the D-Pad
        int DirectionX, DirectionY;
        gamepad_direction( &DirectionX, &DirectionY );        
        
        // check if button A (shot) was just pressed
        bool ShotPressed = (gamepad_button_a() == 1);
        
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 2: Apply game logic for this frame
        

        
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 3: Draw the scene on screen
        

        
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 4: Wait for next frame
        end_frame();
    }
}
