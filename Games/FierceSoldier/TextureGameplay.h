// begin include guard
#ifndef TEXTUREREGIONS_TEXTUREGAMEPLAY
#define TEXTUREREGIONS_TEXTUREGAMEPLAY

// define names for single regions
#define RegionPalmTreeTall  0
#define RegionPalmTreeShort  1
#define RegionBushLarge  2
#define RegionBushSmall  3
#define RegionGUIFrame  4
#define RegionPlayerDeath  5
#define RegionPlayerVictory  6
#define RegionBossWheel  7
#define RegionBossWheelBlue  8
#define RegionBoss1  9
#define RegionBoss2  10
#define RegionBoss3  11
#define RegionCannonBullet  12
#define RegionMissileLauncher  13
#define RegionTurretBase  14
#define RegionTurretCannon  15

// define names for first region in region matrices
#define FirstRegionTileset  16
#define FirstRegionNumbers  94
#define FirstRegionHighlight  104
#define FirstRegionPlayerStanding  110
#define FirstRegionPlayerDuck  112
#define FirstRegionPlayerJump  114
#define FirstRegionPlayerWalk  116
#define FirstRegionPlayerBullet  124
#define FirstRegionEnemyBullet  126
#define FirstRegionBossCannon  128
#define FirstRegionMissile  130
#define FirstRegionSoldier  132

// your initialization code must call this function
void DefineRegions_TextureGameplay() 
{
    select_texture( TextureGameplay );

    // define single regions
    select_region( RegionPalmTreeTall );
    define_region( 1,244,  120,403,  41,364 );
    select_region( RegionPalmTreeShort );
    define_region( 124,244,  243,363,  164,324 );
    select_region( RegionBushLarge );
    define_region( 247,244,  326,323,  267,284 );
    select_region( RegionBushSmall );
    define_region( 124,367,  243,405,  164,366 );
    select_region( RegionGUIFrame );
    define_region( 247,327,  317,363,  247,327 );
    select_region( RegionPlayerDeath );
    define_region( 331,356,  414,398,  380,385 );
    select_region( RegionPlayerVictory );
    define_region( 448,269,  492,385,  475,384 );
    select_region( RegionBossWheel );
    define_region( 421,487,  467,533,  444,510 );
    select_region( RegionBossWheelBlue );
    define_region( 471,487,  517,533,  494,510 );
    select_region( RegionBoss1 );
    define_region( 241,484,  417,578,  256,596 );
    select_region( RegionBoss2 );
    define_region( 235,582,  361,720,  250,738 );
    select_region( RegionBoss3 );
    define_region( 365,582,  491,720,  380,738 );
    select_region( RegionCannonBullet );
    define_region( 424,356,  444,376,  434,366 );
    select_region( RegionMissileLauncher );
    define_region( 119,736,  192,810,  147,810 );
    select_region( RegionTurretBase );
    define_region( 196,736,  255,795,  224,764 );
    select_region( RegionTurretCannon );
    define_region( 259,724,  307,749,  271,736 );

    // define region matrices
    define_region_matrix( FirstRegionTileset, 1,1,  40,40,  1,1,  13,6,  0 );
    define_region_matrix( FirstRegionNumbers, 330,244,  345,265,  330,244,  10,1,  1 );
    define_region_matrix( FirstRegionHighlight, 1,409,  72,480,  36,444,  6,1,  1 );
    define_region_matrix( FirstRegionPlayerStanding, 330,269,  386,352,  349,351,  2,1,  1 );
    define_region_matrix( FirstRegionPlayerDuck, 117,484,  176,547,  142,546,  2,1,  1 );
    define_region_matrix( FirstRegionPlayerJump, 1,484,  56,560,  20,563,  2,1,  1 );
    define_region_matrix( FirstRegionPlayerWalk, 1,564,  57,647,  21,646,  4,2,  1 );
    define_region_matrix( FirstRegionPlayerBullet, 247,367,  255,375,  251,371,  1,2,  1 );
    define_region_matrix( FirstRegionEnemyBullet, 259,367,  267,375,  263,371,  1,2,  1 );
    define_region_matrix( FirstRegionBossCannon, 441,389,  479,425,  404,407,  2,1,  1 );
    define_region_matrix( FirstRegionMissile, 441,429,  510,455,  501,442,  1,2,  1 );
    define_region_matrix( FirstRegionSoldier, 1,736,  57,819,  39,818,  2,1,  1 );
}

// end include guard
#endif
