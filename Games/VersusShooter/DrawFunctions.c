// *****************************************************************************
    // include project headers
    #include "DrawFunctions.h"
    #include "LevelMap.h"
    #include "Player.h"
    #include "Globals.h"
// *****************************************************************************


// ---------------------------------------------------------
//   COMPLETE ROOM DRAWING FUNCTION
// ---------------------------------------------------------


void DrawCurrentLevel()
{
    // draw background
    select_texture( TextureGameplay );
    select_region( RegionGround );
    draw_region_at( 0, 0 );
    
    // draw level
    LevelMap_Draw();
    
    // players are drawn over all other objects
    for( int i = 0; i < 4; i++ )
      Player_Draw( &Players[ i ], &CurrentLevelMap.TopLeftOnScreen );
    
    // draw shots over players
    for( int i = 0; i < MAX_TANKSHOTS; i++ )
      TankShot_Draw( &TankShots[ i ], &CurrentLevelMap.TopLeftOnScreen );
    
    // highlights should appear over players
    for( int i = 0; i < MAX_HIGHLIGHTS; i++ )
      Highlight_Draw( &Highlights[ i ], &CurrentLevelMap.TopLeftOnScreen );
}


// ---------------------------------------------------------
//   DRAWING GUI
// ---------------------------------------------------------


void DrawHealthBar( int DrawX, int DrawY, int PlayerID, bool DrawFromRight )
{
    if( DrawFromRight )
    {
        select_region( RegionHealthBarFrame );
        draw_region_at( DrawX - 65, DrawY );
        
        select_region( FirstRegionHealthBars + PlayerID );
        set_drawing_scale( -3 * Players[ PlayerID ].Health, 1 );
        draw_region_zoomed_at( DrawX - 3, DrawY + 3 );
    }
    
    else
    {
        select_region( RegionHealthBarFrame );
        draw_region_at( DrawX, DrawY );
        
        select_region( FirstRegionHealthBars + PlayerID );
        set_drawing_scale( 3 * Players[ PlayerID ].Health, 1 );
        draw_region_zoomed_at( DrawX + 3, DrawY + 3 );
    }
}

// ---------------------------------------------------------

void DrawGUI()
{
    select_texture( TextureGameplay );
    
    // player 1 at top-left corner
    select_region( FirstRegionPlayerIcons );
    draw_region_at( 3, 3 );
    DrawHealthBar( 48, 3, 0, false );
    
    // player 2 at top-right corner
    select_region( FirstRegionPlayerIcons + 1 );
    draw_region_at( 595, 3 );
    DrawHealthBar( 591, 3, 1, true );
    
    // player 3 at bottom-left corner
    select_region( FirstRegionPlayerIcons + 2 );
    draw_region_at( 3, 322 );
    DrawHealthBar( 48, 339, 2, false );
    
    // player 4 at bottom-right corner
    select_region( FirstRegionPlayerIcons + 3 );
    draw_region_at( 595, 322 );
    DrawHealthBar( 591, 339, 3, true );
}
