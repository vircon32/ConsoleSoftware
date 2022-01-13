void DefineTextureRegions()
{
    // ------------------------------------
    // TEXTURE: TITLE
    // ------------------------------------
    
    select_texture( TextureTitle );
    
    select_region( RegionTitleStage );
    define_region_topleft( 0,0,  639,147 );
    
    select_region( RegionTitleBubble );
    define_region( 1,149,  151,299,  76,224 );
    
    select_region( RegionLogoLeft );
    define_region( 153,149,  376,255,  376,255 );
    
    select_region( RegionLogoRight );
    define_region( 378,149,  601,255,  378,255 );
    
    select_region( RegionPressStart );
    define_region( 153,257,  380,288,  267,257 );
    
    select_region( RegionTitleStar );
    define_region_center( 382,257,  392,269 );
    
    select_region( RegionTitleDemo );
    define_region( 396,259,  439,268,  439,268 );
    
    // ------------------------------------
    // TEXTURE: MENUS
    // ------------------------------------
    
    select_texture( TextureMenus );
    
    select_region( RegionMenuTitle );
    define_region_topleft( 18,1,  657,148 );
    
    select_region( RegionMenuWall );
    define_region( 1,1,  16,211,  1,211 );
    
    select_region( RegionMenuBubble );
    define_region_center( 18,150,  168,300 );
    
    select_region( RegionMenuSelection );
    define_region( 170,150,  519,183,  217, 147 );
    
    define_region_matrix( RegionMenuTriple,  170,185,  462,216,  170,185,  1,4,  1 );
    define_region_matrix( RegionMenu1Player,  467,185,  655,216,  467,185,  1,3,  1 );
    
    // ------------------------------------
    // TEXTURE: CUTSCENES
    // ------------------------------------
    
    select_texture( TextureCutscenes );
    
    select_region( RegionEndingText );
    define_region_center( 1,119,  228,150 );
    
    select_region( RegionEndingComeBack );
    define_region_center( 1,102,  382,117 );
    
    select_region( RegionGameOverText );
    define_region_center( 1,152,  196,183 );
    
    select_region( RegionGameOverBubble );
    define_region( 232,121,  291,174,  258,147 );
    
    define_region_matrix( FirstRegionEndingChar,  1,1,  100,98,  50,98,  2,1,  1 );
    define_region_matrix( FirstRegionGameOverChar,  205,1,  300,88,  252,88,  2,1,  1 );
    
    
    // ------------------------------------
    // TEXTURE: GAMEPLAY
    // ------------------------------------
    
    select_texture( TextureGameplay );
    
    // scenery for the 5 worlds
    define_region_matrix( RegionTripleScoreboardWorld1,  1,1,  521,26,  1,1,  1,5,  1 );
    define_region_matrix( RegionTripleCeilingWorld1,  1,136,  521,150,  1,136,  1,5,  1 );
    define_region_matrix( RegionTripleWallWorld1,  564,1,  578,360,  564,1,  5,1,  1 );
    define_region_matrix( RegionClassicScoreboardWorld1,  1,218,  313,243,  1,218,  1,5,  1 );
    define_region_matrix( RegionClassicCeilingWorld1,  1,353,  313,367,  1,353,  1,5,  1 );
    define_region_matrix( RegionVersusScoreboardWorld1,  317,218,  525,243,  317,218,  1,5,  1 );
    define_region_matrix( RegionVersusCeilingWorld1,  317,353,  525,367,  317,353,  1,5,  1 );
    define_region_matrix( RegionVersusWallWorld1,  529,1,  534,360,  529,1,  5,1,  1 );
    
    // playfield: single images
    select_region( RegionDeathZone );
    define_region( 441,487,  441,551,  441,551 );
    
    select_region( RegionCeilingGear );
    define_region_center( 1,435,  129,563 );
    
    select_region( RegionDropCylinderTop );
    define_region_topleft( 593,364,  613,371 );
    
    select_region( RegionDropCylinderBody );
    define_region_topleft( 593,373,  613,373 );
    
    select_region( RegionDropCylinderBottom );
    define_region( 593,375,  613,389,  593,389 );
    
    select_region( RegionLauncherSeparator );
    define_region( 678,339,  679,403,  678,403 );
    
    // playfield: image groups and animations
    define_region_matrix( FirstRegionBubbles1,  646,1,  675,27,  659,14,  1,12,  1 );
    define_region_matrix( FirstRegionBubbles2,  677,1,  706,27,  690,14,  1,12,  1 );
    define_region_matrix( FirstRegionBubbleHighlight,  384,566,  410,592,  397,579,  7,1,  1 );
    define_region_matrix( FirstRegionBubbleBurst,  183,435,  231,483,  207,459,  6,1,  1 );
    
    // launcher: single images
    select_region( RegionLauncherArrow );
    define_region( 133,435,  179,521,  156,487 );
    
    select_region( RegionFrontWheel );
    define_region_center( 529,364,  589,424 );
    
    select_region( RegionNextBanner );
    define_region( 133,525,  173,572,  153,572 );
    
    // launcher: animations
    define_region_matrix( FirstRegionSideWheel,  485,442,  496,482,  496,482,  5,1,  1 );
    
    // event texts: image groups and animations
    define_region_matrix( RegionTextReady,  1,576,  126,607,  62,591,  3,1,  1 );
    define_region_matrix( FirstRegionCountdown,  183,487,  245,561,  213,524,  4,1,  1 );
    
    // scoring: single images
    select_region( RegionComboText );
    define_region_topleft( 445,547,  520,562 );
    
    // scoring: image groups
    define_region_matrix( RegionBurstZero,  445,487,  460,508,  445,487,  10,1,  1 );
    define_region_matrix( RegionFallZero,  445,510,  456,528,  445,510,  11,1,  1 );
    define_region_matrix( RegionComboZero,  445,530,  455,545,  445,530,  10,1,  1 );
    
    // pause menu: single images
    select_region( RegionPauseWindow );
    define_region_topleft( 1,611,  252,761 );
    
    // pause menu: image groups and animations
    define_region_matrix( RegionPauseContinue,  256,611,  437,642,  256,611,  1,2,  1 );
    define_region_matrix( FirstRegionPauseCursor,  683,339,  706,362,  706,336,  1,8,  1 );
    
    // bonus: image groups
    define_region_matrix( RegionBonusZero,  256,679,  277,710,  256,679,  12,1,  1 );
    
    // bonus: single images
    select_region( RegionBonusBar );
    define_region_topleft( 384,596,  616,605 );
    
    select_region( RegionBonusMaxCombo );
    define_region_topleft( 441,611,  649,642 );
    
    select_region( RegionBonusBonus );
    define_region_topleft( 441,644,  566,675 );
    
    // ------------------------------------
    // TEXTURES: CHARACTERS (1 TO 3)
    // ------------------------------------
    
    // only character 1 for the demo
    for( int Texture = TextureCharacter1; Texture <= TextureCharacter1; ++Texture )
    {
        select_texture( Texture );
        
        // single images
        select_region( RegionPlayerAimGuide );
        define_region( 1,1,  587,9,  5,5 );
        
        select_region( RegionPlayerDelimiter );
        define_region( 1,162,  219,226,  1,226 );
        
        select_region( RegionPlayerDeathZone );
        define_region( 223,162,  223,226,  223,226 );
        
        // animations
        define_region_matrix( FirstRegionPlayerIndicator,  227,162,  253,185,  240,185,  2,1,  1 );
        define_region_matrix( FirstRegionPlayerIdle,  1,13,  48,57,  48,57,  6,1,  1 );
        define_region_matrix( FirstRegionPlayerAim,  1,61,  50,107,  50,107,  5,1,  1 );
        define_region_matrix( FirstRegionPlayerShoot,  1,111,  50,157,  50,157,  3,1,  1 );
        define_region_matrix( FirstRegionPlayerLeave,  158,111,  205,155,  205,155,  3,1,  1 );
        define_region_matrix( FirstRegionPlayerCeilingDrop,  309,13,  358,57,  358,57,  2,1,  1 );
        define_region_matrix( FirstRegionPlayerDeath,  415,13,  464,62,  464,62,  4,1,  1 );
        define_region_matrix( FirstRegionPlayerBubbleBurst,  513,171,  562,219,  562,219,  2,1,  1 );
        define_region_matrix( FirstRegionPlayerBubblePlaced,  411,171,  458,215,  458,215,  2,1,  1 );
        define_region_matrix( FirstRegionPlayerReceiveBubbles,  309,171,  356,215,  356,215,  2,1,  1 );
        define_region_matrix( FirstRegionPlayerLevelClear,  309,119,  358,167,  358,166,  6,1,  1 );
    }
    
    // ------------------------------------
    // TEXTURES: WORLD BACKGROUND 1
    // ------------------------------------
    
    select_texture( TextureWorld1 );
    
    select_region( RegionBackgroundFar );
    define_region_topleft( 1,1,  640,220 );
    
    select_region( RegionBackgroundMid );
    define_region( 1,222,  640,380,  1,460 );
    
    select_region( RegionBackgroundNear );
    define_region( 1,382,  640,680,  1,680 );
    
    // ------------------------------------
    // TEXTURES: WORLD BACKGROUND 2
    // ------------------------------------
    
    select_texture( TextureWorld2 );
    
    select_region( RegionBackgroundFar );
    define_region_topleft( 1,1,  640,170 );
    
    select_region( RegionBackgroundMid );
    define_region( 1,172,  640,305,  1,420 );
    
    select_region( RegionBackgroundNear );
    define_region( 1,307,  640,493,  1,493 );
    
    // worlds 3 to 5 are not in the demo
    
    /*
    // ------------------------------------
    // TEXTURES: WORLD BACKGROUND 3
    // ------------------------------------
    
    select_texture( TextureWorld3 );
    
    select_region( RegionBackgroundFar );
    define_region_topleft( 1,1,  640,156 );
    
    select_region( RegionBackgroundMid );
    define_region( 1,158,  640,337,  1,417 );
    
    select_region( RegionBackgroundNear );
    define_region( 1,339,  640,537,  1,537 );
    
    // ------------------------------------
    // TEXTURES: WORLD BACKGROUND 4
    // ------------------------------------
    
    select_texture( TextureWorld4 );
    
    select_region( RegionBackgroundFar );
    define_region_topleft( 1,1,  640,191 );
    
    select_region( RegionBackgroundMid );
    define_region( 1,193,  640,414,  1,498 );
    
    select_region( RegionBackgroundNear );
    define_region( 1,416,  640,597,  1,597 );
    
    // ------------------------------------
    // TEXTURES: WORLD BACKGROUND 5
    // ------------------------------------
    
    select_texture( TextureWorld5 );
    
    select_region( RegionBackgroundFar );
    define_region_topleft( 1,1,  640,191 );
    
    select_region( RegionBackgroundMid );
    define_region( 1,193,  640,346,  1,390 );
    
    select_region( RegionBackgroundNear );
    define_region( 1,348,  640,478,  1,478 );
    */
}
