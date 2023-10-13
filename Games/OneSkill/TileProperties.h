// *****************************************************************************
    // start include guard
    #ifndef TILEPROPERTIES_H
    #define TILEPROPERTIES_H
// *****************************************************************************


// ---------------------------------------------------------
//   PROPERTIES OF THE LEVEL TILESET
// ---------------------------------------------------------


// basic, global definitions
#define MaxTilesInX (16*6)
#define MaxTilesInY  (9*6)
#define TileWidth  40
#define TileHeight 40
#define BackgroundTileWidth 160

// define the different tile types
// (only the ones we care about)
enum TileTypes
{
    Tile_Empty = 0,
    Tile_Crate = 22,
    Tile_OnBlock = 33,
    Tile_Spikes = 44,
    Tile_TimedSpikes,
    Tile_OffBlock,
    Tile_PlayerStart,
    Tile_Door,
    Tile_SkillPost,
    Tile_Golem,
    Tile_Bird
};


// ---------------------------------------------------------
//   PROPERTIES FOR EACH TILE TYPE
// ---------------------------------------------------------


bool TileHasNoBoundaries( int Tile );
bool TileHasLeftBoundary( int Tile );
bool TileHasRightBoundary( int Tile );
bool TileHasTopBoundary( int Tile );
bool TileHasBottomBoundary( int Tile );


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
