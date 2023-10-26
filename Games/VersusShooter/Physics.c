// *****************************************************************************
    // include project headers
    #include "Physics.h"
    #include "Player.h"
    #include "LevelMap.h"
// *****************************************************************************


// ---------------------------------------------------------
//   PLAYER COLLISION WITH LEVEL BOUNDARIES
// ---------------------------------------------------------


void EnforceLevelBoundaries( Player* P, LevelMap* L )
{
    // determine level limits
    int LevelMinX = 0;
    int LevelMinY = 0;
    int LevelMaxX = TileWidth  * L->TilesInX;
    int LevelMaxY = TileHeight * L->TilesInY;
    
    // collisions for movement along X
    if( P->ShapeBox.DeltaPosition.x < 0 )
    {
        float PlayerLeft = Box_Left( &P->ShapeBox );
        float PlayerNextLeft = PlayerLeft + P->ShapeBox.DeltaPosition.x;
        
        if( PlayerNextLeft < LevelMinX )
        {
            P->ShapeBox.DeltaPosition.x = (LevelMinX + 0.1) - PlayerLeft;
            P->ShapeBox.Velocity.x = 0;
            P->ShapeBox.Contacts.Left = true;
        }
    }
    
    else if( P->ShapeBox.DeltaPosition.x > 0 )
    {
        float PlayerRight = Box_Right( &P->ShapeBox );
        float PlayerNextRight = PlayerRight + P->ShapeBox.DeltaPosition.x;
        
        if( PlayerNextRight > LevelMaxX )
        {
            P->ShapeBox.DeltaPosition.x = (LevelMaxX - 0.1) - PlayerRight;
            P->ShapeBox.Velocity.x = 0;
            P->ShapeBox.Contacts.Right = true;
        }
    }
    
    // collisions for movement along Y
    if( P->ShapeBox.DeltaPosition.y < 0 )
    {
        float PlayerTop = Box_Top( &P->ShapeBox );
        float PlayerNextTop = PlayerTop + P->ShapeBox.DeltaPosition.y;
        
        if( PlayerNextTop < LevelMinY )
        {
            P->ShapeBox.DeltaPosition.y = (LevelMinY - 0.1) - PlayerTop;
            P->ShapeBox.Velocity.y = 0;
            P->ShapeBox.Contacts.Top = true;
        }
    }
    
    else if( P->ShapeBox.DeltaPosition.y > 0 )
    {
        float PlayerBottom = Box_Bottom( &P->ShapeBox );
        float PlayerNextBottom = PlayerBottom + P->ShapeBox.DeltaPosition.y;
        
        if( PlayerNextBottom > LevelMaxY )
        {
            P->ShapeBox.DeltaPosition.y = (LevelMaxY - 0.1) - PlayerBottom;
            P->ShapeBox.Velocity.y = 0;
            P->ShapeBox.Contacts.Bottom = true;
        }
    }
}


// ---------------------------------------------------------
//   COLLISION FUNCTIONS
// ---------------------------------------------------------
// note that all these depend on the player position,
// because some tiles are semisolids

bool TileLeftIsExposed( LevelMap* L, int TileX, int TileY )
{
    // at level limits, all tile sides are exposed
    if( TileX == 0 ) return true;
    
    // otherwise, analyze the neighbour tile at the left
    int TileAtLeft = L->Tiles[ TileY ][ TileX-1 ];
    return !TileIsSolid( TileAtLeft );
}

// ---------------------------------------------------------

bool TileRightIsExposed( LevelMap* L, int TileX, int TileY )
{
    // at level limits, all tile sides are exposed
    if( TileX == (L->TilesInX-1) ) return true;
    
    // otherwise, analyze the neighbour tile at the right
    int TileAtRight = L->Tiles[ TileY ][ TileX+1 ];
    return !TileIsSolid( TileAtRight );
}

// ---------------------------------------------------------

bool TileTopIsExposed( LevelMap* L, int TileX, int TileY )
{
    // at level limits, all tile sides are exposed
    if( TileY == 0 ) return true;
    
    // otherwise, analyze the neighbour tile at the top
    int TileAtTop = L->Tiles[ TileY-1 ][ TileX ];
    return !TileIsSolid( TileAtTop );
}

// ---------------------------------------------------------

bool TileBottomIsExposed( LevelMap* L, int TileX, int TileY )
{
    // at level limits, all tile sides are exposed
    if( TileY == (L->TilesInY-1) ) return true;
    
    // otherwise, analyze the neighbour tile at the bottom
    int TileAtBottom = L->Tiles[ TileY+1 ][ TileX ];
    return !TileIsSolid( TileAtBottom );
}


// ---------------------------------------------------------
//   PARTIAL PLAYER-TILE COLLISION FUNCTIONS
// ---------------------------------------------------------


void CollidePlayerWithTileLeft( Player* P, int TileX, int TileY )
{
    // applicable only if player moves right
    if( P->ShapeBox.DeltaPosition.x <= 0 )
      return;
    
    // demand non-zero vertical overlap
    // (otherwise we get incorrect corner-corner collisions)
    float PlayerExpandedTop    = Box_ExpandedTop   ( &P->ShapeBox );
    float PlayerExpandedBottom = Box_ExpandedBottom( &P->ShapeBox );
    int TileTop    = TileHeight * TileY;
    int TileBottom = TileHeight * (TileY+1);
    
    if( PlayerExpandedBottom <= TileTop || PlayerExpandedTop >= TileBottom )
      return;
    
    // now collide tile left against player right
    int TileLeft = TileWidth * TileX;
    float PlayerRight = Box_Right( &P->ShapeBox );
    float PlayerNextRight = PlayerRight + P->ShapeBox.DeltaPosition.x;
    
    // demand that the boundary is trespassed in this frame
    if( PlayerRight <= TileLeft && PlayerNextRight > TileLeft )
    {
        P->ShapeBox.DeltaPosition.x = (TileLeft - 0.1) - PlayerRight;   // 0.1 to avoid errors from rounding
        P->ShapeBox.Velocity.x = 0;
        P->ShapeBox.Contacts.Right = true;
    }
}

// ---------------------------------------------------------

void CollidePlayerWithTileRight( Player* P, int TileX, int TileY )
{
    // applicable only if player moves left
    if( P->ShapeBox.DeltaPosition.x >= 0 )
      return;
    
    // demand non-zero vertical overlap
    // (otherwise we get incorrect corner-corner collisions)
    float PlayerExpandedTop    = Box_ExpandedTop   ( &P->ShapeBox );
    float PlayerExpandedBottom = Box_ExpandedBottom( &P->ShapeBox );
    int TileTop    = TileHeight * TileY;
    int TileBottom = TileHeight * (TileY+1);
    
    if( PlayerExpandedBottom <= TileTop || PlayerExpandedTop >= TileBottom )
      return;
    
    // now collide tile right against player left
    int TileRight = TileWidth * (TileX+1);
    float PlayerLeft = Box_Left( &P->ShapeBox );
    float PlayerNextLeft = PlayerLeft + P->ShapeBox.DeltaPosition.x;
    
    // demand that the boundary is trespassed in this frame
    if( PlayerLeft >= TileRight && PlayerNextLeft < TileRight )
    {
        P->ShapeBox.DeltaPosition.x = (TileRight + 0.1) - PlayerLeft;   // 0.1 to avoid errors from rounding
        P->ShapeBox.Velocity.x = 0;
        P->ShapeBox.Contacts.Left = true;
    }
}

// ---------------------------------------------------------

void CollidePlayerWithTileTop( Player* P, int TileX, int TileY )
{
    // applicable only if player moves down
    if( P->ShapeBox.DeltaPosition.y <= 0 )
      return;
    
    // demand non-zero horizontal overlap
    // (otherwise we get incorrect corner-corner collisions)
    float PlayerExpandedLeft  = Box_ExpandedLeft ( &P->ShapeBox );
    float PlayerExpandedRight = Box_ExpandedRight( &P->ShapeBox );
    int TileLeft  = TileWidth * TileX;
    int TileRight = TileWidth * (TileX+1);
    
    if( PlayerExpandedRight <= TileLeft || PlayerExpandedLeft >= TileRight )
      return;
    
    // now collide tile top against player bottom
    int TileTop = TileHeight * TileY;
    float PlayerBottom = Box_Bottom( &P->ShapeBox );
    float PlayerNextBottom = PlayerBottom + P->ShapeBox.DeltaPosition.y;
    
    // demand that the boundary is trespassed in this frame
    if( PlayerBottom <= TileTop && PlayerNextBottom > TileTop )
    {
        P->ShapeBox.DeltaPosition.y = (TileTop - 0.1) - PlayerBottom;   // 0.1 to avoid errors from rounding
        P->ShapeBox.Velocity.y = 0;
        P->ShapeBox.Contacts.Bottom = true;
    }
}

// ---------------------------------------------------------

void CollidePlayerWithTileBottom( Player* P, int TileX, int TileY )
{
    // applicable only if player moves up
    if( P->ShapeBox.DeltaPosition.y >= 0 )
      return;
    
    // demand non-zero horizontal overlap
    // (otherwise we get incorrect corner-corner collisions)
    float PlayerExpandedLeft  = Box_ExpandedLeft ( &P->ShapeBox );
    float PlayerExpandedRight = Box_ExpandedRight( &P->ShapeBox );
    int TileLeft  = TileWidth * TileX;
    int TileRight = TileWidth * (TileX+1);
    
    if( PlayerExpandedRight <= TileLeft || PlayerExpandedLeft >= TileRight )
      return;
    
    // now collide tile bottom against player top
    int TileBottom = TileHeight * (TileY+1);
    float PlayerTop = Box_Top( &P->ShapeBox );
    float PlayerNextTop = PlayerTop + P->ShapeBox.DeltaPosition.y;
    
    // demand that the boundary is trespassed in this frame
    if( PlayerTop >= TileBottom && PlayerNextTop < TileBottom )
    {
        P->ShapeBox.DeltaPosition.y = (TileBottom + 0.1) - PlayerTop;   // 0.1 to avoid errors from rounding
        P->ShapeBox.Velocity.y = 0;
        P->ShapeBox.Contacts.Top = true;
    }
}


// ---------------------------------------------------------
//   FULL PLAYER-TILE COLLISION FUNCTIONS
// ---------------------------------------------------------


void CollidePlayerWithTile( Player* P, LevelMap* L, int TileX, int TileY )
{
    // do nothing for tile coordinates outside the level
    // discard invalid X values
    if( TileX < 0 || TileX >= L->TilesInX )
      return;
    
    if( TileY < 0 || TileY >= L->TilesInY )
      return;
    
    // nothing to collide with if this tile is empty
    int CurrentTile = L->Tiles[ TileY ][ TileX ];
    
    if( !TileIsSolid( CurrentTile ) )
      return;
    
    // collide player's top against tile's bottom
    if( TileBottomIsExposed( L, TileX, TileY ) )
      CollidePlayerWithTileBottom( P, TileX, TileY );
    
    // collide player's bottom against tile's top
    if( TileTopIsExposed( L, TileX, TileY ) )
      CollidePlayerWithTileTop( P, TileX, TileY );
    
    // collide player's left against tile's right
    if( TileRightIsExposed( L, TileX, TileY ) )
      CollidePlayerWithTileRight( P, TileX, TileY );
    
    // collide player's right against tile's left
    if( TileLeftIsExposed( L, TileX, TileY ) )
      CollidePlayerWithTileLeft( P, TileX, TileY );
}

// ---------------------------------------------------------

void CollidePlayerWithLevel( Player* P, LevelMap* L )
{
    // obtain tile limits to apply collisions
    // (expand region by 1 pixel to avoid rounding errors)
    int MinTileX = (Box_ExpandedLeft  ( &P->ShapeBox ) - 1) / TileWidth;
    int MaxTileX = (Box_ExpandedRight ( &P->ShapeBox ) + 1) / TileWidth;
    int MinTileY = (Box_ExpandedTop   ( &P->ShapeBox ) - 1) / TileHeight;
    int MaxTileY = (Box_ExpandedBottom( &P->ShapeBox ) + 1) / TileHeight;
    
    // collide with each of the tiles
    for( int TileY = MinTileY; TileY <= MaxTileY; ++TileY )
      for( int TileX = MinTileX; TileX <= MaxTileX; ++TileX )
        CollidePlayerWithTile( P, L, TileX, TileY );
}


// ---------------------------------------------------------
//   COLLISIONS BETWEEN TILEMAP AND OTHER OBJECTS
// ---------------------------------------------------------


bool point_overlaps_tilemap( Vector2D* point, tilemap* tm )
{
    if( point->x < 0 || point->x > tilemap_total_width( tm ) )
      return false;
    
    if( point->y < 0 || point->y > tilemap_total_height( tm ) )
      return false;
    
    int tile_step_x = tm->tiles->width  + tm->tiles->gap_x;
    int tile_step_y = tm->tiles->height + tm->tiles->gap_y;
    int tile_x = point->x / tile_step_x;
    int tile_y = point->y / tile_step_y;
    
    return TileIsSolid( tm->map[ tm->tiles->width * tile_y + tile_x ] );
}

// ---------------------------------------------------------

bool box_overlaps_tilemap( Box* B, tilemap* tm )
{
    // determine the range of tiles overlapping box extents
    // (needed to optimize the loop in large maps)
    int tile_step_x = tm->tiles->width  + tm->tiles->gap_x;
    int tile_step_y = tm->tiles->height + tm->tiles->gap_y;
    
    int min_tile_x = max( Box_Left( B ) / tile_step_x, 0 );
    int min_tile_y = max( Box_Top( B )  / tile_step_y, 0 );
    int max_tile_x = min( Box_Right( B )  / tile_step_x, tm->map_width -1 );
    int max_tile_y = min( Box_Bottom( B ) / tile_step_y, tm->map_height-1 );
    
    for( int tile_y = min_tile_y; tile_y <= max_tile_y; ++tile_y )
      for( int tile_x = min_tile_x; tile_x <= max_tile_x; ++tile_x )
        if( TileIsSolid( tm->map[ tm->tiles->width * tile_y + tile_x ] ) )
           return true;
    
    return false;
}

// ---------------------------------------------------------

int* get_tile_at_point( Vector2D* point, tilemap* tm )
{
    if( point->x < 0 || point->x > tilemap_total_width( tm ) )
      return NULL;
    
    if( point->y < 0 || point->y > tilemap_total_height( tm ) )
      return NULL;
    
    int tile_step_x = tm->tiles->width  + tm->tiles->gap_x;
    int tile_step_y = tm->tiles->height + tm->tiles->gap_y;
    int tile_x = point->x / tile_step_x;
    int tile_y = point->y / tile_step_y;
    return &tm->map[ tm->map_width * tile_y + tile_x ];
}
