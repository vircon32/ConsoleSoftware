// ---------------------------------------------------------
//   CREDITS: STATE MANAGEMENT
// ---------------------------------------------------------


int Credits_ElapsedFrames;

// ---------------------------------------------------------

void Credits_ChangeState( int NewState )
{
    GameState = NewState;
    Credits_ElapsedFrames = 0;
}


// ---------------------------------------------------------
//    CREDITS: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


// save text for total time
int[ 15 ] TimeText;

// ---------------------------------------------------------

void Credits_RunState_Initialize()
{
    // stop music and play clear sound
    stop_channel( ChannelMusic );
    play_sound( MusicCredits );
    
    // calculate total game time
    int TotalFrames = get_frame_counter() - GameStartFrames;
    
    // decompose game time in hours, minutes, seconds and hundredths
    int Hours = TotalFrames / (3600 * frames_per_second);
    TotalFrames -= Hours * (3600 * frames_per_second);
    
    int Minutes = TotalFrames / (60 * frames_per_second);
    TotalFrames -= Minutes * (60 * frames_per_second);
    
    int Seconds = TotalFrames / frames_per_second;
    TotalFrames -= Seconds * frames_per_second;
    
    int Hundredths = (100 * TotalFrames) / frames_per_second;
    
    // format our decomposed time into text
    int[ 3 ] NumberString;
    strcpy( TimeText, "" );
    
    if( Hours > 0 )
    {
        itoa( Hours, NumberString, 10 );
        strcat( TimeText, NumberString );
        strcat( TimeText, " H, " );
    }
    
    itoa( Minutes, NumberString, 10 );
    strcat( TimeText, NumberString );
    strcat( TimeText, " M, " );
    
    itoa( Seconds, NumberString, 10 );
    strcat( TimeText, NumberString );
    strcat( TimeText, "." );
    
    itoa( Hundredths, NumberString, 10 );
    strcat( TimeText, NumberString );
    strcat( TimeText, " S" );
    
    // transition immediately
    Credits_ChangeState( Credits_TextRoll );
}

// ---------------------------------------------------------

void Credits_RunState_TextRoll()
{
    // solid black background
    clear_screen( color_black );
    
    // determine base text position
    int FirstTextY = screen_height + 100 - Credits_ElapsedFrames * 1.5;
    
    // draw titles in red
    set_multiply_color( color_red );
    DrawTextFromLeft( 130, FirstTextY, "BASIC PLATFORMER" );
    DrawTextFromLeft( 130, FirstTextY+40, "CREDITS" );
    
    // draw sections in yellow
    set_multiply_color( color_yellow );
    DrawTextFromLeft( 130, FirstTextY+110*1, "PROGRAMMING:" );
    DrawTextFromLeft( 130, FirstTextY+110*2, "SOUND FX:" );
    DrawTextFromLeft( 130, FirstTextY+110*3, "MUSIC:" );
    DrawTextFromLeft( 130, FirstTextY+110*4, "GRAPHICS:" );
    DrawTextFromLeft( 130, FirstTextY+110*5, "YOUR TIME:" );
    
    // draw names in white
    set_multiply_color( color_white );
    DrawTextFromLeft( 130, FirstTextY+110*1+40, "CARRA" );
    DrawTextFromLeft( 130, FirstTextY+110*2+40, "CARRA" );
    DrawTextFromLeft( 130, FirstTextY+110*3+40, "FRAYNVGM" );
    DrawTextFromLeft( 130, FirstTextY+110*4+40, "CARRA" );
    DrawTextFromLeft( 130, FirstTextY+110*5+40, TimeText );
    
    // when done, go to ending scene
    int LastTextY = FirstTextY + 110*5 + 40;
    
    if( LastTextY <= -100 )
    {
        select_texture( TextureGame );
        GameScene = Scene_Ending;
        Gameplay_ChangeState( Ending_Initialize );
    }
}


// ---------------------------------------------------------
//   CREDITS: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Credits_RunStateMachine()
{
    if( GameState == Credits_Initialize )
      Credits_RunState_Initialize();
    
    else if( GameState == Credits_TextRoll )
      Credits_RunState_TextRoll();
    
    // count the frames in current state
    Credits_ElapsedFrames++;
}
