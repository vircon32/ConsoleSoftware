// ---------------------------------------------------------
//   TUTORIAL: STATE MANAGEMENT
// ---------------------------------------------------------


int Tutorial_ElapsedFrames;
int Tutorial_SkillNumber;

// ---------------------------------------------------------

void Tutorial_ChangeState( int NewState )
{
    GameState = NewState;
    Tutorial_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( NewState == Tutorial_FadeOut )
      stop_channel( ChannelMusic );
}


// ---------------------------------------------------------
//   TUTORIAL: DRAWING FUNCTIONS
// ---------------------------------------------------------


int[ 3 ][ 150 ] Tutorial_SkillPostText =
{
    "NICO CAN ONLY USE ONE SKILL AT A TIME.",
    "PRESSING BUTTON X ON A SKILL POST WILL",
    "ALLOW YOU TO CHANGE YOUR CURRENT SKILL."
};

int[ 4 ][ 60 ] Tutorial_SkillTexts =
{
    "YOUR BULLETS CAN KILL ENEMIES AND DESTROY CRATES.",
    
    "ALLOWS YOU TO DOUBLE JUMP AND REACH FAR PLACES.",
    
    "USE MAGIC TO TOGGLE ON/OFF BLOCKS AND OPEN PATHS.",

    "WEARING THEM YOU CAN WALK ON SPIKES AND ENEMIES."
};

// ---------------------------------------------------------

void Tutorial_DrawSkill( int SkillNumber )
{
    // draw GUI
    select_region( FirstRegionSkillIcons + SkillNumber );
    draw_region_at( 260, 130 );
    
    select_region( FirstRegionSkillNamesGUI + SkillNumber );
    draw_region_at( 300, 130 );
    
    // draw walk animation
    int AnimationFrame = (get_frame_counter() / 8 ) % 4;
    select_region( FirstRegionPistolWalk + 4*SkillNumber + AnimationFrame );
    draw_region_at( 320, 230 );
    
    // write skill description
    textfont_print_centered( &Font11x16, screen_width/2, 270, Tutorial_SkillTexts[ SkillNumber ] );
}


// ---------------------------------------------------------
//   TUTORIAL: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Tutorial_RunState_Initialize()
{
    // stop previous music and play tutorial music
    stop_channel( ChannelMusic );
    play_sound_in_channel( MusicTutorial, ChannelMusic );
    
    // transition immediately
    Tutorial_ChangeState( Tutorial_SkillSystem );
}

// ---------------------------------------------------------

void Tutorial_RunState_SkillSystem()
{
    // draw title
    clear_screen( color_black );
    select_texture( TextureGameplay );
    select_region( RegionTextHowToPlay );
    draw_region_at( screen_width/2, 40 );
    
    // draw skill post
    select_region( RegionSkillPost );
    draw_region_at( screen_width/2 - 20, 190 );
    
    // write skill text, line by line
    if( Tutorial_ElapsedFrames >= 40 )
      textfont_print_from_left( &Font11x16, 120, 230, Tutorial_SkillPostText[ 0 ] );
    
    if( Tutorial_ElapsedFrames >= 60 )
      textfont_print_from_left( &Font11x16, 120, 250, Tutorial_SkillPostText[ 1 ] );

    if( Tutorial_ElapsedFrames >= 80 )
      textfont_print_from_left( &Font11x16, 120, 270, Tutorial_SkillPostText[ 2 ] );
    
    // allow player to skip tutorial
    if( AnyButtonPressed() )
    {
        // use a negative value to signal no skill
        Tutorial_SkillNumber = -1;
        Tutorial_ChangeState( Tutorial_FadeOut );
        return;
    }
    
    // automatic change after a given time
    if( Tutorial_ElapsedFrames >= 450 )
    {
        Tutorial_SkillNumber = 0;
        Tutorial_ChangeState( Tutorial_Skills );
    }
}

// ---------------------------------------------------------

void Tutorial_RunState_Skills()
{
    // draw title
    clear_screen( color_black );
    select_texture( TextureGameplay );
    select_region( RegionTextHowToPlay );
    draw_region_at( screen_width/2, 40 );
    
    // draw current skill
    Tutorial_DrawSkill( Tutorial_SkillNumber );
    
    // allow player to skip tutorial
    if( AnyButtonPressed() )
    {
        Tutorial_ChangeState( Tutorial_FadeOut );
        return;
    }
    
    // automatic change after a given time
    if( Tutorial_ElapsedFrames >= 250 )
    {
        Tutorial_SkillNumber++;
        Tutorial_ElapsedFrames = 0;
        
        if( Tutorial_SkillNumber >= 4 )
        {
            Tutorial_SkillNumber = 3;
            Tutorial_ChangeState( Tutorial_FadeOut );
        }
    }
}

// ---------------------------------------------------------

void Tutorial_RunState_FadeOut()
{
    // draw title
    clear_screen( color_black );
    select_texture( TextureGameplay );
    select_region( RegionTextHowToPlay );
    draw_region_at( screen_width/2, 40 );
    
    // draw skill or skill post, depending
    // on the point where we came from
    if( Tutorial_SkillNumber >= 0 )
      Tutorial_DrawSkill( Tutorial_SkillNumber );
    
    else
    {
        select_region( RegionSkillPost );
        draw_region_at( screen_width/2 - 20, 190 );
    }
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * Tutorial_ElapsedFrames / 45 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
    
    // change scene when finished
    if( Tutorial_ElapsedFrames >= 60 )
    {
        GameScene = Scene_Gameplay;
        GameState = Gameplay_Initialize;
    }
}


// ---------------------------------------------------------
//   TITLE: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Tutorial_RunStateMachine()
{
    switch( GameState )
    {
        case Tutorial_Initialize:
          Tutorial_RunState_Initialize();
          break;
        
        case Tutorial_SkillSystem:
          Tutorial_RunState_SkillSystem();
          break;
        
        case Tutorial_Skills:
          Tutorial_RunState_Skills();
          break;
        
        case Tutorial_FadeOut:
          Tutorial_RunState_FadeOut();
          break;
    }
    
    // count the frames in current state
    Tutorial_ElapsedFrames++;
}
