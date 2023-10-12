// *****************************************************************************
    // include project headers
    #include "GameLogic.h"
    #include "TileProperties.h"
    #include "Box.h"
    #include "Physics.h"
    #include "Globals.h"
// *****************************************************************************


// ---------------------------------------------------------
//   LEVEL FUNCTIONS
// ---------------------------------------------------------


void LoadRoom( Room* R )
{
    // read level data
    CurrentRoomMap.TilesInX = R->TilesInX;
    CurrentRoomMap.TilesInY = R->TilesInY;
    MapLevel.map_width = R->TilesInX;
    MapLevel.map_height = R->TilesInY;
    
    // initialize all object counts to zero
    ExistingSkillPosts = 0;
    
    /*
    ExistingCoins = 0;
    ExistingSpikes = 0;
    ExistingKeys = 0;
    ExistingDoors = 0;
    ExistingMorningStars = 0;
    ExistingOnOffBlocks = 0;
    ExistingConveyors = 0;
    ExistingSprings = 0;
    */
    
    // copy the room map in memory
    RoomMap_Load( &CurrentRoomMap, R->MapArray, CurrentRoomMap.TilesInX, CurrentRoomMap.TilesInY );
    
    // process every tile
    for( int TileY = 0; TileY < CurrentRoomMap.TilesInY; TileY++ )
    for( int TileX = 0; TileX < CurrentRoomMap.TilesInX; TileX++ )
    {
        int* TilePointer = &CurrentRoomMap.Tiles[ TileY ][ TileX ];
        int TileValue = *TilePointer;
        
        // set the player start position
        if( TileValue == Tile_PlayerStart )
        {
            *TilePointer = Tile_Empty;
            Player_Create( &Player1, TileX, TileY );
        }
        
        // create a skill post
        else if( TileValue == Tile_SkillPost )
        {
            *TilePointer = Tile_Empty;
            SkillPost_Create( &SkillPosts[ ExistingSkillPosts ], TileX, TileY );
            ExistingSkillPosts++;
        }
        
        /*
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
            Goal_Create( &RoomGoal, TileX, TileY );
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
        */
    }
    
    // configure background rendering
    CurrentRoomMap.BackgroundColor = make_color_rgb( 203, 219, 252 );
    CurrentRoomMap.MountainsMultiplyColor = color_white;
    CurrentRoomMap.CloudsMultiplyColor = color_white;
}

// ---------------------------------------------------------

void ResetRoom()
{
    // reset single objects
    Player_Reset( &Player1 );
    //Goal_Reset( &RoomGoal );
    
    // reset object lists
    for( int i = 0; i < ExistingSkillPosts; i++ )
      SkillPost_Reset( &SkillPosts[ i ] );
  
    /*
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
    */
}


// ---------------------------------------------------------
//   LEVEL GAMEPLAY LOGIC
// ---------------------------------------------------------


void UpdateRoom()
{
    for( int i = 0; i < ExistingSkillPosts; i++ )
      SkillPost_Update( &SkillPosts[ i ], &Player1 );
    
    /*
    Goal_Update( &RoomGoal, &Player1 );
    
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
    */
}
