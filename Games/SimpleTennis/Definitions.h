// ---------------------------------------------------------
//   TEXTURES
// ---------------------------------------------------------

// names for textures

#define TextureSimpleTennis  0
#define TextureFont          1

// ---------------------------------------------------------
//   TEXTURE REGIONS
// ---------------------------------------------------------

// giving names to our texture regions
// makes them easier to define and use
#define RegionGoalLine   0
#define RegionMiddleLine 1
#define Regionpaddle     2
#define RegionBall       3
#define RegionLogo       4

// regions for the score textures
#define RegionScore0 10
/*#define RegionScore1 11
#define RegionScore2 12
#define RegionScore3 13
#define RegionScore4 14
#define RegionScore5 15
#define RegionScore6 16
#define RegionScore7 17
#define RegionScore8 18
#define RegionScore9 19*/

// region for arrow to display direction

#define RegionArrow 20

// ---------------------------------------------------------
//   TEXTURE REGIONS (FONT)
// ---------------------------------------------------------

// these regions are from 0 to 127,
// (the standard ASCII character set)
#define FirstRegionTextFont  0

// menu selection
#define RegionMenuSelectionLeft   128
#define RegionMenuSelectionRight  129
#define RegionMenuSelectionMiddle 130

#define SelectionSideWidth      4
#define SelectionMiddleWidth    12

// ---------------------------------------------------------
//   STATE MACHINE: SCENES AND STATES
// ---------------------------------------------------------

// game scenes
#define Scene_Title     1
#define Scene_GamePlay  2

// scene states: title
#define Title_Start         1
#define Title_Menu          2
#define Title_Wait_New_Game 3

// scene states: gameplay
#define Gameplay_Initialize     1
#define Gameplay_Start_Point    2
#define Gameplay_Playing        3
#define Gameplay_Pause          4
#define Gameplay_Game_Finished  5

// -----------------------------------------------
// MENU OPTIONS
// -----------------------------------------------

//different options
#define Menu_Opponent       1
#define Menu_AIDifficulty   2
#define Menu_Score          3
#define Menu_Start          4

// where to print logo
#define LogoXSize 180
#define LogoYSize 166
#define LogoYPlace (screen_height / 8) + (LogoYSize / 3)

//where to print options
#define Menu_Opponent_Option_X (screen_width / 3) - 50
#define Menu_Opponent_Select_X (screen_width / 3) * 2 - 40
#define Menu_Opponent_Y (screen_height/5) * 3 + 10

#define Menu_AIDifficulty_Option_X (screen_width / 3) - 50
#define Menu_AIDifficulty_Select_X (screen_width / 3) * 2 - 40
#define Menu_AIDifficulty_Y (screen_height/5) * 3 + 40

#define Menu_Score_Option_X (screen_width / 3) - 50
#define Menu_Score_Select_X (screen_width / 3) * 2 - 40
#define Menu_Score_Y (screen_height/5) * 3 + 70

#define Menu_Start_X screen_width / 2 - 60
#define Menu_Start_Y (screen_height/5) * 3 + 120

//options for opponent
#define Menu_VS_AI  1
#define Menu_VS_P2  2

//options for Difficulty
#define Menu_Difficulty_Easy    1
#define Menu_Difficulty_Normal  2
#define Menu_Difficulty_Hard    3

//options for score to win (5 to 9)
#define Menu_Score_5    5
#define Menu_Score_6    6
#define Menu_Score_7    7
#define Menu_Score_8    8
#define Menu_Score_9    9

// -----------------------------------------------
// DEFINITIONS
// -----------------------------------------------

// score dimensions
#define ScoreWidth            22
#define ScoreDistanceToCenter 15
#define ScoreDistanceToTop    10
#define MaxScore              9

// movement speed of the paddles
#define PaddleSpeed 5

// base movement speed of ball
#define BallBaseSpeed 6
#define BallMaxSpeed  14

// goal line
#define GoalLineDistance 5
#define GoalLineWidth    4

// middle line
#define MiddleLineWidth 4

// walls
#define WallWidth 0

// paddle size
#define PaddleSizeX 15
#define PaddleSizeY 83

// paddle distance from screen edges
#define PaddleDistance 25

// ball size
#define BallSize 25

// possible angles
#define MinAngle -60
#define MaxAngle  60

// sounds
#define SoundPaddleHit  0
#define SoundWallHit    1
#define SoundGoalScored 2
#define SoundGameEnd    3
#define SoundMenu       4

// AI difficulty levels
#define Easy (PaddleSizeY / 2) 
#define Normal (PaddleSizeY / 3) + 8
#define Hard (PaddleSizeY / 3)

// Control the time before starting a new game
// because exiting to menu can start a new game
// instantly

#define SecondsUntilNewGame 0.15
#define SecondsBeforeNewPoint 0.6

// ---------------------------------------------------------
//   GLOBALS FOR MENU
// ---------------------------------------------------------
int CursorLine;         // current menu line selected
int Opponent;           // to play vs P2 or AI
int SelectedDifficulty; // if vs AI this is the difficulty
int ScoreToWin;         // number of goals to win a game

// Menu text
int* StartGameText;
int* OpponentText;
int* DifficultyText;
int* ScoreToWinText;

// ---------------------------------------------------------
//   GLOBALS FOR GAME STATE
// ---------------------------------------------------------

int GameScene;        // scene currently running
int GameState;        // state within that scene
int PlayerPaused;     // displays the player who paused

int Players;          // number of players
int Difficulty;       // selected difficulty for AI

int TimeGameFinished;       // Time since a game ended
int TimeAfterLastPoint;     // Time since last point ended