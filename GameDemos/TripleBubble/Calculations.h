// ---------------------------------------------------------
//   AUXILIARY FUNCTIONS
// ---------------------------------------------------------


float DistanceBetween( Vector2D* StartPoint, Vector2D* EndPoint )
{
    float DeltaX = EndPoint->x - StartPoint->x;
    float DeltaY = EndPoint->y - StartPoint->y;
    
    return sqrt( DeltaX * DeltaX + DeltaY * DeltaY );
}

// ---------------------------------------------------------

float AngleBetween( Vector2D* StartPoint, Vector2D* EndPoint )
{
    float DeltaX = EndPoint->x - StartPoint->x;
    float DeltaY = EndPoint->y - StartPoint->y;
    
    return atan2( DeltaY, DeltaX );
}

// ---------------------------------------------------------

// all connected bubbles of the same color are marked as -1
// all other bubbles (discarded) are marked as -2
int FindConnectedBubbles( int Row, int Column, int* MarkedBubbles )
{
    // get the bubble color
    int* ThisBubble = &MarkedBubbles[ BubblesInX*Row + Column ];
    int BubbleColor = *ThisBubble;
    
    if( BubbleColor <= 0 )
      return 0;
    
    // start counting from this bubble and mark
    // it as counted so it won't be counted twice
    int ConnectedBubbles = 1;
    *ThisBubble = -1;
    
    // recurse to count all neighbors in X,Y
    if( Row > 0 )
    {
        int* TopNeighbor = &MarkedBubbles[ BubblesInX*(Row-1) + Column ];
        
        if( *TopNeighbor == BubbleColor )
          ConnectedBubbles += FindConnectedBubbles( Row-1, Column, MarkedBubbles );
        
        else if( *TopNeighbor > 0 )
          *TopNeighbor = -2;
    }
    
    if( Row < (BubblesInY-1) )
    {
        int* BottomNeighbor = &MarkedBubbles[ BubblesInX*(Row+1) + Column ];
        
        if( *BottomNeighbor == BubbleColor )
          ConnectedBubbles += FindConnectedBubbles( Row+1, Column, MarkedBubbles );
        
        else if( *BottomNeighbor > 0 )
          *BottomNeighbor = -2;
    }
    
    if( Column > 0 )
    {
        int* LeftNeighbor = &MarkedBubbles[ BubblesInX*Row + (Column-1) ];
        
        if( *LeftNeighbor == BubbleColor )
          ConnectedBubbles += FindConnectedBubbles( Row, Column-1, MarkedBubbles );
        
        else if( *LeftNeighbor > 0 )
          *LeftNeighbor = -2;
    }
    
    if( Column < (BubblesInX-1) )
    {
        int* RightNeighbor = &MarkedBubbles[ BubblesInX*Row + (Column+1) ];
        
        if( *RightNeighbor == BubbleColor )
          ConnectedBubbles += FindConnectedBubbles( Row, Column+1, MarkedBubbles );
        
        else if( *RightNeighbor > 0 )
          *RightNeighbor = -2;
    }
    
    // recurse in the 2 remaining hexagon
    // sides, which depend on row parity
    if( Row % 2 )
    {
        if( Row > 0 )
          if( Column < (BubblesInX-1) )
            {
                int* NeighborA = &MarkedBubbles[ BubblesInX*(Row-1) + (Column+1) ];
                
                if( *NeighborA == BubbleColor )
                  ConnectedBubbles += FindConnectedBubbles( Row-1, Column+1, MarkedBubbles );
                
                else if( *NeighborA > 0 )
                  *NeighborA = -2;
            }
        
        if( Row < (BubblesInY-1) )
          if( Column < (BubblesInX-1) )
            {
                int* NeighborB = &MarkedBubbles[ BubblesInX*(Row+1) + (Column+1) ];
                
                if( *NeighborB == BubbleColor )
                  ConnectedBubbles += FindConnectedBubbles( Row+1, Column+1, MarkedBubbles );
                
                else if( *NeighborB > 0 )
                  *NeighborB = -2;
            }
    }
    
    else
    {
        if( Row > 0 )
          if( Column > 0 )
            {
                int* NeighborA = &MarkedBubbles[ BubblesInX*(Row-1) + (Column-1) ];
                
                if( *NeighborA == BubbleColor )
                  ConnectedBubbles += FindConnectedBubbles( Row-1, Column-1, MarkedBubbles );
                
                else if( *NeighborA > 0 )
                  *NeighborA = -2;
            }
        
        if( Row < (BubblesInY-1) )
          if( Column > 0 )
            {
                int* NeighborB = &MarkedBubbles[ BubblesInX*(Row+1) + (Column-1) ];
                
                if( *NeighborB == BubbleColor )
                  ConnectedBubbles += FindConnectedBubbles( Row+1, Column-1, MarkedBubbles );
                
                else if( *NeighborB > 0 )
                  *NeighborB = -2;
            }
    }
    
    return ConnectedBubbles;
}

// ---------------------------------------------------------

// all supported bubbles of any color are marked as -1
// all other bubbles are left unmarked
void FindSupportedBubbles( int Row, int Column, int* MarkedBubbles )
{
    // get the bubble color
    int* ThisBubble = &MarkedBubbles[ BubblesInX*Row + Column ];
    int BubbleColor = *ThisBubble;
    
    if( BubbleColor <= 0 )
      return;
    
    // start counting from this bubble and mark
    // it as counted so it won't be counted twice
    *ThisBubble = -1;
    
    // recurse to count all neighbors in X,Y
    if( Row > 0 )
    {
        int* TopNeighbor = &MarkedBubbles[ BubblesInX*(Row-1) + Column ];
        
        if( *TopNeighbor > 0 )
          FindSupportedBubbles( Row-1, Column, MarkedBubbles );
    }
    
    if( Row < (BubblesInY-1) )
    {
        int* BottomNeighbor = &MarkedBubbles[ BubblesInX*(Row+1) + Column ];
        
        if( *BottomNeighbor > 0 )
          FindSupportedBubbles( Row+1, Column, MarkedBubbles );
    }
    
    if( Column > 0 )
    {
        int* LeftNeighbor = &MarkedBubbles[ BubblesInX*Row + (Column-1) ];
        
        if( *LeftNeighbor > 0 )
          FindSupportedBubbles( Row, Column-1, MarkedBubbles );
    }
    
    if( Column < (BubblesInX-1) )
    {
        int* RightNeighbor = &MarkedBubbles[ BubblesInX*Row + (Column+1) ];
        
        if( *RightNeighbor > 0 )
          FindSupportedBubbles( Row, Column+1, MarkedBubbles );
    }
    
    // recurse in the 2 remaining hexagon
    // sides, which depend on row parity
    if( Row % 2 )
    {
        if( Row > 0 )
          if( Column < (BubblesInX-1) )
            {
                int* NeighborA = &MarkedBubbles[ BubblesInX*(Row-1) + (Column+1) ];
                
                if( *NeighborA > 0 )
                  FindSupportedBubbles( Row-1, Column+1, MarkedBubbles );
            }
        
        if( Row < (BubblesInY-1) )
          if( Column < (BubblesInX-1) )
            {
                int* NeighborB = &MarkedBubbles[ BubblesInX*(Row+1) + (Column+1) ];
                
                if( *NeighborB > 0 )
                  FindSupportedBubbles( Row+1, Column+1, MarkedBubbles );
            }
    }
    
    else
    {
        if( Row > 0 )
          if( Column > 0 )
            {
                int* NeighborA = &MarkedBubbles[ BubblesInX*(Row-1) + (Column-1) ];
                
                if( *NeighborA > 0 )
                  FindSupportedBubbles( Row-1, Column-1, MarkedBubbles );
            }
        
        if( Row < (BubblesInY-1) )
          if( Column > 0 )
            {
                int* NeighborB = &MarkedBubbles[ BubblesInX*(Row+1) + (Column-1) ];
                
                if( *NeighborB > 0 )
                  FindSupportedBubbles( Row+1, Column-1, MarkedBubbles );
            }
    }
}

// ---------------------------------------------------------

void CreateFallingBubble( int Row, int Column, int BubbleType )
{
    int CenterY = BubbleMinY + BubbleDistanceY * Row;
    int CenterX = BubbleMinX + BubbleDistanceX * Column;
    
    // offset odd rows
    if( Row % 2 )
      CenterX += BubbleDistanceX / 2;
    
    for( int i = 0; i < 100; ++i )
      if( FallingBubbles[ i ].Type <= 0 )
      {
          FallingBubbles[ i ].Center.x = CenterX;
          FallingBubbles[ i ].Center.y = CenterY;
          FallingBubbles[ i ].Type = BubbleType;
          return;
      }
}

// ---------------------------------------------------------

void AddBubbleToPlayfield( Bubble* ShotBubble )
{
    // find the closest grid position
    int PlayfieldMinY = BubbleMinY - BubbleRadius;
    int PlayfieldMinX = BubbleMinX - BubbleRadius;
    
    int Row = (ShotBubble->Center.y - PlayfieldMinY) / BubbleDistanceY;
    
    if( Row % 2 )
      PlayfieldMinX += BubbleDistanceX/2;
    
    int Column = (ShotBubble->Center.x - PlayfieldMinX) / BubbleDistanceX;
    
    // create the new bubble
    Bubbles[ Row ][ Column ] = ShotBubble->Type;
    AddedBubbleCenter.x = BubbleMinX + BubbleDistanceX*Column;
    AddedBubbleCenter.y = BubbleMinY + BubbleDistanceY*Row;
    
    if( Row % 2 )
      AddedBubbleCenter.x += BubbleDistanceX/2;
  
    // create a copy of the playfield
    memcpy( MarkedBubbles, Bubbles, sizeof( Bubbles ) );
    
    // recursively find all connected bubbles of that same color
    ConnectedBubbles = FindConnectedBubbles( Row, Column, &MarkedBubbles[0][0] );
    
    // if 3 or more bubbles became connected, iterate
    // to remove all of them from the playfield
    if( ConnectedBubbles >= 3 )
    {
        for( int y = 0; y < BubblesInY; ++y )
          for( int x = 0; x < BubblesInX; ++x )
            if( MarkedBubbles[y][x] == -1 )
              Bubbles[y][x] = 0;
    }
    
    // now iterate to find all bubbles that
    // have no attachment and must fall down
    bool ThereAreFallingBubbles = false;
    int[ BubblesInY ][ BubblesInX ] SupportedBubbles;
    memcpy( SupportedBubbles, Bubbles, sizeof( Bubbles ) );
    
    for( int x = 0; x < BubblesInX; ++x )
      if( SupportedBubbles[0][x] > 0 )
        FindSupportedBubbles( 0, x, &SupportedBubbles[0][0] );
    
    for( int y = 0; y < BubblesInY; ++y )
      for( int x = 0; x < BubblesInX; ++x )
        if( SupportedBubbles[y][x] > 0 )
        {
            ThereAreFallingBubbles = true;
            CreateFallingBubble( y, x, Bubbles[y][x] );
            Bubbles[y][x] = 0;
        }
    
    // play fanfare sound if the burst made other bubbles fall
    if( ThereAreFallingBubbles )
      play_sound( SoundBurstFanfare );
}

// ---------------------------------------------------------

// returns the time of collision, or
// 1000000 if no collision happens
float CollideRayWithBubble( Ray* R, Vector2D* BubbleCenter )
{
    // STEP 1: Find time at the nearest line point
    float ScalarProduct = ( (BubbleCenter->x - R->Start.x) * R->Velocity.x )
                        + ( (BubbleCenter->y - R->Start.y) * R->Velocity.y );
    
    float TimeAtNearest = ScalarProduct / (BubbleSpeed*BubbleSpeed);
    
    // STEP 2: Determine nearest point
    Vector2D NearestPoint = R->Start;
    NearestPoint.x += TimeAtNearest * R->Velocity.x;
    NearestPoint.y += TimeAtNearest * R->Velocity.y;
    
    // STEP 3: Calculate line-circle distance
    float MinimumDistance = DistanceBetween( &NearestPoint, BubbleCenter );
    
    // stop if no collision happens
    float TestRadius = 2 * BubbleRadius;
    
    if( MinimumDistance >= TestRadius )
      return 1000000;
    
    // STEP 4: Calculate delta time for intersections
    float DeltaDistance = sqrt( TestRadius*TestRadius - MinimumDistance*MinimumDistance );
    float DeltaTime = DeltaDistance / BubbleSpeed;
    
    return TimeAtNearest - DeltaTime;
}

// ---------------------------------------------------------

// returns true if the trajectory must continue
// (i.e. collision happened with the ceiling or a bubble)
bool CastRay( Ray* R )
{
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
    for( int y = 0; y < BubblesInY; ++y )
    {
        Vector2D BubbleCenter;
        BubbleCenter.y = BubbleMinY + BubbleDistanceY * y;
        BubbleCenter.x = BubbleMinX;
        int BubblesInRow = BubblesInX;
        
        // offset odd rows
        if( y % 2 )
        {
            BubbleCenter.x += BubbleDistanceX / 2;
            BubblesInRow = 15;
        }
        
        for( int x = 0; x < BubblesInRow; ++x )
        {
            int BubbleType = Bubbles[y][x];
            
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

void CalculateAimTrajectory( Vector2D* LaunchCenter, float LaunchAngle )
{
    // initialize
    Ray* CurrentRay = &AimTrajectory[ 0 ];
    CurrentRay->Start.x = LaunchCenter->x;
    CurrentRay->Start.y = LaunchCenter->y;
    
    CurrentRay->Velocity.x = BubbleSpeed * cos( LaunchAngle );
    CurrentRay->Velocity.y = BubbleSpeed * sin( LaunchAngle );
    
    // set 2nd and 3rd rays as initially inactive
    AimTrajectory[ 0 ].Active = true;
    AimTrajectory[ 1 ].Active = false;
    AimTrajectory[ 2 ].Active = false;
    
    // cast rays
    while( CastRay( CurrentRay ) )
    {
        // next ray starts at current ray end
        Ray* NextRay = CurrentRay + 1;
        NextRay->Start = CurrentRay->End;
        
        // reflect speed for next ray
        // (the only option is a wall bounce)
        NextRay->Velocity.x = -CurrentRay->Velocity.x;
        NextRay->Velocity.y = +CurrentRay->Velocity.y;
        
        // iterate
        CurrentRay = NextRay;
    }
}

// ---------------------------------------------------------

bool DoBubblesCollide( Vector2D* Center1, Vector2D* Center2 )
{
    return (DistanceBetween( Center1, Center2 ) < BubbleDiameter);
}

// ---------------------------------------------------------

bool CollideBubbleWithLevel( Bubble* ShotBubble )
{
    // collide with scenery bubbles
    for( int y = 0; y < BubblesInY; ++y )
    {
        Vector2D TestedCenter;
        TestedCenter.y = BubbleMinY + BubbleDistanceY * y;
        TestedCenter.x = BubbleMinX;
        int BubblesInRow = BubblesInX;
        
        // offset odd rows
        if( y % 2 )
        {
            TestedCenter.x += BubbleDistanceX / 2;
            BubblesInRow = 15;
        }
        
        for( int x = 0; x < BubblesInRow; ++x )
        {
            int BubbleType = Bubbles[y][x];
            
            if( BubbleType )
              if( DoBubblesCollide( &ShotBubble->Center, &TestedCenter ) )
              {
                  AddBubbleToPlayfield( ShotBubble );
                  return true;
              }
            
            TestedCenter.x += BubbleDistanceX;
        }
    }
    
    // collision not found
    return false;
}


