// ---------------------------------------------------------
//   ENDING: SUPPORT DEFINITIONS
// ---------------------------------------------------------


int[ 2 ][ 100 ] Ending_SlideTexts =
{
    "NICO COMPLETED HIS ADVENTURE! HE BECAME A MASTER\n"
    "USER OF HIS SKILLS AND OVERCAME ALL CHALLENGES.",
    
    "BUT HE WAS NEVER ABLE TO USE HIS SKILLS AT THE\n"
    "SAME TIME. HE DREAMS OF THE DAY WHEN HE CAN..."
};


// ---------------------------------------------------------
//   ENDING: STATE MANAGEMENT
// ---------------------------------------------------------


int Ending_ElapsedFrames;
int Ending_TextNumber;
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
    select_texture( TextureCutscenes );
    select_region( RegionEndingSlide );
    draw_region_at( 121,57 );
    
    // write the current text
    textfont_print_centered( &Font11x16, screen_width/2, 295, Ending_ShownText );
}


// ---------------------------------------------------------
//    ENDING: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Ending_RunState_Initialize()
{
    // stop music and play story sound
    stop_channel( ChannelMusic );
    play_sound_in_channel( MusicCutscenes, ChannelMusic );
    
    // transition immediately
    Ending_TextNumber = 0;
    Ending_ChangeState( Story_WriteText );
}

// ---------------------------------------------------------

void Ending_RunState_WriteText()
{
    // allow pressing a button to skip text writing
    if( AnyButtonPressed() )
      strcpy( Ending_ShownText, Ending_SlideTexts[ Ending_TextNumber ] );
    
    int CurrentCharacters = strlen( Ending_ShownText );
    int TotalCharacters = strlen( Ending_SlideTexts[ Ending_TextNumber ] );
    
    // keep adding characters
    if( Ending_ElapsedFrames % 5 == 0 )
      if( CurrentCharacters < TotalCharacters )
        {
            Ending_ShownText[ CurrentCharacters ] = Ending_SlideTexts[ Ending_TextNumber ][ CurrentCharacters ];
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
        Ending_TextNumber++;
        strcpy( Ending_ShownText, "" );
        
        if( Ending_TextNumber >= 2 )
        {
            // protect slide during fadeout
            Ending_TextNumber = 1;
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
    
    // when done, begin gameplay
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
    switch( GameState )
    {
        case Story_Initialize:
            Ending_RunState_Initialize();
            break;
        
        case Story_WriteText:
            Ending_RunState_WriteText();
            break;
        
        case Story_Waiting:
            Ending_RunState_Waiting();
            break;
        
        case Story_FadeOut:
            Ending_RunState_FadeOut();
            break;
    }
    
    // count the frames in current state
    Ending_ElapsedFrames++;
}
