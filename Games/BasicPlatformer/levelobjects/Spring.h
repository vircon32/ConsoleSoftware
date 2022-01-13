// ---------------------------------------------------------
//   SPRINGS THAT THROW PLAYER UP ON CONTACT
// ---------------------------------------------------------


struct Spring
{
    int TileX, TileY;
    bool Throwing;
    int ThrowElapsedFrames;
};

// ---------------------------------------------------------

void Spring_Create( Spring* S, int TileX, int TileY )
{
    S->TileX = TileX;
    S->TileY = TileY;
}

// ---------------------------------------------------------

void Spring_Reset( Spring* S )
{
    S->Throwing = false;
    S->ThrowElapsedFrames = 0;
}

// ---------------------------------------------------------

void Spring_Update( Spring* S, Player* P )
{
    if( S->Throwing )
    {
        S->ThrowElapsedFrames++;
        
        if( S->ThrowElapsedFrames >= 12 )
        {
            S->Throwing = false;
            S->ThrowElapsedFrames = 0;
        }
        
        return;
    }
    
    // only acts when player on ground
    if( !P->ShapeBox.PreviousContacts.Bottom )
      return;
    
    // check if player is on this row
    float DistanceY = abs( (TileHeight*S->TileY) - P->ShapeBox.Position.y );
    if( DistanceY > 5 ) return;
    
    // check if player is on this tile
    float TileLeft = TileWidth * S->TileX;
    float TileRight = TileWidth * (S->TileX+1);
    float PlayerLeft = Box_Left( &P->ShapeBox );
    float PlayerRight = Box_Right( &P->ShapeBox );
    
    // trigger spring throw
    if( PlayerRight > TileLeft && PlayerLeft < TileRight )
    {
        S->Throwing = true;
        S->ThrowElapsedFrames = 0;
        P->ShapeBox.Velocity.y = -700;
        play_sound( SoundSpring );
        
        // prevent player from jumping at the same time,
        // which would result in a too large impulse
        P->ShapeBox.Contacts.Bottom = false;
    }
}

// ---------------------------------------------------------

void Spring_Draw( Spring* S, Vector2D* LevelTopLeftOnScreen )
{
    // only draw over the tile when throwing
    if( !S->Throwing )
      return;
    
    float SpringX = (TileWidth  * S->TileX) + LevelTopLeftOnScreen->x;
    float SpringY = (TileHeight * S->TileY) + LevelTopLeftOnScreen->y;
    int AnimationFrame = min( 2, S->ThrowElapsedFrames / 4);
    select_region( FirstRegionSpringTop + AnimationFrame );
    draw_region_at( round(SpringX+0.4), round(SpringY+0.4) );
}
