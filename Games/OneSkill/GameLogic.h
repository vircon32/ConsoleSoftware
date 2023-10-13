// *****************************************************************************
    // start include guard
    #ifndef GAMELOGIC_H
    #define GAMELOGIC_H
    
    // include project headers
    #include "Player.h"
    #include "RoomMap.h"
    
    // now include all objects used in levels
    // (all these use the base headers and the player)
    // order has to be based on their interdependencies
    #include "levelobjects/Highlight.h"
    #include "levelobjects/SkillPost.h"
    #include "levelobjects/ExitDoor.h"
    #include "levelobjects/EntryDoor.h"
    #include "levelobjects/Spike.h"
    #include "levelobjects/PistolShot.h"
    /*
    #include "levelobjects/TimedSpike.h"
    #include "levelobjects/Golem.h"
    #include "levelobjects/Bird.h"
    #include "levelobjects/Crate.h"
    #include "levelobjects/OnOffBlock.h"
    */
// *****************************************************************************


// ---------------------------------------------------------
//   GAMEPLAY OBJECTS (NEEDED BY LEVEL)
// ---------------------------------------------------------


// unique level objects
RoomMap CurrentRoomMap;
Player Player1;
EntryDoor RoomEntry;
ExitDoor RoomExit;

// arrays of level objects
SkillPost[ 10 ] SkillPosts;
int ExistingSkillPosts;

Spike[ 30 ] Spikes;
int ExistingSpikes;

/*
Coin[ 9 ] Coins;
int ExistingCoins;

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
