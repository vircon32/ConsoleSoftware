// *****************************************************************************
    // start include guard
    #ifndef LEVELS_H
    #define LEVELS_H
// *****************************************************************************


// ---------------------------------------------------------
//   DEFINITIONS FOR GAME LEVELS AND THEIR ROOMS
// ---------------------------------------------------------


// a level has the map itself and dimensions
struct Level
{
    int* MapArray;
    int TilesInX, TilesInY;
};

// ---------------------------------------------------------

// the level data itself
Level GameLevel;

// function to initialize level data
void CreateGameLevel();


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
