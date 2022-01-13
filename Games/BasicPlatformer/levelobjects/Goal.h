// ---------------------------------------------------------
//   LEVEL GOAL
// ---------------------------------------------------------


struct Goal
{
    Box EventBox;
    int RequiredCoins;
    bool Enabled;
    bool Collected;
    
    // for go sign animation
    int ElapsedFrames;
};

// ---------------------------------------------------------

void Goal_Create( Goal* G, int TileX, int TileY )
{
    G->EventBox.Position.x = TileWidth  * (TileX + 0.5);
    G->EventBox.Position.y = TileHeight * (TileY + 1);
    G->EventBox.Hotspot.x = 10;
    G->EventBox.Hotspot.y = 36;
    G->EventBox.Width  = 20;
    G->EventBox.Height = 36;
}

// ---------------------------------------------------------

void Goal_Reset( Goal* G )
{
    G->Enabled = false;
    G->Collected = false;
}

// ---------------------------------------------------------

void Goal_Update( Goal* G, Player* P )
{
    // check when all the coins were collected
    if( !G->Enabled && !G->Collected )
      if( P->CollectedCoins >= G->RequiredCoins )
        G->Enabled = true;
    
    // nothing to do when goal is not obtainable
    if( G->Collected ) return;
    
    // check overlap with player
    if( Box_OverlapWith( &P->ShapeBox, &G->EventBox ) )
      if( G->Enabled && P->ShapeBox.PreviousContacts.Bottom )
      {
          G->Collected = true;
          CreateHighlight( &G->EventBox.Position );
          Gameplay_ChangeState( Gameplay_Goal );
      }
    
    G->ElapsedFrames++;
}

// ---------------------------------------------------------

void Goal_Draw( Goal* G, Vector2D* LevelTopLeftOnScreen )
{
    // when all coins are collected, draw the GO sign
    if( !G->Enabled )
      return;
    
    // calculate position on screen
    int GoSignX = LevelTopLeftOnScreen->x + G->EventBox.Position.x;
    int GoSignY = LevelTopLeftOnScreen->y + G->EventBox.Position.y - 50;
    
    // draw current animation frame
    int GoAnimationFrame = ((G->ElapsedFrames & 8) != 0);
    select_region( FirstRegionGoSign + GoAnimationFrame );
    draw_region_at( GoSignX, GoSignY );
    
    // otherwise, warn players they are missing coins!
    // (PENDING)
}
