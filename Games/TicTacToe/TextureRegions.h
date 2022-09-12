void DefineTextureRegions()
{
    // Title Screen Regions
    select_texture(TextureTicTacToe);
    select_region(TicTacToeLogo);
    define_region_topleft(202,0,615,283);
    select_region(AuthorLogo);
    define_region_topleft(202,285,439,302);
    select_region(PressStart);
    define_region_topleft(441,285,570,300);

    // Mark Regions
    select_region(CrossMark);
    define_region_topleft(0,0,99,99);
    select_region(CircleMark);
    define_region_topleft(101,0,199,99);
    select_region(TileMark);
    define_region_topleft(0,101,109,210);
    
    // Player 1 Cursor Regions
    select_region(P1TopLeftCursor);
    define_region_topleft(111,101,125,115);
    select_region(P1TopRightCursor);
    define_region_topleft(127,101,141,115);
    select_region(P1BottomLeftCursor);
    define_region_topleft(159,101,173,115);
    select_region(P1BottomRightCursor);
    define_region_topleft(143,101,157,115);

    // Player 2 Cursor Regions
    select_region(P2TopLeftCursor);
    define_region_topleft(111,117,125,131);
    select_region(P2TopRightCursor);
    define_region_topleft(127,117,141,131);
    select_region(P2BottomLeftCursor);
    define_region_topleft(159,117,173,131);
    select_region(P2BottomRightCursor);
    define_region_topleft(143,117,157,131);

    // Score Background Regions
    select_region(ScoreBackground);
    define_region_topleft(0,304,213,460);
    select_region(CrossScoreIndicator);
    define_region_topleft(111,133,150,172);
    select_region(CircleScoreIndicator);
    define_region_topleft(152,133,191,172);

    // Number Regions
    select_region(N0);
    define_region_topleft(  0,212, 17,235);
    select_region(N1);
    define_region_topleft( 19,212, 36,235);
    select_region(N2);
    define_region_topleft( 38,212, 55,235);
    select_region(N3);
    define_region_topleft( 57,212, 74,235);
    select_region(N4);
    define_region_topleft( 76,212, 93,235);
    select_region(N5);
    define_region_topleft( 95,212,112,235);
    select_region(N6);
    define_region_topleft(114,212,131,235);
    select_region(N7);
    define_region_topleft(133,212,150,235);
    select_region(N8);
    define_region_topleft(152,212,169,235);
    select_region(N9);
    define_region_topleft(171,212,188,235);

    // In Game Keys Information Background
    select_region(GameKeysInformation);
    define_region_topleft(215,304,384,376);
    
}