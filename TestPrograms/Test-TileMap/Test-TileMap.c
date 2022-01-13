#include "time.h"
#include "video.h"
#include "input.h"
#include "math.h"


// ---------------------------------------------------------
//   DEFINITIONS AND CONSTANTS
// ---------------------------------------------------------


// names for texture regions
#define FirstRegionTileSet     0
#define RegionArrowLeft      100
#define RegionArrowRight     101
#define RegionArrowUp        102
#define RegionArrowDown      103

// screen dimensions
#define ScreenWidth   640
#define ScreenHeight  360

// tile map dimensions (2x2 screens)
#define MapTilesX  32
#define MapTilesY  18
#define TileSize   40

// position of map limits, in pixels
#define LevelMinX  0
#define LevelMinY  0
#define LevelMaxX  (MapTilesX * TileSize)
#define LevelMaxY  (MapTilesY * TileSize)

// precalculate camera limits
#define CameraMinX (LevelMinX + ScreenWidth/2)
#define CameraMaxX (LevelMaxX - ScreenWidth/2)
#define CameraMinY (LevelMinY + ScreenHeight/2)
#define CameraMaxY (LevelMaxY - ScreenHeight/2)


// ---------------------------------------------------------
//   GLOBAL VARIABLES
// ---------------------------------------------------------


// external variables in rom
embedded int[ MapTilesY ][ MapTilesX ] TileMap = "obj\\TileMapLayer1.vmap";


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // these selections will never change
    select_texture( 0 );
    select_gamepad( 0 );
    
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    // regions for tileset
    define_region_matrix( FirstRegionTileSet,  0,0,  39,39,  0,0,  8,6,  0 );
    
    // left arrow
    select_region( RegionArrowLeft );
    define_region( 0,241,  15,272,  0,256 );
    
    // right arrow
    select_region( RegionArrowRight );
    define_region( 17,241,  32,272,  32,256 );
    
    // up arrow
    select_region( RegionArrowUp );
    define_region( 34,241,  65,256,  49,241 );
    
    // down arrow
    select_region( RegionArrowDown );
    define_region( 34,258,  65,273,  49,273 );
    
    // ------------------------------------
    // PART 2: PREPARATIONS
    // ------------------------------------
    
    int CameraX = LevelMaxX / 2;
    int CameraY = LevelMaxY / 2;
    
    // ------------------------------------
    // PART 3: MAIN LOOP
    // ------------------------------------
    
    while( true )
    {
        // read inputs
        select_gamepad( 0 );
        
        // move camera as indicated by player
        int DeltaX, DeltaY;
        gamepad_direction( &DeltaX, &DeltaY );
        
        CameraX += 2 * DeltaX;
        CameraY += 2 * DeltaY;
        
        // keep camera within stage limits
        CameraX = max( CameraX, CameraMinX );
        CameraX = min( CameraX, CameraMaxX );
        CameraY = max( CameraY, CameraMinY );
        CameraY = min( CameraY, CameraMaxY );
        
        // calculate map position in screen
        int MapTopLeftX = -CameraX + ScreenWidth/2;
        int MapTopLeftY = -CameraY + ScreenHeight/2;
        
        // ----------------------------------------
        //   RENDER SCENE
        
        // clear screen to black
        clear_screen( color_black );
        
        // reset color modifications
        set_multiply_color( color_white );
        
        // draw only within screen limits
        int MinTileX = max( -MapTopLeftX / TileSize, 0 );
        int MinTileY = max( -MapTopLeftY / TileSize, 0 );
        int MaxTileX = min( (-MapTopLeftX+ScreenWidth)  / TileSize, MapTilesX-1 );
        int MaxTileY = min( (-MapTopLeftY+ScreenHeight) / TileSize, MapTilesY-1 );
        
        // render tile map
        int RenderY = MapTopLeftY + MinTileY * TileSize;
        
        for( int TileY = MinTileY; TileY <= MaxTileY; ++TileY )
        {
            int RenderX = MapTopLeftX + MinTileX * TileSize;
            int* CurrentTile = &TileMap[ TileY ][ MinTileX ];
            
            for( int TileX = MinTileX; TileX <= MaxTileX; ++TileX )
            {
                select_region( FirstRegionTileSet + *CurrentTile );
                draw_region_at( RenderX, RenderY );
                
                RenderX += TileSize;
                ++CurrentTile;
            }
        
            RenderY += TileSize;
        }
        
        // draw the 4 direction arrows in this way:
        // - they blink for better visibility
        // - draw them only when scroll in that direction can be done
        // - draw them in red if camera is now travelling in that direction
        int FrameCount = get_frame_counter();
        
        if( (FrameCount%30) < 25 )
        {
            if( CameraX > CameraMinX )
            {
                if( DeltaX < 0 )  set_multiply_color( color_red );
                else              set_multiply_color( color_white );
                
                select_region( RegionArrowLeft );
                draw_region_at( 20, 180 );
            }
            
            if( CameraX < CameraMaxX )
            {
                if( DeltaX > 0 )  set_multiply_color( color_red );
                else              set_multiply_color( color_white );
                
                select_region( RegionArrowRight );
                draw_region_at( 620, 180 );
            }
            
            if( CameraY > CameraMinY )
            {
                if( DeltaY < 0 )  set_multiply_color( color_red );
                else              set_multiply_color( color_white );
                
                select_region( RegionArrowUp );
                draw_region_at( 320, 20 );
            }
            
            if( CameraY < CameraMaxY )
            {
                if( DeltaY > 0 )  set_multiply_color( color_red );
                else              set_multiply_color( color_white );
                
                select_region( RegionArrowDown );
                draw_region_at( 320, 340 );
            }
        }
        
        // wait until next frame
        end_frame();
    }
}