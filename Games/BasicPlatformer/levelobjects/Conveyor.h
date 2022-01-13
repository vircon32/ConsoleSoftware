// ---------------------------------------------------------
//   CONVEYORS THAT PROPEL PLAYER LEFT
// ---------------------------------------------------------


struct Conveyor
{
    int TileX, TileY;
    bool Propeling;
    int PropelingElapsedFrames;
};

// ---------------------------------------------------------

void Conveyor_Create( Conveyor* C, int TileX, int TileY )
{
    C->TileX = TileX;
    C->TileY = TileY;
}

// ---------------------------------------------------------

void Conveyor_Reset( Conveyor* C )
{
    C->Propeling = false;
    C->PropelingElapsedFrames = 0;
}

// ---------------------------------------------------------

void Conveyor_Update( Conveyor* C, Player* P )
{
    // only active when player on ground
    if( !P->ShapeBox.PreviousContacts.Bottom )
    {
        C->Propeling = false;
        C->PropelingElapsedFrames = 0;
        return;
    }
    
    // check if player is on this row
    float DistanceY = abs( (TileHeight*C->TileY) - P->ShapeBox.Position.y );
    if( DistanceY > 5 ) return;
    
    // check if player is on this tile
    float TileLeft = TileWidth * C->TileX;
    float TileRight = TileWidth * (C->TileX+1);
    float PlayerLeft = Box_Left( &P->ShapeBox );
    float PlayerRight = Box_Right( &P->ShapeBox );
    
    if( PlayerRight > TileLeft && PlayerLeft < TileRight )
    {
        C->Propeling = true;
        C->PropelingElapsedFrames++;
        
        P->GroundSpeedX = -100;
    }
    
    else
    {
        C->Propeling = false;
        C->PropelingElapsedFrames = 0;
    }
}

// ---------------------------------------------------------

void Conveyor_Draw( Conveyor* C, Vector2D* LevelTopLeftOnScreen )
{
    // only draw over the tile when propelling
    if( !C->Propeling )
      return;
    
    // to ensure a known rounding from float, convert
    // coordinates to integer before adding tile dimensions
    // (otherwise objects and scenery could be off by 1 pixel)
    int RenderX = LevelTopLeftOnScreen->x;
    RenderX += C->TileX * TileWidth;
    
    int RenderY = LevelTopLeftOnScreen->y;
    RenderY += C->TileY * TileHeight;
    
    // draw the correct animation frame
    int AnimationFrame = (C->PropelingElapsedFrames / 4) % 2;
    select_region( FirstRegionConveyor + AnimationFrame );
    draw_region_at( RenderX, RenderY );
}
