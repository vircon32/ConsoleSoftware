// ---------------------------------------------------------
//   TEXTURES
// ---------------------------------------------------------


// names for textures
#define TextureTitle       0
#define TextureMenus       1
#define TextureGameplay    2
#define TextureCutscenes   3
#define TextureCharacter1  4
#define TextureCharacter2  5
#define TextureCharacter3  6
#define TextureWorld1      7
#define TextureWorld2      8
#define TextureWorld3      9
#define TextureWorld4      10
#define TextureWorld5      11


// ---------------------------------------------------------
//   TEXTURE REGIONS
// ---------------------------------------------------------


// regions for title
#define RegionTitleStage  0
#define RegionTitleBubble 1
#define RegionLogoLeft    2
#define RegionLogoRight   3
#define RegionPressStart  4
#define RegionTitleStar   5
#define RegionTitleDemo   6

// regions for main menu
#define RegionMenuTitle      0
#define RegionMenuWall       1
#define RegionMenuBubble     2
#define RegionMenuSelection  3
#define RegionMenuTriple     4
#define RegionMenuClassic    5
#define RegionMenuVersus     6
#define RegionMenuOptions    7
#define RegionMenu1Player    8
#define RegionMenu2Players   9
#define RegionMenu3Players   10

// regions for cutscenes
#define RegionGameOverText       0
#define RegionGameOverBubble     1
#define FirstRegionGameOverChar  2
#define RegionEndingText         4
#define RegionEndingComeBack     5
#define FirstRegionEndingChar    6

// regions for world backgrounds
#define RegionBackgroundFar   0
#define RegionBackgroundMid   1
#define RegionBackgroundNear  2

// regions for gameplay: scenery
#define RegionTripleScoreboardWorld1   0
#define RegionTripleCeilingWorld1      5
#define RegionTripleWallWorld1         10
#define RegionClassicScoreboardWorld1  15
#define RegionClassicCeilingWorld1     20
#define RegionVersusScoreboardWorld1   25
#define RegionVersusCeilingWorld1      30
#define RegionVersusWallWorld1         35

// regions for gameplay: playfield
#define RegionDeathZone             40
#define RegionCeilingGear           41
#define RegionDropCylinderTop       42
#define RegionDropCylinderBody      43
#define RegionDropCylinderBottom    44
#define RegionLauncherSeparator     45
#define FirstRegionBubbles1         50
#define FirstRegionBubbles2         70
#define FirstRegionBubbleHighlight  90
#define FirstRegionBubbleBurst      100

// regions for gameplay: launcher
#define RegionLauncherArrow    110
#define RegionFrontWheel       111
#define RegionNextBanner       112
#define FirstRegionSideWheel   115

// regions for gameplay: event texts
#define RegionTextReady       120
#define RegionTextClear       121
#define RegionTextFailed      122
#define FirstRegionCountdown  125

// regions for gameplay: scoring
#define RegionBurstZero  130
#define RegionComboZero  140
#define RegionFallZero   150
#define RegionFallPlus   160
#define RegionComboText  161

// regions for gameplay: pause menu
#define RegionPauseWindow       170
#define RegionPauseContinue     171
#define RegionPauseQuit         172
#define FirstRegionPauseCursor  173

// regions for gameplay: bonus
#define RegionBonusBar       190
#define RegionBonusMaxCombo  191
#define RegionBonusBonus     192
#define RegionBonusZero      200
#define RegionBonusPlus      210
#define RegionBonusX         211

// regions for characters
#define RegionPlayerAimGuide             0
#define RegionPlayerDelimiter            1  
#define RegionPlayerDeathZone            2
#define FirstRegionPlayerIndicator       3
#define FirstRegionPlayerIdle            10
#define FirstRegionPlayerAim             20
#define FirstRegionPlayerShoot           30
#define FirstRegionPlayerLeave           40
#define FirstRegionPlayerCeilingDrop     50
#define FirstRegionPlayerDeath           60
#define FirstRegionPlayerBubbleBurst     70
#define FirstRegionPlayerBubblePlaced    80
#define FirstRegionPlayerReceiveBubbles  90
#define FirstRegionPlayerLevelClear      100


// ---------------------------------------------------------
//   SOUNDS
// ---------------------------------------------------------


// names for musics
#define MusicWorld1          0
#define MusicWorld2          1
#define MusicWorld3          2
#define MusicWorld4          3
#define MusicWorld5          4

// names for sound effects
#define SoundTitleBubble     5
#define SoundTitleFanfare    6
#define SoundTitleStart      7
#define SoundMenuMove        8
#define SoundMenuAccept      9
#define SoundMenuCancel      10
#define SoundLauncherMove    11
#define SoundBubbleShoot     12
#define SoundBubbleBounce    13
#define SoundBubblePlaced    14
#define SoundBubblesBurst    15
#define SoundBurstFanfare    16
#define SoundChangeLauncher  17
#define SoundCountdown       18
#define SoundCeilingDrop     19
#define SoundPause           20
#define SoundLevelClear      21
#define SoundDeath           22
#define SoundReady           23
#define SoundClear           24
#define SoundFailed          25
#define SoundCombo           26
#define SoundComboNice       27
#define SoundComboGreat      28


// ---------------------------------------------------------
//   SOUND CHANNELS
// ---------------------------------------------------------


#define ChannelLauncherMove  14
#define ChannelMusic         15


// ---------------------------------------------------------
//   STATE MACHINE: SCENES AND STATES
// ---------------------------------------------------------


// game scenes
#define Scene_Title          10
#define Scene_MainMenu       20
#define Scene_Intro          30
#define Scene_Tutorial       40
#define Scene_ArcadeTriple   50
#define Scene_ArcadeClassic  60
#define Scene_VersusGame     70
#define Scene_Map            80
#define Scene_GameOver       90
#define Scene_Ending         100

// scene states: title
#define Title_Initialize    1
#define Title_BubbleUp      2
#define Title_Burst         3
#define Title_Waiting       4
#define Title_StartPressed  5

// scene states: main menu
#define MainMenu_Initialize   1
#define MainMenu_FadeIn       2
#define MainMenu_Choose       3
#define MainMenu_SetPlayers   4
#define MainMenu_FadeOut      5

// scene states: arcade classic & triple
#define Arcade_Initialize   1
#define Arcade_InitLevel    2
#define Arcade_LevelIntro   3
#define Arcade_Gameplay     4
#define Arcade_LevelClear   5
#define Arcade_Death        6
#define Arcade_Bonus        7
#define Arcade_FadeOut      8
#define Arcade_PauseMenu    9
#define Arcade_Quit         10

// scene states: game over
#define GameOver_Initialize   1
#define GameOver_Burst        2
#define GameOver_Wait         3
#define GameOver_FadeOut      4

// scene states: ending
#define Ending_Initialize   1
#define Ending_Wait         2
#define Ending_FadeOut      3

// states: playfield
#define Playfield_Initialize    1
#define Playfield_Wait          2
#define Playfield_CeilingDrop   3
#define Playfield_LevelClear    4
#define Playfield_Death         5

// states: launcher (function)
#define Launcher_Initialize    1
#define Launcher_Aim           2
#define Launcher_Shoot         3
#define Launcher_BubbleBurst   4
#define Launcher_BubblePlaced  5

// states: launcher (owning)
#define LauncherOwning_Used       1
#define LauncherOwning_Unused     2
#define LauncherOwning_CharEnter  3
#define LauncherOwning_CharLeave  4

// states: character
#define Character_Initialize      1
#define Character_Aim             2
#define Character_LeaveLauncher   3
#define Character_EnterLauncher   4
#define Character_Shoot           5
#define Character_BubbleBurst     6
#define Character_BubblePlaced    7
#define Character_CeilingDrop     8
#define Character_ReceiveBubbles  9
#define Character_LevelClear      10
#define Character_Death           11

// actual game state
int GameScene = Scene_Title;
int GameState = Title_Initialize;
int GlobalElapsedFrames = 0;


// ---------------------------------------------------------
//   DATA STRUCTURES
// ---------------------------------------------------------


struct Vector2D
{
    float x,y;
};

struct Ray
{
    Vector2D Start, End;
    Vector2D Velocity;
    bool Active;
};


// ---------------------------------------------------------
//   AUXILIARY FUNCTIONS
// ---------------------------------------------------------


float DistanceBetween( Vector2D* StartPoint, Vector2D* EndPoint )
{
    float DeltaX = EndPoint->x - StartPoint->x;
    float DeltaY = EndPoint->y - StartPoint->y;
    
    return sqrt( DeltaX * DeltaX + DeltaY * DeltaY );
}

// ---------------------------------------------------------

float AngleBetween( Vector2D* StartPoint, Vector2D* EndPoint )
{
    float DeltaX = EndPoint->x - StartPoint->x;
    float DeltaY = EndPoint->y - StartPoint->y;
    
    return atan2( DeltaY, DeltaX );
}
