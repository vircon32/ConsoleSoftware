// ---------------------------------------------------------
//   TEXTURES
// ---------------------------------------------------------


// names for textures
#define TextureGame      0
#define TextureTextFont  1


// ---------------------------------------------------------
//   TEXTURE REGIONS (GAME)
// ---------------------------------------------------------


// regions for title
#define RegionLogo   0
#define Region2021   1
#define RegionCarra  2

// regions for ending and game over
#define RegionCharacterWin     10
#define RegionCharacterLose    11
#define RegionCharacterShadow  12

// regions for world clear
#define RegionWorldClearBackground  20
#define RegionWorldClearBar         21

// regions for player character
#define RegionPlayerIdle       30
#define RegionPlayerPush       31
#define RegionPlayerVictory    32
#define RegionPlayerDeath      33
#define RegionPlayerJumpFront  34
#define RegionPlayerJumpSide   35
#define RegionPlayerFallFront  36
#define RegionPlayerFallSide   37
#define FirstRegionPlayerWalk  38

// regions for level backgrounds
#define RegionGradientWorld1    50
#define RegionBackgroundWorld1  60
#define FirstRegionTileset      70

// other regions for gameplay
#define RegionCollectedKey          90
#define RegionMorningStarLink       91
#define RegionMorningStarBall       92
#define RegionOnOffToggle           93
#define RegionGUIFrameLeft          94
#define RegionGUIFrameRight         95
#define RegionGUIDigitZero          100
#define FirstRegionConveyor         110
#define FirstRegionCircleHighlight  120
#define FirstRegionCoinMotion       130
#define FirstRegionGoSign           140
#define FirstRegionSpringTop        150


// ---------------------------------------------------------
//   TEXTURE REGIONS (TEXT FONT)
// ---------------------------------------------------------


// these regions are from 0 to 127,
// (the standard ASCII character set)
#define FirstRegionTextFont  0


// ---------------------------------------------------------
//   SOUNDS
// ---------------------------------------------------------


// names for musics
#define MusicTitle       0
#define MusicWorld1      1
#define MusicWorld2      2
#define MusicWorld3      3
#define MusicWorldClear  4
#define MusicCredits     5
#define MusicEnding      6
#define MusicGameOver    7

// names for sound effects
#define SoundJump      8
#define SoundLand      9
#define SoundCoin      10
#define SoundOnOff     11
#define SoundSpring    12
#define SoundGetKey    13
#define SoundOpenDoor  14
#define SoundGoal      15
#define SoundDeath     16
#define SoundPause     17
#define SoundStart     18
#define SoundHurry     19


// ---------------------------------------------------------
//   SOUND CHANNELS
// ---------------------------------------------------------


#define ChannelOnOff  14
#define ChannelMusic  15


// ---------------------------------------------------------
//   STATE MACHINE: SCENES AND STATES
// ---------------------------------------------------------


// game scenes
#define Scene_Title        1
#define Scene_Gameplay     2
#define Scene_WorldClear   3
#define Scene_Credits      4
#define Scene_Ending       5
#define Scene_GameOver     6

// scene states: title
#define Title_Initialize    11
#define Title_FadeIn        12
#define Title_Waiting       13
#define Title_StartPressed  14

// scene states: gameplay
#define Gameplay_Initialize  21
#define Gameplay_LevelIntro  22
#define Gameplay_Level       23
#define Gameplay_Pause       24
#define Gameplay_Death       25
#define Gameplay_Goal        26
#define Gameplay_FadeOut     27

// scene states: world clear
#define WorldClear_Initialize  31
#define WorldClear_TextDown    32
#define WorldClear_Waiting     33
#define WorldClear_FadeOut     34

// scene states: credits
#define Credits_Initialize  41
#define Credits_TextRoll    42

// scene states: ending
#define Ending_Initialize   51
#define Ending_FadeIn       52
#define Ending_TextAppears  53
#define Ending_FadeOut      54

// scene states: game over
#define GameOver_Initialize   61
#define GameOver_FadeIn       62
#define GameOver_TextAppears  63
#define GameOver_FadeOut      64


// ---------------------------------------------------------
//   GAME RELATED DEFINITIONS
// ---------------------------------------------------------


#define NumberOfWorlds  3
#define LevelsPerWorld  10
#define LivesPerWorld   9


// ---------------------------------------------------------
//   GLOBAL VARIABLES
// ---------------------------------------------------------


// globals for game state
int GameScene;   // scene currently running
int GameState;   // state within that scene

// globals for game session
int PlayerLives;        // includes current life: life zero is not played
int WorldNumber;        // starts from 1, not 0
int LevelNumber;        // starts from 1, not 0
int GameStartFrames;    // allows us to determine total play time

// set initial state for the whole game
void ResetGameSession()
{
    PlayerLives = LivesPerWorld;
    WorldNumber = 1;
    LevelNumber = 1;
    GameStartFrames = get_frame_counter();
}
