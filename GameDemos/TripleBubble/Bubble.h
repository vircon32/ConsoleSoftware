// ---------------------------------------------------------
//   GAMEPLAY CONSTANTS
// ---------------------------------------------------------


// bubble size
#define BubbleDiameter  24 // on screen it's 26
#define BubbleRadius    (BubbleDiameter/2)

// bubble placement on grids
#define BubbleDistanceX 26
#define BubbleDistanceY 23

// shooting speed
#define BubbleSpeed     5


struct Bubble
{
    Vector2D Center;
    Vector2D Velocity;
    int Type;
};

// ---------------------------------------------------------

bool DoBubblesCollide( Vector2D* Center1, Vector2D* Center2 )
{
    return (DistanceBetween( Center1, Center2 ) < BubbleDiameter);
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
