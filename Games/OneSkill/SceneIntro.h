// ---------------------------------------------------------
//   INTRO: SUPPORT DEFINITIONS
// ---------------------------------------------------------


int[ 2 ][ 100 ] Intro_SlideTexts =
{
    "NICO WAS ABLE TO LEARN 4 DIFFERENT SKILLS, BUT\n"
    "NEVER COULD USE MORE THAN ONE SKILL AT A TIME.",

    "BUT HE DISCOVERED A METHOD TO CHANGE SKILLS\n"
    "AND STARTED AN ADVENTURE TO TRAIN HIMSELF..."
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
int Intro_TextNumber;
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
    select_texture( TextureCutscenes );
    select_region( RegionIntroSlide );
    draw_region_at( 121,57 );
    
    // write the current text
    textfont_print_centered( &Font11x16, screen_width/2, 295, Intro_ShownText );
}


// ---------------------------------------------------------
//    INTRO: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Intro_RunState_Initialize()
{
    // stop music and play story sound
    stop_channel( ChannelMusic );
    play_sound_in_channel( MusicCutscenes, ChannelMusic );
    
    // transition immediately
    Intro_TextNumber = 0;
    Intro_ChangeState( Story_WriteText );
}

// ---------------------------------------------------------

void Intro_RunState_WriteText()
{
    // allow pressing a button to skip text writing
    if( AnyButtonPressed() )
      strcpy( Intro_ShownText, Intro_SlideTexts[ Intro_TextNumber ] );
    
    int CurrentCharacters = strlen( Intro_ShownText );
    int TotalCharacters = strlen( Intro_SlideTexts[ Intro_TextNumber ] );
    
    // keep adding characters
    if( Intro_ElapsedFrames % 5 == 0 )
      if( CurrentCharacters < TotalCharacters )
        {
            Intro_ShownText[ CurrentCharacters ] = Intro_SlideTexts[ Intro_TextNumber ][ CurrentCharacters ];
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
        Intro_TextNumber++;
        strcpy( Intro_ShownText, "" );
        
        if( Intro_TextNumber >= 2 )
        {
            // protect slide during fadeout
            Intro_TextNumber = 1;
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
        GameScene = Scene_Tutorial;
        GameState = Tutorial_Initialize;
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
