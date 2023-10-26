// *****************************************************************************
    // start include guard
    #ifndef GLOBALS_H
    #define GLOBALS_H
    
    // include additional libraries
    #include "numberfont.h"
    #include "tilemap.h"
// *****************************************************************************


// ---------------------------------------------------------
//   GLOBAL VARIABLES
// ---------------------------------------------------------


// globals for game state
int GameScene;      // scene currently running
int GameState;      // state within that scene

// global objects
numberfont FontNumbers;
tileset TilesLevel;
tilemap MapLevel;


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
