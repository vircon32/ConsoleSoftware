// *****************************************************************************
    // start include guard
    #ifndef LEVELS_H
    #define LEVELS_H
// *****************************************************************************


// ---------------------------------------------------------
//   DEFINITIONS FOR GAME LEVELS AND THEIR ROOMS
// ---------------------------------------------------------

// a level is formed by 2 rooms, which each have the map
// itself and dimensions (not all rooms have the same size)
struct Room
{
    int* MapArray;
    int TilesInX, TilesInY;
};

struct Level
{
    Room[ 2 ] Rooms;
};

// ---------------------------------------------------------

// the level data itself
Level[ 2 ] GameLevels;

// function to initialize level data
void CreateGameLevels();


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
