// begin include guard
#ifndef TEXTUREREGIONS_TEXTUREGAMEPLAY
#define TEXTUREREGIONS_TEXTUREGAMEPLAY

// define names for single regions
#define RegionTextChangeSkill  0
#define RegionTextLevel  1
#define RegionTextClear  2
#define RegionTextHowToPlay  3
#define RegionTextPause  4
#define RegionDashCharacter  5
#define RegionSkillCircle  6
#define RegionEndLevelDoor  7
#define RegionEndRoomDoor  8
#define RegionClosedDoor  9
#define RegionSignIn  10
#define RegionSignUp  11
#define RegionBlockOnOffSwitching  12
#define RegionTimedSpikesSwitching  13
#define RegionSkillPost  14
#define RegionGolem  15
#define RegionGolemDeath  16
#define RegionGolemHand  17
#define RegionBirdDeath  18
#define RegionPlayerDeath  19
#define RegionSkillsIconGUI  20
#define RegionSkillsIconChange  21
#define RegionNeutralJumpUp  22
#define RegionPistolJumpUp  23
#define RegionCapeJumpUp  24
#define RegionWandJumpUp  25
#define RegionBootsJumpUp  26
#define RegionNeutralJumpDown  27
#define RegionPistolJumpDown  28
#define RegionCapeJumpDown  29
#define RegionWandJumpDown  30
#define RegionBootsJumpDown  31

// define names for first region in region matrices
#define FirstRegionMountains  32
#define FirstRegionClouds  36
#define FirstRegionTileset  40
#define FirstRegionCircleHighlight  95
#define FirstRegionDigits  101
#define FirstRegionSkillIcons  111
#define FirstRegionSkillNamesGUI  115
#define FirstRegionSkillNamesChange  119
#define FirstRegionBirdFlap  123
#define FirstRegionSkillSelection  125
#define FirstRegionBullet  127
#define FirstRegionNeutralIdle  129
#define FirstRegionPistolIdle  131
#define FirstRegionCapeIdle  133
#define FirstRegionWandIdle  135
#define FirstRegionBootsIdle  137
#define FirstRegionNeutralWalk  139
#define FirstRegionPistolWalk  143
#define FirstRegionCapeWalk  147
#define FirstRegionWandWalk  151
#define FirstRegionBootsWalk  155
#define FirstRegionEnterDoor  159
#define FirstRegionShotBreaking  166

// your initialization code must call this function
void DefineRegions_TextureGameplay() 
{
    select_texture( TextureGameplay );

    // define single regions
    select_region( RegionTextChangeSkill );
    define_region( 168,689,  422,720,  168,689 );
    select_region( RegionTextLevel );
    define_region( 168,724,  279,755,  168,724 );
    select_region( RegionTextClear );
    define_region( 283,724,  411,755,  283,724 );
    select_region( RegionTextHowToPlay );
    define_region( 168,759,  397,790,  282,759 );
    select_region( RegionTextPause );
    define_region( 592,705,  705,736,  648,719 );
    select_region( RegionDashCharacter );
    define_region( 168,654,  189,685,  168,654 );
    select_region( RegionSkillCircle );
    define_region( 1,654,  164,817,  82,735 );
    select_region( RegionEndLevelDoor );
    define_region( 644,1,  738,83,  672,83 );
    select_region( RegionEndRoomDoor );
    define_region( 644,87,  693,151,  649,151 );
    select_region( RegionClosedDoor );
    define_region( 695,87,  744,151,  700,151 );
    select_region( RegionSignIn );
    define_region( 725,167,  762,178,  744,178 );
    select_region( RegionSignUp );
    define_region( 750,182,  771,204,  761,204 );
    select_region( RegionBlockOnOffSwitching );
    define_region( 725,208,  764,247,  725,208 );
    select_region( RegionTimedSpikesSwitching );
    define_region( 725,249,  764,288,  725,249 );
    select_region( RegionSkillPost );
    define_region( 644,285,  683,364,  644,364 );
    select_region( RegionGolem );
    define_region( 532,376,  609,442,  573,442 );
    select_region( RegionGolemDeath );
    define_region( 532,444,  609,510,  573,510 );
    select_region( RegionGolemHand );
    define_region( 532,514,  562,532,  532,532 );
    select_region( RegionBirdDeath );
    define_region( 444,514,  528,581,  487,569 );
    select_region( RegionPlayerDeath );
    define_region( 532,536,  564,579,  553,579 );
    select_region( RegionSkillsIconGUI );
    define_region( 441,705,  480,744,  441,705 );
    select_region( RegionSkillsIconChange );
    define_region( 484,705,  523,744,  484,705 );
    select_region( RegionNeutralJumpUp );
    define_region( 613,464,  645,508,  630,505 );
    select_region( RegionPistolJumpUp );
    define_region( 689,336,  738,380,  706,377 );
    select_region( RegionCapeJumpUp );
    define_region( 441,656,  484,700,  469,697 );
    select_region( RegionWandJumpUp );
    define_region( 705,602,  745,656,  722,653 );
    select_region( RegionBootsJumpUp );
    define_region( 749,653,  781,699,  766,696 );
    select_region( RegionNeutralJumpDown );
    define_region( 649,464,  683,501,  666,505 );
    select_region( RegionPistolJumpDown );
    define_region( 742,336,  791,373,  759,377 );
    select_region( RegionCapeJumpDown );
    define_region( 488,656,  533,693,  516,697 );
    select_region( RegionWandJumpDown );
    define_region( 749,602,  789,649,  766,653 );
    select_region( RegionBootsJumpDown );
    define_region( 708,660,  745,699,  725,703 );

    // define region matrices
    define_region_matrix( FirstRegionMountains, 1,1,  160,192,  1,1,  4,1,  0 );
    define_region_matrix( FirstRegionClouds, 1,194,  160,372,  1,194,  4,1,  0 );
    define_region_matrix( FirstRegionTileset, 1,376,  40,415,  1,376,  11,5,  0 );
    define_region_matrix( FirstRegionCircleHighlight, 1,579,  72,650,  36,614,  6,1,  1 );
    define_region_matrix( FirstRegionDigits, 191,654,  212,685,  191,654,  10,1,  1 );
    define_region_matrix( FirstRegionSkillIcons, 748,1,  787,40,  748,1,  1,4,  1 );
    define_region_matrix( FirstRegionSkillNamesGUI, 644,155,  721,185,  647,155,  1,4,  1 );
    define_region_matrix( FirstRegionSkillNamesChange, 527,705,  588,720,  558,720,  1,4,  1 );
    define_region_matrix( FirstRegionBirdFlap, 444,376,  528,443,  487,425,  1,2,  1 );
    define_region_matrix( FirstRegionSkillSelection, 441,748,  480,787,  441,748,  2,1,  1 );
    define_region_matrix( FirstRegionBullet, 766,167,  775,176,  770,171,  2,1,  1 );
    define_region_matrix( FirstRegionNeutralIdle, 613,420,  645,460,  630,460,  2,1,  1 );
    define_region_matrix( FirstRegionPistolIdle, 689,292,  738,332,  706,332,  2,1,  1 );
    define_region_matrix( FirstRegionCapeIdle, 595,514,  639,554,  624,554,  2,1,  1 );
    define_region_matrix( FirstRegionWandIdle, 441,602,  481,652,  458,652,  2,1,  1 );
    define_region_matrix( FirstRegionBootsIdle, 613,376,  648,416,  630,416,  2,1,  1 );
    define_region_matrix( FirstRegionNeutralWalk, 689,470,  723,510,  706,510,  2,2,  1 );
    define_region_matrix( FirstRegionPistolWalk, 689,384,  738,424,  707,424,  2,2,  1 );
    define_region_matrix( FirstRegionCapeWalk, 595,558,  640,598,  624,598,  4,1,  1 );
    define_region_matrix( FirstRegionWandWalk, 527,602,  569,652,  545,652,  4,1,  1 );
    define_region_matrix( FirstRegionBootsWalk, 537,656,  573,696,  554,695,  4,1,  1 );
    define_region_matrix( FirstRegionEnterDoor, 525,777,  558,817,  541,817,  7,1,  1 );
    define_region_matrix( FirstRegionShotBreaking, 763,470,  782,489,  772,479,  1,4,  1 );
}

// end include guard
#endif
