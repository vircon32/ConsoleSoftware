// ---------------------------------------------------------
//   KEYS FOR LOCKED DOORS
// ---------------------------------------------------------


// keys are drawn as level tiles, so no draw function needed
struct Key
{
    int* HomeTile;
    Box EventBox;
    bool Collected;
};

// ---------------------------------------------------------

void Key_Create( Key* K, int* HomeTile, int TileX, int TileY )
{
    K->EventBox.Position.x = TileWidth  * (TileX + 0.5);
    K->EventBox.Position.y = TileHeight * (TileY + 0.5);
    K->EventBox.Hotspot.x = 20;
    K->EventBox.Hotspot.y = 12;
    K->EventBox.Width  = 40;
    K->EventBox.Height = 24;
    K->HomeTile = HomeTile;
}

// ---------------------------------------------------------

void Key_Reset( Key* K )
{
    *K->HomeTile = Tile_Key;
    K->Collected = false;
}

// ---------------------------------------------------------

void Key_Update( Key* K, Player* P )
{
    // process only when available to collect
    if( K->Collected )
      return;
    
    // check if key is collected
    if( Box_OverlapWith( &P->ShapeBox, &K->EventBox ) )
    {
        *K->HomeTile = Tile_Empty;
        K->Collected = true;
        P->CollectedKeys++;
        play_sound( SoundGetKey );
        CreateHighlight( &K->EventBox.Position );
    }
}
