void DefineTextureRegions()
{
    // ------------------------------------
    // REGIONS FOR TEXT FONT
    // ------------------------------------
    
    // regions for common characters
    select_texture( TextureTextFont );
    define_region_matrix( FirstRegionTextFont,  0,0,  21,31,  0,31,  16,8,  0 );
    
    // redefine regions for custom-width characters
    select_region( 'M' );
    define_region( 22,0,  46,31,  22,31 );
    select_region( 'm' );
    define_region( 22,0,  46,31,  22,31 );
    
    select_region( 'W' );
    define_region( 66,0,  90,31,  66,31 );
    select_region( 'w' );
    define_region( 66,0,  90,31,  66,31 );
    
    select_region( 'I' );
    define_region( 110,0,  121,31,  110,31 );
    select_region( 'i' );
    define_region( 110,0,  121,31,  110,31 );
    
    // ------------------------------------
    // REGIONS FOR TITLE
    // ------------------------------------
    
    select_texture( TextureGame );
    
    select_region( RegionLogo );
    define_region_topleft( 124,467,  449,583 );
    
    select_region( Region2021 );
    define_region( 164,369,  224,384,  164,384 );
    
    select_region( RegionCarra );
    define_region( 228,369,  286,384,  286,384 );
    
    // ------------------------------------
    // REGIONS FOR WORLD CLEAR
    // ------------------------------------
    
    select_region( RegionWorldClearBackground );
    define_region_topleft( 336,344,  415,388 );
    
    select_region( RegionWorldClearBar );
    define_region( 1,627,  233,636,  117,627 );
    
    // ------------------------------------
    // REGIONS FOR ENDING AND GAME OVER
    // ------------------------------------
    
    select_region( RegionCharacterWin );
    define_region( 1,467,  120,580,  60,556 );
    
    select_region( RegionCharacterLose );
    define_region( 344,184,  482,340,  416,319 );
    
    select_region( RegionCharacterShadow );
    define_region( 168,587,  286,622,  227,622 );
    
    // ------------------------------------
    // REGIONS FOR PLAYER CHARACTER
    // ------------------------------------
    
    define_region_matrix( RegionPlayerIdle,  162,182,  201,221,  181,220,  4,3,  1 );
    
    // ------------------------------------
    // REGIONS FOR LEVEL BACKGROUNDS
    // ------------------------------------
    
    define_region_matrix( RegionGradientWorld1,  486,1,  486,360,  486,1,  3,1,  1 );
    define_region_matrix( RegionBackgroundWorld1,  1,1,  160,180,  1,180,  3,1,  1 );
    define_region_matrix( FirstRegionTileset,  1,182,  40,221,  1,182,  4,5,  0 );
    
    // ------------------------------------
    // OTHER REGIONS FOR GAMEPLAY
    // ------------------------------------
    
    select_region( RegionCollectedKey );
    define_region( 290,369,  319,385,  319,385 );
    
    select_region( RegionMorningStarLink );
    define_region_center( 460,344,  473,357 );
    
    select_region( RegionMorningStarBall );
    define_region_center( 290,587,  325,622 );
    
    select_region( RegionOnOffToggle );
    define_region_topleft( 441,392,  480,431 );
    
    select_region( RegionGUIFrameLeft );
    define_region_topleft( 1,587,  164,623 );
    
    select_region( RegionGUIFrameRight );
    define_region( 329,610,  436,646,  436,610 );
    
    define_region_matrix( RegionGUIDigitZero,  164,344,  179,365,  164,344,  10,1,  1 );
    define_region_matrix( FirstRegionConveyor,  419,344,  456,365,  418,335,  1,4,  1 );
    define_region_matrix( FirstRegionCircleHighlight,  1,392,  72,463,  36,427,  6,1,  1 );
    define_region_matrix( FirstRegionCoinMotion,  218,307,  247,340,  232,323,  4,1,  1 );
    define_region_matrix( FirstRegionGoSign,  164,307,  188,330,  176,330,  2,1,  1 );
    define_region_matrix( FirstRegionSpringTop,  329,587,  368,606,  329,607,  3,1,  1 );
}
