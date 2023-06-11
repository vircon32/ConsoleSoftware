// ---------------------------------------------------------
//   TEXTURES
// ---------------------------------------------------------


// names for textures
enum GameTextures
{
    TextureMahjong
};


// ---------------------------------------------------------
//   SOUNDS
// ---------------------------------------------------------


enum GameSounds
{
    // names for musics
    MusicTitle,
    MusicGameplay,
    
    // names for sound effects
    SoundTitleLogo,
    SoundPressStart,
    SoundMenuMove,
    SoundMenuAccept,
    SoundMenuCancel,
    SoundWin,
    SoundQuitGame,
    SoundSelectTile,
    SoundUndoSelect,
    SoundMatchTiles,
    SoundUnmatchTiles,
    SoundHighlight,
    SoundTileCollide,
    SoundWrongMove
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
    Scene_ChooseLayout,
    Scene_Gameplay,
    Scene_QuitGame,
    Scene_Win
};

// scene states: title
enum StatesTitle
{
    Title_Initialize = 11,
    Title_FadeIn,
    Title_TilesFall,
    Title_Flash,
    Title_Waiting,
    Title_StartPressed
};

// scene states: choose layout
enum StatesChooseLayout
{
    ChooseLayout_Initialize = 21,
    ChooseLayout_FadeIn,
    ChooseLayout_Choosing,
    ChooseLayout_Chosen
};

// scene states: gameplay
enum StatesGameplay
{
    Gameplay_Initialize = 31,
    Gameplay_FadeIn,
    Gameplay_PlacingLayers,
    Gameplay_MovingCursor,
    Gameplay_Picking,
    Gameplay_Unpicking,
    Gameplay_Pairing,
    Gameplay_Undoing,
    Gameplay_Redoing,
    Gameplay_ShowingHint,
    Gameplay_ShowAlert
};

// scene states: quit game
enum StatesQuitGame
{
    QuitGame_Initialize = 41,
    QuitGame_Choose,
    QuitGame_FadeOut
};

// scene states: win
enum StatesWin
{
    Win_Initialize = 51,
    Win_Highlight,
    Win_Choose,
    Win_FadeOut
};


// ---------------------------------------------------------
//   GLOBAL VARIABLES
// ---------------------------------------------------------


// globals for game state
int GameScene;   // scene currently running
int GameState;   // state within that scene

// numeric text font
numberfont FontTiles;

// cursor position and selection
int CursorX, CursorY;
int PointedTileIndex;       // tile currently under the mouse (-1 if none)
int[ 2 ] PickedTileIndex;   // the 2 tiles picked to form a pair (-1 if none)
