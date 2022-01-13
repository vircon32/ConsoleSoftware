// include Vircon headers
#include "video.h"
#include "audio.h"
#include "input.h"
#include "time.h"
#include "memcard.h"
#include "misc.h"
#include "string.h"

// include project headers
#include "GameDefinitions.h"
#include "GameScene.h"
#include "LoadAndSave.h"


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main()
{
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    select_texture( TextureGame );
    
    // our 3 tiles
    define_region_matrix( RegionTile,  277,1,  356,80,  277,1,  3,1,  1 );
    
    // the dog house
    select_region( RegionDogHouse );
    define_region_topleft( 277,82,  418,144 );
    
    // the 4 button actions
    define_region_matrix( RegionLoad,  420,82,  506,101,  420,82,  1,4,  1 );
    
    // load/save window background
    select_region( RegionWindow );
    define_region_topleft( 1,1,  275,127 );
    
    // window content areas
    select_region( RegionContentsText );
    define_region_topleft( 1,129,  134,209 );

    select_region( RegionContentsInfo );
    define_region_topleft( 136,129,  268,209 );
    
    // ------------------------------------
    // PART 2: INITIALIZATIONS
    // ------------------------------------
    
    // global program configuration
    select_gamepad( 0 );
    srand( get_time() );
    
    // create a game signature
    memset( &GameSignature, 0, sizeof( game_signature ) );
    strcpy( GameSignature, "MEMORYCARDTEST" );
    
    // set initial scene values
    ResetGameScene();
    
    // ------------------------------------
    // PART 3: MAIN LOOP
    // ------------------------------------
    
    while( true )
    {
        // process movement
        if( gamepad_left() == 1 )
          GameState.PlayerPosition.x--;
        
        if( gamepad_right() == 1 )
          GameState.PlayerPosition.x++;
        
        if( gamepad_up() == 1 )
          GameState.PlayerPosition.y--;
        
        if( gamepad_down() == 1 )
          GameState.PlayerPosition.y++;
        
        // limit movement to game field
        clamp( &GameState.PlayerPosition.x, 0, TilesInX-1 );
        clamp( &GameState.PlayerPosition.y, 0, TilesInY-1 );
        
        // pick item when positions match
        if( GameState.PlayerPosition == GameState.ItemPosition )
        {
            GameState.Score++;
            CreateNewItem();
        }
        
        // process buttons for load/save
        if( gamepad_button_a() == 1 )
          LoadGameScene();
        
        else if( gamepad_button_b() == 1 )
          SaveGameScene();
        
        // refresh the screen and end
        DrawGameScene();
        end_frame();
    }
}