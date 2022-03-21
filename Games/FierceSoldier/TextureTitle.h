// begin include guard
#ifndef TEXTUREREGIONS_TEXTURETITLE
#define TEXTUREREGIONS_TEXTURETITLE

// define names for single regions
#define RegionTitleBackground  0

// define names for first region in region matrices
#define FirstRegionLogo  1

// your initialization code must call this function
void DefineRegions_TextureTitle() 
{
    select_texture( TextureTitle );

    // define single regions
    select_region( RegionTitleBackground );
    define_region( 1,1,  640,184,  1,184 );

    // define region matrices
    define_region_matrix( FirstRegionLogo, 1,188,  337,355,  1,188,  2,1,  1 );
}

// end include guard
#endif
