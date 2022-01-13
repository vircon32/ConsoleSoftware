// remaining frames to highlight some situation
int DirectionHighlightFrames = 0;
int TileHighlightFrames = 0;
int ScoreHighlightFrames = 0;


// ---------------------------------------------------------
//   GENERAL DRAWING FUNCTIONS
// ---------------------------------------------------------


void DrawBackground()
{
    // background scroll is synchronized with played frames
    int DrawX = PlayedFrames % 640;
    
    // either draw the full background...
    if( !DrawX )
    {
        select_region( RegionBackground );
        draw_region_at( 0,0 );
    }
    
    // ... or split in 2 regions when needed
    else
    {
        select_region( RegionBGLeft );
        define_region_topleft( DrawX,0,  639,359 );
        draw_region_at( 0,0 );
        
        select_region( RegionBGRight );
        define_region_topleft( 0,0,  DrawX-1,359 );
        draw_region_at( 639-DrawX+1,0 );
    }
}

// ---------------------------------------------------------

void DrawGUI()
{
    // draw the GUI area
    select_region( RegionGUI );
    draw_region_at( 639,0 );
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // limit the displayed time to 9:59:59:99
    int RemainingFrames = PlayedFrames;
    RemainingFrames = min( RemainingFrames, (9*216000 + 59*3600 + 59*60 + 59) );
    
    // split played time in: hours, minutes, seconds, hundreds
    int Hours = RemainingFrames / 216000;
    RemainingFrames -= Hours * 216000;
    
    int Minutes = RemainingFrames / 3600;
    RemainingFrames -= Minutes * 3600;
    
    int Seconds = RemainingFrames / 60;
    RemainingFrames -= Seconds * 60;
    
    int Hundreds = (100 * RemainingFrames) / 60;
    
    // write current time: hours
    select_region( RegionSecondsZero + Hours % 10 );
    draw_region_at( 442,38 );
    
    // write current time: minutes
    select_region( RegionSecondsZero + Minutes / 10 );
    draw_region_at( 482,38 );
    
    select_region( RegionSecondsZero + Minutes % 10 );
    draw_region_at( 504,38 );
    
    // write current time: seconds
    select_region( RegionSecondsZero + Seconds / 10 );
    draw_region_at( 544,38 );
    
    select_region( RegionSecondsZero + Seconds % 10 );
    draw_region_at( 566,38 );
    
    // write current time: hundredths
    select_region( RegionHundredsZero + Hundreds / 10 );
    draw_region_at( 601,54 );
    
    select_region( RegionHundredsZero + Hundreds % 10 );
    draw_region_at( 613,54 );
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // write score (6 digits, green)
    int Base = 100000;
    int DrawX = 482;
    int FullColor = make_color_rgba( 0,220,50,255 );
    int GrayedColor = make_color_rgba( 0,220,50,128 );
    
    while( Base > 0 )
    {
        int Digit = (Score/Base) % 10;
        bool GrayOut = (Score < Base && Digit == 0);
        
        if( GrayOut )
          set_multiply_color( GrayedColor );
        else
          set_multiply_color( FullColor );
        
        select_region( RegionScoreZero + Digit );
        draw_region_at( DrawX,143 );
        
        Base /= 10;
        DrawX += 17;
    }
    
    set_multiply_color( color_white );
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // write number of moves (6 digits, white)
    Base = 100000;
    DrawX = 482;
    FullColor = color_white;
    GrayedColor = make_color_rgba( 255,255,255,128 );
    
    while( Base > 0 )
    {
        int Digit = (Moves/Base) % 10;
        bool GrayOut = (Moves < Base && Digit == 0);
        
        if( GrayOut )
          set_multiply_color( GrayedColor );
        else
          set_multiply_color( FullColor );
        
        select_region( RegionScoreZero + Digit );
        draw_region_at( DrawX,246 );
        
        Base /= 10;
        DrawX += 17;
    }
    
    set_multiply_color( color_white );
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // write the points just scored, and make them blink
    if( ScoreHighlightFrames > 0 && (ScoreHighlightFrames & 4) )
      if( LastScoreIncrease > 0 )
        {
            set_multiply_color( color_cyan );
            
            // write the number from right to left
            // (i.e. variable number of digits)
            Base = 1;
            DrawX = 572;
            
            do
            {
                int Digit = (LastScoreIncrease/Base) % 10;
                select_region( RegionHundredsZero + Digit );
                draw_region_at( DrawX,177 );
                
                Base *= 10;
                DrawX -= 12;
            }
            while( Base <= LastScoreIncrease );
            
            // add a plus sign before the number
            select_region( RegionPointsPlus );
            draw_region_at( DrawX-3,177 );
            
            set_multiply_color( color_white );
        }
}

// ---------------------------------------------------------

void HighlightLastDirection()
{
    if( DirectionHighlightFrames <= 0 )
      return;
    
    // choose direction
    if( MoveX < 0 )
    {
        set_multiply_color( make_color_rgba( 255,255,255,80+10*DirectionHighlightFrames ) );
        select_region( RegionIndicatorV );
        draw_region_at( 64-0,180 );
    }
    
    else if( MoveX > 0 )
    {
        set_multiply_color( make_color_rgba( 255,255,255,50+10*DirectionHighlightFrames ) );
        select_region( RegionIndicatorV );
        set_drawing_scale( -1, 1 );
        draw_region_zoomed_at( 362+0,180 );
    }
    
    else if( MoveY < 0 )
    {
        set_multiply_color( make_color_rgba( 255,255,255,80+10*DirectionHighlightFrames ) );
        select_region( RegionIndicatorH );
        draw_region_at( 213,31-0 );
    }
    
    else if( MoveY > 0 )
    {
        set_multiply_color( make_color_rgba( 255,255,255,50+10*DirectionHighlightFrames ) );
        select_region( RegionIndicatorH );
        set_drawing_scale( 1, -1 );
        draw_region_zoomed_at( 213,329+0 );
    }
    
    set_multiply_color( color_white );
}

// ---------------------------------------------------------

void DrawBoard()
{
    // draw the board itself
    select_region( RegionBoard );
    draw_region_at( 51, 18 );
    
    // draw every tile
    for( int y = 0; y < 4; y++ )
      for( int x = 0; x < 4; x++ )
        {
            if( CurrentBoard[y][x] <= 0 )
              continue;
            
            select_region( RegionTile2 - 1 + CurrentBoard[ y ][ x ] );
            draw_region_at( 100 + 75*x, 67 + 75*y );
        }
    
    // draw direction indication
    HighlightLastDirection();
    
    // highlight tiles that just merged
    if( TileHighlightFrames > 0 )
    {
        set_blending_mode( blending_add );
        select_region( RegionTileHighlight );
        
        for( int y = 0; y < 4; y++ )
          for( int x = 0; x < 4; x++ )
            if( MergedTiles[ y ][ x ] > 0 )
              draw_region_at( 100 + 75*x, 67 + 75*y );
              
        set_blending_mode( blending_alpha );
    }
}


// ---------------------------------------------------------
//   DRAWING TRANSITIONS
// ---------------------------------------------------------


void DrawTransition( TileTransition* Transition )
{
    // do not draw empty tiles
    if( Transition->Value <= 0 )
      return;
    
    // set opacity
    set_multiply_color( make_color_rgba( 255,255,255, Transition->Opacity ) );
    
    // draw tile
    select_region( RegionTile2 - 1 + Transition->Value );
    draw_region_at( Transition->PositionX, Transition->PositionY );
    
    // revert to full opacity
    set_multiply_color( color_white );
}

// ---------------------------------------------------------

void DrawTransitionBoard()
{
    // draw the board itself
    select_region( RegionBoard );
    draw_region_at( 51, 18 );
    
    // draw every transition
    for( int y = 0; y < 4; y++ )
      for( int x = 0; x < 4; x++ )
        DrawTransition( &TransitionBoard[ y ][ x ] );
    
    // draw direction indication
    HighlightLastDirection();
}
