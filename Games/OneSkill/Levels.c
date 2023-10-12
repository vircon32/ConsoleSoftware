// *****************************************************************************
    // include project headers
    #include "Levels.h"
// *****************************************************************************


// ---------------------------------------------------------
//   DATA FOR ALL LEVELS
// ---------------------------------------------------------

// levels are read from external files, and then
// stored as a series of arrays in the cartridge;
// these arrays have the form [ height ][ width ]
// in order for the levels to be stored in the
// usual order (left to right and top to bottom)
embedded int[ 18 ][ 32 ] Level1_1 = "obj/Level1-1.vmap";
embedded int[  9 ][ 16 ] Level1_2 = "obj/Level1-2.vmap";
embedded int[  9 ][ 16 ] Level2_1 = "obj/Level2-1.vmap";
embedded int[  9 ][ 16 ] Level2_2 = "obj/Level2-2.vmap";


// ---------------------------------------------------------
//   INITIALIZE LEVEL DATA
// ---------------------------------------------------------


void CreateGameLevels()
{
    GameLevels[ 0 ].Rooms[ 0 ].TilesInX = 32;
    GameLevels[ 0 ].Rooms[ 0 ].TilesInY = 18;
    GameLevels[ 0 ].Rooms[ 0 ].MapArray = &Level1_1[ 0 ][ 0 ];
    
    GameLevels[ 0 ].Rooms[ 1 ].TilesInX = 16;
    GameLevels[ 0 ].Rooms[ 1 ].TilesInY = 9;
    GameLevels[ 0 ].Rooms[ 1 ].MapArray = &Level1_2[ 0 ][ 0 ];
    
    GameLevels[ 1 ].Rooms[ 0 ].TilesInX = 16;
    GameLevels[ 1 ].Rooms[ 0 ].TilesInY = 9;
    GameLevels[ 1 ].Rooms[ 0 ].MapArray = &Level2_1[ 0 ][ 0 ];
    
    GameLevels[ 1 ].Rooms[ 1 ].TilesInX = 16;
    GameLevels[ 1 ].Rooms[ 1 ].TilesInY = 9;
    GameLevels[ 1 ].Rooms[ 1 ].MapArray = &Level2_2[ 0 ][ 0 ];
}
