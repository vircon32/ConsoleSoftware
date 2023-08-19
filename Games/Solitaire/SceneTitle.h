// ---------------------------------------------------------
//   TITLE: STATE MANAGEMENT
// ---------------------------------------------------------


int Title_ElapsedFrames;
int Title_DealtCards;

// cards being dealt at the center
Card[ 4 ] Title_Aces = 
{
    { Diamonds, 0 },
    { Clubs   , 0 },
    { Hearts  , 0 },
    { Spades  , 0 }
};

// cards shown at the corners
Card[ 4 ] Title_Figures = 
{
    { Hearts  , 11 },
    { Spades  , 12 },
    { Clubs   , 10 },
    { Diamonds, 10 }
};

// ---------------------------------------------------------

void Title_ChangeState( int NewState )
{
    GameState = NewState;
    Title_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( NewState == Title_StartPressed )
      play_sound( SoundTitleStart );
    
    else if( NewState == Title_Flash )
      play_sound( SoundTitleLogo );
      
    else if( NewState == Title_Waiting )
      play_sound_in_channel( MusicTitle, ChannelMusic );
}


// ---------------------------------------------------------
//   TITLE: DRAWING FUNCTIONS
// ---------------------------------------------------------


void Title_DrawScene( bool DrawLogo )
{
    // draw background color
    clear_screen( make_color_rgb( 107, 132, 136 ) );
    
    // draw title mat
    select_region( RegionTitleMat );
    draw_region_at( 151, 37 );
    
    // when logo is requested, show extra elements
    if( DrawLogo )
    {
        select_region( RegionLogo );
        draw_region_at( 212, 70 );
        
        select_region( Region2022 );
        draw_region_at( 7, 244 );
        
        select_region( RegionCarra );
        draw_region_at( 630, 244 );
        
        // draw ace cards
        for( int i = 0; i < 4; i++ )
          Card_DrawFront( &Title_Aces[ i ], 202 + 78*i, 164 );
        
        // draw figure cards in rotation
        int GlobalFrames = get_frame_counter();
        float BaseGlobalAngle = (2 * pi * GlobalFrames) / 180;
        
        Card_DrawRotated( &Title_Figures[ 0 ],  37,   3, BaseGlobalAngle + pi * 0.0 );
        Card_DrawRotated( &Title_Figures[ 1 ], 601,   3, BaseGlobalAngle + pi * 0.5 );
        Card_DrawRotated( &Title_Figures[ 2 ],  37, 263, BaseGlobalAngle + pi * 1.5 );
        Card_DrawRotated( &Title_Figures[ 3 ], 601, 263, BaseGlobalAngle + pi * 1.0 );
    }
}


// ---------------------------------------------------------
//   TITLE: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Title_RunState_Initialize()
{
    // initial configurations
    select_gamepad( 0 );
    select_texture( TextureSolitaire );
    
    // transition immediately
    Title_ChangeState( Title_FadeIn );
}

// ---------------------------------------------------------

void Title_RunState_FadeIn()
{
    // draw scene
    Title_DrawScene( false );
    
    // draw the 4 cards to deal
    for( int i = 3; i >= 0; i-- )
      Card_DrawBack( 311 + 6 * i, 55 );
    
    // fade-in effect
    int BlackLevel = max( 0, 255 - 255 * Title_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    
    // change state when finished
    if( Title_ElapsedFrames >= 60 )
    {
        Title_ChangeState( Title_Dealing );
        Title_DealtCards = 0;
    }
}

// ---------------------------------------------------------

void Title_RunState_Dealing()
{
    // draw scene
    Title_DrawScene( false );
    
    // these are constant
    int InitialY = 55;
    int FinalY = 164;
    
    // draw remaining face down cards
    for( int i = 3; i > Title_DealtCards; i-- )
      Card_DrawBack( 311 + 6 * i, InitialY );
    
    // draw previously dealt cards face up
    for( int i = 0; i < Title_DealtCards; i++ )
      Card_DrawFront( &Title_Aces[ i ], 202 + 78 * i, FinalY );
    
    // determine positions for dealt card movement
    int InitialX = 311 + 6 * Title_DealtCards;
    int FinalX = 202 + 78 * Title_DealtCards;
    
    // draw the card currently being dealt
    Card* DealtCard = &Title_Aces[ Title_DealtCards ];
    
    // part 1: card flip
    if( Title_ElapsedFrames < 20 )
    {
        if( Title_ElapsedFrames == 1 )
          play_sound( SoundCardFlip );
        
        Card_DrawFlipping( DealtCard, InitialX, InitialY, Title_ElapsedFrames / 20.0 );
    }
    
    // part 2: move to final spot
    else
    {
        if( Title_ElapsedFrames == 20 )
          play_sound( SoundCardSlide );
        
        float MotionProportion = (Title_ElapsedFrames - 20) / 20.0;
        int DrawX = InitialX + (FinalX - InitialX) * fmin( MotionProportion, 1.0 );
        int DrawY = InitialY + (FinalY - InitialY) * fmin( MotionProportion, 1.0 );
        Card_DrawFront( DealtCard, DrawX, DrawY );
    }
    
    // we allow to skip the dealing with the start button
    if( gamepad_button_start() == 1 )
    {
        Title_ChangeState( Title_Flash );
        return;
    }
    
    // count the dealt cards
    if( Title_ElapsedFrames > 40 )
    {
        Title_DealtCards++;
        Title_ElapsedFrames = 0;
        
        // if all cards were dealt this part is done
        if( Title_DealtCards >= 4 )
          Title_ChangeState( Title_Flash );
    }
}

// ---------------------------------------------------------

void Title_RunState_Flash()
{
    // draw scene
    Title_DrawScene( true );
    
    // fade from white
    int WhiteLevel = 128 * Title_ElapsedFrames / 6;
    clear_screen( make_color_rgba( 255,255,255,WhiteLevel ) );
    
    // change scene when finished
    if( Title_ElapsedFrames >= 6 )
        Title_ChangeState( Title_Waiting );
}

// ---------------------------------------------------------

void Title_RunState_Waiting()
{
    // draw scene
    Title_DrawScene( true );
    
    // draw "press start" and have it blink
    if( (Title_ElapsedFrames/30) % 2 )
    {
        select_region( RegionPressStart );
        draw_region_at( 208, 291 );
    }
    
    // change state when start pressed on first gamepad
    select_gamepad( 0 );
        
    if( gamepad_button_start() == 1 )
    {
        Title_ChangeState( Title_StartPressed );
        return;
    }
}

// ---------------------------------------------------------

void Title_RunState_StartPressed()
{
    // draw scene
    Title_DrawScene( true );
    
    // draw "press start" and have it blink
    if( (Title_ElapsedFrames/5) % 2 )
    {
        select_region( RegionPressStart );
        draw_region_at( 208, 291 );
    }
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * Title_ElapsedFrames / 45 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
    
    // change scene when finished
    if( Title_ElapsedFrames >= 60 )
    {
        GameScene = Scene_Menu;
        GameState = Menu_Initialize;
    }
}


// ---------------------------------------------------------
//   TITLE: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Title_RunStateMachine()
{
    switch( GameState )
    {
        case Title_Initialize:
            Title_RunState_Initialize();
            break;
            
        case Title_FadeIn:
            Title_RunState_FadeIn();
            break;
            
        case Title_Dealing:
            Title_RunState_Dealing();
            break;
            
        case Title_Flash:
            Title_RunState_Flash();
            break;
            
        case Title_Waiting:
            Title_RunState_Waiting();
            break;
            
        case Title_StartPressed:
            Title_RunState_StartPressed();
            break;
    }
    
    // count the frames in current state
    Title_ElapsedFrames++;
}
