// begin include guard
#ifndef TEXTUREREGIONS_TEXTURECUTSCENES
#define TEXTUREREGIONS_TEXTURECUTSCENES

// define names for single regions
#define RegionIntroSlide  0
#define RegionEndingSlide  1

// your initialization code must call this function
void DefineRegions_TextureCutscenes() 
{
    select_texture( TextureCutscenes );

    // define single regions
    select_region( RegionIntroSlide );
    define_region( 0,0,  411,191,  0,0 );
    select_region( RegionEndingSlide );
    define_region( 0,193,  411,384,  0,193 );
}

// end include guard
#endif
