// include Vircon libraries
#include "time.h"
#include "video.h"
#include "audio.h"
#include "math.h"
#include "input.h"
#include "misc.h"


// ---------------------------------------------------------
//   DEFINITIONS AND CONSTANTS
// ---------------------------------------------------------


// names for texture regions
#define FirstRegionScenery   0
#define FirstRegionBricks   10
#define FirstRegionBall     20
#define RegionPaddle        30

// names for sounds
#define SoundMusic              0
#define SoundPaddleBounce       1
#define SoundWallBounce         2
#define SoundRegularBlock       3
#define SoundNonBreakableBlock  4

// names for sound channels
#define ChannelMusic  15

// screen dimensions
#define ScreenWidth   640
#define ScreenHeight  360

// tile map dimensions
#define ScreenTilesX  16
#define ScreenTilesY   9
#define TileSize      40

// brick dimensions
#define BricksInX    12
#define BricksInY    10
#define BrickWidth   40
#define BrickHeight  20

// position of level limits
#define LevelMinX  TileSize
#define LevelMaxX  (ScreenWidth - TileSize)
#define LevelMinY  TileSize
#define LevelMaxY  ScreenHeight

// paddle dimensions
#define PaddleWidth   80
#define PaddleHeight  15

// ball dimensions
#define BallDiameter  16
#define BallRadius    (BallDiameter / 2)

// movement speeds (in pixels per frame)
#define PaddleSpeed  10
#define BallSpeed     4

// ---------------------------------------------------------
//   GLOBAL VARIABLES
// ---------------------------------------------------------


// external variables in rom
embedded int[ ScreenTilesY ][ ScreenTilesX ] MapScenery = "obj\\TileMapScenery.vmap";
embedded int[ BricksInY ][ BricksInX ] MapBricks = "obj\\TileMapBricks.vmap";

// we need a copy of the brick layout in ram so
// that bricks can be removed (rom is read-only)
int[ BricksInY ][ BricksInX ] BrickTypes;

// position of the background texture
struct Vector2D
{
   float x, y; 
};

Vector2D BackgroundPosition;

// make these global so that collision functions can access them
Vector2D BallPosition, OldBallPosition;
Vector2D BallVelocity;
Vector2D PaddlePosition;


// ---------------------------------------------------------
//   SCENE DRAWING FUNCTIONS
// ---------------------------------------------------------


void DrawBackground()
{
    // draw background as a repeating texture
    int DrawingY = BackgroundPosition.y;
    select_region( 0 );
    
    for( int y = 0; y < 10; y++ )
    {
        int DrawingX = BackgroundPosition.x;
        
        for( int x = 0; x < 17; x++ )
        {
            draw_region_at( DrawingX, DrawingY );
            DrawingX += TileSize;
        }
        
        DrawingY += TileSize;
    }
    
    // after drawing, make it move diagonally
    BackgroundPosition.x -= 0.6667;
    BackgroundPosition.y -= 0.6667;
    
    // wrap background position cyclically
    if( BackgroundPosition.x <= -40 )
    {
        BackgroundPosition.x = 0;
        BackgroundPosition.y = 0;
    }
}

// ---------------------------------------------------------

void DrawScenery()
{
    int DrawingY = 0;
    int* MapPosition = &MapScenery[0][0];
    
    for( int y = 0; y < 9; y++ )
    {
        int DrawingX = 0;
        
        for( int x = 0; x < 16; x++ )
        {
            if( *MapPosition )
            {
                select_region( FirstRegionScenery + *MapPosition );
                draw_region_at( DrawingX, DrawingY );
            }
            
            DrawingX += TileSize;
            ++MapPosition;
        }
        
        DrawingY += TileSize;
    }
}

// ---------------------------------------------------------

void DrawBricks()
{
    // draw bricks from their copied tile map
    int DrawingY = 60;
    
    for( int y = 0; y < BricksInY; y++ )
    {
        int DrawingX = 80;
        
        for( int x = 0; x < BricksInX; x++ )
        {
            int BrickType = BrickTypes[ y ][ x ];
            
            if( BrickType )
            {
                select_region( FirstRegionBricks + BrickType );
                draw_region_at( DrawingX, DrawingY );
            }
            
            DrawingX += BrickWidth;
        }
        
        DrawingY += BrickHeight;
    }
}


// ---------------------------------------------------------
//   COLLISION DETECTION FUNCTIONS
// ---------------------------------------------------------


bool CollideBallWithRectangle( int RectangleMinX, int RectangleMinY, int RectangleMaxX, int RectangleMaxY, bool CollideLeft, bool CollideTop, bool CollideRight, bool CollideBottom )
{
    // study ball-rectangle overlap in X and Y
    bool XOverlap = ((BallPosition.x+BallRadius) > RectangleMinX && (BallPosition.x-BallRadius) < RectangleMaxX);
    bool YOverlap = ((BallPosition.y+BallRadius) > RectangleMinY && (BallPosition.y-BallRadius) < RectangleMaxY);
    
    if( !XOverlap || !YOverlap )
      return false;
    
    // PART 1: Collisions along X component
    
    // case 1: ball moves right
    if( CollideLeft )
      if( (OldBallPosition.x+BallRadius) <= RectangleMinX && (BallPosition.x+BallRadius) > RectangleMinX )
      {
          BallPosition.x = RectangleMinX - BallRadius - 0.1;
          OldBallPosition.x = BallPosition.x;
          BallVelocity.x = -fabs( BallVelocity.x );
      }
    
    // case 2: ball moves left
    if( CollideRight )
      if( (OldBallPosition.x-BallRadius) >= RectangleMaxX && (BallPosition.x-BallRadius) < RectangleMaxX )
      {
          BallPosition.x = RectangleMaxX + BallRadius + 0.1;
          OldBallPosition.x = BallPosition.x;
          BallVelocity.x = fabs( BallVelocity.x );
      }
    
    // PART 2: Collisions along Y component
    
    // case 1: ball moves down
    if( CollideTop )
      if( (OldBallPosition.y+BallRadius) <= RectangleMinY && (BallPosition.y+BallRadius) > RectangleMinY )
      {
          BallPosition.y = RectangleMinY - BallRadius - 0.1;
          OldBallPosition.y = BallPosition.y;
          BallVelocity.y = -fabs( BallVelocity.y );
      }
    
    // case 2: ball moves up
    if( CollideBottom )
      if( (OldBallPosition.y-BallRadius) >= RectangleMaxY && (BallPosition.y-BallRadius) < RectangleMaxY )
      {
          BallPosition.y = RectangleMaxY + BallRadius + 0.1;
          OldBallPosition.y = BallPosition.y;
          BallVelocity.y = fabs( BallVelocity.y );
      }
    
    return true;
}

// ---------------------------------------------------------

bool CollideBallWithBrick( int BrickX, int BrickY )
{
    // discard invalid Y values
    if( BrickY < 0 || BrickY >= BricksInY )
      return false;
    
    // discard invalid X values
    if( BrickX < 0 || BrickX >= BricksInX )
      return false;
    
    int* BrickHealth = &BrickTypes[ BrickY ][ BrickX ];
    
    // determine what of the 4 sides of the brick
    // are exposed (depends on what neighbors exist)
    bool LeftExposed = (BrickX == 0 || !BrickTypes[ BrickY ][ BrickX-1 ]);
    bool TopExposed  = (BrickY == 0 || !BrickTypes[ BrickY-1 ][ BrickX ]);
    
    bool RightExposed  = (BrickX == (BricksInX-1) || !BrickTypes[ BrickY ][ BrickX+1 ]);
    bool BottomExposed = (BrickY == (BricksInY-1) || !BrickTypes[ BrickY+1 ][ BrickX ]);
    
    // collide with brick, it it exists
    if( *BrickHealth )
    {
        int BrickLeft = 80 + BrickX * BrickWidth;
        int BrickTop  = 60 + BrickY * BrickHeight;
        
        bool HasCollided = CollideBallWithRectangle( BrickLeft, BrickTop, BrickLeft+BrickWidth, BrickTop+BrickHeight, LeftExposed, TopExposed, RightExposed, BottomExposed );
        
        // react to collision
        if( HasCollided )
        {
            // regular bricks are destroyed
            if( *BrickHealth == 1 )
            {
                *BrickHealth = 0;
                play_sound( SoundRegularBlock );
            }
            
            // unbreakable bricks remain
            else
              play_sound( SoundNonBreakableBlock );
            
            // allow only 1 collision per frame
            return true;
        }
    }
    
    return false;
}

// ---------------------------------------------------------

void CollideBallWithBricks()
{
    // determine region of bricks to collide
    // (at most, collide with a 2x2 brick region)
    int MinBrickX = ((BallPosition.x-BallRadius-1)-80) / BrickWidth;
    int MaxBrickX = ((BallPosition.x+BallRadius+1)-80) / BrickWidth;
    int MinBrickY = ((BallPosition.y-BallRadius-1)-60) / BrickHeight;
    int MaxBrickY = ((BallPosition.y+BallRadius+1)-60) / BrickHeight;
    
    // depending on ball speed direction, we will
    // need to iterate forward or backward in X and Y
    if( BallVelocity.x > 0 )
    {
        if( BallVelocity.y > 0 )
        {
            // iterate towards bottom-right
            for( int BrickY = MinBrickY; BrickY <= MaxBrickY; ++BrickY )
              for( int BrickX = MinBrickX; BrickX <= MaxBrickX; ++BrickX )
                if( CollideBallWithBrick( BrickX, BrickY ) )
                  break;
        }
        
        else
        {
            // iterate towards top-right
            for( int BrickY = MaxBrickY; BrickY >= MinBrickY; --BrickY )
              for( int BrickX = MinBrickX; BrickX <= MaxBrickX; ++BrickX )
                if( CollideBallWithBrick( BrickX, BrickY ) )
                  break;
        }
    }
    
    else
    {
        if( BallVelocity.y > 0 )
        {
            // iterate towards bottom-left
            for( int BrickY = MinBrickY; BrickY <= MaxBrickY; ++BrickY )
              for( int BrickX = MaxBrickX; BrickX >= MinBrickX; --BrickX )
                if( CollideBallWithBrick( BrickX, BrickY ) )
                  break;
        }
        
        else
        {
            // iterate towards top-left
            for( int BrickY = MaxBrickY; BrickY >= MinBrickY; --BrickY )
              for( int BrickX = MaxBrickX; BrickX >= MinBrickX; --BrickX )
                if( CollideBallWithBrick( BrickX, BrickY ) )
                  break;
        }
    }
}

// ---------------------------------------------------------

bool CollideBallWithPaddle( Vector2D* PaddlePosition )
{
    int PaddleMinX = PaddlePosition->x-PaddleWidth/2;
    int PaddleMaxX = PaddlePosition->x+PaddleWidth/2;
    int PaddleMinY = PaddlePosition->y;
    int PaddleMaxY = PaddlePosition->y+PaddleHeight;
    
    if( CollideBallWithRectangle( PaddleMinX, PaddleMinY, PaddleMaxX, PaddleMaxY, true, true, true, true ) )
      play_sound( SoundPaddleBounce );
}

// ---------------------------------------------------------

void CollideBallWithWalls()
{
    // collide ball with level limits
    bool BallCollided = false;
    
    // collide in X
    if( BallPosition.x > (LevelMaxX-BallRadius) && BallVelocity.x > 0 )
    {
        BallPosition.x = LevelMaxX - BallRadius - 0.1;
        BallVelocity.x = -BallVelocity.x;
        BallCollided = true;
    }
    
    else if( BallPosition.x < (LevelMinX+BallRadius) && BallVelocity.x < 0 )
    {
        BallPosition.x = LevelMinX + BallRadius + 0.1;
        BallVelocity.x = -BallVelocity.x;
        BallCollided = true;
    }
    
    // collide in Y
    if( BallPosition.y > (LevelMaxY-BallRadius) && BallVelocity.y > 0 )
    {
        BallPosition.y = LevelMaxY - BallRadius - 0.1;
        BallVelocity.y = -BallVelocity.y;
        BallCollided = true;
    }
    
    else if( BallPosition.y < (LevelMinY+BallRadius) && BallVelocity.y < 0 )
    {
        BallPosition.y = LevelMinY + BallRadius + 0.1;
        BallVelocity.y = -BallVelocity.y;
        BallCollided = true;
    }
    
    // play bounce sound
    if( BallCollided )
      play_sound( SoundWallBounce );
}


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    select_texture( 0 );
    
    // tileset for the scenery
    define_region_matrix( FirstRegionScenery,  0,0,  39,39,  0,0,  3,3,  1 );
    
    // tileset for the bricks
    define_region_matrix( FirstRegionBricks,  123,0,  162,19,  123,0,  1,3,  1 );
    
    // animation frames for the ball
    define_region_matrix( FirstRegionBall,  0,139,  20,159,  10,149,  4,1,  1 );
    
    // paddle (hotspot at center top)
    select_region( RegionPaddle );
    define_region( 0,123,  79,137,  39,123 );
    
    // ------------------------------------
    // PART 2: PREPARE THE LEVEL
    // ------------------------------------
    
    // copy the brick layout to RAM! This is
    // needed since the original array in ROM
    // cannot be modified when bricks break
    memcpy( &BrickTypes[0][0], &MapBricks[0][0], BricksInX * BricksInY );
    
    // initial background position
    BackgroundPosition.x = 0;
    BackgroundPosition.y = 0;
    
    // set initial ball parameters
    BallPosition.x = 200; //ScreenWidth / 2;
    BallPosition.y = 340; //250;
    
    BallVelocity.x =  BallSpeed * cos( pi/4 );
    BallVelocity.y = -BallSpeed * sin( pi/4 );
    
    int BallRegionOffset = 0;
    int BallElapsedFrames = 0;
    
    // set initial paddle parameters
    PaddlePosition.x = ScreenWidth / 2;
    PaddlePosition.y = ScreenHeight - TileSize;
    
    // gamepad 1 is the only one used
    select_gamepad( 0 );
    
    // adjust channel volumes
    select_channel( 0 );
    set_channel_volume( 0.6 );
    
    for( int i = 0; i <= sound_channels; i++ )
    {
        select_channel( i );
        set_channel_volume( 0.5 );
    }
    
    // begin playing music
    select_sound( SoundMusic );
    set_sound_loop( true );
    play_sound_in_channel( SoundMusic, ChannelMusic );
    
    // ------------------------------------
    // PART 3: MAIN LOOP
    // ------------------------------------
    
    while( true )
    {
        // make paddle move according to inputs
        // (with a little initial inertia)
        if( gamepad_left() > 3 )       PaddlePosition.x -= PaddleSpeed;
        else if( gamepad_left() > 0 )  PaddlePosition.x -= PaddleSpeed / 2;
        
        if( gamepad_right() > 3 )       PaddlePosition.x += PaddleSpeed;
        else if( gamepad_right() > 0 )  PaddlePosition.x += PaddleSpeed / 2;
        
        // paddle must stay within level limits
        if( PaddlePosition.x > (LevelMaxX-PaddleWidth/2) )   PaddlePosition.x = (LevelMaxX-PaddleWidth/2);
        if( PaddlePosition.x < (LevelMinX+PaddleWidth/2) )   PaddlePosition.x = (LevelMinX+PaddleWidth/2);
        
        // move ball
        OldBallPosition = BallPosition;
        BallPosition.x += BallVelocity.x;
        BallPosition.y += BallVelocity.y;
        
        // collide ball the different elements present
        CollideBallWithBricks();
        CollideBallWithPaddle( &PaddlePosition );
        CollideBallWithWalls();
        
        // draw the scene
        DrawBackground();
        DrawScenery();
        DrawBricks();
        
        // draw the paddle
        select_region( RegionPaddle );
        draw_region_at( PaddlePosition.x, PaddlePosition.y );
        
        // draw the ball
        select_region( FirstRegionBall + BallRegionOffset );
        draw_region_at( BallPosition.x, BallPosition.y );
        
        // advance ball animation
        ++BallElapsedFrames;
        
        if( BallElapsedFrames >= 4 )
        {
            BallElapsedFrames = 0;
            ++BallRegionOffset;
            
            BallRegionOffset &= 3;
        }
        
        // wait until next frame
        end_frame();
    }
}
