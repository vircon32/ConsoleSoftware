// *****************************************************************************
    // start include guard
    #ifndef GLOBALS_H
    #define GLOBALS_H
    
    // include additional libraries
    #include "textfont.h"
    #include "tilemap.h"
// *****************************************************************************


// ---------------------------------------------------------
//   GLOBAL VARIABLES
// ---------------------------------------------------------


// globals for game state
int GameScene;      // scene currently running
int GameState;      // state within that scene

// globals for game session
int LevelNumber;    // starts from 1, not 0
int RoomNumber;     // starts from 1, not 0

// set initial state for the whole game
void ResetGameSession()
{
    LevelNumber = 1;
    RoomNumber = 1;
}

// global objects
textfont Font11x16;
tileset TilesLevel, TilesMountains, TilesClouds;
tilemap MapLevel, MapMountains, MapClouds;

// artificial "map" for cloud and mountain backgrounds
int[ 1 ][ 16 ] MapBackgrounds = {{ 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3 }};

// current wheel center
int SkillWheelCenterX;
int SkillWheelCenterY;


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
