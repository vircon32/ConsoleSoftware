// ---------------------------------------------------------
//   LOCKED DOORS THAT ARE OPENED WITH A KEY
// ---------------------------------------------------------


// doors are drawn as level tiles, so no draw function needed
struct Door
{
    int* HomeTile;
    int TileX, TileY;
    bool Opened;
};

// ---------------------------------------------------------

void Door_Create( Door* D, int* HomeTile, int TileX, int TileY )
{
    D->HomeTile = HomeTile;
    D->TileX = TileX;
    D->TileY = TileY;
}

// ---------------------------------------------------------

void Door_Reset( Door* D )
{
    *D->HomeTile = Tile_Door;
    D->Opened = false;
}

// ---------------------------------------------------------

void Door_Update( Door* D, Player* P )
{
    // process only when available to open
    if( D->Opened )
      return;
    
    // check if door is opened
    if( P->CollectedKeys <= 0 )
      return;
    
    float DistanceX = abs(P->ShapeBox.Position.x - TileWidth*(D->TileX+0.5));
    float DistanceY = abs((P->ShapeBox.Position.y-15) - TileHeight*(D->TileY+0.5));
    
    if( DistanceX < 42 && DistanceY < 10 )
      if( P->ShapeBox.Contacts.Left || P->ShapeBox.Contacts.Right )  
      {
          *D->HomeTile = Tile_Empty;
          D->Opened = true;
          P->CollectedKeys--;
          play_sound( SoundOpenDoor );
          
          Vector2D HighlightPosition;
          HighlightPosition.x = TileWidth  * (D->TileX + 0.5);
          HighlightPosition.y = TileHeight * (D->TileY + 0.5);
          CreateHighlight( &HighlightPosition );
      }
}
