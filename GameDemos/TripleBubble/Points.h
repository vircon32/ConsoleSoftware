struct Points
{
    bool Active;
    int ElapsedFrames;
    int BurstPoints;
    int FallPoints;
    int ComboCounter;
    Vector2D Position;
};

// ---------------------------------------------------------

void Points_Create( Points* P, Playfield* F, int ComboCounter )
{
    // initialize points
    P->Active = true;
    P->ElapsedFrames = 0;
    
    // determine position
    P->Position = F->AddedBubbleCenter;
    P->Position.x -= 40;
    P->Position.y += 15;
    
    // calculate burst points
    int BurstExtraBubbles = F->LastShotConnectedBubbles - 3;
    P->BurstPoints = 500 + 300 * BurstExtraBubbles;
    
    // calculate fall points
    P->FallPoints = 500 * F->LastShotFallingBubbles;
    
    // save combo counter
    P->ComboCounter = ComboCounter;
}

// ---------------------------------------------------------

void Points_Update( Points* P )
{
    if( !P->Active ) return;
    
    // ascend slightly
    if( P->ElapsedFrames & 1 )
      P->Position.y -= 1;
    
    // process combo events
    if( P->ElapsedFrames == 30 )
    {
        if( P->ComboCounter == 3 )
          play_sound( SoundCombo );
        
        else if( P->ComboCounter == 6 )
          play_sound( SoundComboNice );
          
        if( P->ComboCounter == 10 )
          play_sound( SoundComboGreat );
    }
    
    // points last a certain time
    P->ElapsedFrames++;
    
    if( P->ElapsedFrames > 80 )
      P->Active = false;
}

// ---------------------------------------------------------

void Points_Draw( Points* P )
{
    if( !P->Active ) return;
    
    // common variables
    int[ 8 ] NumberString;
    Vector2D Position = P->Position;
    Position.y += 35;
    Vector2D DigitPosition;
    
    // apply blink effect
    if( P->ElapsedFrames & 4 )
      set_multiply_color( color_gray );
    else
      set_multiply_color( color_white );
    
    select_texture( TextureGameplay );
    
    // draw burst points
    if( P->ElapsedFrames < 50 )
    {
        itoa( P->BurstPoints, NumberString, 10 );
        DigitPosition = Position;
        int* Digit = NumberString;
        
        while( *Digit )
        {
            select_region( RegionBurstZero + *Digit - '0' );
            draw_region_at( DigitPosition.x, DigitPosition.y );
            DigitPosition.x += 17;
            Digit++;
        }
    }
    
    // advance downwards
    Position.y += 26;
    
    // draw fall points
    if( P->FallPoints > 0 && P->ElapsedFrames >= 20 )
    {
        itoa( P->FallPoints, NumberString, 10 );
        DigitPosition = Position;
        int* Digit = NumberString;
        
        // first, write the plus symbol
        select_region( RegionFallPlus );
        draw_region_at( DigitPosition.x, DigitPosition.y );
        DigitPosition.x += 13;
        
        // now write the number
        while( *Digit )
        {
            select_region( RegionFallZero + *Digit - '0' );
            draw_region_at( DigitPosition.x, DigitPosition.y );
            DigitPosition.x += 13;
            Digit++;
        }
    }
    
    // advance downwards
    if( P->FallPoints > 0 )
      Position.y += 23;
    
    // draw combo counter
    if( P->ComboCounter > 2 && P->ElapsedFrames >= 40 )
    {
        itoa( P->ComboCounter, NumberString, 10 );
        DigitPosition = Position;
        int* Digit = NumberString;
        
        // first, write the combo indicator
        select_region( RegionComboText );
        draw_region_at( DigitPosition.x, DigitPosition.y );
        DigitPosition.x += 77;
        
        // now write the number
        while( *Digit )
        {
            select_region( RegionComboZero + *Digit - '0' );
            draw_region_at( DigitPosition.x, DigitPosition.y );
            DigitPosition.x += 12;
            Digit++;
        }
    }
    
    set_multiply_color( color_white );
}
