// begin include guard
#ifndef TEXTUREREGIONS_TEXTURETITLE
#define TEXTUREREGIONS_TEXTURETITLE

// define names for single regions
#define RegionPressStart  0
#define RegionChooseLevel  1
#define Region2023  2
#define RegionCarra  3
#define RegionMarginLeft  4
#define RegionMarginRight  5

// define names for first region in region matrices
#define FirstRegionLevels  6
#define FirstRegionCursorUp  10

// your initialization code must call this function
void DefineRegions_TextureTitle() 
{
    select_texture( TextureTitle );

    // define single regions
    select_region( RegionPressStart );
    define_region( 1,5,  224,36,  112,5 );
    select_region( RegionChooseLevel );
    define_region( 1,59,  261,90,  131,59 );
    select_region( Region2023 );
    define_region( 1,40,  62,55,  1,55 );
    select_region( RegionCarra );
    define_region( 66,40,  124,55,  124,55 );
    select_region( RegionMarginLeft );
    define_region( 1,1,  118,1,  1,1 );
    select_region( RegionMarginRight );
    define_region( 120,1,  237,1,  237,1 );

    // define region matrices
    define_region_matrix( FirstRegionLevels, 1,94,  56,137,  1,94,  4,1,  1 );
    define_region_matrix( FirstRegionCursorUp, 1,141,  24,164,  12,141,  8,1,  1 );
}

// end include guard
#endif
