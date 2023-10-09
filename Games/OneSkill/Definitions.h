// ---------------------------------------------------------
//   TEXTURES
// ---------------------------------------------------------


// names for textures
enum Textures
{
    TextureTitle,
    TextureGameplay,
    TextureCutscenes,
    TextureFont11x16
};


// ---------------------------------------------------------
//   SOUNDS
// ---------------------------------------------------------


enum Sounds
{
    MusicTitle,
    MusicGameplay,
    MusicCutscenes,
    MusicTutorial,
    
    SoundTitleSlide,
    SoundTitleLogo,
    SoundPressStart,
    SoundMenuMove,
    SoundMenuAccept,
    SoundMenuCancel,
    SoundStoryLetter,
    SoundPause,
    SoundDeath,
    SoundEnterDoor,
    SoundLevelClear,
    SoundJump,
    SoundLand,
    SoundGolemWalk,
    SoundGolemDeath,
    SoundBirdFlap,
    SoundBirdDeath,
    SoundEnemyHit,
    SoundShoot,
    SoundShotDestroyed,
    SoundBoxDestroyed,
    SoundSpikesSwitch,
    SoundWandMagic,
    SoundDoubleJump,
    SoundBootsLand
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
    Scene_Tutorial,
    Scene_Intro,
    Scene_Gameplay,
    Scene_ChangeSkill,
    Scene_Ending
};

// scene states: title
enum StatesTitle
{
    Title_Initialize = 11,
    Title_FadeIn,
    Title_SlideDown,
    Title_Waiting,
    Title_StartPressed
};

// scene states: tutorial
enum StatesTutorial
{
    Tutorial_Initialize = 21,
    Tutorial_SkillSystem,
    Tutorial_Skills,
    Tutorial_FadeOut
};

// scene states: story (intro & ending)
enum StatesStory
{
    Story_Initialize = 31,
    Story_WriteText,
    Story_Waiting,
    Story_FadeOut
};

// scene states: gameplay
enum StatesGameplay
{
    Gameplay_Initialize = 41,
    Gameplay_LevelIntro,
    Gameplay_Level,
    Gameplay_Pause,
    Gameplay_Death,
    Gameplay_RoomClear,
    Gameplay_LevelClear,
    Gameplay_FadeOut
};

// scene states: change skill
enum StatesChangeSkill
{
    ChangeSkill_Initialize = 51,
    ChangeSkill_IconsAppear,
    ChangeSkill_Rotate,
    ChangeSkill_FadeOut
};


// ---------------------------------------------------------
//   GAME RELATED DEFINITIONS
// ---------------------------------------------------------


#define NumberOfLevels  2
#define RoomsPerLevel   2


// ---------------------------------------------------------
//   GLOBAL VARIABLES
// ---------------------------------------------------------


// globals for game state
int GameScene;      // scene currently running
int GameState;      // state within that scene

// globals for game session
int LevelNumber;    // starts from 1, not 0
int RoomNumber;     // starts from 1, not 0

// set initial state for the whole game
void ResetGameSession()
{
    LevelNumber = 1;
    RoomNumber = 1;
}

// global objects
textfont Font11x16;
