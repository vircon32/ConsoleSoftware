// begin include guard
#ifndef TEXTUREREGIONS_TEXTUREGAMEPLAY
#define TEXTUREREGIONS_TEXTUREGAMEPLAY

// define names for single regions
#define RegionBackground  0
#define RegionDialogGameOver  1
#define RegionDialogQuit  2
#define RegionTopLine  3

// define names for first region in region matrices
#define FirstRegionPieceTiles  4
#define FirstRegionPieces  132
#define FirstRegionDigitsLevel  140
#define FirstRegionDigitsLines  150
#define FirstRegionCursorRight  160

// your initialization code must call this function
void DefineRegions_TextureGameplay() 
{
    select_texture( TextureGameplay );

    // define single regions
    select_region( RegionBackground );
    define_region( 0,0,  639,359,  0,0 );
    select_region( RegionDialogGameOver );
    define_region( 263,363,  537,479,  400,417 );
    select_region( RegionDialogQuit );
    define_region( 263,483,  537,586,  400,524 );
    select_region( RegionTopLine );
    define_region( 365,590,  556,601,  365,590 );

    // define region matrices
    define_region_matrix( FirstRegionPieceTiles, 1,363,  16,378,  1,363,  16,8,  0 );
    define_region_matrix( FirstRegionPieces, 1,494,  64,525,  1,494,  4,2,  1 );
    define_region_matrix( FirstRegionDigitsLevel, 1,562,  22,589,  1,562,  10,1,  1 );
    define_region_matrix( FirstRegionDigitsLines, 1,593,  15,613,  1,593,  10,1,  1 );
    define_region_matrix( FirstRegionCursorRight, 163,593,  186,616,  186,604,  8,1,  1 );
}

// end include guard
#endif
