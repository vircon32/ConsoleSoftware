// ---------------------------------------------------------
// ---------------------  Colors  --------------------------
// ---------------------------------------------------------

#define color_default   0xFFDA2F3E

// ---------------------------------------------------------
//   TEXTURES
// ---------------------------------------------------------

#define TextureTicTacToe  0

// ---------------------------------------------------------
//   TEXTURE REGIONS
// ---------------------------------------------------------

// regions for title
#define CrossMark           0
#define CircleMark          1
#define TicTacToeLogo       2
#define AuthorLogo          3
#define TileMark            4
#define PressStart          5

// Game Cursors
#define P1TopLeftCursor       6
#define P1TopRightCursor      7
#define P1BottomLeftCursor    8
#define P1BottomRightCursor   9

#define P2TopLeftCursor       10
#define P2TopRightCursor      11
#define P2BottomLeftCursor    12
#define P2BottomRightCursor   13

// Game Indicators
#define ScoreBackground         14
#define CrossScoreIndicator     15
#define CircleScoreIndicator    16

// Score Numbers
#define N0 17
#define N1 18
#define N2 19
#define N3 20
#define N4 21
#define N5 22
#define N6 23
#define N7 24
#define N8 25
#define N9 26

// In Game Keys Information Background
#define GameKeysInformation 27

// ---------------------------------------------------------
// ------------------ Tiles Positions ----------------------
// ---------------------------------------------------------

#define X11   46
#define Y11   19
#define X12   158
#define Y12   19
#define X13   270
#define Y13   19

#define X21   46
#define Y21   131
#define X22   158
#define Y22   131
#define X23   270
#define Y23   131

#define X31   46
#define Y31   243
#define X32   158
#define Y32   243
#define X33   270
#define Y33   243

// ---------------------------------------------------------
// ------------------  Score Positions  --------------------
// ---------------------------------------------------------

#define P1Score_X 473
#define P1Score_Y 65

#define P2Score_X 473
#define P2Score_Y 129

// ---------------------------------------------------------
//   STATE MACHINE: SCENES AND STATES
// ---------------------------------------------------------


// game scenes
#define Title_Initialize    1
#define InGame              2


// ---------------------------------------------------------
//   GLOBALS FOR GAME STATE
// ---------------------------------------------------------


int    GameScene;           // scene currently running
int    GameState;           // state within that scene
int    ElapsedFrames;       // Elapsed Frames Counter
int    CurrentPlayer;       // Current Player
int    XCursorPosition;     // X Cursor Position
int    YCursorPosition;     // Y Cursor Position
int    P1Score;             // Player 1 Score
int    P2Score;             // Player 2 Score
int[9] Tile;                // Current Game Status on Tiles
                            // 0 1 2  |  0 --> empty
                            // 3 4 5  |  1 --> P1 Marked (X)
                            // 6 7 8  |  2 --> P2 Marked (O)
// Tile Drawing Coordinates From Tile Index
int[9] XTileCoord = {41,153,265,41,153,265,41,153,265};
int[9] YTileCoord = {14,14,14,126,126,126,238,238,238};

