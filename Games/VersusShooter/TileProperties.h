// *****************************************************************************
    // start include guard
    #ifndef TILEPROPERTIES_H
    #define TILEPROPERTIES_H
// *****************************************************************************


// ---------------------------------------------------------
//   PROPERTIES OF THE LEVEL TILESET
// ---------------------------------------------------------


// basic, global definitions
#define MaxTilesInX (16*1)
#define MaxTilesInY  (9*1)
#define TileWidth  40
#define TileHeight 40

// define the different tile types
// (only the ones we care about)
enum TileTypes
{
    Tile_Empty = 0,
    Tile_Crate,
    Tile_Stone,
    Tile_Player1Start,
    Tile_Player2Start,
    Tile_Player3Start,
    Tile_Player4Start
};


// ---------------------------------------------------------
//   PROPERTIES FOR EACH TILE TYPE
// ---------------------------------------------------------


bool TileIsSolid( int Tile );


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
