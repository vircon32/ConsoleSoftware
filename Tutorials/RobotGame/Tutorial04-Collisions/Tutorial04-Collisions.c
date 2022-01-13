/* -----------------------------------------------
   Our previous step in this tutorial prevented
   our robot from going out of the map bounds,
   but there was no collision detection. We
   should also prevent movements where there is
   no floor, or where there are solid objects
   that would block our way.
   
   Since we are already creating our stage with
   a tile map, we will determine which tiles
   can be traversed and which ones cannot be
   entered. We will also consider the robot as
   another rectangular shape in order to model
   everything as collisions between 2D
   axis-aligned rectangles.
/  -------------------------------------------- */


// include Vircon libraries
#include "video.h"
#include "input.h"
#include "time.h"


// -----------------------------------------------
// DEFINITIONS
// -----------------------------------------------

// giving names to our texture regions
// makes them easier to define and use
#define RegionRobotRight  0
#define RegionRobotLeft   1
#define RegionRobotDown   2
#define RegionRobotUp     3

// Our tile set is 8x5 tiles, so it needs 40 regions.
// We will define them as consecutive, so we can use
// them as an index. So we only need to name the first one.
#define FirstRegionTileSet  10

// movement speed of our robot
// (increased, since our map is larger now)
#define RobotSpeed 3

// tile dimensions in pixels
#define TileWidth  96
#define TileHeight 96

// tile map dimensions in tiles
#define MapTilesInX  14
#define MapTilesInY  7

// tile map dimensions in pixels
#define TileMapWidth  (TileWidth  * MapTilesInX)
#define TileMapHeight (TileHeight * MapTilesInY)

// for collisions, we will approximate the
// robot shape using a rectangle with these
// dimensions, where our position is at the
// center of that rectangle
#define RobotWidth   64
#define RobotHeight  54


// -----------------------------------------------
// GLOBAL VARIABLES
// -----------------------------------------------


// Our tile map is loaded from an external file,
// and will be stored as a 2D array in cardtidge ROM.
// Find out more about this in the previous tutorial.
embedded int[ MapTilesInY ][ MapTilesInX ] TileMap = "obj\\RobotLab.vmap";

// position of our robot in pixels, relative to map's
// top-left corner; initially we place the robot at the
// center (hence the 0.5) of tile [3,3] in the tile map,
// which is an empty floor tile
int RobotX = TileWidth  * 3.5;
int RobotY = TileHeight * 3.5;

// position of our "camera" in pixels, relative to the
// top-left corner of the map. That point in the map
// will be drawn at the center of our screen
int CameraX, CameraY;


// -----------------------------------------------
// COLLISION DETECTION FUNCTIONS
// -----------------------------------------------
// Our collision detection is very simple: we
// treat axes X and Y as totally independent.
// Also, since the map is made of tiles we will
// collide against each tile individually.
// -----------------------------------------------

// this prevents the robot from exiting map limits in X
void KeepRobotXInsideMap()
{
    // restrict horizontal edges of robot rectangle
    int RobotMinX = RobotWidth/2 + 1;
    int RobotMaxX = TileMapWidth - RobotWidth/2 - 1;
    
    if( RobotX < RobotMinX )
      RobotX = RobotMinX;
    
    if( RobotX > RobotMaxX )
      RobotX = RobotMaxX;
}

// -----------------------------------------------

// this prevents the robot from exiting map limits in Y
void KeepRobotYInsideMap()
{
    // restrict vertical edges of robot rectangle
    int RobotMinY = RobotHeight/2 + 1;
    int RobotMaxY = TileMapHeight - RobotHeight/2 - 1;
    
    if( RobotY < RobotMinY )
      RobotY = RobotMinY;
    
    if( RobotY > RobotMaxY )
      RobotY = RobotMaxY;
}

// -----------------------------------------------

// this function determines if a tile from our
// tile set can be entered by our robot
bool TileIsTraversable( int Tile )
{
    // this logic is based on the positions of
    // tiles within our tile set; it could be
    // simplified if the tileset was reorganized
    // to group non-traversable tiles together
    if( Tile == 0 ) return false;
    if( Tile == 28 || Tile == 29 ) return false;
    if( Tile >= 33 && Tile <= 37 ) return false;
    
    return true;
}

// -----------------------------------------------

// this function solves collisions for robot movement in X
void CollideRobotAlongX( int DirectionX )
{
    // determine the 4 robot rectangle boundaries
    int RobotLeft   = RobotX - RobotWidth/2;
    int RobotRight  = RobotX + RobotWidth/2;
    int RobotTop    = RobotY - RobotHeight/2;
    int RobotBottom = RobotY + RobotHeight/2;
    
    // Determine the range of map tiles that our
    // robot rectangle overlaps with. We always
    // need to make sure we don't have X or Y
    // positions outside the map, since trying
    // to access them may cause a memory error
    // and halt our game. However in this case
    // we already guarantee that because we
    // keep the robot rectangle completely
    // inside the tile map every frame.
    int MinTileX = RobotLeft   / TileWidth;
    int MaxTileX = RobotRight  / TileWidth;
    int MinTileY = RobotTop    / TileHeight;
    int MaxTileY = RobotBottom / TileHeight;
    
    // We now iterate vertically to check all
    // tiles at robot's left and right edges.
    // We only need to correct position if the
    // robot rectangle is overlapping some tile
    // considered as "non traversable"
    for( int TileY = MinTileY; TileY <= MaxTileY; TileY++ )
    {
        // if the robot has stepped over a tile at
        // its left, correct position to the right
        if( DirectionX < 0 )
          if( !TileIsTraversable( TileMap[ TileY ][ MinTileX ] ) )
            RobotX = TileWidth * (MinTileX+1) + RobotWidth/2 + 1;
        
        // if the robot has stepped over a tile at
        // its right, correct position to the left
        if( DirectionX > 0 )
          if( !TileIsTraversable( TileMap[ TileY ][ MaxTileX ] ) )
            RobotX = TileWidth * MaxTileX - RobotWidth/2 - 1;
    }
}

// -----------------------------------------------

// this function solves collisions for robot movement in Y
void CollideRobotAlongY( int DirectionY )
{
    // determine the 4 robot rectangle boundaries
    int RobotLeft   = RobotX - RobotWidth/2;
    int RobotRight  = RobotX + RobotWidth/2;
    int RobotTop    = RobotY - RobotHeight/2;
    int RobotBottom = RobotY + RobotHeight/2;
    
    // Determine the range of map tiles that our
    // robot rectangle overlaps with. We always
    // need to make sure we don't have X or Y
    // positions outside the map, since trying
    // to access them may cause a memory error
    // and halt our game. However in this case
    // we already guarantee that because we
    // keep the robot rectangle completely
    // inside the tile map every frame.
    int MinTileX = RobotLeft   / TileWidth;
    int MaxTileX = RobotRight  / TileWidth;
    int MinTileY = RobotTop    / TileHeight;
    int MaxTileY = RobotBottom / TileHeight;
    
    // We now iterate horizontally to check all
    // tiles at robot's left and right edges.
    // We only need to correct position if the
    // robot rectangle is overlapping some tile
    // considered as "non traversable"
    for( int TileX = MinTileX; TileX <= MaxTileX; TileX++ )
    {
        // if the robot has stepped over a tile at
        // its top, correct position to the bottom
        if( DirectionY < 0 )
          if( !TileIsTraversable( TileMap[ MinTileY ][ TileX ] ) )
            RobotY = TileHeight * (MinTileY+1) + RobotHeight/2 + 1;
        
        // if the robot has stepped over a tile at
        // its bottom, correct position to the top
        if( DirectionY > 0 )
          if( !TileIsTraversable( TileMap[ MaxTileY ][ TileX ] ) )
            RobotY = TileHeight * MaxTileY - RobotHeight/2 - 1;
    }
}


// -----------------------------------------------
// SCENE DRAWING FUNCTIONS
// -----------------------------------------------


// calling this will point the camera to our robot,
// but it will adjust it so that the map remains on
// screen if the robot approaches map edges
void UpdateCameraPosition()
{
    // initially just point camera to the robot
    CameraX = RobotX;
    CameraY = RobotY;
    
    // restrict camera horizontally
    int CameraMinX = screen_width/2;
    int CameraMaxX = TileMapWidth - screen_width/2;
    
    if( CameraX < CameraMinX )
      CameraX = CameraMinX;
    
    if( CameraX > CameraMaxX )
      CameraX = CameraMaxX;
    
    // restrict camera vertically
    int CameraMinY = screen_height/2;
    int CameraMaxY = TileMapHeight - screen_height/2;
    
    if( CameraY < CameraMinY )
      CameraY = CameraMinY;
    
    if( CameraY > CameraMaxY )
      CameraY = CameraMaxY;
}

// -----------------------------------------------

// Once we determine the map's top-left position on
// screen (given by camera position), the tile map can
// be drawn with 2 nested loops. Note that this draw
// function is unoptimized: it will always draw the
// whole map, even if only a portion is visible. Also,
// a usual convention we use here is to consider tile
// number 0 to be "empty", and therefore not drawn.
void DrawTileMap()
{
    // calculate map position in screen
    int MapTopLeftX = -CameraX + screen_width/2;
    int MapTopLeftY = -CameraY + screen_height/2;
    
    // outer loop for Y
    int RenderY = MapTopLeftY;
    
    for( int TileY = 0; TileY < MapTilesInY; TileY++ )
    {
        // inner loop for X
        int RenderX = MapTopLeftX;
        
        for( int TileX = 0; TileX < MapTilesInX; TileX++ )
        {
            // don't draw the empty tile
            int CurrentTile = TileMap[ TileY ][ TileX ];
            
            if( CurrentTile )
            {
                select_region( FirstRegionTileSet + CurrentTile );
                draw_region_at( RenderX, RenderY );
            }
            
            RenderX += TileWidth;
        }
    
        RenderY += TileHeight;
    }
}

// -----------------------------------------------

// to draw the robot we need to determine which region
// is drawn, depending on the direction of movement
void DrawRobot( int DirectionX, int DirectionY )
{
    // our robot can move in 8 directions, but
    // we have not drawn the diagonals. So we
    // give priority to left and right
    if( DirectionX > 0 )
      select_region( RegionRobotRight );
    
    else if( DirectionX < 0 )
      select_region( RegionRobotLeft );
    
    else if( DirectionY < 0 )
      select_region( RegionRobotUp );
      
    else if( DirectionY > 0 )
      select_region( RegionRobotDown );
    
    // when not moving, the robot faces down
    else
      select_region( RegionRobotDown );
    
    // our robot has now been updated so we can draw
    // it; but we need to convert its position (which
    // is relative to the map) to a position on screen
    int RobotRenderX = RobotX - CameraX + screen_width/2;
    int RobotRenderY = RobotY - CameraY + screen_height/2;
    draw_region_at( RobotRenderX, RobotRenderY );
}


// -----------------------------------------------
// MAIN FUNCTION
// -----------------------------------------------


// Our strategy is to separate movements in X and Y.
// So first we will do all movement and collisions
// along one axis and then the other. So, inside a
// frame, the object would not move diagonally but
// instead move as in stairs (first X and then Y).

// Before performing collision detection we must
// always prevent the robot from stepping outside of
// map limits, or a memory violation may happen!

void main( void )
{
    // -----------------------------------------------
    // PART 1: DEFINE OUR TEXTURE REGIONS
    
    // select texture 0 (the only one in our ROM)
    select_texture( 0 );
    
    // we have an 8x5 matrix of tile regions
    // that we will define with consecutive IDs
    // and with hotspot at their top left
    define_region_matrix( FirstRegionTileSet,  0,0,  TileWidth-1,TileHeight-1,  0,0,  8,5,  0 );
    
    // we have a 4x1 matrix of robot regions
    // that we will define with consecutive IDs
    // and with hotspot near the bottom center
    define_region_matrix( RegionRobotRight,  769,1,  834,81,  801,53,  1,4,  1 );
    
    // -----------------------------------------------
    // PART 2: INITIALIZATIONS
    
    // we will control the robot with player 1
    select_gamepad( 0 );
    
    // -----------------------------------------------
    // PART 3: MAIN LOOP
    
    // our game never ends: keep repeating
    // our game logic for every frame (60 fps)
    while( true )
    {
        // - - - - - - - - - - - - - - - - - - -
        // STEP 1: Process player inputs
        
        // the robot will move in the direction
        // that we are currently pressing
        int DirectionX, DirectionY;
        gamepad_direction( &DirectionX, &DirectionY );
        
        // - - - - - - - - - - - - - - - - - - -
        // STEP 2: Process movement along X
        
        // Before performing collision detection we must
        // always prevent the robot from stepping outside of
        // map limits, or a memory violation may happen!
        RobotX += RobotSpeed * DirectionX;
        KeepRobotXInsideMap();
        CollideRobotAlongX( DirectionX );
        
        // - - - - - - - - - - - - - - - - - - -
        // STEP 3: Process movement along Y
        
        // exactly the same process as in X axis
        RobotY += RobotSpeed * DirectionY;
        KeepRobotYInsideMap();
        CollideRobotAlongY( DirectionY );
        
        // - - - - - - - - - - - - - - - - - - -
        // STEP 4: Draw the scene
        
        // robot may have moved, so make camera follow it
        UpdateCameraPosition();
        
        // clear the screen to dark blue
        int DarkBlue = make_color_rgb( 0,32,128 );
        clear_screen( DarkBlue );
        
        // draw our tile map over the blue background
        DrawTileMap();
        
        // finally draw our robot on top
        DrawRobot( DirectionX, DirectionY );
        
        // - - - - - - - - - - - - - - - - - - -
        // we have ended this frame, so wait for
        // the next in order to control game speed
        end_frame();
    }
}
