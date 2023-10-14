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
    
    EntryDoor_Draw( &RoomEntry, &CurrentRoomMap.TopLeftOnScreen );
    ExitDoor_Draw( &RoomExit, &CurrentRoomMap.TopLeftOnScreen );
    
    for( int i = 0; i < ExistingSkillPosts; i++ )
      SkillPost_Draw( &SkillPosts[ i ], &CurrentRoomMap.TopLeftOnScreen );
    
    for( int i = 0; i < ExistingTimedSpikes; i++ )
      TimedSpike_Draw( &TimedSpikes[ i ], &CurrentRoomMap.TopLeftOnScreen );
        
    for( int i = 0; i < ExistingGolems; i++ )
      Golem_Draw( &Golems[ i ], &CurrentRoomMap.TopLeftOnScreen );
    
    for( int i = 0; i < ExistingBirds; i++ )
      Bird_Draw( &Birds[ i ], &CurrentRoomMap.TopLeftOnScreen );
    
    for( int i = 0; i < 10; i++ )
      PistolShot_Draw( &PistolShots[ i ], &CurrentRoomMap.TopLeftOnScreen );
    
    // the player is drawn over all other objects
    Player_Draw( &Player1, &CurrentRoomMap.TopLeftOnScreen );
    
    // highlights should appear over the player
    for( int i = 0; i < 10; i++ )
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
