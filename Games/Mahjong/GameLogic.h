// ---------------------------------------------------------
//   GAME RELATED DEFINITIONS
// ---------------------------------------------------------


// board dimensions, in number of grid positions
// (a full tile takes up 2x2 grid positions)
#define BoardWidth (16 * 2)
#define BoardHeight (8 * 2)
#define BoardLayers 5

// define types for our 3D layout grids
typedef int[ BoardHeight ][ BoardWidth ] BoardLayer;
typedef BoardLayer[ BoardLayers ] GameBoard;

// define similar types for calculated tile shadows;
// they will need 1 more tile across both X and Y
// since shadows can extend over the full layer size
typedef int[ BoardHeight + 1 ][ BoardWidth + 1 ] ShadowLayer;
typedef ShadowLayer[ BoardLayers ] ShadowBoard;

// global variables for our 3D layout arrays
GameBoard Board;
ShadowBoard Shadows;

// a full mahjong deck contains:
// - 4 full suits of 9 bamboo  (36 tiles)
// - 4 full suits of 9 dots    (36 tiles)
// - 4 full suits of 9 symbols (36 tiles)
// - 4 sets of 4 winds         (16 tiles)
// - 4 sets of 3 dragons       (12 tiles)
// - 1 set of 4 seasons        (04 tiles)
// - 1 set of 4 flower plants  (04 tiles)
#define TilesInDeck 144


// ---------------------------------------------------------
//   ALERT DIALOGS
// ---------------------------------------------------------


// possible dialog titles
enum AlertTitles
{
    AlertTitle_WrongMove,
    AlertTitle_NoPairs
};

// possible dialog body messages
enum AlertMessages
{
    AlertMessage_NoTile,
    AlertMessage_CannotPick,
    AlertMessage_CannotPair,
    AlertMessage_UndoOrRestart
};


// ---------------------------------------------------------
//   TILE LAYOUTS
// ---------------------------------------------------------


// the 4 layouts are taken from external files, layer by layer
embedded BoardLayer TurtleLayer0 = "obj/TurtleLayer0.vmap";
embedded BoardLayer TurtleLayer1 = "obj/TurtleLayer1.vmap";
embedded BoardLayer TurtleLayer2 = "obj/TurtleLayer2.vmap";
embedded BoardLayer TurtleLayer3 = "obj/TurtleLayer3.vmap";
embedded BoardLayer TurtleLayer4 = "obj/TurtleLayer4.vmap";

embedded BoardLayer BridgeLayer0 = "obj/BridgeLayer0.vmap";
embedded BoardLayer BridgeLayer1 = "obj/BridgeLayer1.vmap";
embedded BoardLayer BridgeLayer2 = "obj/BridgeLayer2.vmap";
embedded BoardLayer BridgeLayer3 = "obj/BridgeLayer3.vmap";
embedded BoardLayer BridgeLayer4 = "obj/BridgeLayer4.vmap";

embedded BoardLayer CastleLayer0 = "obj/CastleLayer0.vmap";
embedded BoardLayer CastleLayer1 = "obj/CastleLayer1.vmap";
embedded BoardLayer CastleLayer2 = "obj/CastleLayer2.vmap";
embedded BoardLayer CastleLayer3 = "obj/CastleLayer3.vmap";
embedded BoardLayer CastleLayer4 = "obj/CastleLayer4.vmap";

embedded BoardLayer PyramidsLayer0 = "obj/PyramidsLayer0.vmap";
embedded BoardLayer PyramidsLayer1 = "obj/PyramidsLayer1.vmap";
embedded BoardLayer PyramidsLayer2 = "obj/PyramidsLayer2.vmap";
embedded BoardLayer PyramidsLayer3 = "obj/PyramidsLayer3.vmap";
embedded BoardLayer PyramidsLayer4 = "obj/PyramidsLayer4.vmap";

// ---------------------------------------------------------

// now structure related layers as arrays to handle them together
typedef BoardLayer*[ BoardLayers ] BoardLayout;
BoardLayout LayoutTurtle   = { &TurtleLayer0  , &TurtleLayer1  , &TurtleLayer2  , &TurtleLayer3  , &TurtleLayer4   };
BoardLayout LayoutBridge   = { &BridgeLayer0  , &BridgeLayer1  , &BridgeLayer2  , &BridgeLayer3  , &BridgeLayer4   };
BoardLayout LayoutCastle   = { &CastleLayer0  , &CastleLayer1  , &CastleLayer2  , &CastleLayer3  , &CastleLayer4   };
BoardLayout LayoutPyramids = { &PyramidsLayer0, &PyramidsLayer1, &PyramidsLayer2, &PyramidsLayer3, &PyramidsLayer4 };

// ---------------------------------------------------------

// this function will initialize the board from a given layout
void CopyLayout( BoardLayout* Layout )
{
    for( int z = 0; z < BoardLayers; ++z )
      memcpy( Board[ z ], (*Layout)[ z ], sizeof( BoardLayer ) );
}


// ---------------------------------------------------------
//   BASIC TILE LOGIC
// ---------------------------------------------------------


int GetTileSuit( int Tile )
{
    // suits are 9 tiles, but our tileset makes them 2x2 
    return (Tile / 18);
}

// ---------------------------------------------------------

int GetTileValue( int Tile )
{
    // suits are 9 tiles, but our tileset makes them 2x2 
    return ((Tile % 18) / 2);
}

// ---------------------------------------------------------

// each tile has its 4 quarters stored in the layout, but
// only the top-left quarter for each tile is actually used
// to detect a tile; the other 3 quarters only help to
// determine if a tile is free and calculate their shadows
bool IsTileTopLeft( int Tile )
{
    return (((Tile / 18) % 2) == 0)
        && ((Tile % 2) == 0);
}

// ---------------------------------------------------------

bool DoTilesMatch( int Tile1, int Tile2 )
{
    int Suit1 = GetTileSuit( Tile1 );
    int Suit2 = GetTileSuit( Tile2 );
    if( Suit1 != Suit2 ) return false;
    
    int Value1 = GetTileValue( Tile1 );
    int Value2 = GetTileValue( Tile2 );
    
    // regular matches require the same tile
    if( Suit1 > 0 )
      return (Value1 == Value2);
    
    // bonus matches require only the same bonus type
    // (any 2 seasons or 2 plants can be matched)
    return ((Value1 <= 4) == (Value2 <= 4));
}


// ---------------------------------------------------------
//   IDENTIFY TILES WITHIN A LAYOUT
// ---------------------------------------------------------


struct TileInfo
{
    int x, y, z;    // position of top-left tile in the 3D array
    int ID;         // encodes tile suit and value (0 means tile was removed)
};

// This array is used through many parts of the code.
// Layouts will be processed to find each of the deck
// tiles, and the results are stored here. The tiles
// are stored in drawing order. From that point most
// of the game functions will process the layout using
// this array, and the layout 3D grid will only be used
// for very specific things, like checking a tile's
// adjacent positions to determine if it is selectable.
TileInfo[ TilesInDeck ] LayoutTiles;

// ---------------------------------------------------------

// careful! every layout is expected to contain
// exactly 144 tiles, each with their 4 quarters
void FindLayoutTiles()
{
    int ArrayPosition = 0;
    
    // tiles have to be found in drawing order,
    // so adjust iteration order for that
    for( int z = 0; z < BoardLayers; ++z )
    for( int x = BoardWidth-1; x >= 0; --x )
    for( int y = 0; y < BoardHeight; ++y )
    {
        int TileID = Board[ z ][ y ][ x ];
        if( !TileID ) continue;
        if( !IsTileTopLeft( TileID ) ) continue;
        
        LayoutTiles[ ArrayPosition ].x = x;
        LayoutTiles[ ArrayPosition ].y = y;
        LayoutTiles[ ArrayPosition ].z = z;
        LayoutTiles[ ArrayPosition ].ID = TileID;
        ArrayPosition++;
    }
}

// ---------------------------------------------------------

// this function sets a specific tile in the layout to
// a given face value (i.e. 3 bamboo); to do it correctly
// the tile will be set both in the 3D grid and in the
// corresponding index in the layout tiles array
void SetLayoutTile( int Index, int TileID )
{
    if( Index < 0 || Index >= TilesInDeck )
      return;
    
    // set tile ID (but not position) in the tile array
    TileInfo* Tile = &LayoutTiles[ Index ];
    Tile->ID = TileID;
    
    // now replace the 4 parts in the 3D array
    if( TileID == 0 )
    {
        // tile is removed, so all quarters become zeroes
        Board[ Tile->z ][ Tile->y   ][ Tile->x   ] = 0;
        Board[ Tile->z ][ Tile->y   ][ Tile->x+1 ] = 0;
        Board[ Tile->z ][ Tile->y+1 ][ Tile->x   ] = 0;
        Board[ Tile->z ][ Tile->y+1 ][ Tile->x+1 ] = 0;
    }
    
    else
    {
        Board[ Tile->z ][ Tile->y   ][ Tile->x   ] = Tile->ID;
        Board[ Tile->z ][ Tile->y   ][ Tile->x+1 ] = Tile->ID + 1;
        Board[ Tile->z ][ Tile->y+1 ][ Tile->x   ] = Tile->ID + 18;
        Board[ Tile->z ][ Tile->y+1 ][ Tile->x+1 ] = Tile->ID + 19;
    }
}


// ---------------------------------------------------------
//   GAMEPLAY LOGIC
// ---------------------------------------------------------


bool IsTileFree( TileInfo* Tile )
{
    // condition 1: it cannot be covered, even partially
    if( Tile->z < (BoardLayers - 1) )
    {
        if( Board[ Tile->z+1 ][ Tile->y   ][ Tile->x   ] )  return false;
        if( Board[ Tile->z+1 ][ Tile->y   ][ Tile->x+1 ] )  return false;
        if( Board[ Tile->z+1 ][ Tile->y+1 ][ Tile->x   ] )  return false;
        if( Board[ Tile->z+1 ][ Tile->y+1 ][ Tile->x+1 ] )  return false;
    }
    
    // condition 2: one of the sides has to be free
    if( Tile->x == 0 ) return true;
    if( Tile->x >= (BoardWidth - 2) ) return true;
    
    if( !Board[ Tile->z ][ Tile->y   ][ Tile->x+2 ]
    &&  !Board[ Tile->z ][ Tile->y+1 ][ Tile->x+2 ] )
      return true;
    
    if( !Board[ Tile->z ][ Tile->y   ][ Tile->x-1 ]
    &&  !Board[ Tile->z ][ Tile->y+1 ][ Tile->x-1 ] )
      return true;
    
    return false;
}

// ---------------------------------------------------------

int CountRemainingTiles()
{
    int RemainingTiles = 0;
    
    for( int i = 0; i < TilesInDeck; ++i )
      if( LayoutTiles[ i ].ID )
        RemainingTiles++;
    
    return RemainingTiles;
}

// ---------------------------------------------------------

// only called at game start when layers are
// being placed, so no need to check if the
// tile has been removed
bool IsLayerEmpty( int z )
{
    for( int i = 0; i < TilesInDeck; ++i )
      if( LayoutTiles[ i ].z == z )
        return false;
    
    return true;
}

// ---------------------------------------------------------

bool GameHasEnded()
{
    // game ends when no tiles remain
    for( int i = 0; i < TilesInDeck; i++ )
      if( LayoutTiles[ i ].ID != 0 )
        return false;
    
    return true;
}


// ---------------------------------------------------------
//   USER ACTIONS
// ---------------------------------------------------------


// all possible actions for a player
#define NumberOfActions 7

// can be drawn as: FirstRegionActions + <Action>
enum Actions
{
    Action_PickTile,
    Action_MakePair,
    Action_UndoPick,
    Action_UndoLast,
    Action_RedoNext,
    Action_ShowHint,
    Action_QuitGame
};

// global indicators of allowed actions
bool[ NumberOfActions ] ActionsAllowed;

// ---------------------------------------------------------

void ClearActions()
{
    for( int Action = 0; Action < NumberOfActions; Action++ )
      ActionsAllowed[ Action ] = false;
}
