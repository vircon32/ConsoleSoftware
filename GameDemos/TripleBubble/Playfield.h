#define MaxBubblesInX      20
#define MaxBubblesInY      12
#define MaxFallingBubbles  100

typedef int[ MaxBubblesInY ][ MaxBubblesInX ] BubbleArray;

// used for bubble animation effect
int FirstRegionBubbles = FirstRegionBubbles1;

// we can have our playfields in different sizes
// and positions in the different game modes;
// therefore we will not use this to try to draw
// 
struct Playfield
{
    // limits on screen
    int MinX, MaxX;
    int MinY, MaxY;
    int DeathY;
    
    // limit region for bubble centers
    int BubbleMinX, BubbleMaxX;
    int BubbleMinY;
    
    // data for descending ceiling
    int CeilingDescendedRows;
    int CeilingY;
    int ShotsPerDescent;
    int ShotsBeforeNextDescent;
    
    // bubble field
    // we need a copy to be able to modify the playfield
    // (when bubbles get added or removed)
    int BubblesInX, BubblesInY;
    BubbleArray Bubbles, ConnectedBubbles, SupportedBubbles;
    int LastShotConnectedBubbles;
    Vector2D AddedBubbleCenter;
    
    // bubbles falling when their support was popped
    Bubble[ MaxFallingBubbles ] FallingBubbles;
    int LastShotFallingBubbles;
    
    // counters for remaining bubbles
    int[ 8 ] BubbleColorCounts;
    int RemainingBubbles;
    
    // current state
    int State;
    int ElapsedFrames;
};

// ---------------------------------------------------------
//     PLAYFIELD METHODS
// ---------------------------------------------------------


void Playfield_Create( Playfield* P, int LeftX, int BottomY, int BubblesInX, int BubblesInY )
{
    // playfield size
    P->BubblesInX = BubblesInX;
    P->BubblesInY = BubblesInY;
    
    // determine total limits on screen
    P->MinX = LeftX;
    P->MaxX = LeftX + BubblesInX * BubbleDistanceX;
    P->MaxY = BottomY;
    P->MinY = P->MaxY - (BubblesInY-1) * BubbleDistanceY - 65 - 1;
    P->DeathY = P->MaxY - 65;
    
    // determine limits for bubble centers
    P->BubbleMinX = P->MinX + BubbleRadius + 2 - 1;
    P->BubbleMaxX = P->MaxX - BubbleRadius - 2 - 1;
    P->BubbleMinY = P->MinY + BubbleRadius + 2;
    
    // configure ceiling descent
    P->CeilingY = P->MinY;
    P->ShotsPerDescent = 6;
    P->ShotsBeforeNextDescent = P->ShotsPerDescent;
}

// ---------------------------------------------------------

void Playfield_ChangeState( Playfield* P, int NewState )
{
    int OldState = P->State;
    P->State = NewState;
    P->ElapsedFrames = 0;
    
    // transition-specific events
    if( NewState == Playfield_CeilingDrop )
      play_sound( SoundCeilingDrop );
      
    if( NewState == Playfield_Death )
      P->ShotsBeforeNextDescent = P->ShotsPerDescent;
}

// ---------------------------------------------------------

void Playfield_Reset( Playfield* P )
{
    Playfield_ChangeState( P, Playfield_Initialize );
}

// ---------------------------------------------------------

int Playfield_GetBubbleCenterX( Playfield* P, int Row, int Column )
{
    int CenterX = P->BubbleMinX + BubbleDistanceX * Column;
    
    // offset odd rows
    if( Row % 2 )
      CenterX += BubbleDistanceX / 2;
    
    return CenterX;
}

// ---------------------------------------------------------

int Playfield_GetBubbleCenterY( Playfield* P, int Row )
{
    int CenterY = P->BubbleMinY + BubbleDistanceY * Row;
    return CenterY;
}

// ---------------------------------------------------------

void Playfield_UpdateBubbleCounts( Playfield* P )
{
    // update counts for each regular bubble color
    memset( P->BubbleColorCounts, 0, sizeof(P->BubbleColorCounts) );
    
    for( int y = 0; y < P->BubblesInY; ++y )
      for( int x = 0; x < P->BubblesInX; ++x )
        if( P->Bubbles[ y ][ x ] > 0 && P->Bubbles[ y ][ x ] < 8 )
          P->BubbleColorCounts[ P->Bubbles[ y ][ x ] ] += 1;
    
    // now update the total bubble count
    P->RemainingBubbles = 0;
    
    for( int y = 0; y < P->BubblesInY; ++y )
      for( int x = 0; x < P->BubblesInX; ++x )
        if( P->Bubbles[ y ][ x ] > 0 )
          P->RemainingBubbles++;
}

// ---------------------------------------------------------

bool Playfield_CheckCeilingDrop( Playfield* P )
{
    P->ShotsBeforeNextDescent--;
    
    if( P->ShotsBeforeNextDescent > 0 )
      return false;
    
    // activate drop
    P->ShotsBeforeNextDescent = P->ShotsPerDescent;
    Playfield_ChangeState( P, Playfield_CeilingDrop );
    return true;
}

// ---------------------------------------------------------

bool Playfield_CheckLevelClear( Playfield* P )
{
    Playfield_UpdateBubbleCounts( P );
    
    if( P->RemainingBubbles <= 0 )
    {
        Playfield_ChangeState( P, Playfield_LevelClear );
        return true;
    }
    
    return false;
}

// ---------------------------------------------------------

bool Playfield_CheckDeath( Playfield* P )
{
    // find the first row below the death line
    int DeathRow = 11 - P->CeilingDescendedRows;
    
    for( int y = DeathRow; y < MaxBubblesInY; ++y )
      for( int x = 0; x < P->BubblesInX; ++x )
        if( P->Bubbles[ y ][ x ] )
        {
            Playfield_ChangeState( P, Playfield_Death );
            return true;
        }
    
    return false;
}

// ---------------------------------------------------------

int Playfield_GetNewBubbleType( Playfield* P )
{
    // first chech if the level has ended
    Playfield_UpdateBubbleCounts( P );
    
    if( P->RemainingBubbles <= 0 )
      return 0;
    
    // now keep choosing at random until we
    // get a color with remaining bubbles
    while( true )
    {
       int Type = 1 + rand() % 7;
       
       if( P->BubbleColorCounts[ Type ] > 0 )
         return Type;
    }
}

// ---------------------------------------------------------

int Playfield_FindConnectedBubbles( Playfield* P, int Row, int Column )
{
    // get the bubble color
    int* ThisBubble = &P->ConnectedBubbles[ Row ][ Column ];
    int BubbleColor = *ThisBubble;
    
    if( BubbleColor <= 0 )
      return 0;
    
    // start counting from this bubble and mark
    // it as counted so it won't be counted twice
    int NumberOfConnectedBubbles = 1;
    *ThisBubble = -1;
    
    // recurse to count all neighbors in X,Y
    if( Row > 0 )
    {
        int* TopNeighbor = &P->ConnectedBubbles[ Row-1 ][ Column ];
        
        if( *TopNeighbor == BubbleColor )
          NumberOfConnectedBubbles += Playfield_FindConnectedBubbles( P, Row-1, Column );
        
        else if( *TopNeighbor > 0 )
          *TopNeighbor = -2;
    }
    
    if( Row < (P->BubblesInY-1) )
    {
        int* BottomNeighbor = &P->ConnectedBubbles[ Row+1 ][ Column ];
        
        if( *BottomNeighbor == BubbleColor )
          NumberOfConnectedBubbles += Playfield_FindConnectedBubbles( P, Row+1, Column );
        
        else if( *BottomNeighbor > 0 )
          *BottomNeighbor = -2;
    }
    
    if( Column > 0 )
    {
        int* LeftNeighbor = &P->ConnectedBubbles[ Row ][ Column-1 ];
        
        if( *LeftNeighbor == BubbleColor )
          NumberOfConnectedBubbles += Playfield_FindConnectedBubbles( P, Row, Column-1 );
        
        else if( *LeftNeighbor > 0 )
          *LeftNeighbor = -2;
    }
    
    if( Column < (P->BubblesInX-1) )
    {
        int* RightNeighbor = &P->ConnectedBubbles[ Row ][ Column+1 ];
        
        if( *RightNeighbor == BubbleColor )
          NumberOfConnectedBubbles += Playfield_FindConnectedBubbles( P, Row, Column+1 );
        
        else if( *RightNeighbor > 0 )
          *RightNeighbor = -2;
    }
    
    // recurse in the 2 remaining hexagon
    // sides, which depend on row parity
    if( Row % 2 )
    {
        if( Row > 0 )
          if( Column < (P->BubblesInX-1) )
            {
                int* NeighborA = &P->ConnectedBubbles[ Row-1 ][ Column+1 ];
                
                if( *NeighborA == BubbleColor )
                  NumberOfConnectedBubbles += Playfield_FindConnectedBubbles( P, Row-1, Column+1 );
                
                else if( *NeighborA > 0 )
                  *NeighborA = -2;
            }
        
        if( Row < (P->BubblesInY-1) )
          if( Column < (P->BubblesInX-1) )
            {
                int* NeighborB = &P->ConnectedBubbles[ Row+1 ][ Column+1 ];
                
                if( *NeighborB == BubbleColor )
                  NumberOfConnectedBubbles += Playfield_FindConnectedBubbles( P, Row+1, Column+1 );
                
                else if( *NeighborB > 0 )
                  *NeighborB = -2;
            }
    }
    
    else
    {
        if( Row > 0 )
          if( Column > 0 )
            {
                int* NeighborA = &P->ConnectedBubbles[ Row-1 ][ Column-1 ];
                
                if( *NeighborA == BubbleColor )
                  NumberOfConnectedBubbles += Playfield_FindConnectedBubbles( P, Row-1, Column-1 );
                
                else if( *NeighborA > 0 )
                  *NeighborA = -2;
            }
        
        if( Row < (P->BubblesInY-1) )
          if( Column > 0 )
            {
                int* NeighborB = &P->ConnectedBubbles[ Row+1 ][ Column-1 ];
                
                if( *NeighborB == BubbleColor )
                  NumberOfConnectedBubbles += Playfield_FindConnectedBubbles( P, Row+1, Column-1 );
                
                else if( *NeighborB > 0 )
                  *NeighborB = -2;
            }
    }
    
    return NumberOfConnectedBubbles;    
}

// ---------------------------------------------------------

void Playfield_FindSupportedBubbles( Playfield* P, int Row, int Column )
{
    // get the bubble color
    int* ThisBubble = &P->SupportedBubbles[ Row ][ Column ];
    int BubbleColor = *ThisBubble;
    
    if( BubbleColor <= 0 )
      return;
    
    // start counting from this bubble and mark
    // it as counted so it won't be counted twice
    *ThisBubble = -1;
    
    // recurse to count all neighbors in X,Y
    if( Row > 0 )
    {
        int* TopNeighbor = &P->SupportedBubbles[ Row-1 ][ Column ];
        
        if( *TopNeighbor > 0 )
          Playfield_FindSupportedBubbles( P, Row-1, Column );
    }
    
    if( Row < (P->BubblesInY-1) )
    {
        int* BottomNeighbor = &P->SupportedBubbles[ Row+1 ][ Column ];
        
        if( *BottomNeighbor > 0 )
          Playfield_FindSupportedBubbles( P, Row+1, Column );
    }
    
    if( Column > 0 )
    {
        int* LeftNeighbor = &P->SupportedBubbles[ Row ][ Column-1 ];
        
        if( *LeftNeighbor > 0 )
          Playfield_FindSupportedBubbles( P, Row, Column-1 );
    }
    
    if( Column < (P->BubblesInX-1) )
    {
        int* RightNeighbor = &P->SupportedBubbles[ Row ][ Column+1 ];
        
        if( *RightNeighbor > 0 )
          Playfield_FindSupportedBubbles( P, Row, Column+1 );
    }
    
    // recurse in the 2 remaining hexagon
    // sides, which depend on row parity
    if( Row % 2 )
    {
        if( Row > 0 )
          if( Column < (P->BubblesInX-1) )
            {
                int* NeighborA = &P->SupportedBubbles[ Row-1 ][ Column+1 ];
                
                if( *NeighborA > 0 )
                  Playfield_FindSupportedBubbles( P, Row-1, Column+1 );
            }
        
        if( Row < (P->BubblesInY-1) )
          if( Column < (P->BubblesInX-1) )
            {
                int* NeighborB = &P->SupportedBubbles[ Row+1 ][ Column+1 ];
                
                if( *NeighborB > 0 )
                  Playfield_FindSupportedBubbles( P, Row+1, Column+1 );
            }
    }
    
    else
    {
        if( Row > 0 )
          if( Column > 0 )
            {
                int* NeighborA = &P->SupportedBubbles[ Row-1 ][ Column-1 ];
                
                if( *NeighborA > 0 )
                  Playfield_FindSupportedBubbles( P, Row-1, Column-1 );
            }
        
        if( Row < (P->BubblesInY-1) )
          if( Column > 0 )
            {
                int* NeighborB = &P->SupportedBubbles[ Row+1 ][ Column-1 ];
                
                if( *NeighborB > 0 )
                  Playfield_FindSupportedBubbles( P, Row+1, Column-1 );
            }
    }    
}

// ---------------------------------------------------------

void Playfield_CreateFallingBubble( Playfield* P, int Row, int Column, int BubbleType )
{
    int BubbleMinX = P->MinX + BubbleRadius + 2;
    int BubbleMinY = P->CeilingY + BubbleRadius + 2;
    
    int CenterY = BubbleMinY + BubbleDistanceY * Row;
    int CenterX = BubbleMinX + BubbleDistanceX * Column;
    
    // offset odd rows
    if( Row % 2 )
      CenterX += BubbleDistanceX / 2;
    
    for( int i = 0; i < MaxFallingBubbles; ++i )
      if( P->FallingBubbles[ i ].Type <= 0 )
      {
          P->FallingBubbles[ i ].Center.x = CenterX;
          P->FallingBubbles[ i ].Center.y = CenterY;
          P->FallingBubbles[ i ].Type = BubbleType;
          return;
      }    
}

// ---------------------------------------------------------

void Playfield_UpdateFallingBubbles( Playfield* P )
{
    for( int i = 0; i < MaxFallingBubbles; ++i )
      if( P->FallingBubbles[ i ].Type > 0 )
      {
          P->FallingBubbles[ i ].Center.y *= 1.03;
          
          if( P->FallingBubbles[ i ].Center.y > (P->MaxY + 15) )
            P->FallingBubbles[ i ].Type = 0;  
      }
}

// ---------------------------------------------------------

// return indicates if bubbles were burst
bool Playfield_AddBubble( Playfield* P, Bubble* AddedBubble )
{
    // find the closest grid position
    int Row = (AddedBubble->Center.y - P->CeilingY) / BubbleDistanceY;
    int Column = (AddedBubble->Center.x - P->MinX) / BubbleDistanceX;
    
    if( Row % 2 )
      Column = (AddedBubble->Center.x - P->MinX - BubbleDistanceX/2) / BubbleDistanceX;
    
    // create the new bubble
    int BubbleMinX = P->MinX + BubbleRadius + 2;
    int BubbleMinY = P->CeilingY + BubbleRadius + 2;
    
    P->Bubbles[ Row ][ Column ] = AddedBubble->Type;
    P->AddedBubbleCenter.x = BubbleMinX + BubbleDistanceX*Column;
    P->AddedBubbleCenter.y = BubbleMinY + BubbleDistanceY*Row;
    
    if( Row % 2 )
      P->AddedBubbleCenter.x += BubbleDistanceX/2;
    
    // create a copy of the playfield
    memcpy( &P->ConnectedBubbles, &P->Bubbles, sizeof( P->Bubbles ) );
    
    // recursively find all connected bubbles of that same color
    P->LastShotConnectedBubbles = Playfield_FindConnectedBubbles( P, Row, Column );
    
    // if 3 or more bubbles became connected, iterate
    // to remove all of them from the playfield
    if( P->LastShotConnectedBubbles >= 3 )
    {
        for( int y = 0; y < P->BubblesInY; ++y )
          for( int x = 0; x < P->BubblesInX; ++x )
            if( P->ConnectedBubbles[y][x] == -1 )
              P->Bubbles[y][x] = 0;
    }
    
    // now iterate to find all bubbles that
    // have no attachment and must fall down
    P->LastShotFallingBubbles = 0;
    memcpy( &P->SupportedBubbles, &P->Bubbles, sizeof( P->Bubbles ) );
    
    for( int x = 0; x < P->BubblesInX; ++x )
      if( P->SupportedBubbles[0][x] > 0 )
        Playfield_FindSupportedBubbles( P, 0, x );
    
    for( int y = 0; y < P->BubblesInY; ++y )
      for( int x = 0; x < P->BubblesInX; ++x )
        if( P->SupportedBubbles[y][x] > 0 )
        {
            P->LastShotFallingBubbles++;
            Playfield_CreateFallingBubble( P, y, x, P->Bubbles[y][x] );
            P->Bubbles[y][x] = 0;
        }
    
    // play fanfare sound if the burst made other bubbles fall
    if( P->LastShotFallingBubbles > 0 )
      play_sound( SoundBurstFanfare );
    
    // indicate to the caller if there was burst
    return (P->LastShotConnectedBubbles >= 3);
}

// ---------------------------------------------------------

// starting point is filled in the ray itself
// returns true if bubble trajectory continues after this ray
bool Playfield_CastRay( Playfield* P, Ray* R )
{
    int BubbleMinX = P->MinX + BubbleRadius + 2;
    int BubbleMaxX = P->MaxX - BubbleRadius + 2;
    int BubbleMinY = P->CeilingY + BubbleRadius + 2;
    bool TrajectoryContinues = true;
    
    // make this ray active
    R->Active = true;
    
    // collide with ceiling
    float MinimumTime = (BubbleMinY - R->Start.y) / R->Velocity.y;
    
    // collide with left wall
    if( R->Velocity.x < 0 )
    {
        float Time = (BubbleMinX - R->Start.x) / R->Velocity.x;
        if( Time < MinimumTime ) MinimumTime = Time;
    }
    
    // collide with right wall
    if( R->Velocity.x > 0 )
    {
        float Time = (BubbleMaxX - R->Start.x) / R->Velocity.x;
        if( Time < MinimumTime ) MinimumTime = Time;        
    }
    
    // collide with level bubbles
    for( int y = 0; y < P->BubblesInY; ++y )
    {
        Vector2D BubbleCenter;
        BubbleCenter.y = BubbleMinY + BubbleDistanceY * y;
        BubbleCenter.x = BubbleMinX;
        int BubblesInRow = P->BubblesInX;
        
        // offset odd rows
        if( y % 2 )
        {
            BubbleCenter.x += BubbleDistanceX / 2;
            BubblesInRow--;
        }
        
        for( int x = 0; x < BubblesInRow; ++x )
        {
            int BubbleType = P->Bubbles[y][x];
            
            if( BubbleType )
            {
                float Time = CollideRayWithBubble( R, &BubbleCenter );
                
                if( Time < MinimumTime )
                {
                    MinimumTime = Time;
                    TrajectoryContinues = false;
                }
            }
            
            BubbleCenter.x += BubbleDistanceX;
        }
    }
    
    // now provide the position at that time
    R->End.x = R->Start.x + MinimumTime * R->Velocity.x;
    R->End.y = R->Start.y + MinimumTime * R->Velocity.y;
    
    // at ceiling, ray ends
    if( R->End.y <= (BubbleMinY+2) )
      TrajectoryContinues = false;
    
    return TrajectoryContinues;
}

// ---------------------------------------------------------

// returns true if there was a collision
bool Playfield_CollideBubble( Playfield* P, Bubble* CollidedBubble )
{
    int BubbleMinX = P->MinX + BubbleRadius + 2;
    int BubbleMinY = P->CeilingY + BubbleRadius + 2;
    
    // first, collide with the ceiling
    if( CollidedBubble->Center.y <= P->BubbleMinY )
    {
        // create new bubble in playfield
        CollidedBubble->Center.y = P->BubbleMinY;
        Playfield_AddBubble( P, CollidedBubble );
        return true;
    }
    
    // after that, collide with playfield bubbles
    for( int y = 0; y < P->BubblesInY; ++y )
    {
        Vector2D TestedCenter;
        TestedCenter.y = BubbleMinY + BubbleDistanceY * y;
        TestedCenter.x = BubbleMinX;
        int BubblesInRow = P->BubblesInX;
        
        // offset odd rows
        if( y % 2 )
        {
            TestedCenter.x += BubbleDistanceX / 2;
            BubblesInRow--;
        }
        
        for( int x = 0; x < BubblesInRow; ++x )
        {
            int BubbleType = P->Bubbles[y][x];
            
            if( BubbleType )
              if( DoBubblesCollide( &CollidedBubble->Center, &TestedCenter ) )
              {
                  Playfield_AddBubble( P, CollidedBubble );
                  return true;
              }
            
            TestedCenter.x += BubbleDistanceX;
        }
    }
    
    // collision not found
    return false;
}

// ---------------------------------------------------------

void Playfield_DrawFallingBubbles( Playfield* P )
{
    // darken effect
    set_multiply_color( make_gray( 192 ) );
    select_texture( TextureGameplay );
    
    for( int i = 0; i < MaxFallingBubbles; ++i )
      if( P->FallingBubbles[ i ].Type > 0 )
      {
          select_region( FirstRegionBubbles + P->FallingBubbles[ i ].Type );
          draw_region_at( P->FallingBubbles[ i ].Center.x, P->FallingBubbles[ i ].Center.y );
      }
    
    set_multiply_color( color_white );
}

// ---------------------------------------------------------

void Playfield_DrawDeathLine( Playfield* P )
{
    int PlayfieldWidth = P->MaxX - P->MinX + 1;
    select_region( RegionDeathZone );
    set_drawing_scale( PlayfieldWidth, 1 );
    draw_region_zoomed_at( P->MinX, P->MaxY );
}

// ---------------------------------------------------------

void Playfield_Draw( Playfield* P )
{    
    // draw descending ceiling
    //
    
    // draw bubbles
    for( int y = 0; y < P->BubblesInY; ++y )
    {
        int CenterY = P->BubbleMinY + BubbleDistanceY * y;
        int CenterX = P->BubbleMinX;
        int BubblesInRow = P->BubblesInX;
        
        // offset Y to alert of drop (in 2 levels)
        if( P->ShotsBeforeNextDescent == 2 )
          if( !(P->ElapsedFrames % 16) )
            CenterY -= 2;
        
        if( P->ShotsBeforeNextDescent == 1 )
          if( (P->ElapsedFrames % 6) < 2 )
            CenterY -= 2;
        
        // offset odd rows
        if( y % 2 )
        {
            CenterX += BubbleDistanceX / 2;
            BubblesInRow--;
        }
        
        for( int x = 0; x < BubblesInRow; ++x )
        {
            int BubbleType = P->Bubbles[y][x];
            
            if( BubbleType )
            {
                select_region( FirstRegionBubbles + BubbleType );
                draw_region_at( CenterX, CenterY );
            }
            
            CenterX += BubbleDistanceX;
        }
    }
    
    Playfield_DrawDeathLine( P );
}


// ---------------------------------------------------------
//   PLAYFIELD: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Playfield_RunState_Initialize( Playfield* P )
{
    // clear all bubbles
    // (the intro will fill them)
    memset( P->Bubbles, 0, sizeof(P->Bubbles) );
    
    // clear all falling bubbles
    for( int i = 0; i < MaxFallingBubbles; ++i )
      P->FallingBubbles[ i ].Type = 0;
      
    // reset state
    P->CeilingY = P->MinY;
    P->BubbleMinY = P->MinY + BubbleRadius + 2;
    P->CeilingDescendedRows = 0;
    P->ShotsBeforeNextDescent = P->ShotsPerDescent;
    P->LastShotConnectedBubbles = 0;
    P->LastShotFallingBubbles = 0;
    P->RemainingBubbles = 0;
    
    // transition immediately
    Playfield_ChangeState( P, Playfield_Wait );
}

// ---------------------------------------------------------

void Playfield_RunState_Wait( Playfield* P )
{
    Playfield_UpdateFallingBubbles( P );
    
    // we do not transition explicitely here:
    // other objects will be causing that
}

// ---------------------------------------------------------

void Playfield_RunState_CeilingDrop( Playfield* P )
{
    Playfield_UpdateFallingBubbles( P );
    
    // lower the ceiling
    if( P->ElapsedFrames % 2 )
    {
        P->BubbleMinY++;
        P->CeilingY++;
    }
    
    // when finished, go back to wait
    if( P->CeilingY >= P->MinY + (P->CeilingDescendedRows+1) * BubbleDistanceY )
    {
        P->CeilingDescendedRows++;
        
        if( !Playfield_CheckDeath( P ) )
          Playfield_ChangeState( P, Playfield_Wait );
    }
}

// ---------------------------------------------------------

void Playfield_RunState_LevelClear( Playfield* P )
{
    Playfield_UpdateFallingBubbles( P );
    
    // otherwise, go back to wait
    //
}

// ---------------------------------------------------------

void Playfield_RunState_Death( Playfield* P )
{
    Playfield_UpdateFallingBubbles( P );
    
}


// ---------------------------------------------------------
//   PLAYFIELD: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Playfield_RunStateMachine( Playfield* P )
{
    // optimize by checking for common states first
    if( P->State == Playfield_Wait )
      Playfield_RunState_Wait( P );
    
    else if( P->State == Playfield_CeilingDrop )
      Playfield_RunState_CeilingDrop( P );
    
    else if( P->State == Playfield_LevelClear )
      Playfield_RunState_LevelClear( P );
    
    else if( P->State == Playfield_Death )
      Playfield_RunState_Death( P );
    
    else if( P->State == Playfield_Initialize )
      Playfield_RunState_Initialize( P );
    
    // count time in present state
    P->ElapsedFrames++;
}
