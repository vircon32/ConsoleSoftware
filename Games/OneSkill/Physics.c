// *****************************************************************************
    // include project headers
    #include "Physics.h"
    #include "Player.h"
    #include "RoomMap.h"
// *****************************************************************************


// ---------------------------------------------------------
//   PLAYER COLLISION WITH LEVEL BOUNDARIES
// ---------------------------------------------------------


void EnforceRoomBoundaries( Player* P, RoomMap* R )
{
    // determine level limits
    int RoomMinX = 0;
    int RoomMinY = 0;
    int RoomMaxX = TileWidth  * R->TilesInX;
    
    // collisions for movement along X
    if( P->ShapeBox.DeltaPosition.x < 0 )
    {
        float PlayerLeft = Box_Left( &P->ShapeBox );
        float PlayerNextLeft = PlayerLeft + P->ShapeBox.DeltaPosition.x;
        
        if( PlayerNextLeft < RoomMinX )
        {
            P->ShapeBox.DeltaPosition.x = (RoomMinX + 0.1) - PlayerLeft;
            P->ShapeBox.Velocity.x = 0;
            P->ShapeBox.Contacts.Left = true;
        }
    }
    
    else if( P->ShapeBox.DeltaPosition.x > 0 )
    {
        float PlayerRight = Box_Right( &P->ShapeBox );
        float PlayerNextRight = PlayerRight + P->ShapeBox.DeltaPosition.x;
        
        if( PlayerNextRight > RoomMaxX )
        {
            P->ShapeBox.DeltaPosition.x = (RoomMaxX - 0.1) - PlayerRight;
            P->ShapeBox.Velocity.x = 0;
            P->ShapeBox.Contacts.Right = true;
        }
    }
    
    // collisions for movement along Y
    if( P->ShapeBox.DeltaPosition.y < 0 )
    {
        float PlayerTop = Box_Top( &P->ShapeBox );
        float PlayerNextTop = PlayerTop + P->ShapeBox.DeltaPosition.y;
        
        if( PlayerNextTop < RoomMinY )
        {
            P->ShapeBox.DeltaPosition.y = (RoomMinY - 0.1) - PlayerTop;
            P->ShapeBox.Velocity.y = 0;
            P->ShapeBox.Contacts.Top = true;
        }
    }
    
    // do not enforce the bottom!
    // our character needs to be allowed to fall
}


// ---------------------------------------------------------
//   COLLISION FUNCTIONS
// ---------------------------------------------------------
// note that all these depend on the player position,
// because some tiles are semisolids

bool TileLeftIsExposed( Player* P, RoomMap* R, int TileX, int TileY )
{
    // at level limits, all tile sides are exposed
    if( TileX == 0 ) return true;
    
    // otherwise, analyze the neighbour tile at the left
    int TileAtLeft = R->Tiles[ TileY ][ TileX-1 ];
    
    // case 1: player's bottom against tile's top
    if( Box_Bottom( &P->ShapeBox ) < (TileHeight * TileY)  )
      return !TileHasTopBoundary( TileAtLeft );
    
    // case 2: player's top against tile's bottom
    else if( Box_Top( &P->ShapeBox ) > (TileHeight * (TileY+1)) )
      return !TileHasBottomBoundary( TileAtLeft );
    
    // case 3: player is inside tile's own row
    else return true;
}

// ---------------------------------------------------------

bool TileRightIsExposed( Player* P, RoomMap* R, int TileX, int TileY )
{
    // at level limits, all tile sides are exposed
    if( TileX == (R->TilesInX-1) ) return true;
    
    // otherwise, analyze the neighbour tile at the right
    int TileAtRight = R->Tiles[ TileY ][ TileX+1 ];
    
    // case 1: player's bottom against tile's top
    if( Box_Bottom( &P->ShapeBox ) < (TileHeight * TileY)  )
      return !TileHasTopBoundary( TileAtRight );
    
    // case 2: player's top against tile's bottom
    else if( Box_Top( &P->ShapeBox ) > (TileHeight * (TileY+1)) )
      return !TileHasBottomBoundary( TileAtRight );
    
    // case 3: player is inside tile's own row
    else return true;
}

// ---------------------------------------------------------

bool TileTopIsExposed( Player* P, RoomMap* R, int TileX, int TileY )
{
    // at level limits, all tile sides are exposed
    if( TileY == 0 ) return true;
    
    // otherwise, analyze the neighbour tile at the top
    int TileAtTop = R->Tiles[ TileY-1 ][ TileX ];
    
    // case 1: player's right against tile's left
    if( Box_Right( &P->ShapeBox ) < (TileWidth * TileX)  )
      return !TileHasLeftBoundary( TileAtTop );
    
    // case 2: player's left against tile's right
    else if( Box_Left( &P->ShapeBox ) > (TileWidth * (TileX+1)) )
      return !TileHasRightBoundary( TileAtTop );
    
    // case 3: player is inside tile's own column
    else return true;
}

// ---------------------------------------------------------

bool TileBottomIsExposed( Player* P, RoomMap* R, int TileX, int TileY )
{
    // at level limits, all tile sides are exposed
    if( TileY == (R->TilesInY-1) ) return true;
    
    // otherwise, analyze the neighbour tile at the bottom
    int TileAtBottom = R->Tiles[ TileY+1 ][ TileX ];
    
    // case 1: player's right against tile's left
    if( Box_Right( &P->ShapeBox ) < (TileWidth * TileX)  )
      return !TileHasLeftBoundary( TileAtBottom );
    
    // case 2: player's left against tile's right
    else if( Box_Left( &P->ShapeBox ) > (TileWidth * (TileX+1)) )
      return !TileHasRightBoundary( TileAtBottom );
    
    // case 3: player is inside tile's own column
    else return true;
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
        P->ShapeBox.Velocity.y = 7;                                     // a little down speed to avoid double landings
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


void CollidePlayerWithTile( Player* P, RoomMap* R, int TileX, int TileY )
{
    // do nothing for tile coordinates outside the level
    // discard invalid X values
    if( TileX < 0 || TileX >= R->TilesInX )
      return;
    
    if( TileY < 0 || TileY >= R->TilesInY )
      return;
    
    // nothing to collide with if this tile is empty
    int CurrentTile = R->Tiles[ TileY ][ TileX ];
    
    if( TileHasNoBoundaries( CurrentTile ) )
      return;
    
    // collide player's top against tile's bottom
    if( TileHasBottomBoundary( CurrentTile ) )
      if( TileBottomIsExposed( P, R, TileX, TileY ) )
        CollidePlayerWithTileBottom( P, TileX, TileY );
        
    // collide player's bottom against tile's top
    if( TileHasTopBoundary( CurrentTile ) )
      if( TileTopIsExposed( P, R, TileX, TileY ) )
        CollidePlayerWithTileTop( P, TileX, TileY );
    
    // collide player's left against tile's right
    if( TileHasRightBoundary( CurrentTile ) )
      if( TileRightIsExposed( P, R, TileX, TileY ) )
        CollidePlayerWithTileRight( P, TileX, TileY );
    
    // collide player's right against tile's left
    if( TileHasLeftBoundary( CurrentTile ) )
      if( TileLeftIsExposed( P, R, TileX, TileY ) )
        CollidePlayerWithTileLeft( P, TileX, TileY );
}

// ---------------------------------------------------------

void CollidePlayerWithRoom( Player* P, RoomMap* R )
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
        CollidePlayerWithTile( P, R, TileX, TileY );
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
    
    return TileHasLeftBoundary( tm->map[ tm->array_width * tile_y + tile_x ] );
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
        if( TileHasLeftBoundary( tm->map[ tm->array_width * tile_y + tile_x ] ) )
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
    return &tm->map[ tm->array_width * tile_y + tile_x ];
}
