// ---------------------------------------------------------
//   TEXTURES
// ---------------------------------------------------------


// names for textures
enum GameTextures
{
    TextureTitle,
    TextureStory,
    TextureGameplay,
    TextureBGLevel1,
    TextureBGLevel2,
    TextureBGLevel3,
    TextureFont11x16,
    TextureFont22x32
};


// ---------------------------------------------------------
//   TEXTURE REGIONS
//   (only the ones defined manually, without the editor)
// ---------------------------------------------------------


// regions for level backgrounds
#define RegionLevelBackground  0


// ---------------------------------------------------------
//   SOUNDS
// ---------------------------------------------------------


enum GameSounds
{
    // names for musics
    MusicTitle,
    MusicStory,
    MusicGameplay,

    // names for sound effects
    SoundStartGame,
    SoundStoryLetter,
    SoundJump,
    SoundLand,
    SoundShoot,
    SoundDeath,
    SoundLevelClear,
    SoundPause,
    SoundEnemyShoot,
    SoundEnemyHit,
    SoundEnemyDeath,
    SoundExplosionSmall,
    SoundExplosionLarge,
    SoundMissile
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
    Scene_Intro,
    Scene_Gameplay,
    Scene_Credits,
    Scene_Ending,
    Scene_GameOver
};

// scene states: title
enum StatesTitle
{
    Title_Initialize = 11,
    Title_FadeIn,
    Title_Waiting,
    Title_StartPressed
};

// scene states: story (intro & ending)
enum StatesStory
{
    Story_Initialize = 21,
    Story_WriteText,
    Story_Waiting,
    Story_FadeOut
};

// scene states: gameplay
enum StatesGameplay
{
    Gameplay_Initialize = 31,
    Gameplay_LevelIntro,
    Gameplay_Level,
    Gameplay_Pause,
    Gameplay_Death,
    Gameplay_LevelClear,
    Gameplay_FadeOut
};

// scene states: game over
enum StatesGameOver
{
    GameOver_Initialize = 41,
    GameOver_FadeIn,
    GameOver_TextAppears,
    GameOver_FadeOut
};


// ---------------------------------------------------------
//   GAME RELATED DEFINITIONS
// ---------------------------------------------------------


#define NumberOfLevels  3
#define InitialLives    9


// ---------------------------------------------------------
//   GLOBAL VARIABLES
// ---------------------------------------------------------


// globals for game state
int GameScene;   // scene currently running
int GameState;   // state within that scene

// globals for game session
int PlayerLives;        // includes current life: life zero is not played
int LevelNumber;        // starts from 1, not 0

// set initial state for the whole game
void ResetGameSession()
{
    PlayerLives = InitialLives;
    LevelNumber = 1;
}
