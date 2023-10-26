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
    TextureGameplay
};


// ---------------------------------------------------------
//   SOUNDS
// ---------------------------------------------------------


enum Sounds
{
    MusicLevel,
    
    SoundCountdown,
    SoundShoot,
    SoundTankHit,
    SoundPressStart,
    SoundExplosionSmall,
    SoundExplosionLarge,
    SoundRespawn
};


// ---------------------------------------------------------
//   SOUND CHANNELS
// ---------------------------------------------------------


#define ChannelMusic    15


// ---------------------------------------------------------
//   STATE MACHINE: SCENES AND STATES
// ---------------------------------------------------------


// game scenes
enum GameScenes
{
    Scene_Title = 1,
    Scene_Gameplay
};

// scene states: title
enum StatesTitle
{
    Title_Initialize = 11,
    Title_FadeIn,
    Title_Waiting,
    Title_StartPressed
};

// scene states: gameplay
enum StatesGameplay
{
    Gameplay_Initialize = 21,
    Gameplay_Countdown,
    Gameplay_Level
};


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
