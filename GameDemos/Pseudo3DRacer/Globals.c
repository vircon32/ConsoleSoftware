// ---------------------------------------------------------
//   DEFINITIONS AND CONSTANTS
// ---------------------------------------------------------


// game textures
#define TextureRoad 0

// game sounds
#define SoundRoad 0
#define SoundEngine 1

// sound channels
#define ChannelRoad 14
#define ChannelEngine 15

// road dimensions in pixels
// (same units used in texture and 3D space)
#define RoadWidth 512
#define RoadLength 512

// the first texture region where we
// will store the road split in scanlines
#define FirstRegionRoadLines 100

// screen heights for horizons
// (drawn horizon is lower in order
// to avoid infinite distances)
#define TrueHorizonY   104
#define DrawnHorizonY  107

// distance ranges for road objects
#define ObjectNearZ 5
#define ObjectZRange 8000
#define ObjectFarZ (ObjectZRange + ObjectNearZ)

// distance ranges for mountains
#define MountainNearZ  2300
#define MountainFarZ   17300
#define MountainZRange (MountainFarZ - MountainNearZ)

// used for points and distances in 3D space
struct Vector3D
{
    float x, y, z;
};


// ---------------------------------------------------------
//   GLOBALS
// ---------------------------------------------------------


// arrays to store per-line precalculations
float[ screen_height ] LineDistanceZ;
float[ screen_height ] LineDrawingScale;
int[ screen_height ] LineMultiplyColor;

// road horizontal curvature level
float CurveXFactor;

// clouds current position over the background
float CloudsX;

// movement variables for the car
float TraveledDistance;
float CarSpeed;
