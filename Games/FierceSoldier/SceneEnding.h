// ---------------------------------------------------------
//   ENDING: SUPPORT DEFINITIONS
// ---------------------------------------------------------


int[ 3 ][ 100 ] Ending_SlideTexts =
{
    "SO, YOU MANAGED TO DESTROY ALL THOSE GUYS\n"
    "WITHOUT RUNNING OUT OF BULLETS SOMEHOW",

    "WE DON'T REALLY KNOW IF THEY HAD ANY EVIL\n"
    "PLANS BUT YOU TAUGHT THEM A LESSON. GOOD!",

    "THIS SMALL GAME WAS MADE FOR THE 1ST JAM\n"
    "OF VIRCON32. SHOUTOUTS TO MEDIAVIDA GAMEDEV!"
};


// ---------------------------------------------------------
//   ENDING: STATE MANAGEMENT
// ---------------------------------------------------------


int Ending_ElapsedFrames;
int Ending_SlideNumber;
int[ 100 ] Ending_ShownText;

// ---------------------------------------------------------

void Ending_ChangeState( int NewState )
{
    GameState = NewState;
    Ending_ElapsedFrames = 0;
}


// ---------------------------------------------------------
//   ENDING: DRAW FUNCTIONS
// ---------------------------------------------------------


void Ending_DrawScene()
{
    // solid background
    clear_screen( color_black );
    
    // draw current slide with its frame
    select_texture( TextureStory );
    select_region( RegionSlideFrame );
    draw_region_at( 121,57 );
    
    select_region( FirstRegionEndingSlides + Ending_SlideNumber );
    draw_region_at( 121,57 );
    
    // write the current text
    FontSmall_PrintTextCentered( 320,295, Ending_ShownText );
}


// ---------------------------------------------------------
//    ENDING: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Ending_RunState_Initialize()
{
    // stop music and play story sound
    stop_channel( ChannelMusic );
    play_sound_in_channel( MusicStory, ChannelMusic );
    
    // transition immediately
    Ending_SlideNumber = 0;
    Ending_ChangeState( Story_WriteText );
}

// ---------------------------------------------------------

void Ending_RunState_WriteText()
{
    // allow pressing a button to skip text writing
    if( AnyButtonPressed() )
      strcpy( Ending_ShownText, Ending_SlideTexts[ Ending_SlideNumber ] );
    
    int CurrentCharacters = strlen( Ending_ShownText );
    int TotalCharacters = strlen( Ending_SlideTexts[ Ending_SlideNumber ] );
    
    // keep adding characters
    if( Ending_ElapsedFrames % 5 == 0 )
      if( CurrentCharacters < TotalCharacters )
        {
            Ending_ShownText[ CurrentCharacters ] = Ending_SlideTexts[ Ending_SlideNumber ][ CurrentCharacters ];
            Ending_ShownText[ CurrentCharacters + 1 ] = 0;
            
            // don't make print sound on spaces,
            // for a more realistic typewriter sound
            if( Ending_ShownText[ CurrentCharacters ] != ' ' )
              play_sound( SoundStoryLetter );
        }
    
    // draw the scene
    Ending_DrawScene();
    
    // transition when the text is complete
    if( CurrentCharacters >= TotalCharacters )
      Ending_ChangeState( Story_Waiting );
}

// ---------------------------------------------------------

void Ending_RunState_Waiting()
{
    // transition when any button is pressed,
    if( AnyButtonPressed() )
    {
        Ending_SlideNumber++;
        strcpy( Ending_ShownText, "" );
        
        if( Ending_SlideNumber >= 3 )
        {
            // protect slide during fadeout
            Ending_SlideNumber = 2;
            Ending_ChangeState( Story_FadeOut );
        }
        else
          Ending_ChangeState( Story_WriteText );
    }
}

// ---------------------------------------------------------

void Ending_RunState_FadeOut()
{
    // draw scene
    Ending_DrawScene();
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * Ending_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
    
    // when done, go back to title
    if( Ending_ElapsedFrames >= 60 )
    {
        stop_channel( ChannelMusic );
        GameScene = Scene_Title;
        GameState = Title_Initialize;
    }
}


// ---------------------------------------------------------
//   ENDING: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Ending_RunStateMachine()
{
    if( GameState == Story_Initialize )
      Ending_RunState_Initialize();
    
    else if( GameState == Story_WriteText )
      Ending_RunState_WriteText();
    
    else if( GameState == Story_Waiting )
      Ending_RunState_Waiting();
    
    else if( GameState == Story_FadeOut )
      Ending_RunState_FadeOut();
    
    // count the frames in current state
    Ending_ElapsedFrames++;
}
