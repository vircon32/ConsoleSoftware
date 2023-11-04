// begin include guard
#ifndef TEXTUREREGIONS_TEXTUREGEMMAZE
#define TEXTUREREGIONS_TEXTUREGEMMAZE

// define names for single regions
#define RegionGuiGems  0
#define RegionGuiTime  1
#define RegionPlayer  2
#define RegionTextStart  3
#define RegionTextDeath  4
#define RegionTextClear  5

// define names for first region in region matrices
#define FirstRegionTileSet  6
#define FirstRegionNumbers  14

// your initialization code must call this function
void DefineRegions_TextureGemMaze() 
{
    select_texture( TextureGemMaze );

    // define single regions
    select_region( RegionGuiGems );
    define_region( 1,103,  94,140,  1,103 );
    select_region( RegionGuiTime );
    define_region( 98,103,  179,140,  179,103 );
    select_region( RegionPlayer );
    define_region( 183,103,  222,142,  202,122 );
    select_region( RegionTextStart );
    define_region( 1,169,  106,239,  53,204 );
    select_region( RegionTextDeath );
    define_region( 110,169,  188,239,  149,204 );
    select_region( RegionTextClear );
    define_region( 1,243,  122,313,  61,278 );

    // define region matrices
    define_region_matrix( FirstRegionTileSet, 0,0,  49,49,  0,0,  4,2,  0 );
    define_region_matrix( FirstRegionNumbers, 1,144,  16,165,  1,144,  10,1,  1 );
}

// end include guard
#endif
