void Title_RunStateMachine() {

    clear_screen(color_default);
    select_region(TicTacToeLogo);
    draw_region_at(109,20);
    select_region(AuthorLogo);
    draw_region_at(16,324);

    if( ElapsedFrames%90<=45 ) {
        select_region(PressStart);
        draw_region_at(80,252);
    }
    ElapsedFrames++;
    
    // Start Pressing Verification on Joystick 1
    select_gamepad( 0 );

    if( gamepad_button_start() == 1 ) {
        // Change State Machine
        GameState = InGame;
        // Reset X Y Cursor
        XCursorPosition = 0;
        YCursorPosition = 0;
        for (int i = 0; i<9; i++) Tile[i]=0;
        CurrentPlayer = 1;
        return;
    }
}