// begin include guard
#ifndef TEXTUREREGIONS_TEXTUREMAHJONG
#define TEXTUREREGIONS_TEXTUREMAHJONG

// define names for single regions
#define RegionTableMat  0
#define Region2023  1
#define RegionCarra  2
#define RegionLogo  3
#define RegionLogoChinese  4
#define RegionPressStart  5
#define RegionBlackSlope  6
#define RegionDialogWin  7
#define RegionDialogQuit  8
#define RegionHintHighlight  9
#define RegionGUIShadow  10
#define RegionGUIRemainingTiles  11
#define RegionTileEdgeBottom  12
#define RegionTileEdgeLeft  13
#define RegionTileEdgeTop  14
#define RegionTileEdgeRight  15
#define RegionLayoutHighlight  16
#define RegionChooseLayout  17
#define RegionSelectionHighlight  18

// define names for first region in region matrices
#define FirstRegionTileFronts  19
#define FirstRegionCursorDialog  199
#define FirstRegionActions  207
#define FirstRegionTitleTiles  214
#define FirstRegionSelection  216
#define FirstRegionTarget  218
#define FirstRegionPairAnimation  220
#define FirstRegionDigits  226
#define FirstRegionAlertTitles  236
#define FirstRegionAlertMessages  238
#define FirstRegionTileShadows  242
#define FirstRegionLayoutNames  250
#define FirstRegionLayoutImages  254

// your initialization code must call this function
void DefineRegions_TextureMahjong() 
{
    select_texture( TextureMahjong );

    // define single regions
    select_region( RegionTableMat );
    define_region( 0,0,  639,359,  0,0 );
    select_region( Region2023 );
    define_region( 430,569,  490,584,  430,584 );
    select_region( RegionCarra );
    define_region( 430,588,  488,603,  488,603 );
    select_region( RegionLogo );
    define_region( 1,602,  273,721,  1,602 );
    select_region( RegionLogoChinese );
    define_region( 643,237,  753,288,  643,237 );
    select_region( RegionPressStart );
    define_region( 1,566,  224,597,  1,566 );
    select_region( RegionBlackSlope );
    define_region( 757,237,  758,237,  757,237 );
    select_region( RegionDialogWin );
    define_region( 513,363,  787,479,  650,421 );
    select_region( RegionDialogQuit );
    define_region( 513,483,  787,586,  650,528 );
    select_region( RegionHintHighlight );
    define_region( 724,295,  771,352,  733,304 );
    select_region( RegionGUIShadow );
    define_region( 798,332,  896,332,  798,332 );
    select_region( RegionGUIRemainingTiles );
    define_region( 798,336,  896,387,  798,387 );
    select_region( RegionTileEdgeBottom );
    define_region( 724,356,  752,359,  724,316 );
    select_region( RegionTileEdgeLeft );
    define_region( 717,317,  720,359,  721,316 );
    select_region( RegionTileEdgeTop );
    define_region( 761,243,  790,243,  761,244 );
    select_region( RegionTileEdgeRight );
    define_region( 793,247,  793,286,  763,247 );
    select_region( RegionLayoutHighlight );
    define_region( 607,725,  762,830,  610,728 );
    select_region( RegionChooseLayout );
    define_region( 277,615,  578,646,  427,615 );
    select_region( RegionSelectionHighlight );
    define_region( 798,475,  827,514,  798,475 );

    // define region matrices
    define_region_matrix( FirstRegionTileFronts, 1,363,  15,382,  1,363,  18,10,  0 );
    define_region_matrix( FirstRegionCursorDialog, 228,569,  251,592,  251,580,  8,1,  1 );
    define_region_matrix( FirstRegionActions, 798,1,  896,46,  798,1,  1,7,  1 );
    define_region_matrix( FirstRegionTitleTiles, 643,1,  794,116,  695,106,  1,2,  1 );
    define_region_matrix( FirstRegionSelection, 798,391,  833,436,  801,394,  2,1,  1 );
    define_region_matrix( FirstRegionTarget, 798,440,  828,471,  813,456,  2,1,  1 );
    define_region_matrix( FirstRegionPairAnimation, 277,650,  348,721,  312,685,  6,1,  1 );
    define_region_matrix( FirstRegionDigits, 277,596,  287,611,  277,596,  10,1,  1 );
    define_region_matrix( FirstRegionAlertTitles, 274,363,  509,402,  393,402,  1,2,  1 );
    define_region_matrix( FirstRegionAlertMessages, 274,447,  509,475,  393,446,  1,4,  1 );
    define_region_matrix( FirstRegionTileShadows, 643,315,  659,336,  643,317,  4,2,  1 );
    define_region_matrix( FirstRegionLayoutNames, 717,655,  864,670,  717,670,  1,4,  1 );
    define_region_matrix( FirstRegionLayoutImages, 1,725,  150,824,  1,725,  4,1,  1 );
}

// end include guard
#endif
