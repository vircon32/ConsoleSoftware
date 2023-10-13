// ---------------------------------------------------------
//   DOOR TO EXIT A ROOM OR LEVEL
// ---------------------------------------------------------


struct ExitDoor
{
    Box EventBox;
    bool IsLevelDoor;
    bool ShowingSign;
    bool WasEntered;
};

// ---------------------------------------------------------

void ExitDoor_Create( ExitDoor* D, int TileX, int TileY )
{
    D->EventBox.Position.x = TileWidth  * TileX;
    D->EventBox.Position.y = TileHeight * TileY;
    D->EventBox.Hotspot.x = -10;
    D->EventBox.Hotspot.y = 0;
    D->EventBox.Width  = 20;
    D->EventBox.Height = TileHeight;
    D->IsLevelDoor = (RoomNumber == 2);
}

// ---------------------------------------------------------

void ExitDoor_Reset( ExitDoor* D )
{
    D->ShowingSign = false;
    D->WasEntered = false;
}

// ---------------------------------------------------------

void ExitDoor_Update( ExitDoor* D, Player* P )
{
    D->ShowingSign = false;
    
    // check overlap with player
    if( Box_OverlapWith( &P->ShapeBox, &D->EventBox ) )
    {
        // exit only allowed on ground
        if( !P->ShapeBox.Contacts.Bottom ) return;
        D->ShowingSign = true;
        
        // if player pressed up, signal door entry
        if( gamepad_up() == 1 )
          D->WasEntered = true;
    }
}

// ---------------------------------------------------------

void ExitDoor_Draw( ExitDoor* D, Vector2D* LevelTopLeftOnScreen )
{
    // draw the door itself
    float DoorX = D->EventBox.Position.x + LevelTopLeftOnScreen->x;
    float DoorY = D->EventBox.Position.y + 40 + LevelTopLeftOnScreen->y;
    
    if( D->IsLevelDoor )
      select_region( RegionEndLevelDoor );
    else
      select_region( RegionEndRoomDoor );
    
    draw_region_at( round(DoorX+0.4), round(DoorY+0.4) );
    
    // draw button sign when skill change is accessible
    if( D->ShowingSign )
    {
        // make the sign blink
        if( GameScene != Scene_Gameplay )
          return;
        
        if( (get_frame_counter() / 20) % 2 )
          return;
        
        float SignX = DoorX + 20;
        float SignY = DoorY - 75;
        
        if( D->IsLevelDoor )
          SignY -= 15;
        
        select_region( RegionSignIn );
        draw_region_at( round(SignX+0.4), round(SignY+0.4) );
    }
}
