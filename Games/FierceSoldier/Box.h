// ---------------------------------------------------------
//   BASIC GEOMETRY STRUCTURES FOR BOXES
// ---------------------------------------------------------


// a basic 2D vector
struct Vector2D
{
   float x, y; 
};

// ---------------------------------------------------------

// information on a box's 4 sides
struct BoxSides
{
    bool Left, Right, Top, Bottom;
};


// ---------------------------------------------------------
//   AXIS-ALIGNED BOXES FOR COLLISION AND EVENT DETECTION
// ---------------------------------------------------------


// in this case we limit boxes to be static, since the only
// object in the game that moves and can collide is the
// player; we can handle collisions statically since there
// are never 2 moving objects, and tiles will have priority
struct Box
{
    // box shape definition
    Vector2D Position;
    Vector2D Hotspot;
    float Width, Height;
    
    // motion dynamics
    Vector2D Velocity;
    Vector2D Acceleration;
    
    // tentative movement in current frame
    Vector2D DeltaPosition;
    Vector2D DeltaVelocity;
    
    // currently active contacts
    BoxSides Contacts;
    BoxSides PreviousContacts;
};


// ---------------------------------------------------------
//   CURRENT BOX BOUNDARIES
// ---------------------------------------------------------


float Box_Left( Box* B )
{
    return B->Position.x - B->Hotspot.x;
}

// ---------------------------------------------------------

float Box_Right( Box* B )
{
    return B->Position.x - B->Hotspot.x + B->Width;
}
// ---------------------------------------------------------

float Box_Top( Box* B )
{
    return B->Position.y - B->Hotspot.y;
}

// ---------------------------------------------------------

float Box_Bottom( Box* B )
{
    return B->Position.y - B->Hotspot.y + B->Height;
}


// ---------------------------------------------------------
//   BOX BOUNDARIES EXPANDED WITH THIS FRAME'S MOTION
// ---------------------------------------------------------


float Box_ExpandedLeft( Box* B )
{
    return B->Position.x - B->Hotspot.x + min( 0, B->DeltaPosition.x );
}

// ---------------------------------------------------------

float Box_ExpandedRight( Box* B )
{
    return B->Position.x - B->Hotspot.x + B->Width + max( 0, B->DeltaPosition.x );
}
// ---------------------------------------------------------

float Box_ExpandedTop( Box* B )
{
    return B->Position.y - B->Hotspot.y + min( 0, B->DeltaPosition.y );
}

// ---------------------------------------------------------

float Box_ExpandedBottom( Box* B )
{
    return B->Position.y - B->Hotspot.y + B->Height + max( 0, B->DeltaPosition.y );
}


// ---------------------------------------------------------
//   GEOMETRIC RELATIONS OF BOXES
// ---------------------------------------------------------


// we don't consider just touching (i.e. boundary equality)
// to be overlapping, or else spikes would kill from neighbour tiles
bool Box_OverlapWith( Box* B, Box* B2 )
{
    bool CollisionX = (Box_Right( B ) > Box_Left( B2 ) && Box_Right( B2 ) > Box_Left( B ));
    bool CollisionY = (Box_Bottom( B ) > Box_Top( B2 ) && Box_Bottom( B2 ) > Box_Top( B ));
    return CollisionX && CollisionY;
}


// ---------------------------------------------------------
//   MOVEMENT SIMULATION FUNCTIONS
// ---------------------------------------------------------


void Box_FullStop( Box* B )
{
    memset( &B->Contacts, 0, sizeof(BoxSides) );
    memset( &B->PreviousContacts, 0, sizeof(BoxSides) );
    
    B->Velocity.x      = B->Velocity.y      = 0;
    B->Acceleration.x  = B->Acceleration.y  = 0;
    B->DeltaPosition.x = B->DeltaPosition.y = 0;
    B->DeltaVelocity.x = B->DeltaVelocity.y = 0;
}

// ---------------------------------------------------------

void Box_BeginNewFrame( Box* B )
{
    B->PreviousContacts = B->Contacts;
    memset( &B->Contacts, 0, sizeof(BoxSides) );
    
    B->Acceleration.x  = B->Acceleration.y  = 0;
    B->DeltaPosition.x = B->DeltaPosition.y = 0;
    B->DeltaVelocity.x = B->DeltaVelocity.y = 0;
}

// ---------------------------------------------------------

void Box_Simulate( Box* B )
{
    B->DeltaPosition.x = frame_time * B->Velocity.x;
    B->DeltaPosition.y = frame_time * B->Velocity.y;
    B->DeltaVelocity.x = frame_time * B->Acceleration.x;
    B->DeltaVelocity.y = frame_time * B->Acceleration.y;
}

// ---------------------------------------------------------

void Box_Move( Box* B )
{
    B->Position.x += B->DeltaPosition.x;
    B->Position.y += B->DeltaPosition.y;
    B->Velocity.x += B->DeltaVelocity.x;
    B->Velocity.y += B->DeltaVelocity.y;
}
