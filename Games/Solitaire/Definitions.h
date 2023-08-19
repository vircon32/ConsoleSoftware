// ---------------------------------------------------------
//   TEXTURES
// ---------------------------------------------------------


// names for textures
enum GameTextures
{
    TextureSolitaire
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
    SoundTitleStart,
    SoundMenuMove,
    SoundMenuAccept,
    SoundMenuCancel,
    SoundDealLadders,
    SoundDraw,
    SoundCardSlide,
    SoundCardDrop,
    SoundCardFlip,
    SoundWrongMove,
    SoundQuitGame,
    SoundWin,
    SoundWinHighlight
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
    Scene_Gameplay,
    Scene_QuitGame,
    Scene_Win
};

// scene states: title
enum StatesTitle
{
    Title_Initialize = 11,
    Title_FadeIn,
    Title_Dealing,
    Title_Flash,
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
    Gameplay_DealingLadders,
    Gameplay_FlippingLadders,
    Gameplay_Waiting,
    Gameplay_Moving,
    Gameplay_ReplaceDrawDeck,
    Gameplay_Drawing,
    Gameplay_FlipLadderTop,
    Gameplay_Picking,
    Gameplay_Unpicking,
    Gameplay_Dropping,
    Gameplay_WrongMove
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
//   GENERAL DEFINITIONS
// ---------------------------------------------------------


enum GameModes
{
    Mode_Easy = 0,
    Mode_Normal,
    Mode_Hard
};

// globals for game state
int GameScene;   // scene currently running
int GameState;   // state within that scene

// game configuration
int DrawnCards = 2;
