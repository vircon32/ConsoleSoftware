// ---------------------------------------------------------
//   PROPERTIES OF THE LEVEL TILESET
// ---------------------------------------------------------


// basic, global definitions
#define MaxTilesInX (16*6)
#define MaxTilesInY  (9*6)
#define TileWidth  40
#define TileHeight 40

// define the different tile types
enum SpecialTiles
{
    Tile_Empty = 0,
    Tile_PalmTreeTall = 65,
    Tile_PalmTreeShort,
    Tile_BushLarge,
    Tile_BushSmall,
    Tile_PlayerStart,
    Tile_Soldier,
    Tile_Turret,
    Tile_Launcher,
    Tile_EmptyEvent1,
    Tile_EmptyEvent2,
    Tile_Boss1,
    Tile_Boss2,
    Tile_Boss3
};

// ---------------------------------------------------------

bool[ 13 * 6 ] TileHasTopBoundary =
{
    0,1,1,1,1,1,1,1,1,1,1,1,1,
    0,1,1,1,1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,0,0,0,1,1,
    0,1,1,1,1,1,0,1,1,0,0,0,0,
    0,0,0,0,0,0,0,1,1,0,1,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0
};

bool[ 13 * 6 ] TileHasBottomBoundary =
{
    0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,1,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0
};

bool[ 13 * 6 ] TileHasLeftBoundary =
{
    0,0,0,0,1,0,0,0,0,0,0,0,0,
    0,0,0,0,1,0,0,0,0,0,1,0,1,
    0,0,0,0,1,0,0,0,0,0,1,1,0,
    0,0,0,0,1,0,0,0,0,0,1,1,0,
    0,0,0,0,1,0,0,0,0,0,1,0,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0
};

bool[ 13 * 6 ] TileHasRightBoundary =
{
    0,0,0,0,0,0,1,0,0,0,0,0,0,
    0,0,0,0,0,0,1,0,0,0,1,0,1,
    0,0,0,0,0,0,1,0,0,0,1,0,1,
    0,0,0,0,0,1,0,0,0,0,1,0,1,
    0,0,0,0,0,1,0,0,0,0,0,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0
};
