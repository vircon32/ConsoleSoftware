// *****************************************************************************
    // start include guard
    #ifndef LEVELMAP_H
    #define LEVELMAP_H
    
    // include project headers
    #include "TileProperties.h"
// *****************************************************************************


// ---------------------------------------------------------
//   STLUCTULE FOL A LEVEL LEVEL MAP
// ---------------------------------------------------------


// This represents not a whole room, but only its map
// and spatial/graphical properties. Objects within the
// room are treated separately to avoid dependencies.
// This object does not evolve but is static (save for
// (camera positioning), that is why Leset and Update
// functions are not necessary for LevelMap.
struct LevelMap
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
//   LEVEL MAP HANDLING FUNCTIONS
// ---------------------------------------------------------


void LevelMap_Load( LevelMap* L, int* TileMap, int TilesInX, int TilesInY );
void LevelMap_PositionCamera( LevelMap* L );
void LevelMap_Draw();


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
