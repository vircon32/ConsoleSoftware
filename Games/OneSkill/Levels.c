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
embedded int[ 12 ][ 64 ] Level1_1 = "obj/Level1-1.vmap";
embedded int[ 25 ][ 53 ] Level1_2 = "obj/Level1-2.vmap";
embedded int[ 28 ][ 50 ] Level2_1 = "obj/Level2-1.vmap";
embedded int[ 12 ][ 80 ] Level2_2 = "obj/Level2-2.vmap";
embedded int[ 12 ][ 50 ] Level3_1 = "obj/Level3-1.vmap";
embedded int[ 27 ][ 40 ] Level3_2 = "obj/Level3-2.vmap";
embedded int[ 12 ][ 82 ] Level4_1 = "obj/Level4-1.vmap";
embedded int[ 32 ][ 45 ] Level4_2 = "obj/Level4-2.vmap";


// ---------------------------------------------------------
//   INITIALIZE LEVEL DATA
// ---------------------------------------------------------


void CreateGameLevels()
{
    GameLevels[ 0 ].Rooms[ 0 ].TilesInX = 64;
    GameLevels[ 0 ].Rooms[ 0 ].TilesInY = 12;
    GameLevels[ 0 ].Rooms[ 0 ].MapArray = &Level1_1[ 0 ][ 0 ];
    
    GameLevels[ 0 ].Rooms[ 1 ].TilesInX = 53;
    GameLevels[ 0 ].Rooms[ 1 ].TilesInY = 25;
    GameLevels[ 0 ].Rooms[ 1 ].MapArray = &Level1_2[ 0 ][ 0 ];
    
    GameLevels[ 1 ].Rooms[ 0 ].TilesInX = 50;
    GameLevels[ 1 ].Rooms[ 0 ].TilesInY = 28;
    GameLevels[ 1 ].Rooms[ 0 ].MapArray = &Level2_1[ 0 ][ 0 ];
    
    GameLevels[ 1 ].Rooms[ 1 ].TilesInX = 80;
    GameLevels[ 1 ].Rooms[ 1 ].TilesInY = 12;
    GameLevels[ 1 ].Rooms[ 1 ].MapArray = &Level2_2[ 0 ][ 0 ];
    
    GameLevels[ 2 ].Rooms[ 0 ].TilesInX = 50;
    GameLevels[ 2 ].Rooms[ 0 ].TilesInY = 12;
    GameLevels[ 2 ].Rooms[ 0 ].MapArray = &Level3_1[ 0 ][ 0 ];
    
    GameLevels[ 2 ].Rooms[ 1 ].TilesInX = 40;
    GameLevels[ 2 ].Rooms[ 1 ].TilesInY = 27;
    GameLevels[ 2 ].Rooms[ 1 ].MapArray = &Level3_2[ 0 ][ 0 ];
    
    GameLevels[ 3 ].Rooms[ 0 ].TilesInX = 82;
    GameLevels[ 3 ].Rooms[ 0 ].TilesInY = 12;
    GameLevels[ 3 ].Rooms[ 0 ].MapArray = &Level4_1[ 0 ][ 0 ];
    
    GameLevels[ 3 ].Rooms[ 1 ].TilesInX = 45;
    GameLevels[ 3 ].Rooms[ 1 ].TilesInY = 32;
    GameLevels[ 3 ].Rooms[ 1 ].MapArray = &Level4_2[ 0 ][ 0 ];
}
