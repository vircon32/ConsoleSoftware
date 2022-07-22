// begin include guard
#ifndef TEXTUREREGIONS_TEXTUREMINESWEEPER
#define TEXTUREREGIONS_TEXTUREMINESWEEPER

// define names for single regions
#define RegionUpperBoard  0
#define RegionFrameBottomHard  1
#define RegionFrameTopEasy  2
#define RegionFrameBottomEasy  3
#define RegionFrameBottomMedium  4
#define RegionFrameRightHard  5
#define RegionFrameLeftEasy  6
#define RegionFrameRightEasy  7
#define RegionFrameLeftMedium  8
#define RegionFrameRightMedium  9
#define RegionInstructionsLeftHard  10
#define RegionInstructionsLeftEasy  11
#define RegionInstructionsRightEasy  12
#define RegionDialogWin  13
#define RegionDialogLose  14
#define RegionDialogQuit  15
#define RegionTitleGradient  16
#define RegionTitleLetters  17
#define RegionTitleLogo  18
#define Region2022  19
#define RegionCarra  20
#define RegionTextChooseMode  21
#define RegionTextPressStart  22
#define RegionExplosionSmall  23
#define RegionExplosionLarge  24

// define names for first region in region matrices
#define FirstRegionTilesCoveredLarge  25
#define FirstRegionTilesRevealedLarge  29
#define FirstRegionTilesCoveredSmall  33
#define FirstRegionTilesRevealedSmall  37
#define FirstRegionIconsLarge  41
#define FirstRegionIconsSmall  51
#define FirstRegionNumbersDigital  61
#define FirstRegionCursorMenu  71
#define FirstRegionCursorLarge  79
#define FirstRegionCursorSmall  83
#define FirstRegionSmileys  87
#define FirstRegionMenuOptions  92
#define FirstRegionModeIndicators  95
#define FirstRegionRevealLarge  98
#define FirstRegionRevealSmall  102
#define FirstRegionFlagLarge  106
#define FirstRegionUnflagLarge  109
#define FirstRegionFlagSmall  112
#define FirstRegionUnflagSmall  115
#define FirstRegionTilesCoveredLarge2  118

// your initialization code must call this function
void DefineRegions_TextureMineSweeper() 
{
    select_texture( TextureMineSweeper );

    // define single regions
    select_region( RegionUpperBoard );
    define_region( 1,1,  640,35,  1,1 );
    select_region( RegionFrameBottomHard );
    define_region( 1,39,  601,42,  1,39 );
    select_region( RegionFrameTopEasy );
    define_region( 1,46,  317,47,  1,46 );
    select_region( RegionFrameBottomEasy );
    define_region( 1,51,  317,56,  1,51 );
    select_region( RegionFrameBottomMedium );
    define_region( 1,60,  321,63,  1,60 );
    select_region( RegionFrameRightHard );
    define_region( 39,67,  42,391,  42,391 );
    select_region( RegionFrameLeftEasy );
    define_region( 206,67,  209,391,  206,67 );
    select_region( RegionFrameRightEasy );
    define_region( 213,67,  216,391,  213,67 );
    select_region( RegionFrameLeftMedium );
    define_region( 381,67,  382,391,  381,67 );
    select_region( RegionFrameRightMedium );
    define_region( 386,67,  387,391,  386,67 );
    select_region( RegionInstructionsLeftHard );
    define_region( 1,67,  35,391,  1,391 );
    select_region( RegionInstructionsLeftEasy );
    define_region( 46,67,  202,391,  46,391 );
    select_region( RegionInstructionsRightEasy );
    define_region( 220,67,  377,391,  377,391 );
    select_region( RegionDialogWin );
    define_region( 391,46,  665,162,  528,102 );
    select_region( RegionDialogLose );
    define_region( 391,166,  665,282,  528,222 );
    select_region( RegionDialogQuit );
    define_region( 391,286,  665,389,  528,329 );
    select_region( RegionTitleGradient );
    define_region( 669,1,  669,360,  669,1 );
    select_region( RegionTitleLetters );
    define_region( 1,395,  343,497,  176,395 );
    select_region( RegionTitleLogo );
    define_region( 1,501,  343,652,  176,501 );
    select_region( Region2022 );
    define_region( 347,566,  407,581,  347,581 );
    select_region( RegionCarra );
    define_region( 411,566,  469,581,  469,581 );
    select_region( RegionTextChooseMode );
    define_region( 347,395,  607,426,  347,395 );
    select_region( RegionTextPressStart );
    define_region( 347,430,  570,461,  459,430 );
    select_region( RegionExplosionSmall );
    define_region( 615,677,  633,756,  624,718 );
    select_region( RegionExplosionLarge );
    define_region( 637,617,  669,756,  653,689 );

    // define region matrices
    define_region_matrix( FirstRegionTilesCoveredLarge, 1,656,  37,692,  1,656,  4,1,  1 );
    define_region_matrix( FirstRegionTilesRevealedLarge, 155,656,  191,692,  155,656,  4,1,  1 );
    define_region_matrix( FirstRegionTilesCoveredSmall, 223,736,  243,756,  223,736,  4,1,  1 );
    define_region_matrix( FirstRegionTilesRevealedSmall, 313,736,  333,756,  313,736,  4,1,  1 );
    define_region_matrix( FirstRegionIconsLarge, 1,696,  37,732,  1,696,  10,1,  1 );
    define_region_matrix( FirstRegionIconsSmall, 1,736,  21,756,  1,736,  10,1,  1 );
    define_region_matrix( FirstRegionNumbersDigital, 347,615,  359,635,  347,615,  10,1,  1 );
    define_region_matrix( FirstRegionCursorMenu, 349,639,  372,662,  372,650,  8,1,  1 );
    define_region_matrix( FirstRegionCursorLarge, 383,688,  423,728,  385,690,  4,1,  1 );
    define_region_matrix( FirstRegionCursorSmall, 427,732,  451,756,  429,734,  4,1,  1 );
    define_region_matrix( FirstRegionSmileys, 347,585,  373,611,  347,585,  5,1,  1 );
    define_region_matrix( FirstRegionMenuOptions, 347,465,  479,496,  347,465,  1,3,  1 );
    define_region_matrix( FirstRegionModeIndicators, 483,465,  549,478,  483,465,  1,3,  1 );
    define_region_matrix( FirstRegionRevealLarge, 613,393,  665,445,  621,401,  1,4,  1 );
    define_region_matrix( FirstRegionRevealSmall, 579,430,  609,460,  584,435,  1,4,  1 );
    define_region_matrix( FirstRegionFlagLarge, 489,513,  525,549,  489,513,  1,3,  1 );
    define_region_matrix( FirstRegionUnflagLarge, 529,513,  565,549,  529,513,  1,3,  1 );
    define_region_matrix( FirstRegionFlagSmall, 569,692,  589,712,  569,692,  1,3,  1 );
    define_region_matrix( FirstRegionUnflagSmall, 591,692,  611,712,  591,692,  1,3,  1 );
    define_region_matrix( FirstRegionTilesCoveredLarge2, 1,760,  39,798,  1,760,  4,1,  1 );
}

// end include guard
#endif
