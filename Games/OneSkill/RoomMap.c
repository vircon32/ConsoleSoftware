// *****************************************************************************
    // include additional libraries
    #include "tilemap.h"
    
    // include project headers
    #include "RoomMap.h"
    #include "Definitions.h"
    #include "Globals.h"
    #include "Box.h"
// *****************************************************************************


// ---------------------------------------------------------
//   ROOM MAP HANDLING FUNCTIONS
// ---------------------------------------------------------


void RoomMap_Load( RoomMap* R, int* TileMap, int TilesInX, int TilesInY )
{
    // first, clear any previous room
    memset( R->Tiles, 0, MaxTilesInX * MaxTilesInY );
    
    // copy all tiles
    for( int TileY = 0; TileY < TilesInY; TileY++ )
      for( int TileX = 0; TileX < TilesInX; TileX++ )
        R->Tiles[ TileY ][ TileX ] = *(TileMap++);
    
    // finally, set room dimensions
    R->TilesInX = TilesInX;
    R->TilesInY = TilesInY;
}

// ---------------------------------------------------------

void RoomMap_PositionCamera( RoomMap* R )
{
    // focus camera at player center, and clip to map
    MapLevel.camera_x = Player1.ShapeBox.Position.x;
    MapLevel.camera_y = Player1.ShapeBox.Position.y - 18;
    tilemap_clip_camera_position( &MapLevel );
    
    R->CameraPosition.x = MapLevel.camera_x;
    R->CameraPosition.y = MapLevel.camera_y;
    
    // now, from camera position, calculate map position in screen
    // (this is used to draw other objects in the room)
    R->TopLeftOnScreen.x = -R->CameraPosition.x + screen_width  / 2;
    R->TopLeftOnScreen.y = -R->CameraPosition.y + screen_height / 2;
}

// ---------------------------------------------------------

void RoomMap_Draw( RoomMap* R )
{
    // apply a proportion to the scroll speed,
    // taking reference at left in X and bottom in Y
    int OffsetX = MapLevel.camera_x - screen_width/2;
    OffsetX = max( OffsetX, 0 );
    
    int OffsetY = (tilemap_total_height( &MapLevel ) - screen_height/2) - MapLevel.camera_y;
    OffsetY = max( OffsetY, 0 );
    
    MapMountains.camera_x = screen_width/2 + OffsetX * 0.4;
    MapMountains.camera_y = tilemap_total_height( &MapMountains ) - screen_height/2 - OffsetY * 0.4;
    
    MapClouds.camera_x = screen_width/2 + OffsetX * 0.7;
    MapClouds.camera_y = tilemap_total_height( &MapClouds ) - screen_height/2 - OffsetY * 0.7;
    
    // flat background color
    clear_screen( R->BackgroundColor );
    
    // draw our 2 parallax maps
    set_multiply_color( R->MountainsMultiplyColor );
    tilemap_draw( &MapMountains );
    
    set_multiply_color( R->CloudsMultiplyColor );
    tilemap_draw( &MapClouds );
    
    // restore neutral multiply color
    set_multiply_color( color_white );
    
    // finally draw the foreground layer
    tilemap_draw( &MapLevel );
}
