// ---------------------------------------------------------
//   NAMES FOR ALL ASSETS IN THE ROM
// ---------------------------------------------------------


// names for textures
enum Textures
{
    TextureGemMaze
};

// ---------------------------------------------------------

// names for sound effects
enum Sounds
{
    SoundStart,
    SoundCollectRedGem,
    SoundCollectBlueGem,
    SoundCollectTime,
    SoundDeath,
    SoundTimeOut,
    SoundGoal
};


// ---------------------------------------------------------
//   DEFINITIONS FOR GAME LOGIC
// ---------------------------------------------------------


// game is handled as a state machine
enum GameStates
{
    StateStart,
    StatePlay,
    StateDeath,
    StateGoal
};

// ---------------------------------------------------------

// the different tiles in our map
enum TileTypes
{
    TileEmpty,
    TileWall,
    TileSpikes,
    TileGoal,
    TilePlayer,
    TileTime,
    TileRedGem,
    TileBlueGem
};


// ---------------------------------------------------------
//   GLOBAL VARIABLES
// ---------------------------------------------------------


// game objects from our libraries
tileset TilesMaze;
tilemap MapMaze;
numberfont FontNumbers;

// ---------------------------------------------------------

// state of our game session
int GameState;
int RemainingTimeFrames;
int CollectedGems;
