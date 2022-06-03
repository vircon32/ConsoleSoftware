// ---------------------------------------------------------
//   DATA FOR ALL LEVELS
// ---------------------------------------------------------

// levels are read from external files, and then
// stored as a series of arrays in the cartridge;
// these arrays have the form [ height ][ width ]
// in order for the levels to be stored in the
// usual order (left to right and top to bottom)

// world 1 (levels 01 to 10)
embedded int[  9 ][ 16 ] Level01 = "obj/Level01.vmap";
embedded int[  9 ][ 29 ] Level02 = "obj/Level02.vmap";
embedded int[ 16 ][ 24 ] Level03 = "obj/Level03.vmap";
embedded int[ 15 ][ 28 ] Level04 = "obj/Level04.vmap";
embedded int[ 15 ][ 35 ] Level05 = "obj/Level05.vmap";
embedded int[ 13 ][ 37 ] Level06 = "obj/Level06.vmap";
embedded int[ 19 ][ 24 ] Level07 = "obj/Level07.vmap";
embedded int[ 13 ][ 30 ] Level08 = "obj/Level08.vmap";
embedded int[ 20 ][ 27 ] Level09 = "obj/Level09.vmap";
embedded int[ 20 ][ 27 ] Level10 = "obj/Level10.vmap";

// world 2 (levels 11 to 20)
embedded int[ 14 ][ 32 ] Level11 = "obj/Level11.vmap";
embedded int[ 16 ][ 28 ] Level12 = "obj/Level12.vmap";
embedded int[ 17 ][ 29 ] Level13 = "obj/Level13.vmap";
embedded int[ 13 ][ 34 ] Level14 = "obj/Level14.vmap";
embedded int[ 16 ][ 33 ] Level15 = "obj/Level15.vmap";
embedded int[ 14 ][ 38 ] Level16 = "obj/Level16.vmap";
embedded int[ 22 ][ 31 ] Level17 = "obj/Level17.vmap";
embedded int[ 17 ][ 31 ] Level18 = "obj/Level18.vmap";
embedded int[ 11 ][ 39 ] Level19 = "obj/Level19.vmap";
embedded int[ 18 ][ 36 ] Level20 = "obj/Level20.vmap";

// world 3 (levels 21 to 30)
embedded int[ 16 ][ 36 ] Level21 = "obj/Level21.vmap";
embedded int[ 16 ][ 37 ] Level22 = "obj/Level22.vmap";
embedded int[ 26 ][ 24 ] Level23 = "obj/Level23.vmap";
embedded int[ 31 ][ 16 ] Level24 = "obj/Level24.vmap";
embedded int[ 18 ][ 64 ] Level25 = "obj/Level25.vmap";
embedded int[ 17 ][ 33 ] Level26 = "obj/Level26.vmap";
embedded int[ 16 ][ 32 ] Level27 = "obj/Level27.vmap";
embedded int[ 14 ][ 36 ] Level28 = "obj/Level28.vmap";
embedded int[ 19 ][ 25 ] Level29 = "obj/Level29.vmap";
embedded int[ 17 ][ 26 ] Level30 = "obj/Level30.vmap";

// level is formed by the map itself and additional data;
// since not all levels have the same size,we need to have
// a pointer array to be able to traverse the level sequence
struct Level
{
    int* MapArray;
    int TilesInX, TilesInY;
    int TimeInSeconds;
};

Level[ 30 ] GameLevels;

// we need to use this because currently the compiler
// does not support initialization lists for arrays
void CreateGameLevels()
{
    // - - - - - - - - - - - - - - - - - -
    // WORLD 1 (LEVELS 01 TO 10)
    
    GameLevels[ 0 ].TilesInX = 16;
    GameLevels[ 0 ].TilesInY = 9;
    GameLevels[ 0 ].TimeInSeconds = 40;
    GameLevels[ 0 ].MapArray = &Level01[ 0 ][ 0 ];
    
    GameLevels[ 1 ].TilesInX = 29;
    GameLevels[ 1 ].TilesInY = 9;
    GameLevels[ 1 ].TimeInSeconds = 50;
    GameLevels[ 1 ].MapArray = &Level02[ 0 ][ 0 ];
    
    GameLevels[ 2 ].TilesInX = 24;
    GameLevels[ 2 ].TilesInY = 16;
    GameLevels[ 2 ].TimeInSeconds = 70;
    GameLevels[ 2 ].MapArray = &Level03[ 0 ][ 0 ];
    
    GameLevels[ 3 ].TilesInX = 28;
    GameLevels[ 3 ].TilesInY = 15;
    GameLevels[ 3 ].TimeInSeconds = 80;
    GameLevels[ 3 ].MapArray = &Level04[ 0 ][ 0 ];
    
    GameLevels[ 4 ].TilesInX = 35;
    GameLevels[ 4 ].TilesInY = 15;
    GameLevels[ 4 ].TimeInSeconds = 100;
    GameLevels[ 4 ].MapArray = &Level05[ 0 ][ 0 ];
    
    GameLevels[ 5 ].TilesInX = 37;
    GameLevels[ 5 ].TilesInY = 13;
    GameLevels[ 5 ].TimeInSeconds = 60;
    GameLevels[ 5 ].MapArray = &Level06[ 0 ][ 0 ];
    
    GameLevels[ 6 ].TilesInX = 24;
    GameLevels[ 6 ].TilesInY = 19;
    GameLevels[ 6 ].TimeInSeconds = 65;
    GameLevels[ 6 ].MapArray = &Level07[ 0 ][ 0 ];
    
    GameLevels[ 7 ].TilesInX = 30;
    GameLevels[ 7 ].TilesInY = 13;
    GameLevels[ 7 ].TimeInSeconds = 85;
    GameLevels[ 7 ].MapArray = &Level08[ 0 ][ 0 ];
    
    GameLevels[ 8 ].TilesInX = 27;
    GameLevels[ 8 ].TilesInY = 20;
    GameLevels[ 8 ].TimeInSeconds = 70;
    GameLevels[ 8 ].MapArray = &Level09[ 0 ][ 0 ];
    
    GameLevels[ 9 ].TilesInX = 27;
    GameLevels[ 9 ].TilesInY = 20;
    GameLevels[ 9 ].TimeInSeconds = 75;
    GameLevels[ 9 ].MapArray = &Level10[ 0 ][ 0 ];
    
    // - - - - - - - - - - - - - - - - - -
    // WORLD 2 (LEVELS 11 TO 20)
    
    GameLevels[ 10 ].TilesInX = 32;
    GameLevels[ 10 ].TilesInY = 14;
    GameLevels[ 10 ].TimeInSeconds = 55;
    GameLevels[ 10 ].MapArray = &Level11[ 0 ][ 0 ];
    
    GameLevels[ 11 ].TilesInX = 28;
    GameLevels[ 11 ].TilesInY = 16;
    GameLevels[ 11 ].TimeInSeconds = 60;
    GameLevels[ 11 ].MapArray = &Level12[ 0 ][ 0 ];
    
    GameLevels[ 12 ].TilesInX = 29;
    GameLevels[ 12 ].TilesInY = 17;
    GameLevels[ 12 ].TimeInSeconds = 60;
    GameLevels[ 12 ].MapArray = &Level13[ 0 ][ 0 ];
    
    GameLevels[ 13 ].TilesInX = 34;
    GameLevels[ 13 ].TilesInY = 13;
    GameLevels[ 13 ].TimeInSeconds = 55;
    GameLevels[ 13 ].MapArray = &Level14[ 0 ][ 0 ];
    
    GameLevels[ 14 ].TilesInX = 16;
    GameLevels[ 14 ].TilesInY = 33;
    GameLevels[ 14 ].TimeInSeconds = 85;
    GameLevels[ 14 ].MapArray = &Level15[ 0 ][ 0 ];
    
    GameLevels[ 15 ].TilesInX = 38;
    GameLevels[ 15 ].TilesInY = 14;
    GameLevels[ 15 ].TimeInSeconds = 60;
    GameLevels[ 15 ].MapArray = &Level16[ 0 ][ 0 ];
    
    GameLevels[ 16 ].TilesInX = 31;
    GameLevels[ 16 ].TilesInY = 22;
    GameLevels[ 16 ].TimeInSeconds = 110;
    GameLevels[ 16 ].MapArray = &Level17[ 0 ][ 0 ];
    
    GameLevels[ 17 ].TilesInX = 31;
    GameLevels[ 17 ].TilesInY = 17;
    GameLevels[ 17 ].TimeInSeconds = 65;
    GameLevels[ 17 ].MapArray = &Level18[ 0 ][ 0 ];
    
    GameLevels[ 18 ].TilesInX = 39;
    GameLevels[ 18 ].TilesInY = 11;
    GameLevels[ 18 ].TimeInSeconds = 40;
    GameLevels[ 18 ].MapArray = &Level19[ 0 ][ 0 ];
    
    GameLevels[ 19 ].TilesInX = 36;
    GameLevels[ 19 ].TilesInY = 18;
    GameLevels[ 19 ].TimeInSeconds = 150;
    GameLevels[ 19 ].MapArray = &Level20[ 0 ][ 0 ];
    
    // - - - - - - - - - - - - - - - - - -
    // WORLD 3 (LEVELS 21 TO 30)
    
    GameLevels[ 20 ].TilesInX = 36;
    GameLevels[ 20 ].TilesInY = 16;
    GameLevels[ 20 ].TimeInSeconds = 65;
    GameLevels[ 20 ].MapArray = &Level21[ 0 ][ 0 ];
    
    GameLevels[ 21 ].TilesInX = 37;
    GameLevels[ 21 ].TilesInY = 16;
    GameLevels[ 21 ].TimeInSeconds = 85;
    GameLevels[ 21 ].MapArray = &Level22[ 0 ][ 0 ];
    
    GameLevels[ 22 ].TilesInX = 24;
    GameLevels[ 22 ].TilesInY = 26;
    GameLevels[ 22 ].TimeInSeconds = 120;
    GameLevels[ 22 ].MapArray = &Level23[ 0 ][ 0 ];
    
    GameLevels[ 23 ].TilesInX = 16;
    GameLevels[ 23 ].TilesInY = 31;
    GameLevels[ 23 ].TimeInSeconds = 75;
    GameLevels[ 23 ].MapArray = &Level24[ 0 ][ 0 ];
    
    GameLevels[ 24 ].TilesInX = 64;
    GameLevels[ 24 ].TilesInY = 18;
    GameLevels[ 24 ].TimeInSeconds = 120;
    GameLevels[ 24 ].MapArray = &Level25[ 0 ][ 0 ];
    
    GameLevels[ 25 ].TilesInX = 33;
    GameLevels[ 25 ].TilesInY = 17;
    GameLevels[ 25 ].TimeInSeconds = 60;
    GameLevels[ 25 ].MapArray = &Level26[ 0 ][ 0 ];
    
    GameLevels[ 26 ].TilesInX = 32;
    GameLevels[ 26 ].TilesInY = 16;
    GameLevels[ 26 ].TimeInSeconds = 50;
    GameLevels[ 26 ].MapArray = &Level27[ 0 ][ 0 ];
    
    GameLevels[ 27 ].TilesInX = 36;
    GameLevels[ 27 ].TilesInY = 14;
    GameLevels[ 27 ].TimeInSeconds = 35;
    GameLevels[ 27 ].MapArray = &Level28[ 0 ][ 0 ];
    
    GameLevels[ 28 ].TilesInX = 25;
    GameLevels[ 28 ].TilesInY = 19;
    GameLevels[ 28 ].TimeInSeconds = 50;
    GameLevels[ 28 ].MapArray = &Level29[ 0 ][ 0 ];
    
    GameLevels[ 29 ].TilesInX = 26;
    GameLevels[ 29 ].TilesInY = 17;
    GameLevels[ 29 ].TimeInSeconds = 40;
    GameLevels[ 29 ].MapArray = &Level30[ 0 ][ 0 ];    
}
