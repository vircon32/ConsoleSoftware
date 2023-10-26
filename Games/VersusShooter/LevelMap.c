// *****************************************************************************
    // include additional libraries
    #include "tilemap.h"
    
    // include project headers
    #include "LevelMap.h"
    #include "Definitions.h"
    #include "Globals.h"
    #include "Box.h"
// *****************************************************************************


// ---------------------------------------------------------
//   LEVEL MAP HANDLING FUNCTIONS
// ---------------------------------------------------------


void LevelMap_Load( LevelMap* L, int* TileMap, int TilesInX, int TilesInY )
{
    // first, clear any previous room
    memset( L->Tiles, 0, MaxTilesInX * MaxTilesInY );
    
    // copy all tiles
    for( int TileY = 0; TileY < TilesInY; TileY++ )
      for( int TileX = 0; TileX < TilesInX; TileX++ )
        L->Tiles[ TileY ][ TileX ] = *(TileMap++);
    
    // finally, set room dimensions
    L->TilesInX = TilesInX;
    L->TilesInY = TilesInY;
}

// ---------------------------------------------------------

void LevelMap_PositionCamera( LevelMap* L )
{
    // focus camera at player center, and clip to map
    MapLevel.camera_x = screen_width / 2;
    MapLevel.camera_y = screen_height / 2;
    tilemap_clip_camera_position( &MapLevel );
    
    L->CameraPosition.x = MapLevel.camera_x;
    L->CameraPosition.y = MapLevel.camera_y;
    
    // now, from camera position, calculate map position in screen
    // (this is used to draw other objects in the room)
    L->TopLeftOnScreen.x = -L->CameraPosition.x + screen_width  / 2;
    L->TopLeftOnScreen.y = -L->CameraPosition.y + screen_height / 2;
}

// ---------------------------------------------------------

void LevelMap_Draw()
{
    tilemap_draw( &MapLevel );
}
