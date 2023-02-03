/* ---------------------------------------------------------
   This file is similar to the one for road objects, but
   here we implement a second list for mountains, which
   are always positioned much farther away and don't
   follow the same distance loop as closer objects.
 -------------------------------------------------------- */


// ---------------------------------------------------------
//   DEFINITIONS FOR OUR MOUNTAINS LIST
// ---------------------------------------------------------


// use empty objects as sentinel nodes
RoadObject MountainsStart, MountainsEnd;

// we will have fixed rows of a single mountain at each side
#define MOUNTAINS_IN_ROW 2
#define MOUNTAIN_ROWS 12
#define MAX_MOUNTAINS (MOUNTAIN_ROWS * MOUNTAINS_IN_ROW)

// use a pool of objects to avoid needing dynamic memory
RoadObject[ MAX_MOUNTAINS ] Mountains;


// ---------------------------------------------------------
//   LIST IMPLEMENTATION
// ---------------------------------------------------------


void Mountains_Initialize()
{
    MountainsStart.Previous = NULL;
    MountainsStart.Next = &MountainsEnd;
    MountainsEnd.Previous = &MountainsStart;
    MountainsEnd.Next = NULL;
}

// ---------------------------------------------------------

void Mountains_PushFront( RoadObject* NewObject )
{
    // insert as the first object
    NewObject->Previous = &MountainsStart;
    NewObject->Next = MountainsStart.Next;
    
    MountainsStart.Next->Previous = NewObject;
    MountainsStart.Next = NewObject;
}

// ---------------------------------------------------------

void Mountains_PushBack( RoadObject* NewObject )
{
    // insert as the last object
    NewObject->Previous = MountainsEnd.Previous;
    NewObject->Next = &MountainsEnd;
    
    MountainsEnd.Previous->Next = NewObject;
    MountainsEnd.Previous = NewObject;
}

// ---------------------------------------------------------

RoadObject* Mountains_PopFront()
{
    RoadObject* FirstObject = MountainsStart.Next;
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

RoadObject* Mountains_PopBack()
{
    RoadObject* LastObject = MountainsEnd.Previous;
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

// this function creates a set of mountains at both
// sides of the road, distributed evenly along their
// full distance range
void Mountains_Populate()
{
    for( int Row = 0; Row < MOUNTAIN_ROWS; ++Row )
    {
        float RowBaseZ = MountainNearZ + (MountainZRange * Row / MOUNTAIN_ROWS);
        int i = Row * MOUNTAINS_IN_ROW;
        
        // 1 mountain left, 1 right
        Mountains[ i ].Position.y = 0;
        Mountains[ i ].Position.z = RowBaseZ;
        Mountains[ i ].Position.x = -4000;
        Mountains[ i ].RegionID = RegionMountain;
        Mountains_PushFront( &Mountains[ i ] );
        
        Mountains[ i+1 ] = Mountains[ i ];
        Mountains[ i+1 ].Position.x = 4000;
        Mountains_PushFront( &Mountains[ i+1 ] );
    }
}

// ---------------------------------------------------------

// update our mountains as the scene changes
void Mountains_Update()
{
    float StepDistance = CarSpeed / 60.0;
    RoadObject* Object = Mountains;
    
    // update all objects
    for( int i = 0; i < MAX_OBJECTS; ++i )
    {
        Object->Position.z -= StepDistance;
        Object++;
    }
    
    // when going forward
    if( StepDistance > 0 )
    {
        RoadObject* LastObject = MountainsEnd.Previous;
        if( !LastObject ) return;
        
        while( LastObject != &MountainsStart )
        {
            if( LastObject->Position.z > MountainNearZ )
              break;
          
            RoadObject* RecycledObject = Mountains_PopBack();
            RecycledObject->Position.z += MountainZRange;
            Mountains_PushFront( RecycledObject );
            
            LastObject = MountainsEnd.Previous;
        }
    }
    
    // when going backwards
    if( StepDistance < 0 )
    {
        RoadObject* FirstObject = MountainsStart.Next;
        if( !FirstObject ) return;
        
        while( FirstObject != &MountainsEnd )
        {
            if( FirstObject->Position.z < MountainFarZ )
              break;
          
            RoadObject* RecycledObject = Mountains_PopFront();
            RecycledObject->Position.z -= MountainZRange;
            Mountains_PushBack( RecycledObject );
            
            FirstObject = MountainsStart.Next;
        }
    }
}

// ---------------------------------------------------------

// draw all our mountains on screen
void Mountains_Draw()
{
    RoadObject* Object = MountainsStart.Next;
    
    while( Object != &MountainsEnd )
    {
        // calculate position on screen
        CalculateScreenPosition( &Object->Position, &Object->ScreenX, &Object->ScreenY );
        
        // we need to atenuate a bit the transparency effect
        // since mountains are normally quite farther away
        set_multiply_color( LineMultiplyColor[ Object->ScreenY + 18 ] );
        
        // for mountains the stored per-line scale factors are
        // too imprecise, so we have to calculate them; we also
        // enlarge the scale factor to avoid needing large regions
        float ScaleFactor = 300.0 / (Object->Position.z + 256.0);
        set_drawing_scale( ScaleFactor*10, ScaleFactor*10 );
        
        // draw on screen
        select_region( Object->RegionID );
        draw_region_zoomed_at( Object->ScreenX, Object->ScreenY );
        
        // iterate
        Object = Object->Next;
    }
    
    // restore multiply color
    set_multiply_color( color_white );
}
