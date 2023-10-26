// begin include guard
#ifndef TEXTUREREGIONS_TEXTUREGAMEPLAY
#define TEXTUREREGIONS_TEXTUREGAMEPLAY

// define names for single regions
#define RegionLogo  0
#define RegionPressStart  1
#define Region2023  2
#define RegionCarra  3
#define RegionGround  4
#define RegionHealthBarFrame  5
#define RegionShadowTank  6
#define RegionShadowTankDiagonal  7

// define names for first region in region matrices
#define FirstRegionTileset  8
#define FirstRegionPlayerIcons  20
#define FirstRegionHealthBars  24
#define FirstRegionNumbers  28
#define FirstRegionCircleHighlight  38
#define FirstRegionTankBodiesUp  44
#define FirstRegionTankBodiesUpRight  48
#define FirstRegionTankBodiesRight  52
#define FirstRegionTurrets  56
#define FirstRegionBullets  64
#define FirstRegionShotBreaking  72

// your initialization code must call this function
void DefineRegions_TextureGameplay() 
{
    select_texture( TextureGameplay );

    // define single regions
    select_region( RegionLogo );
    define_region( 391,438,  576,528,  483,438 );
    select_region( RegionPressStart );
    define_region( 164,438,  387,469,  276,438 );
    select_region( Region2023 );
    define_region( 1,486,  61,501,  1,501 );
    select_region( RegionCarra );
    define_region( 65,486,  123,501,  123,501 );
    select_region( RegionGround );
    define_region( 0,0,  639,359,  0,0 );
    select_region( RegionHealthBarFrame );
    define_region( 1,505,  66,522,  1,505 );
    select_region( RegionShadowTank );
    define_region( 478,536,  532,590,  501,560 );
    select_region( RegionShadowTankDiagonal );
    define_region( 536,536,  599,598,  562,565 );

    // define region matrices
    define_region_matrix( FirstRegionTileset, 1,363,  40,402,  1,363,  4,3,  0 );
    define_region_matrix( FirstRegionPlayerIcons, 164,473,  205,507,  164,473,  4,1,  1 );
    define_region_matrix( FirstRegionHealthBars, 70,505,  70,516,  70,505,  4,1,  1 );
    define_region_matrix( FirstRegionNumbers, 405,604,  426,635,  415,619,  10,1,  0 );
    define_region_matrix( FirstRegionCircleHighlight, 164,363,  235,434,  199,398,  6,1,  1 );
    define_region_matrix( FirstRegionTankBodiesUp, 1,536,  51,586,  26,561,  4,1,  1 );
    define_region_matrix( FirstRegionTankBodiesUpRight, 211,536,  275,600,  243,568,  4,1,  1 );
    define_region_matrix( FirstRegionTankBodiesRight, 195,604,  245,654,  220,629,  4,1,  1 );
    define_region_matrix( FirstRegionTurrets, 1,590,  23,639,  12,630,  8,1,  1 );
    define_region_matrix( FirstRegionBullets, 604,363,  615,374,  609,368,  1,8,  1 );
    define_region_matrix( FirstRegionShotBreaking, 619,363,  638,382,  628,372,  1,4,  1 );
}

// end include guard
#endif
