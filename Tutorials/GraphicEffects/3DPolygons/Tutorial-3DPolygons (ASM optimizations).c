// include Vircon libraries
#include "video.h"
#include "input.h"
#include "time.h"
#include "math.h"
#include "misc.h"

// include our support libraries
#include "3DObjects.h"
#include "Icosahedron.h"


// -----------------------------------------------
// DEFINITIONS FOR TEXTURES
// -----------------------------------------------


// give names to each texture in our cartridge
#define Texture3DPolygons  0

// names for our texture regions
// within the background texture
#define RegionFullLine  0
#define RegionSegment   1


// -----------------------------------------------
// DEFINITIONS FOR GAMEPLAY
// -----------------------------------------------


// our only sample object
Object Icosahedron;


// -----------------------------------------------
// VERTEX FUNCTIONS
// -----------------------------------------------


// the 3 angles rotate around an axis each, in order X->Y->Z
void RotateVertex( Vertex* V, float AngleAroundX, float AngleAroundY, float AngleAroundZ )
{
    // rotate around X axis
    V->WorldPosition.y = cos( AngleAroundX ) * V->PositionInObject.y - sin( AngleAroundX ) * V->PositionInObject.z;
    V->WorldPosition.z = sin( AngleAroundX ) * V->PositionInObject.y + cos( AngleAroundX ) * V->PositionInObject.z;
    V->WorldPosition.x = V->PositionInObject.x;
    
    // rotate around Y axis
    // (we need a temporary copy of the position)
    float AuxX = cos( AngleAroundY ) * V->WorldPosition.x - sin( AngleAroundY ) * V->WorldPosition.z;
    float AuxZ = sin( AngleAroundY ) * V->WorldPosition.x + cos( AngleAroundY ) * V->WorldPosition.z;
    float AuxY = V->WorldPosition.y;
    
    // rotate around Z axis
    V->WorldPosition.x = cos( AngleAroundZ ) * AuxX - sin( AngleAroundZ ) * AuxY;
    V->WorldPosition.y = sin( AngleAroundZ ) * AuxX + cos( AngleAroundZ ) * AuxY;
    V->WorldPosition.z = AuxZ;
}

// -----------------------------------------------

// our simplified version assumes a static camera
// positioned at point { x = 0, y =0, z = -1000 }
// and always facing along Z axis
void ProjectVertex( Vertex* V )
{
    float ZDistance = V->WorldPosition.z + 1000;
    
    // careful! Origin of camera coordinates is at screen center, not at screen origin
    V->ScreenProjection.x = (screen_width/2) + 500*V->WorldPosition.x / ZDistance;
    
    // careful! Screen Y is inverted (it goes down, but in our 3D scene it's up)
    V->ScreenProjection.y = (screen_height/2) - 500*V->WorldPosition.y / ZDistance;
}


// -----------------------------------------------
// POLYGON FUNCTIONS
// -----------------------------------------------


// this is used to avoid drawing polygons that are not
// facing the camera, and therefore hidden behind others
bool IsFacingCamera( Polygon* P )
{
    // obtain 2 consecutive pixel-pixel vectors
    Pixel2D V1, V2;
    V1.x = P->Vertices[ 1 ]->ScreenProjection.x - P->Vertices[ 0 ]->ScreenProjection.x;
    V1.y = P->Vertices[ 1 ]->ScreenProjection.y - P->Vertices[ 0 ]->ScreenProjection.y;
    
    V2.x = P->Vertices[ 2 ]->ScreenProjection.x - P->Vertices[ 1 ]->ScreenProjection.x;
    V2.y = P->Vertices[ 2 ]->ScreenProjection.y - P->Vertices[ 1 ]->ScreenProjection.y;
    
    // perform a vector product
    float ZComponent = V1.x * V2.y - V1.y * V2.x;
    
    return (ZComponent <= 0);
}

// -----------------------------------------------

// calculating the outer normal vector of a polygon
// lets us determine how a light source affects it
void CalculateNormal( Polygon* P )
{
    // obtain 2 consecutive vertex-vertex vectors
    Vector3D V1, V2;
    V1.x = P->Vertices[ 1 ]->WorldPosition.x - P->Vertices[ 0 ]->WorldPosition.x;
    V1.y = P->Vertices[ 1 ]->WorldPosition.y - P->Vertices[ 0 ]->WorldPosition.y;
    V1.z = P->Vertices[ 1 ]->WorldPosition.z - P->Vertices[ 0 ]->WorldPosition.z;
    
    V2.x = P->Vertices[ 2 ]->WorldPosition.x - P->Vertices[ 1 ]->WorldPosition.x;
    V2.y = P->Vertices[ 2 ]->WorldPosition.y - P->Vertices[ 1 ]->WorldPosition.y;
    V2.z = P->Vertices[ 2 ]->WorldPosition.z - P->Vertices[ 1 ]->WorldPosition.z;
    
    // perform a vector product
    P->OuterNormal.x = V1.z * V2.y - V1.y * V2.z;
    P->OuterNormal.y = V1.x * V2.z - V1.z * V2.x;
    P->OuterNormal.z = V1.y * V2.x - V1.x * V2.y;
    
    // to ensure a true reference we need to
    // normalize the result to a unit vector
    float Modulus2 = (P->OuterNormal.x * P->OuterNormal.x)
                   + (P->OuterNormal.y * P->OuterNormal.y)
                   + (P->OuterNormal.z * P->OuterNormal.z);
    
    float Modulus = sqrt( Modulus2 );
    P->OuterNormal.x /= Modulus;
    P->OuterNormal.y /= Modulus;
    P->OuterNormal.z /= Modulus;
}

// -----------------------------------------------

// we will do basic shading: each polygon has a "neutral color"
// corresponding to a 50% light level. If it receives more or
// less light than that we will make the color lighter or darker
void DetermineLightedColor( Polygon* P )
{
    // extract neutral color components, disregarding alpha
    int Red   = P->NeutralColor & 255;
    int Green = (P->NeutralColor >> 8) & 255;
    int Blue  = (P->NeutralColor >> 16) & 255;
    
    // determine the light level in range [-1,+1]
    // (here we just assume light comes from the right)
    float LightLevel = P->OuterNormal.x;
    
    // for positive light level interpolate towards white
    if( LightLevel > 0 )
    {
        float WhitenessFactor = LightLevel;
        
        Red   = Red   + WhitenessFactor * (255-Red  );
        Green = Green + WhitenessFactor * (255-Green);
        Blue  = Blue  + WhitenessFactor * (255-Blue );
    }
    
    // for negative light level interpolate towards black
    else
    {
        float BlacknessFactor = 1 + LightLevel;
        
        Red   = Red   * BlacknessFactor;
        Green = Green * BlacknessFactor;
        Blue  = Blue  * BlacknessFactor;
    }
    
    // save the polygon's resulting color
    P->LightedColor = make_color_rgb( Red, Green, Blue );
}

// -----------------------------------------------

// our technique to render a polygon is to decompose
// it in a series of horizontal scanlines, 1 pixel each
void RenderFlatPolygon( Polygon* P )
{
    // polygons facing away from the camera are not drawn
    if( !IsFacingCamera( P ) )
      return;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - -
    // STEP 1: Preparations
    
    // project all vertices to the screen,
    // and keep track of which one is the
    // topmost and the lowest
    int MinimumY = +1000;
    int MaximumY = -1000;
    
    for( int i = 0; i < P->NumberOfVertices; i++ )
    {
        if( P->Vertices[ i ]->ScreenProjection.y < MinimumY )
          MinimumY = P->Vertices[ i ]->ScreenProjection.y;
        
        if( P->Vertices[ i ]->ScreenProjection.y > MaximumY )
          MaximumY = P->Vertices[ i ]->ScreenProjection.y;
    }
    
    // to be able to iterate, duplicate the
    // first vertex and form a closed cycle
    P->Vertices[ P->NumberOfVertices ] = P->Vertices[ 0 ];
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - -
    // STEP 2: Determine all scanlines to draw the polygon
    
    // we will use a set of values
    int[ screen_height ] LineMinX, LineMaxX;
    int Lines = MaximumY - MinimumY + 1;
    memset( &LineMinX[ MinimumY ], +1000, Lines );
    memset( &LineMaxX[ MinimumY ], -1000, Lines );
    
    // these are used only for assembler optimizations
    int* FirstMin = LineMinX;
    int* FirstMax = LineMaxX;
    
    // interpolate between each vertex and the next
    // to determine the extension of each scanline
    for( int i = 0; i < P->NumberOfVertices; i++ )
    {
        // take reference to the 2 screen points
        Pixel2D Pixel1 = P->Vertices[ i ]->ScreenProjection;
        Pixel2D Pixel2 = P->Vertices[ i+1 ]->ScreenProjection;
        
        // if their scanline is the same
        // we have no need to interpolate
        if( Pixel1.y == Pixel2.y )
        {
            LineMinX[ Pixel1.y ] = min( Pixel1.x, Pixel2.x );
            LineMaxX[ Pixel1.y ] = max( Pixel1.x, Pixel2.x );
            continue;
        }
        
        // ensure that in path V1->V2 screen Y always
        // grows (and otherwise, swap them to make it so)
        if( Pixel1.y > Pixel2.y )
        {
            Pixel2 = P->Vertices[ i ]->ScreenProjection;
            Pixel1 = P->Vertices[ i+1 ]->ScreenProjection;
        }
        
        // initialize our interpolation of x in this polygon edge
        float CurrentX = Pixel1.x;
        float XIncrement = (Pixel2.x - Pixel1.x);
        XIncrement /= (Pixel2.y - Pixel1.y);
        
        // interpolate filling extreme points of each scanline
        for( int y = Pixel1.y; y <= Pixel2.y; y++ )
        {
            asm
            {
                // R0 = CurrentX
                "mov R0, {CurrentX}"
                
                // R1 = y
                "mov R1, {y}"
                
                // R2 = &LineMinX[ y ]
                "mov R2, {FirstMin}"
                "iadd R2, R1"
                
                // R3 = &LineMaxX[ y ]
                "mov R3, {FirstMax}"
                "iadd R3, R1"
                
                // convert CurrentX to int for comparisons
                "mov R4, R0"
                "cfi R4"
                
                // update minimum if needed
                "mov R5, R4"
                "mov R6, [R2]"
                "imin R5, R6"
                "mov [R2], R5"
                
                // update maximum if needed
                "mov R5, R4"
                "mov R6, [R3]"
                "imax R5, R6"
                "mov [R3], R5"
                
                // increment CurrentX
                "mov R1, {XIncrement}"
                "fadd R0, R1"
                "mov {CurrentX}, R0"
            };
            
            /*
            if( CurrentX < LineMinX[ y ] )
              LineMinX[ y ] = CurrentX;
            
            if( CurrentX > LineMaxX[ y ] )
              LineMaxX[ y ] = CurrentX;
            
            CurrentX += XIncrement;*/
        }
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - -
    // STEP 3: Draw all the scanlines saved previously
    
    // For each scanline we will redefine the same segment
    // region as needed. Every time it is drawn at a different
    // screen Y and with different length, but in the same color
    set_multiply_color( P->LightedColor );
    select_region( RegionSegment );
    
    for( int ScreenY = MinimumY; ScreenY <= MaximumY; ScreenY++ )
    {
        asm
        {
            // R0 = ScreenY
            "mov R0, {ScreenY}"
            
            // R1 = LineMaxX[ ScreenY ]
            "mov R1, {FirstMax}"
            "iadd R1, R0"
            "mov R1, [R1]"
            
            // R1 = LineMinX[ ScreenY ]
            "out GPU_RegionMaxX, R1"
            "mov R1, {FirstMin}"
            "iadd R1, R0"
            "mov R1, [R1]"
            
            // redefine the region
            "out GPU_RegionMinX, R1"
            "out GPU_RegionHotspotX, R1"
            
            // draw the modified region
            "out GPU_DrawingPointX, R1"
            "out GPU_DrawingPointY, R0"
            "out GPU_Command, GPUCommand_DrawRegion"
        };
        
        // define_region_topleft( LineMinX[ ScreenY ], 0, LineMaxX[ ScreenY ], 0 );
        // draw_region_at( LineMinX[ ScreenY ], ScreenY );
    }
}


// -----------------------------------------------
// OBJECT FUNCTIONS
// -----------------------------------------------


// this function applies all needed processing so that
// the object is updated and can be drawn on screen
void ProcessObject( Object* O )
{
    // first rotate and then project every vertex
    for( int i = 0; i < O->NumberOfVertices; i++ )
    {
        RotateVertex( &O->Vertices[ i ], O->AngleAroundX, O->AngleAroundY, O->AngleAroundZ );
        ProjectVertex( &O->Vertices[ i ] );
    }
    
    // then for every polygon calculate normal and lighting
    for( int i = 0; i < O->NumberOfPolygons; i++ )
    {
        CalculateNormal( &O->Polygons[ i ] );
        DetermineLightedColor( &O->Polygons[ i ] );
    }
}

// -----------------------------------------------

// drawing a 3D object is simply drawing all its faces
void RenderFlatObject( Object* O )
{
    for( int i = 0; i < O->NumberOfPolygons; i++ )
      RenderFlatPolygon( &O->Polygons[ i ] );
}


// -----------------------------------------------
// MAIN FUNCTION
// -----------------------------------------------


void main( void )
{
    // -----------------------------------------------
    // PART 1: DEFINE OUR TEXTURE REGIONS
    
    select_texture( Texture3DPolygons );
    
    // (we don't define any region: our polygon
    // drawing functions will use them dynamically)
    
    // -----------------------------------------------
    // PART 2: DEMO INITIALIZATIONS
    
    // we will control the ship with gamepad 1
    select_gamepad( 0 );
    
    // create our 3D object
    MakeIcosahedron( &Icosahedron, 400 );
    
    // -----------------------------------------------
    // PART 3: MAIN LOOP
    
    // our demo never ends: keep repeating
    // our logic for every frame (60 fps)
    while( true )
    {
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 1: Simulate the scene
        
        // rotate our object around all axes
        Icosahedron.AngleAroundY += 2*pi/150;
        Icosahedron.AngleAroundZ += 2*pi/300;
        Icosahedron.AngleAroundX += 2*pi/450;
        
        // calculate our object rotation and lighting
        ProcessObject( &Icosahedron );
        
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 2: Draw the scene on screen
        
        // set a dark blue background
        clear_screen( make_color_rgb( 0,40,80 ) );
        
        // draw our object
        RenderFlatObject( &Icosahedron );
        
        // - - - - - - - - - - - - - - - - - - - - - -
        // Step 3: Wait for next frame
        end_frame();
    }
}
