// ---------------------------------------------------------
//   COLLECTABLE COINS
// ---------------------------------------------------------


struct Coin
{
    Box EventBox;
    int AnimationFrame;
    int ElapsedFrames;
    bool Collected;
};

// ---------------------------------------------------------

void Coin_Create( Coin* C, int TileX, int TileY )
{
    C->EventBox.Position.x = TileWidth  * (TileX + 0.5);
    C->EventBox.Position.y = TileHeight * (TileY + 0.5);
    C->EventBox.Hotspot.x = 10;
    C->EventBox.Hotspot.y = 10;
    C->EventBox.Width  = 20;
    C->EventBox.Height = 20;
}

// ---------------------------------------------------------

void Coin_Reset( Coin* C )
{
    C->Collected = false;
    C->AnimationFrame = rand() % 4;
    C->ElapsedFrames = 0;
}

// ---------------------------------------------------------

void Coin_Update( Coin* C, Player* P )
{
    // process only when not collected
    if( C->Collected )
      return;
    
    // check if coin is collected
    if( Box_OverlapWith( &P->ShapeBox, &C->EventBox ) )
    {
        C->Collected = true;
        C->ElapsedFrames = 0;
        P->CollectedCoins++;
        play_sound( SoundCoin );
        CreateHighlight( &C->EventBox.Position );
        return;
    }
    
    // advance coin animation each 8 frames
    C->ElapsedFrames++;
    
    if( (C->ElapsedFrames & 7) == 0 )
    {
        C->AnimationFrame++;
        C->AnimationFrame %= 4;
    }
}

// ---------------------------------------------------------

void Coin_Draw( Coin* C, Vector2D* LevelTopLeftOnScreen )
{
    // draw only when available to collect
    if( C->Collected )
      return;
    
    // calculate position on screen
    int CoinX = LevelTopLeftOnScreen->x + C->EventBox.Position.x;
    int CoinY = LevelTopLeftOnScreen->y + C->EventBox.Position.y;
    
    // draw coin
    select_region( FirstRegionCoinMotion + C->AnimationFrame );
    draw_region_at( CoinX, CoinY );
}
