// ---------------------------------------------------------
//   DEFINITIONS FOR THE UNDO STACK
// ---------------------------------------------------------


// basic definitions
#define UndoStackCapacity 10

struct UndoPair
{
    int Index1, ID1;
    int Index2, ID2;
};

// globals for the stack itself
UndoPair[ UndoStackCapacity ] UndoStack;
int UndoStackSize;
int UndoStackPosition;


// ---------------------------------------------------------
//   FUNCTIONS TO HANDLE THE UNDO STACK
// ---------------------------------------------------------


void ResetUndoStack()
{
    memset( &UndoStack, 0, sizeof( UndoStack ) );
    UndoStackSize = 0;
    UndoStackPosition = -1;
}

// ---------------------------------------------------------

// this adds a new matched pair to the stack, when the
// pair is formed by normal gameplay (i.e not a redo)
void AddUndoPair( int Index1, int Index2, int ID1, int ID2 )
{
    // case 1: rewrite previous redo stacks
    if( UndoStackPosition >= 0 && UndoStackPosition < (UndoStackSize-1) )
    {
        UndoStackPosition++;
        UndoStackSize = UndoStackPosition + 1;
    }
    
    // case 2: expand the stack
    else if( UndoStackSize < UndoStackCapacity )
    {
        UndoStackSize++;
        UndoStackPosition++;
    }
    
    // case 3: forget the first move and shift the rest to make place
    else
    {
        for( int i = 0; i < (UndoStackCapacity-1); ++i )
          UndoStack[ i ] = UndoStack[ i + 1 ];
    }
    
    // now fill the pair at the resulting current position
    UndoStack[ UndoStackPosition ].Index1 = Index1;
    UndoStack[ UndoStackPosition ].Index2 = Index2;
    UndoStack[ UndoStackPosition ].ID1 = ID1;
    UndoStack[ UndoStackPosition ].ID2 = ID2;
}

// ---------------------------------------------------------

void UndoLastPair()
{
    if( UndoStackSize <= 0 || UndoStackPosition < 0 )
      return;
    
    SetLayoutTile( UndoStack[ UndoStackPosition ].Index1, UndoStack[ UndoStackPosition ].ID1 );
    SetLayoutTile( UndoStack[ UndoStackPosition ].Index2, UndoStack[ UndoStackPosition ].ID2 );
    
    UndoStackPosition--;
}

// ---------------------------------------------------------

void RedoNextPair()
{
   if( UndoStackPosition >= (UndoStackSize - 1) )
     return;
   
   UndoStackPosition++;
   
   SetLayoutTile( UndoStack[ UndoStackPosition ].Index1, 0 );
   SetLayoutTile( UndoStack[ UndoStackPosition ].Index2, 0 );
}
