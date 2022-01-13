// ---------------------------------------------------------
//   TEXTURES
// ---------------------------------------------------------


// names for textures
#define Texture2048  0


// ---------------------------------------------------------
//   TEXTURE REGIONS
// ---------------------------------------------------------


// regions for title
#define RegionTitleLogo      0
#define RegionPressStart     1
#define RegionTileHighlight  2
#define Region2021           3
#define RegionCarra          4

// regions for gameplay
#define RegionBackground  5
#define RegionGUI         6
#define RegionBoard       7
#define RegionBGLeft      8
#define RegionBGRight     9
#define RegionPointsPlus  10
#define RegionIndicatorH  11
#define RegionIndicatorV  12

// regions for dialogs
#define RegionDialogWin     20
#define RegionDialogLose    21
#define RegionDialogEnd     22
#define RegionDialogYesNo   23
#define RegionDialogExit    24
#define RegionTextContinue  25
#define RegionTextEnded     26

// region groups
#define RegionTile2         30
#define RegionSecondsZero   50
#define RegionHundredsZero  60
#define RegionScoreZero     70
#define FirstRegionMerge    80
#define FirstRegionCursor   90


// ---------------------------------------------------------
//   SOUNDS
// ---------------------------------------------------------


// names for musics
#define MusicTitle     0
#define MusicGameplay  1

// names for sound effects
#define SoundTitleTiles     2
#define SoundTitleLogo      3
#define SoundTitleStart     4
#define SoundQuitGame       5
#define SoundMenuMove       6
#define SoundMenuAccept     7
#define SoundMenuCancel     8
#define SoundMoveBoard      9
#define SoundTileCollision  10
#define SoundTileAdded      11
#define SoundNewHighest     12
#define SoundWin            13
#define SoundLose           14


// ---------------------------------------------------------
//   SOUND CHANNELS
// ---------------------------------------------------------


#define ChannelMusic  15


// ---------------------------------------------------------
//   STATE MACHINE: SCENES AND STATES
// ---------------------------------------------------------


// game scenes
#define Scene_Title     1
#define Scene_Gameplay  2
#define Scene_QuitGame  3
#define Scene_Win2048   4
#define Scene_Win65536  5
#define Scene_Lose      6

// scene states: title
#define Title_Initialize    1
#define Title_TilesAppear   2
#define Title_Waiting       3
#define Title_StartPressed  4

// scene states: gameplay
#define Gameplay_Initialize  1
#define Gameplay_Waiting     2
#define Gameplay_Transition  3
#define Gameplay_AddTile     4
#define Gameplay_NewHighest  5

// scene states: all dialog scenes (common)
#define Dialog_Initialize  1
#define Dialog_Highlight   2
#define Dialog_Choose      3
#define Dialog_FadeOut     4


// ---------------------------------------------------------
//   GLOBALS FOR GAME STATE
// ---------------------------------------------------------


int GameScene;   // scene currently running
int GameState;   // state within that scene
