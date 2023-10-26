// *****************************************************************************
    // include project headers
    #include "GameLogic.h"
    #include "TileProperties.h"
    #include "Box.h"
    #include "Physics.h"
    #include "Globals.h"
// *****************************************************************************


// ---------------------------------------------------------
//   LEVEL FUNCTIONS
// ---------------------------------------------------------


void LoadLevel( Level* L )
{
    // read level data
    CurrentLevelMap.TilesInX = L->TilesInX;
    CurrentLevelMap.TilesInY = L->TilesInY;
    MapLevel.map_width = L->TilesInX;
    MapLevel.map_height = L->TilesInY;
    
    // copy the level map in memory
    LevelMap_Load( &CurrentLevelMap, L->MapArray, CurrentLevelMap.TilesInX, CurrentLevelMap.TilesInY );
    
    // process every tile
    for( int TileY = 0; TileY < CurrentLevelMap.TilesInY; TileY++ )
    for( int TileX = 0; TileX < CurrentLevelMap.TilesInX; TileX++ )
    {
        int* TilePointer = &CurrentLevelMap.Tiles[ TileY ][ TileX ];
        int TileValue = *TilePointer;
        
        // set start position for all players
        if( TileValue >= Tile_Player1Start && TileValue <= Tile_Player4Start )
        {
            int PlayerID = TileValue - Tile_Player1Start;
            Player_Create( &Players[ PlayerID ], TileX, TileY, PlayerID );
            *TilePointer = Tile_Empty;
        }
    }
}

// ---------------------------------------------------------

void ResetLevel()
{
    // reset single objects
    for( int i = 0; i < 4; i++ )
      Player_Reset( &Players[ i ] );

    // disable all tank shots
    for( int i = 0; i < MAX_TANKSHOTS; i++ )
      TankShot_Reset( &TankShots[ i ] );
    
    // disable all highlights
    for( int i = 0; i < MAX_HIGHLIGHTS; i++ )
      Highlight_Reset( &Highlights[ i ] );
}


// ---------------------------------------------------------
//   LEVEL GAMEPLAY LOGIC
// ---------------------------------------------------------


void UpdateLevel()
{
    for( int i = 0; i < MAX_TANKSHOTS; i++ )
      TankShot_Update( &TankShots[ i ] );
    
    for( int i = 0; i < MAX_HIGHLIGHTS; i++ )
      Highlight_Update( &Highlights[ i ] );
}
