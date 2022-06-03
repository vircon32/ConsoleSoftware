/* -----------------------------------------------
   In this step of the tutorial we want to make
   our background larger. However textures have
   a size limit of 1024 pixels. To be able to
   draw something larger we have to split it
   into smaller parts. A common way to do that
   is to have a set of rectangular "tiles" and
   place them in a 2D array to form a tile map.
   In this tutorial, the tile map was made with
   the editor Tiled, which is an open source
   program from which we can import maps.
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


// -----------------------------------------------
// GLOBAL VARIABLES
// -----------------------------------------------


// Our tile map is loaded from an external file,
// and will be stored as a 2D array in cardtidge ROM.
// Note that this file cannot be the original TMX
// tilemap in "map" folder since that file is an XML
// and Vircon32 needs it to be converted into binary.
// Also, note that the array is [Y][X] and not [X][Y]:
// this is because we want the tiles to be stored in
// the usual order from left to right and from top to
// bottom, and this memory layout will achieve that.
// When our import tool processes the TMX map file,
// it exports every layer to a different file, using
// the name of that layer as output file name (in
// this case, the Tiled layer is called "RobotLab").
embedded int[ MapTilesInY ][ MapTilesInX ] TileMap = "obj/RobotLab.vmap";

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
// AUXILIARY FUNCTIONS
// -----------------------------------------------


// this function prevents the robot from leaving the
// map by clamping its position to map limits
void KeepRobotInsideMap()
{
    // restrict robot horizontally
    // (we consider robot witdh as 64 pixels)
    int RobotMinX = 32;
    int RobotMaxX = TileMapWidth - 32;
    
    if( RobotX < RobotMinX )
      RobotX = RobotMinX;
    
    if( RobotX > RobotMaxX )
      RobotX = RobotMaxX;
    
    // restrict robot vertically
    // (we consider robot height as 54 pixels)
    int RobotMinY = 27;
    int RobotMaxY = TileMapHeight - 27;
    
    if( RobotY < RobotMinY )
      RobotY = RobotMinY;
    
    if( RobotY > RobotMaxY )
      RobotY = RobotMaxY;
}

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
// be drawn with 2 simple nested loops. Note that this
// draw function is unoptimized: it will always draw
// the whole map, even if only a portion is visible.
// A usual convention we use here is to consider tile
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
// MAIN FUNCTION
// -----------------------------------------------


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
    
    // our robot starts looking down
    int RobotImage = RegionRobotDown;
    
    // we will control the robot with player 1
    select_gamepad( 0 );
    
    // -----------------------------------------------
    // PART 3: MAIN LOOP
    
    // our game never ends: keep repeating
    // our game logic for every frame (60 fps)
    while( true )
    {
        // the robot will move in the direction
        // that we are currently pressing
        int DirectionX, DirectionY;
        gamepad_direction( &DirectionX, &DirectionY );
        
        RobotX += RobotSpeed * DirectionX;
        RobotY += RobotSpeed * DirectionY;
        
        // enforce the stage limits so that our
        // movements cannot make the robot leave the map
        KeepRobotInsideMap();
        
        // robot may have moved, so make camera follow it
        UpdateCameraPosition();
        
        // clear the screen to dark blue
        int DarkBlue = make_color_rgb( 0,32,128 );
        clear_screen( DarkBlue );
        
        // draw our tile map over the blue background
        DrawTileMap();
        
        // our robot can move in 8 directions, but
        // we have not drawn the diagonals. So we
        // give priority to left and right
        if( DirectionX > 0 )
          RobotImage = RegionRobotRight;
        
        else if( DirectionX < 0 )
          RobotImage = RegionRobotLeft;
        
        else if( DirectionY < 0 )
          RobotImage = RegionRobotUp;
          
        else if( DirectionY > 0 )
          RobotImage = RegionRobotDown;
        
        // our robot has now been updated so we can draw
        // it; but we need to convert its position (which
        // is relative to the map) to a position on screen
        int RobotRenderX = RobotX - CameraX + screen_width/2;
        int RobotRenderY = RobotY - CameraY + screen_height/2;
        
        select_region( RobotImage );
        draw_region_at( RobotRenderX, RobotRenderY );
        
        // we have ended this frame, so wait for
        // the next in order to control game speed
        end_frame();
    }
}