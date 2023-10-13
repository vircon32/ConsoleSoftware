// ---------------------------------------------------------
//   DOOR TO EXIT A ROOM OR LEVEL
// ---------------------------------------------------------


// it has no state or behavior, so no need for reset and update
struct EntryDoor
{
    Vector2D Position;
    bool IsPresent;
};

// ---------------------------------------------------------

void EntryDoor_Create( EntryDoor* D, int TileX, int TileY )
{
    D->Position.x = TileWidth  * TileX;
    D->Position.y = TileHeight * TileY;
    D->IsPresent = (RoomNumber > 1);
}

// ---------------------------------------------------------

void EntryDoor_Draw( EntryDoor* D, Vector2D* LevelTopLeftOnScreen )
{
    if( !D->IsPresent )
      return;
      
    float DoorX = D->Position.x + LevelTopLeftOnScreen->x;
    float DoorY = D->Position.y + 40 + LevelTopLeftOnScreen->y;
    select_region( RegionClosedDoor );
    draw_region_at( round(DoorX+0.4), round(DoorY+0.4) );
}
