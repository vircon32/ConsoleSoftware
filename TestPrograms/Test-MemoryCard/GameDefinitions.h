// ---------------------------------------------------------
//   DEFINITIONS AND CONSTANTS
// ---------------------------------------------------------


// names for textures
#define TextureGame  0

// names for texture regions
#define RegionTile          10
#define RegionDog           11
#define RegionBone          12
#define RegionDogHouse      20
#define RegionWindow        30
#define RegionContentsText  31
#define RegionContentsInfo  32
#define RegionLoad          40
#define RegionSave          41
#define RegionCancel        42
#define RegionExit          43

// game scene parameters
#define TileSize 80
#define TilesInX  6
#define TilesInY  4


// ---------------------------------------------------------
//   SCENE DEFINITIONS
// ---------------------------------------------------------


// define our scene data types
struct Point
{
    int x,y;
};

struct SceneData
{
    Point PlayerPosition;
    Point ItemPosition;
    int Score;
};

// game global variables
game_signature GameSignature;
SceneData GameState;


// ---------------------------------------------------------
//   AUXILIARY FUNCTIONS
// ---------------------------------------------------------


void PrintScore( int x, int y, int Score )
{
    // write score with leading zeros up to 4 digits
    // (for this, just add 10000 and discard first digit)
    int[ 6 ] ScoreTextExtended;
    itoa( 10000+Score, ScoreTextExtended, 10 );
    print_at( x, y, &ScoreTextExtended[ 1 ] );    
}

// ---------------------------------------------------------

void clamp( int* Variable, int Minimum, int Maximum )
{
    if( *Variable < Minimum )  *Variable = Minimum;
    if( *Variable > Maximum )  *Variable = Maximum;
}

