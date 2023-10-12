// *****************************************************************************
    // start include guard
    #ifndef ROOMMAP_H
    #define ROOMMAP_H
    
    // include project headers
    #include "TileProperties.h"
// *****************************************************************************


// ---------------------------------------------------------
//   STRUCTURE FOR A LEVEL ROOM MAP
// ---------------------------------------------------------


// This represents not a whole room, but only its map
// and spatial/graphical properties. Objects within the
// room are treated separately to avoid dependencies.
// This object does not evolve but is static (save for
// (camera positioning), that is why Reset and Update
// functions are not necessary for RoomMap.
struct RoomMap
{
    // map data
    int[ MaxTilesInY ][ MaxTilesInX ] Tiles;
    int TilesInX, TilesInY;
    
    // additional room configuration
    // for background graphics
    int BackgroundColor;
    int MountainsMultiplyColor;
    int CloudsMultiplyColor;
    
    // camera positioning
    Vector2D CameraPosition;
    Vector2D TopLeftOnScreen;
};


// ---------------------------------------------------------
//   ROOM MAP HANDLING FUNCTIONS
// ---------------------------------------------------------


void RoomMap_Load( RoomMap* R, int* TileMap, int TilesInX, int TilesInY );
void RoomMap_PositionCamera( RoomMap* R );
void RoomMap_Draw( RoomMap* R );


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
