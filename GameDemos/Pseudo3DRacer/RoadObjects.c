/* ---------------------------------------------------------
   In this file we implement objects that are drawn at
   the sides of the road. Since they must be drawn from
   farthest to closest, we implement a list of road
   objects that we will just cycle (since this demo
   only has a closed loop). But in a real game new
   objects would be inserted for each different zone.
 -------------------------------------------------------- */


// ---------------------------------------------------------
//   AUXILIARY FUNCTIONS
// ---------------------------------------------------------


// convert 3D object positions to 2D drawing positions on screen
void CalculateScreenPosition( Vector3D* Position, int* ScreenX, int* ScreenY )
{
    // calculate screen Y first
    float Y2D = TrueHorizonY + (300.0 * 256.0 / (600.0 / 512.0)) / (Position->z + 256.0);
    *ScreenY = Y2D;         // (300.0 * 256.0 / (600.0 / 512.0)) = 65536     
    
    // apply the same displacements on X as we do for
    // the road center, to make perspective consistent
    float CurveYFactor = ((screen_height-1.0) - TrueHorizonY) / (Y2D - (TrueHorizonY-10));
    float CenterX = screen_width/2 + CurveXFactor * CurveYFactor;
    CenterX -= 7 * CurveXFactor;
    
    // now take into account the object's x position respect to the road center
    *ScreenX = CenterX + (Position->x / 250.0) * (300.0 * 256.0 / (600.0 / 512.0)) / (Position->z + 256.0);
}


// ---------------------------------------------------------
//   DEFINITIONS FOR OUR OBJECTS LIST
// ---------------------------------------------------------


// data we need to store for each object
struct RoadObject
{
    Vector3D Position;
    int ScreenX, ScreenY;
    int RegionID;
    RoadObject* Next;
    RoadObject* Previous;
};

// our list uses empty objects as sentinel nodes
RoadObject ListStart, ListEnd;

// we will have fixed rows of objects, in checkerboard pattern
#define OBJECTS_IN_ROW (2*(3+2)) // 10
#define OBJECT_ROWS 20
#define MAX_OBJECTS (OBJECT_ROWS * OBJECTS_IN_ROW)

// use a pool of objects to avoid needing dynamic memory
RoadObject[ MAX_OBJECTS ] RoadObjects;


// ---------------------------------------------------------
//   LIST IMPLEMENTATION
// ---------------------------------------------------------


void Objects_Initialize()
{
    ListStart.Previous = NULL;
    ListStart.Next = &ListEnd;
    ListEnd.Previous = &ListStart;
    ListEnd.Next = NULL;
}

// ---------------------------------------------------------

// unused function, but could be useful for a list
void Objects_Insert( RoadObject* NewObject )
{
    if( !ListStart.Next )
    {
        ListStart.Next = NewObject;
        ListEnd.Previous = NewObject;
        
        NewObject->Previous = &ListStart;
        NewObject->Next = &ListEnd;
    }
    
    // in other cases sort by distance
    // (furthest to closest)
    else
    {
        RoadObject* Current = ListStart.Next;
        
        while( Current != &ListEnd )
        {
            if( NewObject->Position.x > Current->Position.x )
            {
                // make links for inserted object
                NewObject->Previous = Current->Previous;
                NewObject->Next = Current;
                
                // adjust links of neighbor objects
                Current->Previous->Next = NewObject;
                Current->Previous = NewObject;
                
                return;
            }
            
            Current = Current->Next;
        }
        
        // insert as the last object
        NewObject->Previous = ListEnd.Previous;
        NewObject->Next = &ListEnd;
        
        ListEnd.Previous->Next = NewObject;
        ListEnd.Previous = NewObject;
    }
}

// ---------------------------------------------------------

// unused function, but could be useful for a list
void Objects_Remove( RoadObject* OldObject )
{
    // adjust links of neighbor objects
    OldObject->Previous->Next = OldObject->Next;
    OldObject->Next->Previous = OldObject->Previous;
    
    // delete any links from removed object
    OldObject->Previous = NULL;
    OldObject->Next = NULL;
}

// ---------------------------------------------------------

void Objects_PushFront( RoadObject* NewObject )
{
    // insert as the first object
    NewObject->Previous = &ListStart;
    NewObject->Next = ListStart.Next;
    
    ListStart.Next->Previous = NewObject;
    ListStart.Next = NewObject;
}

// ---------------------------------------------------------

void Objects_PushBack( RoadObject* NewObject )
{
    // insert as the last object
    NewObject->Previous = ListEnd.Previous;
    NewObject->Next = &ListEnd;
    
    ListEnd.Previous->Next = NewObject;
    ListEnd.Previous = NewObject;
}

// ---------------------------------------------------------

RoadObject* Objects_PopFront()
{
    RoadObject* FirstObject = ListStart.Next;
    if( !FirstObject ) return NULL;
    
    // adjust links of neighbor objects
    FirstObject->Previous->Next = FirstObject->Next;
    FirstObject->Next->Previous = FirstObject->Previous;
    
    // delete any links from removed object
    FirstObject->Previous = NULL;
    FirstObject->Next = NULL;
    
    return FirstObject;
}

// ---------------------------------------------------------

RoadObject* Objects_PopBack()
{
    RoadObject* LastObject = ListEnd.Previous;
    if( !LastObject ) return NULL;
    
    // adjust links of neighbor objects
    LastObject->Previous->Next = LastObject->Next;
    LastObject->Next->Previous = LastObject->Previous;
    
    // delete any links from removed object
    LastObject->Previous = NULL;
    LastObject->Next = NULL;
    
    return LastObject;
}

// ---------------------------------------------------------

// this function creates a set of plants at both
// sides of the road, distributed in a checkerboard
// pattern, along their full distance range
void Objects_Populate()
{
    for( int Row = 0; Row < OBJECT_ROWS; ++Row )
    {
        float RowBaseZ = ObjectNearZ + (ObjectZRange * Row / OBJECT_ROWS);
        int i = Row * OBJECTS_IN_ROW;
        
        // 6 plants in the front
        RoadObjects[ i ].Position.y = 0;
        RoadObjects[ i ].Position.z = RowBaseZ;
        RoadObjects[ i ].Position.x = 400;
        RoadObjects[ i ].RegionID = RegionRoadObject2;
        Objects_PushFront( &RoadObjects[ i ] );
        
        RoadObjects[ i+1 ] = RoadObjects[ i ];
        RoadObjects[ i+1 ].Position.x = 1000;
        Objects_PushFront( &RoadObjects[ i+1 ] );
        
        RoadObjects[ i+2 ] = RoadObjects[ i ];
        RoadObjects[ i+2 ].Position.x = 1600;
        Objects_PushFront( &RoadObjects[ i+2 ] );
        
        RoadObjects[ i+3 ] = RoadObjects[ i ];
        RoadObjects[ i+3 ].Position.x = -400;
        Objects_PushFront( &RoadObjects[ i+3 ] );
        
        RoadObjects[ i+4 ] = RoadObjects[ i ];
        RoadObjects[ i+4 ].Position.x = -1000;
        Objects_PushFront( &RoadObjects[ i+4 ] );
        
        RoadObjects[ i+5 ] = RoadObjects[ i ];
        RoadObjects[ i+5 ].Position.x = -1600;
        Objects_PushFront( &RoadObjects[ i+5 ] );
        
        // 4 plants in the rear
        RoadObjects[ i+6 ] = RoadObjects[ i ];
        RoadObjects[ i+6 ].Position.z += ObjectZRange / (2 * OBJECT_ROWS);
        RoadObjects[ i+6 ].Position.x = 700;
        Objects_PushFront( &RoadObjects[ i+6 ] );
        
        RoadObjects[ i+7 ] = RoadObjects[ i+6 ];
        RoadObjects[ i+7 ].Position.x = 1300;
        Objects_PushFront( &RoadObjects[ i+7 ] );
        
        RoadObjects[ i+8 ] = RoadObjects[ i+6 ];
        RoadObjects[ i+8 ].Position.x = -700;
        Objects_PushFront( &RoadObjects[ i+8 ] );
        
        RoadObjects[ i+9 ] = RoadObjects[ i+6 ];
        RoadObjects[ i+9 ].Position.x = -1300;
        Objects_PushFront( &RoadObjects[ i+9 ] );        
    }
}

// ---------------------------------------------------------

// update our objects as the scene changes
void Objects_Update()
{
    float StepDistance = CarSpeed / 60.0;
    RoadObject* Object = RoadObjects;
    
    // update all objects
    for( int i = 0; i < MAX_OBJECTS; ++i )
    {
        Object->Position.z -= StepDistance;
        Object++;
    }
    
    // when going forward
    if( StepDistance > 0 )
    {
        RoadObject* LastObject = ListEnd.Previous;
        if( !LastObject ) return;
        
        while( LastObject != &ListStart )
        {
            if( LastObject->Position.z > ObjectNearZ )
              break;
          
            RoadObject* RecycledObject = Objects_PopBack();
            RecycledObject->Position.z += ObjectZRange;
            Objects_PushFront( RecycledObject );
            
            LastObject = ListEnd.Previous;
        }
    }
    
    // when going backwards
    if( StepDistance < 0 )
    {
        RoadObject* FirstObject = ListStart.Next;
        if( !FirstObject ) return;
        
        while( FirstObject != &ListEnd )
        {
            if( FirstObject->Position.z < ObjectFarZ )
              break;
          
            RoadObject* RecycledObject = Objects_PopFront();
            RecycledObject->Position.z -= ObjectZRange;
            Objects_PushBack( RecycledObject );
            
            FirstObject = ListStart.Next;
        }
    }
}

// ---------------------------------------------------------

// draw all our road objects on screen
void Objects_Draw()
{
    RoadObject* Object = ListStart.Next;
    
    while( Object != &ListEnd )
    {
        // calculate position on screen
        CalculateScreenPosition( &Object->Position, &Object->ScreenX, &Object->ScreenY );
        
        // draw on screen
        select_region( Object->RegionID );
        set_multiply_color( LineMultiplyColor[ Object->ScreenY ] );
        
        float ScaleFactor = LineDrawingScale[ Object->ScreenY ];
        
        if( Object->Position.x < 0 )
          set_drawing_scale( -ScaleFactor*4, ScaleFactor*4 );
        else
          set_drawing_scale( ScaleFactor*4, ScaleFactor*4 );
        
        draw_region_zoomed_at( Object->ScreenX, Object->ScreenY - 3 );
        
        // iterate
        Object = Object->Next;
    }
    
    // restore multiply color
    set_multiply_color( color_white );
}
