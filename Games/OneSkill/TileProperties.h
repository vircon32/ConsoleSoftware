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
    Tile_Box = 22,
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

bool TileHasNoBoundaries( int Tile )
{
    return (Tile == Tile_Empty || Tile >= Tile_OffBlock );
}

// ---------------------------------------------------------

bool TileHasLeftBoundary( int Tile )
{
    return (Tile > Tile_Empty && Tile <= Tile_TimedSpikes);
}

// ---------------------------------------------------------

bool TileHasRightBoundary( int Tile )
{
    return (Tile > Tile_Empty && Tile <= Tile_TimedSpikes);
}

// ---------------------------------------------------------

bool TileHasTopBoundary( int Tile )
{
    // all spikes will let the player sink in and get
    // damaged unless they are currently wearing the boots
    if( Tile == Tile_Spikes || Tile == Tile_TimedSpikes )
      return (PlayerSkill == Skills_Boots);
    
    return (Tile > Tile_Empty && Tile <= Tile_TimedSpikes);
}

// ---------------------------------------------------------

bool TileHasBottomBoundary( int Tile )
{
    return (Tile > Tile_Empty && Tile <= Tile_TimedSpikes);
}
