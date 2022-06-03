// ---------------------------------------------------------
//   DATA FOR ALL LEVELS
// ---------------------------------------------------------

// levels are read from external files, and then
// stored as a series of arrays in the cartridge;
// these arrays have the form [ height ][ width ]
// in order for the levels to be stored in the
// usual order (left to right and top to bottom)
embedded int[ 9 ][ 80 ] Level01Map = "obj/Level01-Map.vmap";
embedded int[ 9 ][ 80 ] Level01Events = "obj/Level01-Events.vmap";

embedded int[ 9 ][ 80 ] Level02Map = "obj/Level02-Map.vmap";
embedded int[ 9 ][ 80 ] Level02Events = "obj/Level02-Events.vmap";

embedded int[ 9 ][ 80 ] Level03Map = "obj/Level03-Map.vmap";
embedded int[ 9 ][ 80 ] Level03Events = "obj/Level03-Events.vmap";

// level is formed by the map itself and additional data;
// since not all levels have the same size,we need to have
// a pointer array to be able to traverse the level sequence
struct Level
{
    int* MapArray;
    int* EventArray;
    int TilesInX, TilesInY;
};

Level[ 3 ] GameLevels;

// we need to use this because currently the compiler
// does not support initialization lists for arrays
void CreateGameLevels()
{
    GameLevels[ 0 ].TilesInX = 80;
    GameLevels[ 0 ].TilesInY = 9;
    GameLevels[ 0 ].MapArray = &Level01Map[ 0 ][ 0 ];
    GameLevels[ 0 ].EventArray = &Level01Events[ 0 ][ 0 ];
    
    GameLevels[ 1 ].TilesInX = 80;
    GameLevels[ 1 ].TilesInY = 9;
    GameLevels[ 1 ].MapArray = &Level02Map[ 0 ][ 0 ];
    GameLevels[ 1 ].EventArray = &Level02Events[ 0 ][ 0 ];
    
    GameLevels[ 2 ].TilesInX = 80;
    GameLevels[ 2 ].TilesInY = 9;
    GameLevels[ 2 ].MapArray = &Level03Map[ 0 ][ 0 ];
    GameLevels[ 2 ].EventArray = &Level03Events[ 0 ][ 0 ];
}
