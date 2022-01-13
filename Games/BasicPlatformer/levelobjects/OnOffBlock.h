// ---------------------------------------------------------
//   ON/OFF BLOCKS THAT TOGGLE PERIODICALLY
// ---------------------------------------------------------


// on/off blocks are represented as level
// tiles, so they wouldn't need a draw function,
// but we use it for extra toggle effects
struct OnOffBlock
{
    int* HomeTile;
    int TileX, TileY;
    int ElapsedFrames;
    bool InitiallyOn;
    bool IsOn;
    bool ShownOnScreen;
};

// adjustable toggle time
int OnOffToggleFrames = 90;

// ---------------------------------------------------------

void OnOffBlock_Create( OnOffBlock* B, int* HomeTile, int TileX, int TileY )
{
    B->HomeTile = HomeTile;
    B->TileX = TileX;
    B->TileY = TileY;
    B->InitiallyOn = (*B->HomeTile == Tile_OnBlock);
    B->ShownOnScreen = false;
}

// ---------------------------------------------------------

void OnOffBlock_Reset( OnOffBlock* B )
{
    B->ElapsedFrames = 0;
    B->IsOn = B->InitiallyOn;
    
    if( B->IsOn )
      *B->HomeTile = Tile_OnBlock;
    else
      *B->HomeTile = Tile_OffBlock;
}

// ---------------------------------------------------------

void OnOffBlock_Update( OnOffBlock* B )
{
    B->ElapsedFrames++;
    
    if( B->ElapsedFrames >= OnOffToggleFrames )
    {
        B->ElapsedFrames = 0;
        B->IsOn = !B->IsOn;
        
        if( B->IsOn )
          *B->HomeTile = Tile_OnBlock;
        else
          *B->HomeTile = Tile_OffBlock;
    }
    
    // play toggle sound only when visible
    // (use a known channel to avoid multiple
    // simultaneous plays from different blocks)
    if( B->ShownOnScreen )
      if( B->ElapsedFrames == (OnOffToggleFrames - 1) )
        play_sound_in_channel( SoundOnOff, ChannelOnOff );
}

// ---------------------------------------------------------

void OnOffBlock_Draw( OnOffBlock* B, Vector2D* LevelTopLeftOnScreen )
{
    // nothing to do if block is not on screen
    int TileMinX = LevelTopLeftOnScreen->x + B->TileX * TileWidth;
    int TileMinY = LevelTopLeftOnScreen->y + B->TileY * TileHeight;
    int TileMaxX = TileMinX + TileWidth  - 1;
    int TileMaxY = TileMinY + TileHeight - 1;
    
    // let the block be at least a few pixels into the screen
    B->ShownOnScreen = false;
    if( TileMaxX < 2 ) return;
    if( TileMaxY < 2 ) return;
    if( TileMinX > (screen_width -2) ) return;
    if( TileMinY > (screen_height-2) ) return;
    B->ShownOnScreen = true;
    
    // to ensure a known rounding from float, convert
    // coordinates to integer before adding tile dimensions
    // (otherwise objects and scenery could be off by 1 pixel)
    int RenderX = LevelTopLeftOnScreen->x;
    RenderX += B->TileX * TileWidth;
    
    int RenderY = LevelTopLeftOnScreen->y;
    RenderY += B->TileY * TileHeight;
    
    // draw toggle effect over the block for a few frames
    if( B->ElapsedFrames >= (OnOffToggleFrames - 4) )
    {
        select_region( RegionOnOffToggle );
        draw_region_at( RenderX, RenderY );
    }
}
