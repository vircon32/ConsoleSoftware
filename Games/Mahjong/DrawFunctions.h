// ---------------------------------------------------------
//   DEFINITIONS FOR GRAPHICS
// ---------------------------------------------------------


// tile dimensions on screen
#define TileWidth 30
#define TileHeight 40
#define HalfTileWidth  (TileWidth / 2)
#define HalfTileHeight (TileHeight / 2)

// each layer has an offset from the one
// below in order to show tile edges
#define LayersOffset 4

// global placement of the first layer
#define FirstLayerLeftX  134
#define FirstLayerTopY   20

// screen height of each action in the list
#define ActionsStepY  46


// ---------------------------------------------------------
//   DETERMINE POINTED TILE FROM CURSOR
// ---------------------------------------------------------


void UpdatePointedTile()
{
    for( int i = TilesInDeck - 1; i >= 0 ; i-- )
    {
        if( LayoutTiles[ i ].ID == 0 )
          continue;
        
        int BaseX = FirstLayerLeftX + LayersOffset * LayoutTiles[ i ].z;
        int BaseY = FirstLayerTopY  - LayersOffset * LayoutTiles[ i ].z;
        
        // check position along X
        int MinX = BaseX + HalfTileWidth * LayoutTiles[ i ].x; 
        int MaxX = MinX + TileWidth - 1;
        if( CursorX < MinX || CursorX > MaxX )
          continue;
        
        // check position along Y
        int MinY = BaseY + HalfTileHeight * LayoutTiles[ i ].y; 
        int MaxY = MinY + TileHeight - 1;
        if( CursorY < MinY || CursorY > MaxY )
          continue;
        
        // point to this tile
        PointedTileIndex = i;
        return;
    }
    
    // if we arrive here, no tile is pointed
    PointedTileIndex = -1;
}


// ---------------------------------------------------------
//   DETERMINE SHADOWS FOR ALL TILES
// ---------------------------------------------------------


// we draw shadows as a "tilemap", which is updated
// every time the game state changes; we will analyze
// the neighbors of every grid position and determine
// which shadow tile should be shown for it
void DetermineTileShadows()
{
    for( int z = 0; z < BoardLayers; ++z )
    for( int y = 0; y <= BoardHeight; ++y )
    for( int x = 0; x <= BoardWidth; ++x )
    {
        // check if this tile should not have a shadow at all
        bool IsEmpty = true;
        
        if( x > 0 && y < BoardHeight )
          if( Board[ z ][ y ][ x-1 ] != 0 )
            IsEmpty = false;
        
        bool HasFloor = false;
        
        if( z == 0 ) HasFloor = true;
        else if( x == 0 || y == BoardHeight ) HasFloor = false;
        else HasFloor = (Board[ z-1 ][ y ][ x-1 ] != 0);
        
        if( !IsEmpty || !HasFloor )
        {
            Shadows[ z ][ y ][ x ] = 0;
            continue;
        }
        
        // the specific shadow tile is determined by 3 neighbors
        bool HasTopNeighbor = (x > 0) && (y > 0) && (Board[ z ][ y-1 ][ x-1 ] != 0);
        bool HasRightNeighbor = (x < BoardWidth) && (y < BoardHeight) && (Board[ z ][ y ][ x ] != 0);
        bool HasTopRightNeighbor = (y > 0) && (x < BoardWidth) && (Board[ z ][ y-1 ][ x ] != 0);
        
        // save the tile encoding it as 3 bits
        int ShadowTile = HasTopNeighbor + (HasTopRightNeighbor << 1) + (HasRightNeighbor << 2);
        Shadows[ z ][ y ][ x ] = ShadowTile;
    }
}


// ---------------------------------------------------------
//   DRAWING SINGLE LAYERS
// ---------------------------------------------------------


void DrawLayerShadows( int z )
{
    int* ShadowPointer = &Shadows[ z ][ 0 ][ 0 ];
    int Offset = LayersOffset * (z-1);
    int DrawY = FirstLayerTopY - Offset;
    
    for( int y = 0; y <= BoardHeight; ++y )
    {
        // initialize horizontal line
        int DrawX = FirstLayerLeftX - HalfTileWidth + Offset;
        
        for( int x = 0; x <= BoardWidth; ++x )
        {
            int ShadowTileID = *ShadowPointer;
            ShadowPointer++;
            
            // draw shadow only when not empty
            if( ShadowTileID != 0 )
            {
                select_region( FirstRegionTileShadows + ShadowTileID );
                draw_region_at( DrawX, DrawY );
            }
            
            DrawX += HalfTileWidth;
        }
        
        DrawY += HalfTileHeight;
    }
}

// ---------------------------------------------------------

void DrawLayerEdges( int z )
{
    int BaseX = FirstLayerLeftX + LayersOffset * z;
    int BaseY = FirstLayerTopY  - LayersOffset * z;
    
    for( int i = 0; i < TilesInDeck; ++i )
    {
        if( LayoutTiles[ i ].z != z )
          continue;
        
        // draw tile only when not empty
        int TileID = LayoutTiles[ i ].ID;
        if( TileID == 0 ) continue;
        
        // draw tile edges
        select_region( RegionTileEdgeRight );
        draw_region_at( BaseX + HalfTileWidth * LayoutTiles[ i ].x, BaseY + HalfTileHeight * LayoutTiles[ i ].y );
        select_region( RegionTileEdgeTop );
        draw_region();
        select_region( RegionTileEdgeBottom );
        draw_region();
        select_region( RegionTileEdgeLeft );
        draw_region();
        select_region( RegionTileEdgeBottom );
        draw_region();
    }
}

// ---------------------------------------------------------

void DrawLayerTiles( int z )
{
    int BaseX = FirstLayerLeftX + LayersOffset * z;
    int BaseY = FirstLayerTopY  - LayersOffset * z;
    
    for( int i = 0; i < TilesInDeck; ++i )
    {
        if( LayoutTiles[ i ].z != z )
          continue;
        
        // draw tile only when not empty
        int TileID = LayoutTiles[ i ].ID;
        if( TileID == 0 ) continue;
        
        // draw tile front
        select_region( FirstRegionTileFronts + TileID );
        draw_region_at( BaseX + 15 * LayoutTiles[ i ].x, BaseY + 20 * LayoutTiles[ i ].y );
    }
}


// ---------------------------------------------------------
//   DRAWING THE FULL BOARD
// ---------------------------------------------------------


// our draw board function can work in multiple "modes"
// that will draw different gameplay elements over the board
enum BoardDrawMode
{
    DrawBoardOnly,
    DrawSelection,
    DrawPairing,
    DrawHint
};

// the draw board uses these 2 tile indices to determine
// where to draw overlays for each drawing mode (in a way,
// we can consider them as additional function arguments)
int[ 2 ] DrawnTileIndex = { -1, -1 };

// ---------------------------------------------------------

// the function is also provided with the elapsed frames
// so that it can draw animations correctly
void DrawBoard( BoardDrawMode Mode, int ElapsedFrames )
{
    TileInfo* DrawnTile1 = &LayoutTiles[ DrawnTileIndex[ 0 ] ];
    TileInfo* DrawnTile2 = &LayoutTiles[ DrawnTileIndex[ 1 ] ];
    
    for( int z = 0; z < BoardLayers; ++z )
    {
        // draw all regular elements of a layer
        DrawLayerShadows( z );
        DrawLayerEdges( z );
        DrawLayerTiles( z );
        
        if( Mode == DrawBoardOnly )
          continue;
        
        // precalculate tile locations, common for all draw modes
        int BaseX = FirstLayerLeftX + LayersOffset * z;
        int BaseY = FirstLayerTopY  - LayersOffset * z;
        
        int DrawnTile1X = BaseX + HalfTileWidth  * DrawnTile1->x;
        int DrawnTile1Y = BaseY + HalfTileHeight * DrawnTile1->y;
        int DrawnTile2X = BaseX + HalfTileWidth  * DrawnTile2->x;
        int DrawnTile2Y = BaseY + HalfTileHeight * DrawnTile2->y;
        
        // draw different tile overlays depending on the mode
        switch( Mode )
        {
            case DrawSelection:
            {
                // drawn tile 1 = pointed tile
                if( DrawnTile1->ID >= 0 && DrawnTile1->z == z )
                {
                    set_blending_mode( blending_subtract );
                    select_region( RegionSelectionHighlight );
                    draw_region_at( DrawnTile1X, DrawnTile1Y );
                    set_blending_mode( blending_alpha );
                }
                
                // drawn tile 2 = picked tile
                if( DrawnTile2->ID >= 0 && DrawnTile2->z == z )
                {
                    int AnimationFrame = (ElapsedFrames / 8) % 2;
                    select_region( FirstRegionSelection + AnimationFrame );
                    draw_region_at( DrawnTile2X, DrawnTile2Y );
                }
                
                break;
            }
            
            case DrawPairing:
            {
                int AnimationFrame = min( 5, (ElapsedFrames / 5) );
                select_region( FirstRegionPairAnimation + AnimationFrame );
                
                if( DrawnTile1->ID >= 0 && DrawnTile1->z == z )
                  draw_region_at( DrawnTile1X + HalfTileWidth, DrawnTile1Y + HalfTileHeight );
                
                if( DrawnTile2->ID >= 0 && DrawnTile2->z == z )
                  draw_region_at( DrawnTile2X + HalfTileWidth, DrawnTile2Y + HalfTileHeight );
                
                break;
            }
            
            case DrawHint:
            {
                // make highlight blink 3 times
                if( ((ElapsedFrames / 10) % 2) == 0 )
                  break;
                
                set_blending_mode( blending_subtract );
                select_region( RegionSelectionHighlight );
                
                if( DrawnTile1->ID >= 0 && DrawnTile1->z == z )
                  draw_region_at( DrawnTile1X, DrawnTile1Y );
                
                if( DrawnTile2->ID >= 0 && DrawnTile2->z == z )
                  draw_region_at( DrawnTile2X, DrawnTile2Y );
                
                set_blending_mode( blending_alpha );
                break;
            }
            
            default: break;
        }
    }
}


// ---------------------------------------------------------
//   OTHER DRAWING FUNCTIONS
// ---------------------------------------------------------


void DrawCursor( int AnimationFrame )
{
    select_region( FirstRegionTarget + AnimationFrame );
    draw_region_at( CursorX, CursorY );
}

// ---------------------------------------------------------

void DrawAllowedActions()
{
    int DrawY = 0;
    
    for( int i = 0; i < 7; i++ )
      if( ActionsAllowed[ i ] )
      {
          select_region( FirstRegionActions + i );
          draw_region_at( 0, DrawY );
          DrawY += ActionsStepY;
      }
}

// ---------------------------------------------------------

void DrawGUI()
{
    // draw background
    select_region( RegionGUIShadow );
    set_drawing_scale( 1, screen_height );
    draw_region_zoomed_at( 0, 0 );
    
    // display number of tiles left
    select_region( RegionGUIRemainingTiles );
    draw_region_at( 0, screen_height - 1 );
    
    set_multiply_color( color_cyan );
    int RemainingTiles = CountRemainingTiles();
    numberfont_print_int_from_left( &FontTiles, 59, 339, RemainingTiles );
    set_multiply_color( color_white );
    
    // draw allowed actions
    DrawAllowedActions();
}
