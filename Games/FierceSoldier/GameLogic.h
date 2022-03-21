// include base gameplay headers, that
// don't depend on other definitions
#include "TileProperties.h"
#include "Box.h"
#include "LevelMap.h"

// include player, the central game object,
// as well as its physics based interactions
#include "levelobjects/PlayerShot.h"
#include "levelobjects/EnemyShot.h"
#include "levelobjects/CannonBullet.h"
#include "levelobjects/Missile.h"
#include "Player.h"
#include "Physics.h"

// declare level and player here so that
// level objects can access them
LevelMap CurrentLevelMap;
Player Player1;

// now include all objects used in levels
// (all these use the base headers and the player)
// order has to be based on their interdependencies
#include "levelobjects/Highlight.h"
#include "levelobjects/Soldier.h"
#include "levelobjects/Turret.h"
#include "levelobjects/Launcher.h"
#include "levelobjects/Boss1.h"
#include "levelobjects/Boss2.h"
#include "levelobjects/Boss3.h"


// ---------------------------------------------------------
//   GAMEPLAY OBJECTS (NEEDED BY LEVEL)
// ---------------------------------------------------------


// arrays of level objects
Soldier[ 15 ] Soldiers;
int ExistingSoldiers;

Launcher[ 10 ] Launchers;
int ExistingLaunchers;

Turret[ 10 ] Turrets;
int ExistingTurrets;

// level bosses
Boss1 Level1Boss;
Boss2 Level2Boss;
Boss3 Level3Boss;


// ---------------------------------------------------------
//   LEVEL FUNCTIONS
// ---------------------------------------------------------


void LoadLevel( Level* L )
{
    // read level data
    CurrentLevelMap.TilesInX = L->TilesInX;
    CurrentLevelMap.TilesInY = L->TilesInY;
    
    // initialize all object counts to zero
    ExistingSoldiers = 0;
    ExistingTurrets = 0;
    ExistingLaunchers = 0;
    
    // copy the level map in memory
    LevelMap_Load( &CurrentLevelMap, L->MapArray, CurrentLevelMap.TilesInX, CurrentLevelMap.TilesInY );
    
    // process every event tile
    for( int TileY = 0; TileY < CurrentLevelMap.TilesInY; TileY++ )
    for( int TileX = 0; TileX < CurrentLevelMap.TilesInX; TileX++ )
    {
        int TileValue = L->EventArray[ TileY*CurrentLevelMap.TilesInX + TileX ];
        
        // set the player start position
        if( TileValue == Tile_PlayerStart )
          Player_Create( &Player1, TileX, TileY );
        
        // create an enemy soldier
        if( TileValue == Tile_Soldier )
        {
            Soldier_Create( &Soldiers[ ExistingSoldiers ], TileX, TileY );
            ExistingSoldiers++;
        }
        
        // create a rotating turret
        if( TileValue == Tile_Turret )
        {
            Turret_Create( &Turrets[ ExistingTurrets ], TileX, TileY );
            ExistingTurrets++;
        }
        
        // create a missile launcher
        if( TileValue == Tile_Launcher )
        {
            Launcher_Create( &Launchers[ ExistingLaunchers ], TileX, TileY );
            ExistingLaunchers++;
        }
        
        // create level bosses
        if( TileValue == Tile_Boss1 )
          Boss1_Create( &Level1Boss, TileX, TileY );
          
        if( TileValue == Tile_Boss2 )
          Boss2_Create( &Level2Boss, TileX, TileY );
          
        if( TileValue == Tile_Boss3 )
          Boss3_Create( &Level3Boss, TileX, TileY );
    }
    
    // set level background
    CurrentLevelMap.BackgroundTexture = TextureBGLevel1 + LevelNumber - 1;
}

// ---------------------------------------------------------

void ResetLevel()
{
    // reset the map itself
    LevelMap_Reset( &CurrentLevelMap );
    
    // reset single objects
    Player_Reset( &Player1 );
    
    // reset object lists
    for( int i = 0; i < ExistingSoldiers; i++ )
      Soldier_Reset( &Soldiers[ i ] );
    
    for( int i = 0; i < ExistingTurrets; i++ )
      Turret_Reset( &Turrets[ i ] );
      
    for( int i = 0; i < ExistingLaunchers; i++ )
      Launcher_Reset( &Launchers[ i ] );
      
    // disable all highlights
    for( int i = 0; i < 5; i++ )
      Highlight_Reset( &Highlights[ i ] );
    
    // disable all shots
    for( int i = 0; i < 10; i++ )
      PlayerShot_Reset( &PlayerShots[ i ] );
      
    for( int i = 0; i < 15; i++ )
      EnemyShot_Reset( &EnemyShots[ i ] );
    
    for( int i = 0; i < 10; i++ )
      CannonBullet_Reset( &CannonBullets[ i ] );
    
    for( int i = 0; i < 10; i++ )
      Missile_Reset( &Missiles[ i ] );
    
    // reset bosses
    switch( LevelNumber )
    {
        case 1:
            Boss1_Reset( &Level1Boss );
            break;
        case 2:
            Boss2_Reset( &Level2Boss );
            break;
        case 3:
            Boss3_Reset( &Level3Boss );
            break;
    }
}


// ---------------------------------------------------------
//   LEVEL GAMEPLAY LOGIC
// ---------------------------------------------------------


void UpdateLevel()
{
    // update object lists
    for( int i = 0; i < ExistingSoldiers; i++ )
      Soldier_Update( &Soldiers[ i ] );
      
    for( int i = 0; i < ExistingTurrets; i++ )
      Turret_Update( &Turrets[ i ] );
      
    for( int i = 0; i < ExistingLaunchers; i++ )
      Launcher_Update( &Launchers[ i ] );
    
    // update temporary objects
    for( int i = 0; i < 5; i++ )
      Highlight_Update( &Highlights[ i ] );
  
    for( int i = 0; i < 10; i++ )
      PlayerShot_Update( &PlayerShots[ i ] );
  
    for( int i = 0; i < 15; i++ )
      EnemyShot_Update( &EnemyShots[ i ] );
    
    for( int i = 0; i < 10; i++ )
      CannonBullet_Update( &CannonBullets[ i ] );
    
    for( int i = 0; i < 10; i++ )
      Missile_Update( &Missiles[ i ] );
    
    // update bosses
    switch( LevelNumber )
    {
        case 1:
            Boss1_Update( &Level1Boss );
            break;
        case 2:
            Boss2_Update( &Level2Boss );
            break;
        case 3:
            Boss3_Update( &Level3Boss );
            break;
    }
}
