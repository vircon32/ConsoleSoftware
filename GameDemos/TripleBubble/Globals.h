// ---------------------------------------------------------
//   GLOBAL VARIABLES
// ---------------------------------------------------------


// game objects
Playfield CenterField;
Launcher LeftLauncher, CenterLauncher, RightLauncher;
Character Player1;
Points PointsP1;

// level bubble array
// (line 11 is below losing line)
#define TripleLevelWidth   20
#define ClassicLevelWidth  12
#define LevelHeight        10

typedef int[ LevelHeight ][ TripleLevelWidth ] TripleLevel;
typedef int[ LevelHeight ][ ClassicLevelWidth ] ClassicLevel;

// test levels for each mode, designed for quick death and clear
embedded ClassicLevel ClassicLevel_Test = "obj\\Classic_Test.vmap";
embedded TripleLevel TripleLevel_Test = "obj\\Triple_Test.vmap";

// arcade classic levels for the demo
embedded ClassicLevel ClassicLevel_1_1 = "obj\\Classic_1-1.vmap";
embedded ClassicLevel ClassicLevel_1_2 = "obj\\Classic_1-2.vmap";
embedded ClassicLevel ClassicLevel_1_3 = "obj\\Classic_1-3.vmap";
embedded ClassicLevel ClassicLevel_1_4 = "obj\\Classic_1-4.vmap";

// arcade triple levels for the demo
embedded TripleLevel TripleLevel_1_1 = "obj\\Triple_1-1.vmap";
embedded TripleLevel TripleLevel_1_2 = "obj\\Triple_1-2.vmap";
embedded TripleLevel TripleLevel_1_3 = "obj\\Triple_1-3.vmap";
embedded TripleLevel TripleLevel_1_4 = "obj\\Triple_1-4.vmap";
