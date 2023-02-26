// begin include guard
#ifndef TEXTUREREGIONS_TEXTURERPG
#define TEXTUREREGIONS_TEXTURERPG

// define names for single regions
#define RegionTree  0
#define RegionShadowTree  1
#define RegionShadowPerson  2
#define RegionFramePortrait  3
#define RegionFrameNameLeft  4
#define RegionFrameNameCenter  5
#define RegionFrameTextLeft  6
#define RegionFrameTextCenter  7

// define names for first region in region matrices
#define FirstRegionPlayerWalkDown  8
#define FirstRegionPlayerWalkUp  11
#define FirstRegionPlayerWalkRight  14
#define FirstRegionPeasantWalkDown  17
#define FirstRegionPeasantWalkUp  20
#define FirstRegionPeasantWalkRight  23
#define FirstRegionPortraitPeasant  26
#define FirstRegionPortraitPlayer  28
#define FirstRegionCursorDown  30
#define FirstRegionTiles  38

// your initialization code must call this function
void DefineRegions_TextureRPG() 
{
    select_texture( TextureRPG );

    // define single regions
    select_region( RegionTree );
    define_region( 1,1,  91,151,  46,139 );
    select_region( RegionShadowTree );
    define_region( 1,155,  91,185,  46,168 );
    select_region( RegionShadowPerson );
    define_region( 57,189,  91,201,  74,194 );
    select_region( RegionFramePortrait );
    define_region( 163,232,  265,334,  163,232 );
    select_region( RegionFrameNameLeft );
    define_region( 269,232,  281,264,  269,232 );
    select_region( RegionFrameNameCenter );
    define_region( 285,232,  285,264,  285,232 );
    select_region( RegionFrameTextLeft );
    define_region( 269,268,  281,335,  269,268 );
    select_region( RegionFrameTextCenter );
    define_region( 285,268,  285,335,  285,268 );

    // define region matrices
    define_region_matrix( FirstRegionPlayerWalkDown, 95,1,  125,74,  110,72,  3,1,  1 );
    define_region_matrix( FirstRegionPlayerWalkUp, 95,78,  125,151,  110,147,  3,1,  1 );
    define_region_matrix( FirstRegionPlayerWalkRight, 95,155,  125,228,  112,228,  3,1,  1 );
    define_region_matrix( FirstRegionPeasantWalkDown, 193,2,  225,74,  209,72,  3,1,  1 );
    define_region_matrix( FirstRegionPeasantWalkUp, 193,79,  225,151,  209,147,  3,1,  1 );
    define_region_matrix( FirstRegionPeasantWalkRight, 193,155,  225,227,  211,227,  3,1,  1 );
    define_region_matrix( FirstRegionPortraitPeasant, 1,232,  79,310,  1,232,  2,1,  1 );
    define_region_matrix( FirstRegionPortraitPlayer, 1,314,  79,392,  1,314,  2,1,  1 );
    define_region_matrix( FirstRegionCursorDown, 163,369,  186,392,  163,369,  8,1,  1 );
    define_region_matrix( FirstRegionTiles, 297,1,  336,40,  297,1,  3,6,  0 );
}

// end include guard
#endif
