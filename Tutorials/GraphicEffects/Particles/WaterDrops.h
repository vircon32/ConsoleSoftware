// ---------------------------------------------------------
//   DEFINITIONS FOR OUR LISTS OF WATER DROPS
// ---------------------------------------------------------


// our physics constants
vector2d Gravity = { 0, 1000 };
#define BounceImpulse 300

// data we need to store for each drop
struct WaterDrop
{
    // drop info
    vector2d Position;
    vector2d Velocity;
    
    // list node info
    WaterDrop* Next;
    WaterDrop* Previous;
};

// our lists use empty objects as sentinel nodes;
// they also include a count of the stored elements
struct List
{
    WaterDrop Start;
    WaterDrop End;
    int NumberOfDrops;
};

// our 2 lists separate drops by size
// (their behavior is slightly different too)
List LargeDrops, SmallDrops;


// ---------------------------------------------------------
//   AUXILIARY FUNCTIONS
// ---------------------------------------------------------


// just an easier way to produce random
// float numbers inside a given interval
float RandomFloatBetween( float Minimum, float Maximum )
{
    int RangeHundredths = (Maximum - Minimum) * 100;
    return Minimum + (rand() % (RangeHundredths + 1)) / 100.0;
}


// ---------------------------------------------------------
//   LIST IMPLEMENTATION
// ---------------------------------------------------------


// creates the sentinel nodes for
// an initially empty water drop list
void List_Initialize( List* L )
{
    L->Start.Previous = NULL;
    L->Start.Next = &L->End;
    L->End.Previous = &L->Start;
    L->End.Next = NULL;
}

// ---------------------------------------------------------

// after removing, it returns the previous drop
// so that list iterations can keep going
WaterDrop* List_Remove( WaterDrop* OldDrop )
{
    // adjust links of neighbor drops
    OldDrop->Previous->Next = OldDrop->Next;
    OldDrop->Next->Previous = OldDrop->Previous;
    WaterDrop* Previous = OldDrop->Previous;
    
    // free memory from removed drop
    free( OldDrop );
    
    // provide previous node
    return Previous;
}

// ---------------------------------------------------------

// the list is unordered, so it will simply
// create and add a new drop as first element
// (drop properties to be initialized by the caller)
WaterDrop* List_Add( List* L )
{
    // allocate memory for new drop
    WaterDrop* NewDrop = malloc( sizeof(WaterDrop) );
    
    // insert as new first drop
    NewDrop->Next = L->Start.Next;
    NewDrop->Previous = &L->Start;
    L->Start.Next = NewDrop;
    NewDrop->Next->Previous = NewDrop;
}

// ---------------------------------------------------------

// draw on screen all water drops in a same list
// (all of them will share the same texture region)
void List_Draw( List* L, int DropRegionID )
{
    // initialize
    select_region( DropRegionID );
    L->NumberOfDrops = 0;
    
    // iterate the list
    WaterDrop* Drop = L->Start.Next;
    
    while( Drop != &L->End )
    {
        // draw this drop
        draw_region_at( Drop->Position.x, Drop->Position.y );
        L->NumberOfDrops++;
        
        // iterate
        Drop = Drop->Next;
    }
}

// ---------------------------------------------------------

// update our large water drops falling from the tap
void UpdateLargeDrops()
{
    WaterDrop* Drop = LargeDrops.Start.Next;
    
    while( Drop != &LargeDrops.End )
    {
        // apply gravity acceleration
        vector2d Product;
        v2_multiply( &Gravity, frame_time, &Product );
        v2_self_add( &Drop->Velocity, &Product );
        
        // apply velocity
        v2_multiply( &Drop->Velocity, frame_time, &Product );
        v2_self_add( &Drop->Position, &Product );
        
        // when reaching surface, replace this
        // drop with a new small drop bouncing up
        if( Drop->Position.y >= 304 )
        {
            // create new small drop
            WaterDrop* BouncingDrop = List_Add( &SmallDrops );
            BouncingDrop->Position.x = Drop->Position.x;
            BouncingDrop->Position.y = 308;
            BouncingDrop->Velocity.y = -RandomFloatBetween( BounceImpulse * 0.6, BounceImpulse * 1.3 );
            
            // speed on X will depend on its initial X position
            float OffsetX = BouncingDrop->Position.x - 400;
            BouncingDrop->Velocity.x = -320 + OffsetX * 14;
            
            // remove large drop
            Drop = List_Remove( Drop );
        }
        
        // iterate
        Drop = Drop->Next;
    }
}

// ---------------------------------------------------------

// update our small water drops bouncing upwards
void UpdateSmallDrops()
{
    WaterDrop* Drop = SmallDrops.Start.Next;
    
    while( Drop != &SmallDrops.End )
    {
        // apply gravity acceleration
        vector2d Product;
        v2_multiply( &Gravity, frame_time, &Product );
        v2_self_add( &Drop->Velocity, &Product );
        
        // apply velocity
        v2_multiply( &Drop->Velocity, frame_time, &Product );
        v2_self_add( &Drop->Position, &Product );
        
        // when out of screen, remove
        if( Drop->Position.y > 310 )
          Drop = List_Remove( Drop );
        
        // iterate
        Drop = Drop->Next;
    }
}
