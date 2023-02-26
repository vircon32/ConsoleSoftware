// *****************************************************************************
    // include standard Vircon headers
    #include "math.h"
    #include "time.h"
    #include "input.h"
    #include "video.h"
    
    // include additional libraries
    #include "tilemap.h"
    #include "textfont.h"
    #include "collisionbox.h"
    
    // include project headers
    #include "Globals.h"
    #include "TextureRPG.h"
    #include "Character.h"
    #include "Dialog.h"
    #include "Scene.h"
    #include "FontDialog.h"
// *****************************************************************************


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    DefineRegions_TextureRPG();
    FontDialog_DefineRegions();
    
    // ------------------------------------
    // PART 2: DEFINE MAP AND TILES
    // ------------------------------------
    
    // - - - - - - - - - - - - - - - - - - -
    // 2.1: define our tiles
    
    // define dimensions for our tiles
    RPGTiles.width  = 40;
    RPGTiles.height = 40;
    
    // adjacent tiles have neither space nor overlap
    RPGTiles.gap_x = 0;
    RPGTiles.gap_y = 0;
    
    // define texture and regions for our tiles
    RPGTiles.texture_id = TextureRPG;
    RPGTiles.tile_zero_region_id = FirstRegionTiles;
    
    // this particular tile set makes use of its
    // first tile (it's not discarded as transparent)
    RPGTiles.draw_tile_zero = true;
    
    // - - - - - - - - - - - - - - - - - - -
    // 2.2: define our map
    
    // assign our tiles to the map
    RPGMap.tiles = &RPGTiles;
    
    // define the map array
    RPGMap.map = &MapTest[ 0 ][ 0 ];
    RPGMap.map_width  = MapTilesX;
    RPGMap.map_height = MapTilesY;
    
    // - - - - - - - - - - - - - - - - - - -
    // 2.3: define our characters
    
    // define player
    strcpy( Player.Name, "James" );
    Player.TextureID = TextureRPG;
    Player.FirstRegionPortrait = FirstRegionPortraitPlayer;
    Player.FirstRegionWalkRight = FirstRegionPlayerWalkRight;
    Player.FirstRegionWalkUp = FirstRegionPlayerWalkUp;
    Player.FirstRegionWalkDown = FirstRegionPlayerWalkDown;
    
    // define peasant
    strcpy( Peasant.Name, "Peasant" );
    Peasant.TextureID = TextureRPG;
    Peasant.FirstRegionPortrait = FirstRegionPortraitPeasant;
    Peasant.FirstRegionWalkRight = FirstRegionPeasantWalkRight;
    Peasant.FirstRegionWalkUp = FirstRegionPeasantWalkUp;
    Peasant.FirstRegionWalkDown = FirstRegionPeasantWalkDown;
    
    // ------------------------------------
    // PART 2: DEFINE OUR DIALOG FONT
    // ------------------------------------
    
    // define texture and regions for our characters
    FontDialog.texture_id = TextureDialogFont;
    FontDialog.character_zero_region_id = 0;
    
    // define dimensions for our characters
    FontDialog.character_height = 21;
    FontDialog.use_variable_width = true;
    textfont_read_region_widths( &FontDialog );
    
    // adjust characters to make the font variable width
    FontDialog_ConfigureCharacterWidths();
    
    // no gap/overlap between characters, 10 pixels between lines
    FontDialog.character_separation = 0;
    FontDialog.line_separation = 6;
    
    // -----------------------------------------------
    // PART 3: INITIALIZATIONS
    // -----------------------------------------------
    
    // player collision box
    Player.ShapeBox.width = 25;
    Player.ShapeBox.height = 22;
    Player.ShapeBox.hotspot_x = 12;
    Player.ShapeBox.hotspot_y = 21;
    Player.ShapeBox.solid = true;
    
    // player initial state
    Player.ShapeBox.position_x = 380;
    Player.ShapeBox.position_y = 100;
    Player.FaceDirection = Down;
    
    // peasant collision box
    Peasant.ShapeBox.width = 25;
    Peasant.ShapeBox.height = 22;
    Peasant.ShapeBox.hotspot_x = 12;
    Peasant.ShapeBox.hotspot_y = 21;
    Peasant.ShapeBox.solid = true;
    
    // peasant initial state
    Peasant.ShapeBox.position_x = 780;
    Peasant.ShapeBox.position_y = 260;
    Peasant.FaceDirection = Left;
    
    // scene triggers
    PeasantHasSpoken = false;
    
    // we will control the character with gamepad 1
    select_gamepad( 0 );
    
    // ------------------------------------
    // PART 4: MAIN LOOP
    // ------------------------------------
    
    while( true )
    {
        // handle the scene
        Scene_Update();
        Scene_Draw();
        
        // wait until next frame
        end_frame();
    }
}
