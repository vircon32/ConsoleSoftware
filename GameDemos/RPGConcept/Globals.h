// ---------------------------------------------------------
//   DEFINE CONSTANTS
// ---------------------------------------------------------


// game textures
#define TextureRPG 0
#define TextureDialogFont 1

// dimensions of our map, in tiles
#define MapTilesX 24
#define MapTilesY 14

// speed of our characters
#define CharSpeed 2

// tree position is fixed
#define TreeX 480
#define TreeY 310


// ---------------------------------------------------------
//   GLOBAL VARIABLES
// ---------------------------------------------------------


// our map and tiles
tileset RPGTiles;
tilemap RPGMap;

// text font used for dialogs
textfont FontDialog;

// our 2D array containing the map tile IDs
// is stored in ROM from an external file
embedded int[ MapTilesY ][ MapTilesX ] MapTest = "obj/MapTest.vmap";

// all collision areas in our map
collisionbox[ 6 ] BlockedAreas =
{
    { 200, 210, 0, 0, 120, 150, true  },   // fences at the left
    { 450, 286, 0, 0,  56,  44, true  },   // tree base
    { 440, 330, 0, 0, 120, 110, true  },   // fences below tree base
    { 640,  10, 0, 0, 320, 190, true  },   // above the entrance
    { 640, 290, 0, 0, 320, 190, true  },   // below the entrance
    { 653, 200, 0, 0,  80,  90, true  }    // inside the entrance
};

// all event trigger areas in our map
collisionbox[ 1 ] EventAreas = 
{
    { 647, 200, 0, 0,  20,  90, false }    // dialog event trigger
};
