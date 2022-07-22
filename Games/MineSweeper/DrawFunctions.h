// ---------------------------------------------------------
//   TILE SIZE DEFINITIONS
// ---------------------------------------------------------


#define SmallTileSize 19
#define SmallTileSeparation 1
#define SmallTileStep (SmallTileSize + SmallTileSeparation)

#define LargeTileSize 33
#define LargeTileSeparation 2
#define LargeTileStep (LargeTileSize + LargeTileSeparation)


// ---------------------------------------------------------
//   DRAWING SINGLE TILES
// ---------------------------------------------------------


void Tile_DrawLarge( Tile* T, int TopLeftX, int TopLeftY )
{
    // CASE 1: covered tile
    if( !T->Revealed )
    {
        // draw tile
        select_region( FirstRegionTilesCoveredLarge + T->GraphicTile );
        draw_region_at( TopLeftX, TopLeftY );
        
        // draw flag
        if( T->Flagged )
        {
            select_region( FirstRegionIconsLarge );
            draw_region_at( TopLeftX, TopLeftY );
        }
    }
    
    // CASE 2: revealed tile
    else
    {
        // draw tile
        select_region( FirstRegionTilesRevealedLarge + T->GraphicTile );
        draw_region_at( TopLeftX, TopLeftY );
        
        // draw mine state icon
        if( T->MineState > 0 )
        {
            select_region( FirstRegionIconsLarge + T->MineState );
            draw_region_at( TopLeftX, TopLeftY );
        }
    }
}

// ---------------------------------------------------------

void Tile_DrawSmall( Tile* T, int TopLeftX, int TopLeftY )
{
    // CASE 1: covered tile
    if( !T->Revealed )
    {
        // draw tile
        select_region( FirstRegionTilesCoveredSmall + T->GraphicTile );
        draw_region_at( TopLeftX, TopLeftY );
        
        // draw flag
        if( T->Flagged )
        {
            select_region( FirstRegionIconsSmall );
            draw_region_at( TopLeftX, TopLeftY );
        }
    }
    
    // CASE 2: revealed tile
    else
    {
        // draw tile
        select_region( FirstRegionTilesRevealedSmall + T->GraphicTile );
        draw_region_at( TopLeftX, TopLeftY );
        
        // draw mine state icon
        if( T->MineState > 0 )
        {
            select_region( FirstRegionIconsSmall + T->MineState );
            draw_region_at( TopLeftX, TopLeftY );
        }
    }
}


// ---------------------------------------------------------
//   DRAWING FULL BOARDS
// ---------------------------------------------------------


void Board_DrawLarge( Board* B, int TopLeftX, int TopLeftY )
{
    for( int y = 0; y < B->Height; y++ )
      for( int x = 0; x < B->Width; x++ )
        Tile_DrawLarge( &B->Tiles[ y ][ x ], TopLeftX + LargeTileStep*x, TopLeftY + LargeTileStep*y );
}

// ---------------------------------------------------------

void Board_DrawSmall( Board* B, int TopLeftX, int TopLeftY )
{
    for( int y = 0; y < B->Height; y++ )
      for( int x = 0; x < B->Width; x++ )
        Tile_DrawSmall( &B->Tiles[ y ][ x ], TopLeftX + SmallTileStep*x, TopLeftY + SmallTileStep*y );
}


// ---------------------------------------------------------
//   DRAWING THE SCOREBOARD
// ---------------------------------------------------------


void Write2DigitsNumber( int Number, int TopLeftX, int TopLeftY )
{
    select_region( FirstRegionNumbersDigital + Number / 10 );
    draw_region_at( TopLeftX, TopLeftY );
    
    select_region( FirstRegionNumbersDigital + Number % 10 );
    draw_region_at( TopLeftX + 16, TopLeftY );
}

// ---------------------------------------------------------

void Write3DigitsNumber( int Number, int TopLeftX, int TopLeftY )
{
    select_region( FirstRegionNumbersDigital + Number / 100 );
    draw_region_at( TopLeftX, TopLeftY );
    
    select_region( FirstRegionNumbersDigital + (Number/10) % 10 );
    draw_region_at( TopLeftX + 16, TopLeftY );
    
    select_region( FirstRegionNumbersDigital + Number % 10 );
    draw_region_at( TopLeftX + 32, TopLeftY );
}

// ---------------------------------------------------------

// regions for the different smiley expressions
enum SmileysRegions
{
    SmileyNeutral = FirstRegionSmileys,
    SmileyInteresed,
    SmileyHappy,
    SmileyWinning,
    SmileyLosing
};

// ---------------------------------------------------------

void DrawSmiley()
{
    // draw the face with the correct expression
    if( GameScene == Scene_Lose )
      select_region( SmileyLosing );
    
    else if( GameScene == Scene_Win )
      select_region( SmileyWinning );
    
    else if( GameScene == Scene_QuitGame )
      select_region( SmileyInteresed );
    
    // otherwise we are in gameplay scenes
    else switch( GameState )
    {
        case Gameplay_AddingFlag:
        case Gameplay_RemovingFlag:
        case Gameplay_RevealTile:
        case Gameplay_RevealMines:
        case Gameplay_Explosion:
            select_region( SmileyInteresed );
            break;
            
        case Gameplay_PropagateReveal:
            select_region( SmileyHappy );
            break;
            
        default:
            select_region( SmileyNeutral );
            break;
    }
    
    draw_region_at( 318,4 );
}

// ---------------------------------------------------------

void DrawUpperBoard()
{
    // draw the board background
    select_region( RegionUpperBoard );
    draw_region_at( 0,0 );
    
    // show the current difficulty mode
    select_region( FirstRegionModeIndicators + GameMode );
    draw_region_at( 8,11 );
    
    // draw the face with the correct expression
    DrawSmiley();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // limit the displayed time to 99:59:99
    int RemainingFrames = PlayedFrames;
    RemainingFrames = min( RemainingFrames, (99*3600 + 59*60 + 59) );
    
    // split played time in: minutes, seconds, hundredths
    int Minutes = RemainingFrames / 3600;
    RemainingFrames -= Minutes * 3600;
    
    int Seconds = RemainingFrames / 60;
    RemainingFrames -= Seconds * 60;
    
    int Hundredths = (100 * RemainingFrames) / 60;
    
    // write current time: minutes
    Write2DigitsNumber( Minutes, 402, 7 );
    Write2DigitsNumber( Seconds, 443, 7 );
    Write2DigitsNumber( Hundredths, 482, 7 );
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // write number of mines and flags
    Write3DigitsNumber( GameBoard.NumberOfMines, 159, 7 );
    Write3DigitsNumber( GameBoard.NumberOfFlags, 246, 7 );
}

