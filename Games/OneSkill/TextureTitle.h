// begin include guard
#ifndef TEXTUREREGIONS_TEXTURETITLE
#define TEXTUREREGIONS_TEXTURETITLE

// define names for single regions
#define RegionBackgroundStrip  0
#define RegionBlueBand  1
#define RegionLogo  2
#define RegionPressStart  3
#define RegionSkillFrame  4
#define RegionSwitchArrows  5
#define Region2023  6
#define RegionCarra  7
#define RegionDiagonalLine  8

// your initialization code must call this function
void DefineRegions_TextureTitle() 
{
    select_texture( TextureTitle );

    // define single regions
    select_region( RegionBackgroundStrip );
    define_region( 1,1,  1,361,  1,1 );
    select_region( RegionBlueBand );
    define_region( 5,1,  408,360,  5,1 );
    select_region( RegionLogo );
    define_region( 412,1,  662,206,  412,1 );
    select_region( RegionPressStart );
    define_region( 412,210,  635,241,  412,210 );
    select_region( RegionSkillFrame );
    define_region( 412,245,  501,334,  412,245 );
    select_region( RegionSwitchArrows );
    define_region( 505,245,  567,296,  536,270 );
    select_region( Region2023 );
    define_region( 505,300,  565,315,  505,300 );
    select_region( RegionCarra );
    define_region( 505,319,  563,334,  505,319 );
    select_region( RegionDiagonalLine );
    define_region( 666,1,  788,360,  666,360 );
}

// end include guard
#endif
