// ---------------------------------------------------------
//   STATIC SPIKES
// ---------------------------------------------------------


// spikes have no state, so no reset function is needed;
// also they are drawn as a tile, so no draw function needed
struct Spike
{
    Box EventBox;
};

// ---------------------------------------------------------

void Spike_Create( Spike* S, int TileX, int TileY )
{
    S->EventBox.Position.x = TileWidth  * TileX;
    S->EventBox.Position.y = TileHeight * TileY;
    S->EventBox.Hotspot.x = 0;
    S->EventBox.Hotspot.y = 0;
    S->EventBox.Width  = TileWidth;
    S->EventBox.Height = TileHeight;
}

// ---------------------------------------------------------

void Spike_Update( Spike* S, Player* P )
{
    // check overlap with player
    if( Box_OverlapWith( &P->ShapeBox, &S->EventBox ) )
      P->IsDead = true;
}
