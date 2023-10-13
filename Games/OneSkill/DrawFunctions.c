// *****************************************************************************
    // include project headers
    #include "DrawFunctions.h"
    #include "RoomMap.h"
    #include "Player.h"
    #include "Globals.h"
// *****************************************************************************


// ---------------------------------------------------------
//   COMPLETE ROOM DRAWING FUNCTION
// ---------------------------------------------------------


void DrawCurrentRoom()
{
    RoomMap_Draw( &CurrentRoomMap );
    
    for( int i = 0; i < ExistingSkillPosts; i++ )
      SkillPost_Draw( &SkillPosts[ i ], &CurrentRoomMap.TopLeftOnScreen );
    
    EntryDoor_Draw( &RoomEntry, &CurrentRoomMap.TopLeftOnScreen );
    ExitDoor_Draw( &RoomExit, &CurrentRoomMap.TopLeftOnScreen );
    
    /*
    for( int i = 0; i < ExistingCoins; i++ )
      Coin_Draw( &Coins[ i ], &CurrentRoomMap.TopLeftOnScreen );
    
    for( int i = 0; i < ExistingConveyors; i++ )
      Conveyor_Draw( &Conveyors[ i ], &CurrentRoomMap.TopLeftOnScreen );
    
    for( int i = 0; i < ExistingSprings; i++ )
      Spring_Draw( &Springs[ i ], &CurrentRoomMap.TopLeftOnScreen );
    
    for( int i = 0; i < ExistingOnOffBlocks; i++ )
      OnOffBlock_Draw( &OnOffBlocks[ i ], &CurrentRoomMap.TopLeftOnScreen );
    */
    
    // the player is drawn over all other objects
    Player_Draw( &Player1, &CurrentRoomMap.TopLeftOnScreen );
    
    // highlights should appear just below the player
    // but over any background objects
    for( int i = 0; i < 5; i++ )
      Highlight_Draw( &Highlights[ i ], &CurrentRoomMap.TopLeftOnScreen );
}


// ---------------------------------------------------------
//   DRAWING GUI
// ---------------------------------------------------------


void DrawGUI()
{
    if( Player1.Skill <= Skill_None )
      return;
    
    select_texture( TextureGameplay );
    select_region( FirstRegionSkillIcons + (Player1.Skill-1) );
    draw_region_at( 3, 3 );
    
    select_region( FirstRegionSkillNamesGUI + (Player1.Skill-1) );
    draw_region_at( 43, 3 );
}
