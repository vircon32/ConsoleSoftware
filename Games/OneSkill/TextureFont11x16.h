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
    define_region_matrix( FirstRegionFont11x16,  0,0,  21,31,  0,31,  16,8,  0 );
    
    // then we redefine some characters to have different widths
    // (note that, for this font, upper and lowercase letters are the same)
    select_region( FirstRegionFont11x16 + 'M' );
    define_region( 22,0,  46,31,  22,31 );
    select_region( FirstRegionFont11x16 + 'm' );
    define_region( 22,0,  46,31,  22,31 );
    
    select_region( FirstRegionFont11x16 + 'W' );
    define_region( 66,0,  90,31,  66,31 );
    select_region( FirstRegionFont11x16 + 'w' );
    define_region( 66,0,  90,31,  66,31 );
    
    select_region( FirstRegionFont11x16 + 'I' );
    define_region( 110,0,  121,31,  110,31 );
    select_region( FirstRegionFont11x16 + 'i' );
    define_region( 110,0,  121,31,  110,31 );
    
    select_region( FirstRegionFont11x16 + ' ' );
    define_region( 0,64,  15,95,  0,95 );
}

// end include guard
#endif
