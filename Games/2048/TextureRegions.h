void DefineTextureRegions()
{
    select_texture( Texture2048 );
    
    // ------------------------------------
    // REGIONS FOR TITLE
    // ------------------------------------
    
    select_region( RegionTitleLogo );
    define_region_topleft( 610,361,  857,484 );
    
    select_region( RegionPressStart );
    define_region_topleft( 612,488,  835,519 );
    
    select_region( RegionTileHighlight );
    define_region_center( 784,558,  855,629 );
    
    select_region( Region2021 );
    define_region( 612,638,  672,653,  612,653 );
    
    select_region( RegionCarra );
    define_region( 676,638,  734,653,  734,653 );
    
    // ------------------------------------
    // REGIONS FOR GAMEPLAY
    // ------------------------------------
    
    select_region( RegionBackground );
    define_region_topleft( 0,0,  639,359 );
    
    select_region( RegionGUI );
    define_region( 641,0,  858,359,  858,0 );
    
    select_region( RegionBoard );
    define_region_topleft( 1,361,  332,692 );
    
    select_region( RegionPointsPlus );
    define_region_topleft( 734,583,  745,598 );
    
    select_region( RegionIndicatorH );
    define_region( 1,696,  321,711,  161,711 );
    
    select_region( RegionIndicatorV );
    define_region( 859,550,  874,870,  874,710 );
    
    // ------------------------------------
    // REGIONS FOR DIALOGS
    // ------------------------------------
    
    select_region( RegionDialogWin );
    define_region_topleft( 334,361,  608,452 );
    
    select_region( RegionDialogLose );
    define_region_topleft( 334,517,  608,608 );
    
    select_region( RegionDialogEnd );
    define_region_topleft( 334,673,  608,714 );
    
    select_region( RegionDialogYesNo );
    define_region_topleft( 334,454,  608,515 );
    
    select_region( RegionDialogExit );
    define_region_topleft( 334,610,  608,671 );
    
    select_region( RegionTextContinue );
    define_region_topleft( 612,619,  766,634 );
    
    select_region( RegionTextEnded );
    define_region_topleft( 612,602,  766,617 );
    
    
    // ------------------------------------
    // REGION GROUPS
    // ------------------------------------
    
    define_region_matrix( RegionTile2,  1,718,  76,793,  36,753,  8,2,  1 );
    define_region_matrix( RegionSecondsZero,  612,523,  633,554,  612,523,  10,1,  1 );
    define_region_matrix( RegionHundredsZero,  612,583,  622,598,  612,583,  10,1,  1 );
    define_region_matrix( RegionScoreZero,  612,558,  627,579,  612,558,  10,1,  1 );
    define_region_matrix( FirstRegionMerge,  619,726,  690,797,  654,761,  3,2,  1 );
    define_region_matrix( FirstRegionCursor,  619,699,  642,722,  642,711,  8,1,  1 );    
}
