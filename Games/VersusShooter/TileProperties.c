// *****************************************************************************
    // include project headers
    #include "TileProperties.h"
    #include "Globals.h"
// *****************************************************************************

// ---------------------------------------------------------
//   PROPERTIES OF THE LEVEL TILESET
// ---------------------------------------------------------


bool TileIsSolid( int Tile )
{
    return (Tile >= Tile_Crate && Tile <= Tile_Stone );
}
