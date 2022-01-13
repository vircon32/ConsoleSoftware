// ---------------------------------------------------------
//   PLAYER COLLISION WITH LEVEL BOUNDARIES
// ---------------------------------------------------------


void EnforceLevelBoundaries( Player* P, LevelMap* L )
{
    // determine level limits
    int LevelMinX = 0;
    int LevelMinY = 0;
    int LevelMaxX = TileWidth  * L->TilesInX;
    
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
    
    // do not enforce the bottom!
    // our character needs to be allowed to fall
}


// ---------------------------------------------------------
//   COLLISION FUNCTIONS
// ---------------------------------------------------------
// note that all these depend on the player position,
// because some tiles are semisolids

bool TileLeftIsExposed( Player* P, LevelMap* L, int TileX, int TileY )
{
    // at level limits, all tile sides are exposed
    if( TileX == 0 ) return true;
    
    // otherwise, analyze the neighbour tile at the left
    int TileAtLeft = L->Tiles[ TileY ][ TileX-1 ];
    
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

bool TileRightIsExposed( Player* P, LevelMap* L, int TileX, int TileY )
{
    // at level limits, all tile sides are exposed
    if( TileX == (L->TilesInX-1) ) return true;
    
    // otherwise, analyze the neighbour tile at the right
    int TileAtRight = L->Tiles[ TileY ][ TileX+1 ];
    
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

bool TileTopIsExposed( Player* P, LevelMap* L, int TileX, int TileY )
{
    // at level limits, all tile sides are exposed
    if( TileY == 0 ) return true;
    
    // otherwise, analyze the neighbour tile at the top
    int TileAtTop = L->Tiles[ TileY-1 ][ TileX ];
    
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

bool TileBottomIsExposed( Player* P, LevelMap* L, int TileX, int TileY )
{
    // at level limits, all tile sides are exposed
    if( TileY == (L->TilesInY-1) ) return true;
    
    // otherwise, analyze the neighbour tile at the bottom
    int TileAtBottom = L->Tiles[ TileY+1 ][ TileX ];
    
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
    
    if( TileHasNoBoundaries( CurrentTile ) )
      return;
    
    // collide player's top against tile's bottom
    if( TileHasBottomBoundary( CurrentTile ) )
      if( TileBottomIsExposed( P, L, TileX, TileY ) )
        CollidePlayerWithTileBottom( P, TileX, TileY );
        
    // collide player's bottom against tile's top
    if( TileHasTopBoundary( CurrentTile ) )
      if( TileTopIsExposed( P, L, TileX, TileY ) )
        CollidePlayerWithTileTop( P, TileX, TileY );
    
    // collide player's left against tile's right
    if( TileHasRightBoundary( CurrentTile ) )
      if( TileRightIsExposed( P, L, TileX, TileY ) )
        CollidePlayerWithTileRight( P, TileX, TileY );
    
    // collide player's right against tile's left
    if( TileHasLeftBoundary( CurrentTile ) )
      if( TileLeftIsExposed( P, L, TileX, TileY ) )
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
