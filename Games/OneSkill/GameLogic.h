// *****************************************************************************
    // start include guard
    #ifndef GAMELOGIC_H
    #define GAMELOGIC_H
    
    // include project headers
    #include "Player.h"
    #include "RoomMap.h"
// *****************************************************************************


// ---------------------------------------------------------
//   GAMEPLAY OBJECTS (NEEDED BY LEVEL)
// ---------------------------------------------------------


// unique level objects
RoomMap CurrentRoomMap;
Player Player1;
//Goal RoomGoal;
/*
// arrays of level objects
Coin[ 9 ] Coins;
int ExistingCoins;

Spike[ 30 ] Spikes;
int ExistingSpikes;

MorningStar[ 40 ] MorningStars;
int ExistingMorningStars;

Key[ 4 ] Keys;
int ExistingKeys;

Door[ 4 ] Doors;
int ExistingDoors;

OnOffBlock[ 40 ] OnOffBlocks;
int ExistingOnOffBlocks;

Spring[ 10 ] Springs;
int ExistingSprings;

Conveyor[ 40 ] Conveyors;
int ExistingConveyors;
*/

// ---------------------------------------------------------
//   LEVEL FUNCTIONS
// ---------------------------------------------------------


void LoadRoom( Room* R );
void ResetRoom();


// ---------------------------------------------------------
//   LEVEL GAMEPLAY LOGIC
// ---------------------------------------------------------


void UpdateRoom();


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
