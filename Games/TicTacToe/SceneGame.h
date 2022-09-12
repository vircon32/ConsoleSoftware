int CheckValidTile(){
    for (int i = 0; i<3; i++)
        for (int j = 0; j<3; j++)
            if (XCursorPosition==i & YCursorPosition==j & Tile[i+3*j]!=0) return 0;
    return 1;
}

void PrintGameBackground() {
    clear_screen(color_default);
    for (int i = 0; i<9; i++) {
        select_region(TileMark);
        draw_region_at(XTileCoord[i],YTileCoord[i]);    
    }
    select_region(ScoreBackground);
    draw_region_at(400,32);
    select_region(CrossScoreIndicator);
    draw_region_at(424,57);
    select_region(CircleScoreIndicator);
    draw_region_at(424,120);

    select_region(GameKeysInformation);
    draw_region_at(424,236);
}

void PrintScore() {
    // Player 1
    int Base = 100000;
    int DrawX = P1Score_X;
    while( Base > 0 ) {
        int Digit = (P1Score/Base) % 10;
        
        select_region( N0 + Digit );
        draw_region_at( DrawX,P1Score_Y );

        Base /= 10;
        DrawX += 17;
    }
    // Player 2
    Base = 100000;
    DrawX = P2Score_X;
    while( Base > 0 ) {
        int Digit = (P2Score/Base) % 10;
        
        select_region( N0 + Digit );
        draw_region_at( DrawX,P2Score_Y );

        Base /= 10;
        DrawX += 17;
    }
}

void PrintMarkedTiles(){
    for (int i = 0; i<9; i++) {
        if (Tile[i] == 1) select_region(CrossMark ); else
        if (Tile[i] == 2) select_region(CircleMark);
        if (Tile[i] != 0) draw_region_at(XTileCoord[i],YTileCoord[i]);
    }
}

void PrintCursor() {
    if (XCursorPosition==0 & YCursorPosition==0) {
        if (CurrentPlayer==1) select_region(P1TopLeftCursor); else select_region(P2TopLeftCursor);
        draw_region_at(X11-6,Y11-6);
        if (CurrentPlayer==1) select_region(P1TopRightCursor); else select_region(P2TopRightCursor);
        draw_region_at(X11+91,Y11-6);
        if (CurrentPlayer==1) select_region(P1BottomLeftCursor); else select_region(P2BottomLeftCursor);
        draw_region_at(X11-6,Y11+91);
        if (CurrentPlayer==1) select_region(P1BottomRightCursor); else select_region(P2BottomRightCursor);
        draw_region_at(X11+91,Y11+91);
    } else
    if (XCursorPosition==1 & YCursorPosition==0) {
        if (CurrentPlayer==1) select_region(P1TopLeftCursor); else select_region(P2TopLeftCursor);
        draw_region_at(X12-6,Y12-6);
        if (CurrentPlayer==1) select_region(P1TopRightCursor); else select_region(P2TopRightCursor);
        draw_region_at(X12+91,Y12-6);
        if (CurrentPlayer==1) select_region(P1BottomLeftCursor); else select_region(P2BottomLeftCursor);
        draw_region_at(X12-6,Y12+91);
        if (CurrentPlayer==1) select_region(P1BottomRightCursor); else select_region(P2BottomRightCursor);
        draw_region_at(X12+91,Y12+91);
    } else
    if (XCursorPosition==2 & YCursorPosition==0) {
        if (CurrentPlayer==1) select_region(P1TopLeftCursor); else select_region(P2TopLeftCursor);
        draw_region_at(X13-6,Y13-6);
        if (CurrentPlayer==1) select_region(P1TopRightCursor); else select_region(P2TopRightCursor);
        draw_region_at(X13+91,Y13-6);
        if (CurrentPlayer==1) select_region(P1BottomLeftCursor); else select_region(P2BottomLeftCursor);
        draw_region_at(X13-6,Y13+91);
        if (CurrentPlayer==1) select_region(P1BottomRightCursor); else select_region(P2BottomRightCursor);
        draw_region_at(X13+91,Y13+91);
    } else
    if (XCursorPosition==0 & YCursorPosition==1) {
        if (CurrentPlayer==1) select_region(P1TopLeftCursor); else select_region(P2TopLeftCursor);
        draw_region_at(X21-6,Y21-6);
        if (CurrentPlayer==1) select_region(P1TopRightCursor); else select_region(P2TopRightCursor);
        draw_region_at(X21+91,Y21-6);
        if (CurrentPlayer==1) select_region(P1BottomLeftCursor); else select_region(P2BottomLeftCursor);
        draw_region_at(X21-6,Y21+91);
        if (CurrentPlayer==1) select_region(P1BottomRightCursor); else select_region(P2BottomRightCursor);
        draw_region_at(X21+91,Y21+91);
    } else
    if (XCursorPosition==1 & YCursorPosition==1) {
        if (CurrentPlayer==1) select_region(P1TopLeftCursor); else select_region(P2TopLeftCursor);
        draw_region_at(X22-6,Y22-6);
        if (CurrentPlayer==1) select_region(P1TopRightCursor); else select_region(P2TopRightCursor);
        draw_region_at(X22+91,Y22-6);
        if (CurrentPlayer==1) select_region(P1BottomLeftCursor); else select_region(P2BottomLeftCursor);
        draw_region_at(X22-6,Y22+91);
        if (CurrentPlayer==1) select_region(P1BottomRightCursor); else select_region(P2BottomRightCursor);
        draw_region_at(X22+91,Y22+91);
    } else
    if (XCursorPosition==2 & YCursorPosition==1) {
        if (CurrentPlayer==1) select_region(P1TopLeftCursor); else select_region(P2TopLeftCursor);
        draw_region_at(X23-6,Y23-6);
        if (CurrentPlayer==1) select_region(P1TopRightCursor); else select_region(P2TopRightCursor);
        draw_region_at(X23+91,Y23-6);
        if (CurrentPlayer==1) select_region(P1BottomLeftCursor); else select_region(P2BottomLeftCursor);
        draw_region_at(X23-6,Y23+91);
        if (CurrentPlayer==1) select_region(P1BottomRightCursor); else select_region(P2BottomRightCursor);
        draw_region_at(X23+91,Y23+91);
    } else
    if (XCursorPosition==0 & YCursorPosition==2) {
        if (CurrentPlayer==1) select_region(P1TopLeftCursor); else select_region(P2TopLeftCursor);
        draw_region_at(X31-6,Y31-6);
        if (CurrentPlayer==1) select_region(P1TopRightCursor); else select_region(P2TopRightCursor);
        draw_region_at(X31+91,Y31-6);
        if (CurrentPlayer==1) select_region(P1BottomLeftCursor); else select_region(P2BottomLeftCursor);
        draw_region_at(X31-6,Y31+91);
        if (CurrentPlayer==1) select_region(P1BottomRightCursor); else select_region(P2BottomRightCursor);
        draw_region_at(X31+91,Y31+91);
    } else
    if (XCursorPosition==1 & YCursorPosition==2) {
        if (CurrentPlayer==1) select_region(P1TopLeftCursor); else select_region(P2TopLeftCursor);
        draw_region_at(X32-6,Y32-6);
        if (CurrentPlayer==1) select_region(P1TopRightCursor); else select_region(P2TopRightCursor);
        draw_region_at(X32+91,Y32-6);
        if (CurrentPlayer==1) select_region(P1BottomLeftCursor); else select_region(P2BottomLeftCursor);
        draw_region_at(X32-6,Y32+91);
        if (CurrentPlayer==1) select_region(P1BottomRightCursor); else select_region(P2BottomRightCursor);
        draw_region_at(X32+91,Y32+91);
    } else
    if (XCursorPosition==2 & YCursorPosition==2) {
        if (CurrentPlayer==1) select_region(P1TopLeftCursor); else select_region(P2TopLeftCursor);
        draw_region_at(X33-6,Y33-6);
        if (CurrentPlayer==1) select_region(P1TopRightCursor); else select_region(P2TopRightCursor);
        draw_region_at(X33+91,Y33-6);
        if (CurrentPlayer==1) select_region(P1BottomLeftCursor); else select_region(P2BottomLeftCursor);
        draw_region_at(X33-6,Y33+91);
        if (CurrentPlayer==1) select_region(P1BottomRightCursor); else select_region(P2BottomRightCursor);
        draw_region_at(X33+91,Y33+91);
    }
}

void UpdateStateMachine() {
    select_gamepad( 0 );

    // move current player cursor (not yet implemented with a second gamepad)
    if( gamepad_right() == 1 & XCursorPosition<=1 ) XCursorPosition++;
    if( gamepad_left()  == 1 & XCursorPosition>=1 ) XCursorPosition--;
    if( gamepad_up()    == 1 & YCursorPosition>=1 ) YCursorPosition--;
    if( gamepad_down()  == 1 & YCursorPosition<=1 ) YCursorPosition++;
    
    // Player Selected a Tile
    if( gamepad_button_a() == 1 ) {
        // Check if the player can choose the selected tile or not
        if ( CheckValidTile() ) {
            // Mark Current Player VALID selected tile
            if ( CurrentPlayer == 1 ) Tile[XCursorPosition+3*YCursorPosition]=1;
            if ( CurrentPlayer == 2 ) Tile[XCursorPosition+3*YCursorPosition]=2;
        }
        // Exchanging Players (not yet implemented with a second gamepad)
        if ( CurrentPlayer == 1 ) CurrentPlayer=2; else CurrentPlayer=1;
    }

    // Player wants to reset match
    if( gamepad_button_b() == 1 ) {
        // Reset Points
        P1Score=0;
        P2Score=0;
        // Reset Tiles
        for (int i = 0; i<9; i++) Tile[i]=0;
        // Go Back to Title Screen
        GameState = Title_Initialize;
    }
    
}

void Game_RunStateMachine() {

    PrintGameBackground();

    PrintCursor();

    PrintMarkedTiles();

    PrintScore();

    UpdateStateMachine();

    // Go Back to Title if Player pressed B
    if (GameState==Title_Initialize) return;

    int winner = CheckWinCondition();
    if (winner != 0) {
        // Add a point to the winner's score
        if (winner==1) P1Score++;
        if (winner==2) P2Score++;
        // Reset Tiles
        for (int i = 0; i<9; i++) Tile[i]=0;
    };

    ElapsedFrames++;
}

