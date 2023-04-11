// ---------------------------------------------------------
//   GAME TEXTURES
// ---------------------------------------------------------


enum Textures
{
    TextureTitle,
    TextureGameplay
};


// ---------------------------------------------------------
//   GAME SOUNDS
// ---------------------------------------------------------


enum Sounds
{
    // names for musics
    MusicTitle,
    MusicGameplay,
    
    // names for sound effects
    SoundPressStart,
    SoundMenuMove,
    SoundMenuAccept,
    SoundMenuCancel,
    SoundGameOver,
    SoundQuitGame,
    SoundPieceMove,
    SoundPieceRotate,
    SoundPieceDrop,
    SoundPieceLock,
    SoundClear1Line,
    SoundClear2Lines,
    SoundClear3Lines,
    SoundClear4Lines,
    SoundBoardFall,
    SoundLevelAdvance
};


// ---------------------------------------------------------
//   SOUND CHANNELS
// ---------------------------------------------------------


#define ChannelMusic  15


// ---------------------------------------------------------
//   GAME PARAMETERS
// ---------------------------------------------------------

// these "speeds" are actually wait times in
// frames, so they really represent 1/speed
#define DropSpeed 2
#define DASRepeatSpeed 4

// these are a time in frames too
#define DASInitialDelay 20
#define DelayAfterLock  14

// ---------------------------------------------------------

// our possible pieces are completed with an extra
// "no piece" at the beginning to allow for some
// extra configurations like obstacles
enum PieceTypes
{
    NoPiece,
    PieceO,
    PieceT,
    PieceI,
    PieceL,
    PieceJ,
    PieceZ,
    PieceS
};

// gray color is used in highlights and other
// graphic effects, and will be used for obstacles
enum PieceColors
{
    Gray,
    Blue,
    Green,
    Red,
    Purple,
    Yellow,
    Orange,
    Cyan
};

// ---------------------------------------------------------

// a piece model has 4 shapes of 4x4 tiles, that
// correspond to the piece's 4 possible rotations
typedef int[ 4 ][ 4 ] PieceShape;
typedef PieceShape[ 4 ] PieceModel;

// a board includes 4 extra rows at the top
#define BoardWidth 10
#define BoardHeight 24
typedef int[ BoardHeight ][ BoardWidth ] GameBoard;


// ---------------------------------------------------------
//   STATE MACHINE: SCENES AND STATES
// ---------------------------------------------------------


// game scenes
enum Scenes
{
    Scene_Title = 1,
    Scene_ChooseLevel,
    Scene_Gameplay,
    Scene_QuitGame,
    Scene_GameOver
};

// scene states: title
enum StatesTitle
{
    Title_Initialize = 11,
    Title_Waiting,
    Title_StartPressed
};

// scene states: choose level
enum StatesChooseLevel
{
    ChooseLevel_Initialize = 21,
    ChooseLevel_TitleUp,
    ChooseLevel_Choosing,
    ChooseLevel_Chosen
};

// scene states: gameplay
enum StatesGameplay
{
    Gameplay_Initialize = 31,
    Gameplay_FadeIn,
    Gameplay_Playing,
    Gameplay_GameOver,
    Gameplay_FadeOut
};

// scene states: quit game
enum StatesQuitGame
{
    QuitGame_Initialize = 41,
    QuitGame_Choose,
    QuitGame_FadeOut
};

// scene states: game over
enum StatesGameOver
{
    GameOver_Initialize = 51,
    GameOver_Choose,
    GameOver_FadeOut
};


// ---------------------------------------------------------
//   DEFINITIONS FOR THE GAME SESSION
// ---------------------------------------------------------


// the game actually has its own state
// machine, instead of being handled
// directly by the gameplay scene
enum GameStates
{
    Game_Falling,
    Game_Locking,
    Game_LinesHighlight,
    Game_LinesFall,
    Game_LoseHighlight,
    Game_Ended
};

// ---------------------------------------------------------

// all data needed to run the game
struct GameSession
{
    // general state
    GameStates State;
    int ElapsedFrames;
    
    // game progress
    int CurrentLevel;
    int ClearedLines;
    int LevelLines;
    int Score;
    
    // random piece sequence; pieces are
    // generated in batches with each of
    // the 7, where only their order varies
    int LastBagPosition;
    PieceTypes[ 7 ] PieceBag;
    PieceTypes CurrentPiece;
    PieceTypes NextPiece;
    
    // piece movement
    int PieceX, PieceY;
    int PieceRotation;  // 0 to 3
    
    // counters for event highlighting
    int LevelBlinkCounter;
    int LinesBlinkCounter;
    int ScoreBlinkCounter;
};
