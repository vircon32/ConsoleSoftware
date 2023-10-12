// *****************************************************************************
    // include project headers
    #include "TileProperties.h"
    #include "Globals.h"
// *****************************************************************************

// ---------------------------------------------------------
//   PROPERTIES OF THE LEVEL TILESET
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
      return (Player1.Skill == Skill_Boots);
    
    return (Tile > Tile_Empty && Tile <= Tile_TimedSpikes);
}

// ---------------------------------------------------------

bool TileHasBottomBoundary( int Tile )
{
    return (Tile > Tile_Empty && Tile <= Tile_TimedSpikes);
}
