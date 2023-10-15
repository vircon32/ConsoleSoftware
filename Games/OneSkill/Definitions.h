// *****************************************************************************
    // start include guard
    #ifndef DEFINITIONS_H
    #define DEFINITIONS_H
// *****************************************************************************


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
    MusicCutscenes,
    MusicTutorial,
    MusicLevelDay,
    MusicLevelNight,
    
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


#define ChannelMusic    15
#define ChannelEnemies  14
#define ChannelSpikes   13


// ---------------------------------------------------------
//   STATE MACHINE: SCENES AND STATES
// ---------------------------------------------------------


// game scenes
enum GameScenes
{
    Scene_Title = 1,
    Scene_Intro,
    Scene_Tutorial,
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
    Gameplay_RoomIntro,
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
    ChangeSkill_Rotating,
    ChangeSkill_Waiting,
    ChangeSkill_FadeOut
};


// ---------------------------------------------------------
//   GAME RELATED DEFINITIONS
// ---------------------------------------------------------


// all available skills
enum PlayerSkills
{
    Skill_None = 0,     // only usable at level start
    Skill_Pistol,
    Skill_Cape,
    Skill_Wand,
    Skill_Boots
};


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
