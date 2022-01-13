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
#define Tile_Empty          0
#define Tile_PlayerStart    1
#define Tile_Goal           2
#define Tile_Coin           3
#define Tile_Spike          4
#define Tile_Key            5
#define Tile_OffBlock       6
#define Tile_Spring         7
#define Tile_MorningStar    8
#define Tile_Door           9
#define Tile_OnBlock        10
#define Tile_Conveyor       11
#define Tile_EarthBottom    12
#define Tile_EarthTop       13
#define Tile_Bricks         14
#define Tile_PurpleBlock    15
#define Tile_Box            16
#define Tile_Bridge         17
#define Tile_OneWayLeft     18
#define Tile_OneWayRight    19

// ---------------------------------------------------------

bool TileHasAllBoundaries( int Tile )
{
    return (Tile >= Tile_Spring && Tile <= Tile_Box);
}

// ---------------------------------------------------------

bool TileHasNoBoundaries( int Tile )
{
    return (Tile < Tile_Spring);
}

// ---------------------------------------------------------

bool TileHasLeftBoundary( int Tile )
{
    return (TileHasAllBoundaries( Tile ) || Tile == Tile_OneWayLeft);
}

// ---------------------------------------------------------

bool TileHasRightBoundary( int Tile )
{
    return (TileHasAllBoundaries( Tile ) || Tile == Tile_OneWayRight);
}

// ---------------------------------------------------------

bool TileHasTopBoundary( int Tile )
{
    return (TileHasAllBoundaries( Tile ) || Tile == Tile_Bridge);
}

// ---------------------------------------------------------

bool TileHasBottomBoundary( int Tile )
{
    return TileHasAllBoundaries( Tile );
}
