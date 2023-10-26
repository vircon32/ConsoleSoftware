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
embedded int[ 9 ][ 16 ] Level1 = "obj/Level.vmap";


// ---------------------------------------------------------
//   INITIALIZE LEVEL DATA
// ---------------------------------------------------------


void CreateGameLevel()
{
    GameLevel.TilesInX = 16;
    GameLevel.TilesInY = 9;
    GameLevel.MapArray = &Level1[ 0 ][ 0 ];
}
