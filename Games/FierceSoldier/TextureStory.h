// begin include guard
#ifndef TEXTUREREGIONS_TEXTURESTORY
#define TEXTUREREGIONS_TEXTURESTORY

// define names for single regions
#define RegionSlideFrame  0

// define names for first region in region matrices
#define FirstRegionIntroSlides  1
#define FirstRegionEndingSlides  4

// your initialization code must call this function
void DefineRegions_TextureStory() 
{
    select_texture( TextureStory );

    // define single regions
    select_region( RegionSlideFrame );
    define_region( 1,546,  412,737,  7,552 );

    // define region matrices
    define_region_matrix( FirstRegionIntroSlides, 1,1,  400,180,  1,1,  1,3,  1 );
    define_region_matrix( FirstRegionEndingSlides, 402,1,  801,180,  402,1,  1,3,  1 );
}

// end include guard
#endif
