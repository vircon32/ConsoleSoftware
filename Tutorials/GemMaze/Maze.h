// ---------------------------------------------------------
//   DEFINITION OF OUR MAZE AS AN ARRAY OF TILES
// ---------------------------------------------------------


// dimensions of our maze
#define MAZE_WIDTH  20
#define MAZE_HEIGHT 14

// we could import a tilemap as an external file,
// but for simplicity the array is populated here
int[ MAZE_HEIGHT ][ MAZE_WIDTH ] OriginalMazeArray =
{
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
    { 1,6,7,6,0,1,1,0,0,0,0,6,6,5,1,0,0,0,0,1 },
    { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,6,6,0,3 },
    { 1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1 },
    { 1,6,6,6,0,0,1,2,6,6,2,0,0,0,0,0,0,1,1,1 },
    { 1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1 },
    { 1,0,0,0,1,6,6,0,0,0,0,0,0,0,1,0,0,0,7,1 },
    { 1,1,0,1,1,7,6,0,2,0,0,0,0,6,1,0,0,2,0,1 },
    { 1,0,0,0,1,1,1,1,1,0,0,1,1,1,1,0,0,2,0,1 },
    { 1,0,0,0,1,0,0,0,0,0,0,2,0,0,0,0,0,0,0,1 },
    { 1,0,0,0,1,0,0,6,0,0,0,0,0,0,0,0,6,0,0,1 },
    { 1,0,6,0,0,0,0,6,0,0,0,0,0,0,0,5,2,6,0,1 },
    { 4,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,6,0,0,1 },
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
};

// ---------------------------------------------------------

// we will need to keep the original array for resets,
// since we will modify it when objects are collected; so
// we will make a working copy of it in this second array
int[ MAZE_HEIGHT ][ MAZE_WIDTH ] MazeArray;
