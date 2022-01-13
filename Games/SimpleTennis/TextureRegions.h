void DefineTextureRegions()
{
    // REGIONS FOR TEXT

    // font
    select_texture( TextureFont );
    define_region_matrix( FirstRegionTextFont,  0,0,  10,15,  0,15,  16,8,  0 );

    // menu selection
    select_region( RegionMenuSelectionLeft );
    define_region_center( 1,1,  4,26 );

    select_region( RegionMenuSelectionRight );
    define_region_center( 6,1,  9,26 );

    select_region( RegionMenuSelectionMiddle );
    define_region_center( 11,1,  23,26 );

    // LOGO
    // logo is stored in gameplay textures
    // to prevent font texture to become
    // exclusive to this game
    select_texture( TextureSimpleTennis );
    select_region( RegionLogo );
    define_region_center( 59,1,  238,166 );

    // REGIONS FOR GAMEPLAY
    
    // define our goal line region
    select_region( RegionGoalLine );
    define_region_topleft( 0,0,  3,359 );
	
	// define our middle line region
    select_region( RegionMiddleLine );
    define_region_topleft( 5,0,  8,359 );
	
    // paddle region
    select_region( Regionpaddle );
    define_region_center( 33,1, 47,83 );
	
    // Ball region
    select_region( RegionBall );
    define_region_center( 33,85, 57,109 );
	
	// Score region
	define_region_matrix( RegionScore0,  10,1,  31,32,  10,1,  1,10,  1 );

    // Arrow region
    select_region( RegionArrow );
    define_region_center( 10,331, 35,354 );

}


    