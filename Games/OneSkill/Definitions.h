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
#define Scene_Title        1
#define Scene_Intro        2
#define Scene_Tutorial     3
#define Scene_Gameplay     4
#define Scene_Ending       5

// scene states: title
#define Title_Initialize    11
#define Title_FadeIn        12
#define Title_TitleSlide    13
#define Title_Waiting       14
#define Title_StartPressed  15

// scene states: intro
//

// scene states: tutorial
#define Tutorial_Initialize  31
#define Tutorial_FadeIn      32
#define Tutorial_Intro       33
#define Tutorial_SkillPost   34
#define Tutorial_Gun         35
#define Tutorial_Cape        36
#define Tutorial_Boots       37
#define Tutorial_Wand        38
#define Tutorial_FadeOut     39

// scene states: gameplay
#define Gameplay_Initialize  41
#define Gameplay_LevelIntro  42
#define Gameplay_Level       43
#define Gameplay_Pause       44
#define Gameplay_Death       45
#define Gameplay_Goal        46
#define Gameplay_FadeOut     47

// scene states: ending
#define Ending_Initialize   51
#define Ending_FadeIn       52
#define Ending_TextAppears  53
#define Ending_FadeOut      54


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
