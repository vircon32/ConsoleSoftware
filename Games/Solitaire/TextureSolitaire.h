// begin include guard
#ifndef TEXTUREREGIONS_TEXTURESOLITAIRE
#define TEXTUREREGIONS_TEXTURESOLITAIRE

// define names for single regions
#define RegionTableMat  0
#define RegionCardBack  1
#define RegionCardShadow  2
#define RegionCardEdgeDark  3
#define RegionCardSide  4
#define RegionCardLight  5
#define RegionCardEdgeLight  6
#define RegionWinHighlight  7
#define RegionDialogWin  8
#define RegionDialogQuit  9
#define RegionDialogWrongPick  10
#define RegionDialogWrongDrop  11
#define RegionHandInfo  12
#define RegionTitleMat  13
#define RegionLogo  14
#define RegionPressStart  15
#define RegionChooseMode  16
#define RegionMenuSide  17
#define RegionTextNothing  18
#define RegionText1Card  19
#define RegionTextNCards  20
#define Region2022  21
#define RegionCarra  22

// define names for first region in region matrices
#define FirstRegionCardFronts  23
#define FirstRegionActions  75
#define FirstRegionCursorDialog  80
#define FirstRegionDigits  88
#define FirstRegionCursorCard  98
#define FirstRegionCursorStack  100
#define FirstRegionModeTitles  102
#define FirstRegionModeDescriptions  105

// your initialization code must call this function
void DefineRegions_TextureSolitaire() 
{
    select_texture( TextureSolitaire );

    // define single regions
    select_region( RegionTableMat );
    define_region( 1,381,  640,740,  1,381 );
    select_region( RegionCardBack );
    define_region( 900,1,  967,94,  933,1 );
    select_region( RegionCardShadow );
    define_region( 900,98,  967,191,  933,98 );
    select_region( RegionCardEdgeDark );
    define_region( 971,98,  977,191,  943,98 );
    select_region( RegionCardSide );
    define_region( 971,1,  972,94,  971,1 );
    select_region( RegionCardLight );
    define_region( 900,195,  967,288,  933,195 );
    select_region( RegionCardEdgeLight );
    define_region( 971,195,  977,288,  1004,195 );
    select_region( RegionWinHighlight );
    define_region( 506,851,  581,952,  543,855 );
    select_region( RegionDialogWin );
    define_region( 1,744,  275,860,  138,803 );
    select_region( RegionDialogQuit );
    define_region( 279,744,  553,847,  416,790 );
    select_region( RegionDialogWrongPick );
    define_region( 644,625,  879,693,  761,664 );
    select_region( RegionDialogWrongDrop );
    define_region( 644,697,  879,765,  761,736 );
    select_region( RegionHandInfo );
    define_region( 891,862,  981,913,  891,862 );
    select_region( RegionTitleMat );
    define_region( 644,383,  981,621,  644,383 );
    select_region( RegionLogo );
    define_region( 279,851,  495,917,  279,851 );
    select_region( RegionPressStart );
    define_region( 279,921,  502,952,  279,921 );
    select_region( RegionChooseMode );
    define_region( 585,930,  828,961,  585,930 );
    select_region( RegionMenuSide );
    define_region( 789,796,  887,796,  789,796 );
    select_region( RegionTextNothing );
    define_region( 557,788,  631,803,  557,788 );
    select_region( RegionText1Card );
    define_region( 557,807,  616,822,  557,807 );
    select_region( RegionTextNCards );
    define_region( 635,788,  685,803,  635,788 );
    select_region( Region2022 );
    define_region( 620,807,  680,822,  620,807 );
    select_region( RegionCarra );
    define_region( 684,807,  742,822,  742,807 );

    // define region matrices
    define_region_matrix( FirstRegionCardFronts, 1,1,  68,94,  34,1,  13,4,  1 );
    define_region_matrix( FirstRegionActions, 891,625,  981,670,  891,625,  1,5,  1 );
    define_region_matrix( FirstRegionCursorDialog, 689,769,  712,792,  712,780,  8,1,  1 );
    define_region_matrix( FirstRegionDigits, 557,769,  567,784,  557,769,  10,1,  1 );
    define_region_matrix( FirstRegionCursorCard, 1,864,  72,961,  36,866,  2,1,  1 );
    define_region_matrix( FirstRegionCursorStack, 149,864,  220,895,  184,866,  1,2,  1 );
    define_region_matrix( FirstRegionModeTitles, 587,826,  716,857,  587,826,  1,3,  1 );
    define_region_matrix( FirstRegionModeDescriptions, 720,826,  845,841,  720,826,  1,3,  1 );
}

// end include guard
#endif
