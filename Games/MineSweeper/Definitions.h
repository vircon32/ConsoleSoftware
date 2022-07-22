// ---------------------------------------------------------
//   TEXTURES
// ---------------------------------------------------------


// names for textures
enum GameTextures
{
    TextureMineSweeper
};


// ---------------------------------------------------------
//   TEXTURE REGIONS
// ---------------------------------------------------------


// (not needed, all are defined in the editor file)


// ---------------------------------------------------------
//   SOUNDS
// ---------------------------------------------------------


enum GameSounds
{
    // names for musics
    MusicTitle,
    MusicGameplay,

    // names for sound effects
    SoundTitleStart,
    SoundQuitGame,
    SoundMenuMove,
    SoundMenuAccept,
    SoundMenuCancel,
    SoundAddFlag,
    SoundRemoveFlag,
    SoundReveal,
    SoundExplosion,
    SoundWin,
    SoundLose
};

// ---------------------------------------------------------
//   SOUND CHANNELS
// ---------------------------------------------------------


#define ChannelMusic  15


// ---------------------------------------------------------
//   STATE MACHINE: SCENES AND STATES
// ---------------------------------------------------------


// game scenes
enum GameScenes
{
    Scene_Title = 1,
    Scene_Menu,
    Scene_GameEasy,
    Scene_GameMedium,
    Scene_GameHard,
    Scene_QuitGame,
    Scene_Lose,
    Scene_Win
};

// scene states: title
enum StatesTitle
{
    Title_Initialize = 11,
    Title_FadeIn,
    Title_Waiting,
    Title_StartPressed
};

// scene states: menu
enum StatesMenu
{
    Menu_Initialize = 21,
    Menu_FadeIn,
    Menu_Choose,
    Menu_FadeOut
};

// scene states: gameplay
enum StatesGameplay
{
    Gameplay_Initialize = 31,
    Gameplay_FadeIn,
    Gameplay_Waiting,
    Gameplay_Moving,
    Gameplay_AddingFlag,
    Gameplay_RemovingFlag,
    Gameplay_RevealTile,
    Gameplay_PropagateReveal,
    Gameplay_Explosion,
    Gameplay_LevelClear,
    Gameplay_RevealMines
};

// scene states: dialog scenes (common)
enum StatesDialog
{
    Dialog_Initialize = 51,
    Dialog_Highlight,
    Dialog_Choose,
    Dialog_FadeOut
};


// ---------------------------------------------------------
//   GENERAL DEFINITIONS
// ---------------------------------------------------------


enum GameModes
{
    ModeEasy = 0,
    ModeMedium,
    ModeHard
};

// the different board sizes
#define EasyBoardWidth  9
#define EasyBoardHeight 9
#define EasyBoardMines  10

#define MediumBoardWidth  16
#define MediumBoardHeight 16
#define MediumBoardMines  40

#define HardBoardWidth  30
#define HardBoardHeight 16
#define HardBoardMines  99


// ---------------------------------------------------------
//   GLOBAL VARIABLES
// ---------------------------------------------------------


// globals for game state
int GameScene;   // scene currently running
int GameState;   // state within that scene

GameModes GameMode;     // chosen difficulty mode
int PlayedFrames;       // total played time
