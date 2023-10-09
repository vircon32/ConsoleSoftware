// begin include guard
#ifndef TEXTUREREGIONS_TEXTUREFONT11X16
#define TEXTUREREGIONS_TEXTUREFONT11X16

/* -----------------------------------------------
   This file follows a format similar to the
   texture headers created by RegionEditor, but
   it was generated manually.
/  -------------------------------------------- */

#define FirstRegionFont11x16  0


void DefineRegions_TextureFont11x16()
{
    // - - - - - - - - - - - - - - - - - - -
    // 1.1: define regions for full font
    
    select_texture( TextureFont11x16 );
    
    // First we define define 128 consecutive regions (i.e. standard ASCII only)
    // with the same size and hotspot position, as for a constant-width font
    define_region_matrix( FirstRegionFont11x16,  0,0,  10,15,  0,15,  16,8,  0 );
    
    // then we redefine some characters to have different widths
    // (note that, for this font, upper and lowercase letters are the same)
    select_region( FirstRegionFont11x16 + 'M' );
    define_region( 44,160,  55,175,  44,175 );
    
    select_region( FirstRegionFont11x16 + 'W' );
    define_region( 57,160,  68,175,  57,175 );
    
    select_region( FirstRegionFont11x16 + 'I' );
    define_region( 70,160,  77,175,  70,175 );
    
    select_region( FirstRegionFont11x16 + 'T' );
    define_region( 79,160,  88,175,  79,175 );
    
    select_region( FirstRegionFont11x16 + 'E' );
    define_region( 90,160,  99,175,  90,175 );
}

// end include guard
#endif
