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
    #include "levelobjects/Golem.h"
    #include "levelobjects/Bird.h"
    //#include "levelobjects/TimedSpike.h"
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

Golem[ 10 ] Golems;
int ExistingGolems;

Bird[ 10 ] Birds;
int ExistingBirds;


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
