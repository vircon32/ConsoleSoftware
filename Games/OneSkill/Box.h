// *****************************************************************************
    // start include guard
    #ifndef BOX_H
    #define BOX_H
// *****************************************************************************


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
//   FUNCTIONS FOR TREATING BOXES
// ---------------------------------------------------------


// obtain current box boundaries
float Box_Left( Box* B );
float Box_Right( Box* B );
float Box_Top( Box* B );
float Box_Bottom( Box* B );

// box boundaries expanded with this frame's motion
float Box_ExpandedLeft( Box* B );
float Box_ExpandedRight( Box* B );
float Box_ExpandedTop( Box* B );
float Box_ExpandedBottom( Box* B );

// geometric relations between boxes
bool Box_OverlapWith( Box* B, Box* B2 );

// box movement simulation
void Box_FullStop( Box* B );
void Box_BeginNewFrame( Box* B );
void Box_Simulate( Box* B );
void Box_Move( Box* B );


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
