// ---------------------------------------------------------
//   DEFINITIONS USED FOR SOLVING AND SHUFFLING A LAYOUT
// ---------------------------------------------------------


// we will need to save a copy of our layout while rearranging it
TileInfo[ TilesInDeck ] CopiedLayoutTiles;

// ---------------------------------------------------------

// data structures to hold free tiles
struct FreeTile
{
    int Index;
    bool Paired;
};

struct FreeTileArray
{
    FreeTile[ TilesInDeck ] Tiles;
    int NumberOfTiles;
};

// all free tiles are stored here
FreeTileArray AllFreeTiles;

// ---------------------------------------------------------

// data structures to store free pairs
struct FreePair
{
    int Index1, Index2;
};

struct FreePairArray
{
    FreePair[ TilesInDeck/2 ] Pairs;
    int NumberOfPairs;
};

// all free pairs are stored here 
FreePairArray AllFreePairs;


// ---------------------------------------------------------
//   INITIAL "NAIVE" LAYOUT SHUFFLE
// ---------------------------------------------------------


// after loading, the arrays have tiles arranged in a constant order;
// we will need to shuffle them so that every game is different;
// this function does an initial shuffle which will not take any
// measure to ensure the final layout is actually solvable
void ShuffleLayoutTiles()
{
    for( int i = 0; i < 500; ++i )
    {
        // choose 2 different tiles
        int Position1 = rand() % TilesInDeck;
        int Position2 = rand() % TilesInDeck;
        if( Position1 == Position2 ) continue;
        
        // swap them
        int Tile1ID = LayoutTiles[ Position1 ].ID;
        int Tile2ID = LayoutTiles[ Position2 ].ID;
        SetLayoutTile( Position1, Tile2ID );
        SetLayoutTile( Position2, Tile1ID );
    }
}


// ---------------------------------------------------------
//   BASIC SOLVING FUNCTIONS
// ---------------------------------------------------------


// finds all free tiles in the current layout,
// and saves them in our array AllFreeTiles
void FindAllFreeTiles()
{
    int NumberOfTiles = 0;
    
    for( int i = 0; i < TilesInDeck; ++i )
    {
        TileInfo* Tile = &LayoutTiles[ i ];
        if( !Tile->ID ) continue;
        if( !IsTileFree( Tile ) ) continue;
        
        AllFreeTiles.Tiles[ NumberOfTiles ].Index = i;
        AllFreeTiles.Tiles[ NumberOfTiles ].Paired = false;
        NumberOfTiles++;
    }
    
    AllFreeTiles.NumberOfTiles = NumberOfTiles;
}

// ---------------------------------------------------------

// finds all free matchable pairs in the current
// layout, and saves them in our array AllFreePairs
// (if 4 matchable tiles are free, it will only
// include one of the possible ways to pair them)
void FindAllFreePairs()
{
    FindAllFreeTiles();
    int NumberOfPairs = 0;
    
    // iterate (i) over free tiles that are not yet paired
    for( int i = 0; i < AllFreeTiles.NumberOfTiles; ++i )
    {
        if( AllFreeTiles.Tiles[ i ].Paired )
          continue;
        
        int IndexI = AllFreeTiles.Tiles[ i ].Index;
        int TileIDI = LayoutTiles[ IndexI ].ID;
        
        // iterate (j) to find a suitable pair for tile (i)
        for( int j = 0; j < AllFreeTiles.NumberOfTiles; ++j )
        {
            // tiles have to be different and unpaired
            if( i == j ) continue;
            if( AllFreeTiles.Tiles[ j ].Paired ) continue;
            
            int IndexJ = AllFreeTiles.Tiles[ j ].Index;
            int TileIDJ = LayoutTiles[ IndexJ ].ID;
            
            // both tiles have to be matchable
            if( DoTilesMatch( TileIDI, TileIDJ ) )
            {
                // mark both as paired
                AllFreeTiles.Tiles[ i ].Paired = true;
                AllFreeTiles.Tiles[ j ].Paired = true;
                
                // insert the pair in AllFreePairs
                AllFreePairs.Pairs[ NumberOfPairs ].Index1 = IndexI;
                AllFreePairs.Pairs[ NumberOfPairs ].Index2 = IndexJ;
                NumberOfPairs++;
                break;
            }
        }
    }
    
    AllFreePairs.NumberOfPairs = NumberOfPairs;
}

// ---------------------------------------------------------

// given a specific free tile, it will find another
// tile in the layout that can be matched with it
int FindMatchingTile( int TileIndex )
{
    int TileID = LayoutTiles[ TileIndex ].ID;
    
    for( int i = 0; i < TilesInDeck; ++i )
    {
        if( i == TileIndex ) continue;
        if( !LayoutTiles[ i ].ID ) continue;
        
        if( DoTilesMatch( TileID, LayoutTiles[ i ].ID ) )
          return i;
    }
    
    // this should never happen unless we have a bug
    return -1;
}


// ---------------------------------------------------------
//   ACTUAL LAYOUT MODIFIER FUNCTIONS
// ---------------------------------------------------------


// this function will modify the layout so that 2 of the
// currently free tiles can be matched; to do so, it will
// look for a suitable match for one of them and perform
// a swap; this will make unselectable one of the free
// tiles, in exchange for making the needed match tile free
bool MakeMatchingPair()
{
    if( AllFreeTiles.NumberOfTiles < 2 )
      return false;
    
    // select 2 random tiles from AllFreeTiles
    int FreeTile1 = rand() % AllFreeTiles.NumberOfTiles;
    int FreeTile2 = rand() % AllFreeTiles.NumberOfTiles;
    if( FreeTile1 == FreeTile2 ) FreeTile2 = (FreeTile1 + 1) % AllFreeTiles.NumberOfTiles;
    
    // get their indices within LayoutTiles
    int Index1 = AllFreeTiles.Tiles[ FreeTile1 ].Index;
    int Index2 = AllFreeTiles.Tiles[ FreeTile2 ].Index;
    
    // find a matching tile for the first one
    int IndexMatch = FindMatchingTile( Index1 );
    if( IndexMatch < 0 ) return false;
    
    // swap the second tile with the match for the first
    // (this is done BOTH in LayoutTiles and CopiedLayoutTiles)
    int TileID2 = LayoutTiles[ Index2 ].ID;
    int TileIDMatch = LayoutTiles[ IndexMatch ].ID;
    
    LayoutTiles[ Index2 ].ID = TileIDMatch;
    LayoutTiles[ IndexMatch ].ID = TileID2;
    
    CopiedLayoutTiles[ Index2 ].ID = TileIDMatch;
    CopiedLayoutTiles[ IndexMatch ].ID = TileID2;
    
    // signal success
    return true;
}

// ---------------------------------------------------------

// this function will take a potentially unsolvable layout,
// and make any necessary changes so that it can be solved;
// to do this it follows a step-by-step resolution, removing
// pairs and fabricating matchable pairs when none are left
void MakeLayoutSolvable()
{
    BeginSolver:
    
    // make a backup copy so that we don't lose information
    // in the process of solving the board (i.e. removing tiles)
    memcpy( CopiedLayoutTiles, LayoutTiles, sizeof(LayoutTiles) );
    
    while( CountRemainingTiles() > 0 )
    {
        // find all free pairs, or make one if not any
        FindAllFreePairs();
        
        if( AllFreePairs.NumberOfPairs <= 0 )
        {
            // attempt to make a new free pair
            if( !MakeMatchingPair() )
            {
                // reset and reshuffle when it was not possible
                memcpy( LayoutTiles, CopiedLayoutTiles, sizeof(LayoutTiles) );
                ShuffleLayoutTiles();
                goto BeginSolver;
            }
            
            continue;
        }
        
        // remove all pairs in AllFreePairs
        // (ONLY in LayoutTiles, not in the copy)
        for( int i = 0; i < AllFreePairs.NumberOfPairs; ++i )
        {
            SetLayoutTile( AllFreePairs.Pairs[ i ].Index1, 0 );
            SetLayoutTile( AllFreePairs.Pairs[ i ].Index2, 0 );
        }
    }
    
    // now set all tiles BOTH in LayoutTiles and in the board
    for( int i = 0; i < TilesInDeck; ++i )
      SetLayoutTile( i, CopiedLayoutTiles[ i ].ID );
}
