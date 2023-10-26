// *****************************************************************************
    // start include guard
    #ifndef GAMELOGIC_H
    #define GAMELOGIC_H
    
    // include project headers
    #include "Player.h"
    #include "LevelMap.h"
    
    // now include all objects used in levels
    // (all these use the base headers and the player)
    // order has to be based on their interdependencies
    #include "levelobjects/Highlight.h"
    #include "levelobjects/TankShot.h"
// *****************************************************************************


// ---------------------------------------------------------
//   GAMEPLAY OBJECTS (NEEDED BY LEVEL)
// ---------------------------------------------------------


// unique level objects
LevelMap CurrentLevelMap;
Player[ 4 ] Players;


// ---------------------------------------------------------
//   LEVEL FUNCTIONS
// ---------------------------------------------------------


void LoadLevel( Level* L );
void ResetLevel();


// ---------------------------------------------------------
//   LEVEL GAMEPLAY LOGIC
// ---------------------------------------------------------


void UpdateLevel();


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
