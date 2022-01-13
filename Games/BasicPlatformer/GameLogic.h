// include base gameplay headers, that
// don't depend on other definitions
#include "TileProperties.h"
#include "Box.h"
#include "LevelMap.h"

// include player, the central game object,
// as well as its physics based interactions
#include "Player.h"
#include "Physics.h"

// now include all objects used in levels
// (all these use the base headers and the player)
// order has to be based on their interdependencies
#include "levelobjects/Highlight.h"
#include "levelobjects/Coin.h"
#include "levelobjects/Goal.h"
#include "levelobjects/Spike.h"
#include "levelobjects/MorningStar.h"
#include "levelobjects/Key.h"
#include "levelobjects/Door.h"
#include "levelobjects/OnOffBlock.h"
#include "levelobjects/Spring.h"
#include "levelobjects/Conveyor.h"


// ---------------------------------------------------------
//   GAMEPLAY OBJECTS (NEEDED BY LEVEL)
// ---------------------------------------------------------


// unique level objects
LevelMap CurrentLevelMap;
Player Player1;
Goal LevelGoal;

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

// additional game information
int RemainingTimeFrames;


// ---------------------------------------------------------
//   LEVEL FUNCTIONS
// ---------------------------------------------------------


void LoadLevel( Level* L )
{
    // read level data
    CurrentLevelMap.TilesInX = L->TilesInX;
    CurrentLevelMap.TilesInY = L->TilesInY;
    CurrentLevelMap.TimeLimitFrames = L->TimeInSeconds * frames_per_second;
    
    // initialize all object counts to zero
    ExistingCoins = 0;
    ExistingSpikes = 0;
    ExistingKeys = 0;
    ExistingDoors = 0;
    ExistingMorningStars = 0;
    ExistingOnOffBlocks = 0;
    ExistingConveyors = 0;
    ExistingSprings = 0;
    
    // copy the level map in memory
    LevelMap_Load( &CurrentLevelMap, L->MapArray, CurrentLevelMap.TilesInX, CurrentLevelMap.TilesInY );
    
    // process every tile
    for( int TileY = 0; TileY < CurrentLevelMap.TilesInY; TileY++ )
    for( int TileX = 0; TileX < CurrentLevelMap.TilesInX; TileX++ )
    {
        int* TilePointer = &CurrentLevelMap.Tiles[ TileY ][ TileX ];
        int TileValue = *TilePointer;
        
        // set the player start position
        if( TileValue == Tile_PlayerStart )
        {
            *TilePointer = Tile_Empty;
            Player_Create( &Player1, TileX, TileY );
        }
        
        // create a coin
        else if( TileValue == Tile_Coin )
        {
            *TilePointer = Tile_Empty;
            Coin_Create( &Coins[ ExistingCoins ], TileX, TileY );
            ExistingCoins++;
        }
        
        // position the goal
        else if( TileValue == Tile_Goal )
        {
            Goal_Create( &LevelGoal, TileX, TileY );
        }
        
        else if( TileValue == Tile_Spike )
        {
            Spike_Create( &Spikes[ ExistingSpikes ], TileX, TileY );
            ExistingSpikes++;
        }
        
        else if( TileValue == Tile_MorningStar )
        {
            MorningStar_Create( &MorningStars[ ExistingMorningStars ], TileX, TileY );
            ExistingMorningStars++;
        }
        
        else if( TileValue == Tile_Key )
        {
            Key_Create( &Keys[ ExistingKeys ], TilePointer, TileX, TileY );
            ExistingKeys++;
        }
        
        else if( TileValue == Tile_Door )
        {
            Door_Create( &Doors[ ExistingDoors ], TilePointer, TileX, TileY );
            ExistingDoors++;
        }
        
        else if( TileValue == Tile_OnBlock || TileValue == Tile_OffBlock )
        {
            OnOffBlock_Create( &OnOffBlocks[ ExistingOnOffBlocks ], TilePointer, TileX, TileY );
            ExistingOnOffBlocks++;
        }
        
        else if( TileValue == Tile_Conveyor )
        {
            Conveyor_Create( &Conveyors[ ExistingConveyors ], TileX, TileY );
            ExistingConveyors++;
        }
        
        else if( TileValue == Tile_Spring )
        {
            Spring_Create( &Springs[ ExistingSprings ], TileX, TileY );
            ExistingSprings++;
        }
    }
    
    // configure background rendering
    CurrentLevelMap.GradientRegion   = RegionGradientWorld1   - 1 + WorldNumber;
    CurrentLevelMap.BackgroundRegion = RegionBackgroundWorld1 - 1 + WorldNumber;
    
    // the goal needs to know the number of coins
    LevelGoal.RequiredCoins = ExistingCoins;
}

// ---------------------------------------------------------

void ResetLevel()
{
    // reset single objects
    Player_Reset( &Player1 );
    Goal_Reset( &LevelGoal );
    
    // reset object lists
    for( int i = 0; i < ExistingCoins; i++ )
      Coin_Reset( &Coins[ i ] );
    
    for( int i = 0; i < ExistingMorningStars; i++ )
      MorningStar_Reset( &MorningStars[ i ] );
    
    for( int i = 0; i < ExistingKeys; i++ )
      Key_Reset( &Keys[ i ] );
    
    for( int i = 0; i < ExistingDoors; i++ )
      Door_Reset( &Doors[ i ] );
    
    for( int i = 0; i < ExistingOnOffBlocks; i++ )
      OnOffBlock_Reset( &OnOffBlocks[ i ] );
    
    for( int i = 0; i < ExistingConveyors; i++ )
      Conveyor_Reset( &Conveyors[ i ] );
    
    for( int i = 0; i < ExistingSprings; i++ )
      Spring_Reset( &Springs[ i ] );
    
    // disable all highlights
    for( int i = 0; i < 5; i++ )
      Highlight_Reset( &Highlights[ i ] );
    
    // reset timer
    RemainingTimeFrames = CurrentLevelMap.TimeLimitFrames;
}


// ---------------------------------------------------------
//   LEVEL GAMEPLAY LOGIC
// ---------------------------------------------------------


void UpdateLevel()
{
    Goal_Update( &LevelGoal, &Player1 );
    
    for( int i = 0; i < ExistingCoins; i++ )
      Coin_Update( &Coins[ i ], &Player1 );
    
    for( int i = 0; i < ExistingSpikes; i++ )
      Spike_Update( &Spikes[ i ], &Player1 );
    
    for( int i = 0; i < ExistingMorningStars; i++ )
      MorningStar_Update( &MorningStars[ i ], &Player1 );
    
    for( int i = 0; i < ExistingKeys; i++ )
      Key_Update( &Keys[ i ], &Player1 );
    
    for( int i = 0; i < ExistingDoors; i++ )
      Door_Update( &Doors[ i ], &Player1 );
    
    for( int i = 0; i < ExistingOnOffBlocks; i++ )
      OnOffBlock_Update( &OnOffBlocks[ i ] );
    
    for( int i = 0; i < ExistingSprings; i++ )
      Spring_Update( &Springs[ i ], &Player1 );
      
    for( int i = 0; i < ExistingConveyors; i++ )
      Conveyor_Update( &Conveyors[ i ], &Player1 );
      
    for( int i = 0; i < 5; i++ )
      Highlight_Update( &Highlights[ i ] );
}
