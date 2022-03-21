// ---------------------------------------------------------
//   LEVEL MAP "CLASS"
// ---------------------------------------------------------


// This represents not a whole level, but only its map
// and spatial/graphical properties. Objects within the
// level are treated separately to avoid dependencies.
// This object does not evolve but is static (save for
// (camera positioning), that is why Reset and Update
// methods are not necessary for LevelMap.
struct LevelMap
{
    // map data
    int[ MaxTilesInY ][ MaxTilesInX ] Tiles;
    int TilesInX, TilesInY;
    
    // additional level configuration
    int BackgroundTexture;
    
    // camera positioning
    Vector2D CameraPosition;
    Vector2D TopLeftOnScreen;
    int LeftCameraLimit;
};

// ---------------------------------------------------------

void LevelMap_Load( LevelMap* L, int* TileMap, int TilesInX, int TilesInY )
{
    // first, clear any previous level
    memset( L->Tiles, 0, MaxTilesInX * MaxTilesInY );
    
    // copy all tiles
    for( int TileY = 0; TileY < TilesInY; TileY++ )
      for( int TileX = 0; TileX < TilesInX; TileX++ )
        L->Tiles[ TileY ][ TileX ] = *(TileMap++);
    
    // finally, set level dimensions
    L->TilesInX = TilesInX;
    L->TilesInY = TilesInY;
}

// ---------------------------------------------------------

void LevelMap_Reset( LevelMap* L )
{
    L->LeftCameraLimit = 0;
}

// -----------------------------------------------------------------------------

void LevelMap_PositionCamera( LevelMap* L, Vector2D* TargetPosition )
{
    // determine boundaries for camera position
    // so that level map always fills the screen
    int CameraMinX = screen_width  / 2;
    int CameraMinY = screen_height / 2;
    int CameraMaxX = (TileWidth  * L->TilesInX) - screen_width  / 2;
    int CameraMaxY = (TileHeight * L->TilesInY) - screen_height / 2;
    
    // we will follow the target slightly off to the
    // right to allow more visible space on the front
    L->CameraPosition = *TargetPosition;
    L->CameraPosition.x += 50;
    
    // we will prevent player from moving to the left
    int LeftCameraOffset = screen_width/2 - 50;
    int LeftCameraX = L->CameraPosition.x - LeftCameraOffset;
    
    if( LeftCameraX > L->LeftCameraLimit )
      L->LeftCameraLimit = LeftCameraX;
    
    // at the right end, ensure player can go back to
    // the left edge of the screen
    int MaxLeftLimit = CameraMaxX - screen_width/2;
    
    if( L->LeftCameraLimit > MaxLeftLimit )
      L->LeftCameraLimit = MaxLeftLimit;
        
    // limit camera position so that level map fills the screen
    CameraMinX = max( CameraMinX, L->LeftCameraLimit + screen_width/2 );
    
    if( L->CameraPosition.x < CameraMinX )
      L->CameraPosition.x = CameraMinX;
    
    if( L->CameraPosition.x > CameraMaxX )
      L->CameraPosition.x = CameraMaxX;
    
    if( L->CameraPosition.y < CameraMinY )
      L->CameraPosition.y = CameraMinY;
    
    if( L->CameraPosition.y > CameraMaxY )
      L->CameraPosition.y = CameraMaxY;
    
    // now, from camera position, calculate map position in screen
    L->TopLeftOnScreen.x = -L->CameraPosition.x + screen_width  / 2;
    L->TopLeftOnScreen.y = -L->CameraPosition.y + screen_height / 2;
}

// ---------------------------------------------------------

void LevelMap_DrawBackground( LevelMap* L )
{
    // we will now draw the background tiled in X
    select_texture( L->BackgroundTexture );
    select_region( RegionLevelBackground );
    
    // apply a proportion to the scroll speed,
    // taking reference at left in X
    int CameraMinX = screen_width/2;
    int CameraXOffset = L->CameraPosition.x - CameraMinX;
    
    // calculate map position in screen
    // (only needed along X, since Y is not tiled)
    int SlowCameraX = CameraMinX + CameraXOffset / 2;
    int LevelTopLeftX = -SlowCameraX + screen_width /2;
    
    // draw only within screen limits
    int MinTileX = max( -LevelTopLeftX / screen_width , 0 );
    int MaxTileX = min( (-LevelTopLeftX+screen_width)  / screen_width , L->TilesInX-1 );
    
    int RenderX = LevelTopLeftX + MinTileX * screen_width;
    
    for( int TileX = MinTileX; TileX <= MaxTileX; ++TileX )
    {
        draw_region_at( RenderX, 0 );
        RenderX += screen_width;
    }
}

// ---------------------------------------------------------

void LevelMap_DrawForeground( LevelMap* L )
{
    select_texture( TextureGameplay );
    
    // draw only within screen limits
    // (enlarge in x to account for larger tiles)
    int MinTileX = max( -L->TopLeftOnScreen.x / TileWidth-2, 0 );
    int MinTileY = max( -L->TopLeftOnScreen.y / TileHeight, 0 );
    int MaxTileX = min( (-L->TopLeftOnScreen.x+screen_width)  / TileWidth +2, L->TilesInX-1 );
    int MaxTileY = min( (-L->TopLeftOnScreen.y+screen_height) / TileHeight, L->TilesInY-1 );
    
    // to ensure a known rounding from float, convert
    // coordinates to integer before adding tile dimensions
    // (otherwise objects and scenery could be off by 1 pixel)
    int RenderY = L->TopLeftOnScreen.y;
    RenderY += MinTileY * TileHeight;
    
    // render tile map
    for( int TileY = MinTileY; TileY <= MaxTileY; ++TileY )
    {
        int RenderX = L->TopLeftOnScreen.x;
        RenderX += MinTileX * TileWidth;
        
        int* CurrentTile = &L->Tiles[ TileY ][ MinTileX ];
        
        for( int TileX = MinTileX; TileX <= MaxTileX; ++TileX )
        {
            if( *CurrentTile )
            {
                select_region( FirstRegionTileset + *CurrentTile );
                draw_region_at( RenderX, RenderY );
            }
            
            RenderX += TileWidth;
            ++CurrentTile;
        }
        
        RenderY += TileHeight;
    }
}
