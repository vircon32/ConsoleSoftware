// ---------------------------------------------------------
//   INTRO: SUPPORT DEFINITIONS
// ---------------------------------------------------------


int[ 3 ][ 100 ] Intro_SlideTexts =
{
    "YOUR JUNGLE WAS QUIET, BUT THEN ONE DAY\n"
    "SOME GUYS WITH MACHINES STARTED COMING TO IT",

    "THOSE PEOPLE HAD NOT REALLY DONE ANYTHING\n"
    "TO YOU BUT FOR SOME REASON YOU DISLIKE THEM",

    "I GUESS YOU SHOULD SHOOT AT ANYTHING THAT\n"
    "MOVES IN CASE THEY ARE EVIL OR SOMETHING..."
};

// ---------------------------------------------------------

bool AnyButtonPressed()
{
    select_gamepad( 0 );
    
    if( gamepad_button_a() == 1 ) return true;
    if( gamepad_button_b() == 1 ) return true;
    if( gamepad_button_x() == 1 ) return true;
    if( gamepad_button_y() == 1 ) return true;
    if( gamepad_button_l() == 1 ) return true;
    if( gamepad_button_r() == 1 ) return true;
    if( gamepad_button_start() == 1 ) return true;

    return false;
}


// ---------------------------------------------------------
//   INTRO: STATE MANAGEMENT
// ---------------------------------------------------------


int Intro_ElapsedFrames;
int Intro_SlideNumber;
int[ 100 ] Intro_ShownText;

// ---------------------------------------------------------

void Intro_ChangeState( int NewState )
{
    GameState = NewState;
    Intro_ElapsedFrames = 0;
}


// ---------------------------------------------------------
//   INTRO: DRAW FUNCTIONS
// ---------------------------------------------------------


void Intro_DrawScene()
{
    // solid background
    clear_screen( color_black );
    
    // draw current slide with its frame
    select_texture( TextureStory );
    select_region( RegionSlideFrame );
    draw_region_at( 121,57 );
    
    select_region( FirstRegionIntroSlides + Intro_SlideNumber );
    draw_region_at( 121,57 );
    
    // write the current text
    FontSmall_PrintTextCentered( 320,295, Intro_ShownText );
}


// ---------------------------------------------------------
//    INTRO: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Intro_RunState_Initialize()
{
    // stop music and play story sound
    stop_channel( ChannelMusic );
    play_sound_in_channel( MusicStory, ChannelMusic );
    
    // transition immediately
    Intro_SlideNumber = 0;
    Intro_ChangeState( Story_WriteText );
}

// ---------------------------------------------------------

void Intro_RunState_WriteText()
{
    // allow pressing a button to skip text writing
    if( AnyButtonPressed() )
      strcpy( Intro_ShownText, Intro_SlideTexts[ Intro_SlideNumber ] );
    
    int CurrentCharacters = strlen( Intro_ShownText );
    int TotalCharacters = strlen( Intro_SlideTexts[ Intro_SlideNumber ] );
    
    // keep adding characters
    if( Intro_ElapsedFrames % 5 == 0 )
      if( CurrentCharacters < TotalCharacters )
        {
            Intro_ShownText[ CurrentCharacters ] = Intro_SlideTexts[ Intro_SlideNumber ][ CurrentCharacters ];
            Intro_ShownText[ CurrentCharacters + 1 ] = 0;
            
            // don't make print sound on spaces,
            // for a more realistic typewriter sound
            if( Intro_ShownText[ CurrentCharacters ] != ' ' )
              play_sound( SoundStoryLetter );
        }
    
    // draw the scene
    Intro_DrawScene();
    
    // transition when the text is complete
    if( CurrentCharacters >= TotalCharacters )
      Intro_ChangeState( Story_Waiting );
}

// ---------------------------------------------------------

void Intro_RunState_Waiting()
{
    // transition when any button is pressed,
    if( AnyButtonPressed() )
    {
        Intro_SlideNumber++;
        strcpy( Intro_ShownText, "" );
        
        if( Intro_SlideNumber >= 3 )
        {
            // protect slide during fadeout
            Intro_SlideNumber = 2;
            Intro_ChangeState( Story_FadeOut );
        }
        else
          Intro_ChangeState( Story_WriteText );
    }
}

// ---------------------------------------------------------

void Intro_RunState_FadeOut()
{
    // draw scene
    Intro_DrawScene();
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * Intro_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
    
    // when done, begin gameplay
    if( Intro_ElapsedFrames >= 60 )
    {
        stop_channel( ChannelMusic );
        GameScene = Scene_Gameplay;
        GameState = Gameplay_Initialize;
    }
}


// ---------------------------------------------------------
//   INTRO: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Intro_RunStateMachine()
{
    if( GameState == Story_Initialize )
      Intro_RunState_Initialize();
    
    else if( GameState == Story_WriteText )
      Intro_RunState_WriteText();
    
    else if( GameState == Story_Waiting )
      Intro_RunState_Waiting();
    
    else if( GameState == Story_FadeOut )
      Intro_RunState_FadeOut();
    
    // count the frames in current state
    Intro_ElapsedFrames++;
}
