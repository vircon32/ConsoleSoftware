// ---------------------------------------------------------
//   TIME SWITCHING SPIKES
// ---------------------------------------------------------


// define switching speed
#define TimedSpike_SwitchFrames 90

struct TimedSpike
{
    Box EventBox;
    int* HomeTile;
    int ElapsedFrames;
};

// ---------------------------------------------------------

void TimedSpike_Create( TimedSpike* TS, int TileX, int TileY, int* HomeTile )
{
    TS->EventBox.Position.x = TileWidth  * TileX;
    TS->EventBox.Position.y = TileHeight * TileY;
    TS->EventBox.Hotspot.x = 0;
    TS->EventBox.Hotspot.y = 0;
    TS->EventBox.Width  = TileWidth;
    TS->EventBox.Height = TileHeight;
    TS->HomeTile = HomeTile;
}

// ---------------------------------------------------------

void TimedSpike_Reset( TimedSpike* TS )
{
    TS->ElapsedFrames = 0;
    *TS->HomeTile = Tile_TimedSpike;
}

// ---------------------------------------------------------

void TimedSpike_Update( TimedSpike* TS, Player* P )
{
    TS->ElapsedFrames++;
    
    // toggle between extended and hidden
    if( TS->ElapsedFrames >= TimedSpike_SwitchFrames )
    {
        TS->ElapsedFrames = 0;
        
        if( *TS->HomeTile == Tile_TimedSpike )
          *TS->HomeTile = Tile_Empty;
        else
          *TS->HomeTile = Tile_TimedSpike;
        
        play_sound( SoundSpikesSwitch );
    }
    
    // when extended, check overlap with player
    if( *TS->HomeTile == Tile_TimedSpike )
      if( Box_OverlapWith( &P->ShapeBox, &TS->EventBox ) )
        P->IsDead = true;
}

// ---------------------------------------------------------

void TimedSpike_Draw( TimedSpike* TS, Vector2D* LevelTopLeftOnScreen )
{
    // draw only on transitions; when fully
    // extended the home tile is drawn on the map
    // when extended, check overlap with player
    if( *TS->HomeTile == Tile_TimedSpike )
      return;
    
    if( TS->ElapsedFrames > 2 && TS->ElapsedFrames < (TimedSpike_SwitchFrames - 3) )
      return;
    
    int TimedSpikeX = LevelTopLeftOnScreen->x + TS->EventBox.Position.x;
    int TimedSpikeY = LevelTopLeftOnScreen->y + TS->EventBox.Position.y;
    select_region( RegionTimedSpikesSwitching );
    draw_region_at( TimedSpikeX, TimedSpikeY );
}
