// begin include guard
#ifndef TEXTUREREGIONS_TEXTUREPIANO
#define TEXTUREREGIONS_TEXTUREPIANO

// define names for single regions
#define RegionControls  0
#define RegionStatePanel  1
#define RegionWhiteKey  2
#define RegionWhiteKeyPressed  3
#define RegionBlackKey  4
#define RegionBlackKeyPressed  5
#define RegionArrow  6
#define RegionVolumeSlider  7

// define names for first region in region matrices
#define FirstRegionPlayButtons  8

// your initialization code must call this function
void DefineRegions_TexturePiano() 
{
    select_texture( TexturePiano );

    // define single regions
    select_region( RegionControls );
    define_region( 1,1,  269,115,  1,1 );
    select_region( RegionStatePanel );
    define_region( 1,119,  239,233,  1,119 );
    select_region( RegionWhiteKey );
    define_region( 243,119,  269,214,  243,119 );
    select_region( RegionWhiteKeyPressed );
    define_region( 271,119,  297,214,  271,119 );
    select_region( RegionBlackKey );
    define_region( 243,218,  260,275,  243,218 );
    select_region( RegionBlackKeyPressed );
    define_region( 262,218,  279,275,  262,218 );
    select_region( RegionArrow );
    define_region( 144,237,  169,267,  149,267 );
    select_region( RegionVolumeSlider );
    define_region( 173,237,  182,252,  173,237 );

    // define region matrices
    define_region_matrix( FirstRegionPlayButtons, 1,237,  46,282,  1,237,  3,1,  1 );
}

// end include guard
#endif
