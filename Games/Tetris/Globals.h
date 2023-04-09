// load piece models from external files
embedded PieceModel ModelPieceO = "obj/PieceO.vmap";
embedded PieceModel ModelPieceT = "obj/PieceT.vmap";
embedded PieceModel ModelPieceI = "obj/PieceI.vmap";
embedded PieceModel ModelPieceL = "obj/PieceL.vmap";
embedded PieceModel ModelPieceJ = "obj/PieceJ.vmap";
embedded PieceModel ModelPieceZ = "obj/PieceZ.vmap";
embedded PieceModel ModelPieceS = "obj/PieceS.vmap";

// load title map from external file
#define TitleMapWidth  21
#define TitleMapHeight  7
embedded int[ TitleMapHeight ][ TitleMapWidth ] TitleMap =  "obj/Title.vmap";

// ---------------------------------------------------------

// now make an array to point to them
PieceModel*[ 8 ] PieceModels =
{
    NULL,
    &ModelPieceO,
    &ModelPieceT,
    &ModelPieceI,
    &ModelPieceL,
    &ModelPieceJ,
    &ModelPieceZ,
    &ModelPieceS
};

// ---------------------------------------------------------

// these "speeds" are actually 1/speed
// (they represent number of frames)
int[ 30 ] LevelGravitySpeeds = 
{
    48, 43, 38, 33, 28,
    23, 18, 13,  8,  6,
     5,  5,  5,  4,  4,
     4,  3,  3,  3,  2,
     2,  2,  2,  2,  2,
     2,  2,  2,  2,  1
};

// ---------------------------------------------------------

// scoring for lines cleared
int[ 5 ] LineClearBaseScores =
{
    10,     // no lines: points per placed piece
    40,     // 1 line  (single)
    100,    // 2 lines (double)
    300,    // 3 lines (triple)
    1200    // 4 lines (tetris)
};

// ---------------------------------------------------------

// graphic objects for our libraries
numberfont FontLevel, FontLines;
tileset TilesPieces;
tilemap MapBoard, MapTitle;

// ---------------------------------------------------------

// state for game state machine
int GameScene;
int GameState;

// state for the game session
GameSession Game;
GameBoard Board;

// we will use a copy of the board
// for displaying blinks and animations
GameBoard BoardCopy;
tilemap MapBoardCopy;

// we will also need to store the line
// fall distances for the fall animations
int[ BoardHeight ] RowFallDistances;
