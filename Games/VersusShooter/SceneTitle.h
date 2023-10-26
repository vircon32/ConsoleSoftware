// *****************************************************************************
    // start include guard
    #ifndef SCENE_TITLE_H
    #define SCENE_TITLE_H
    
    // include project headers
    #include "Globals.h"
// *****************************************************************************


// ---------------------------------------------------------
//   TITLE: STATE MANAGEMENT
// ---------------------------------------------------------


int Title_ElapsedFrames;

// ---------------------------------------------------------

void Title_ChangeState( int NewState )
{
    GameState = NewState;
    Title_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( NewState == Title_StartPressed )
      play_sound( SoundPressStart );
}


// ---------------------------------------------------------
//   TITLE: DRAWING FUNCTIONS
// ---------------------------------------------------------


void Title_DrawScene()
{
    clear_screen( color_gray );
    
    // draw logo
    select_texture( TextureGameplay );
    select_region( RegionLogo );
    draw_region_at( screen_width/2, 140 );
}


// ---------------------------------------------------------
//   TITLE: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Title_RunState_Initialize()
{
    // transition immediately
    Title_ChangeState( Title_FadeIn );
}

// ---------------------------------------------------------

void Title_RunState_FadeIn()
{
    Title_DrawScene();
    
    // fade-in effect
    int BlackLevel = max( 0, 255 - 255 * Title_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    
    // change state when finished
    if( Title_ElapsedFrames >= 60 )
      Title_ChangeState( Title_Waiting );
}

// ---------------------------------------------------------

void Title_RunState_Waiting()
{
    Title_DrawScene();
    
    // draw "press start" and have it blink
    if( (Title_ElapsedFrames/30) % 2 )
    {
        select_region( RegionPressStart );
        draw_region_at( screen_width/2, 250 );
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
    // draw full scene
    Title_DrawScene();
    select_texture( TextureGameplay );
    
    // draw "press start" and have it blink
    if( (Title_ElapsedFrames/5) % 2 )
    {
        select_region( RegionPressStart );
        draw_region_at( screen_width/2, 250 );
    }
    
    // fade out to black
    int AlphaLevel = min( 255, 255 * Title_ElapsedFrames / 45 );
    clear_screen( make_color_rgba( 0,0,0,AlphaLevel ) );
    
    // change scene when finished
    if( Title_ElapsedFrames >= 60 )
    {
        GameScene = Scene_Gameplay;
        GameState = Gameplay_Initialize;
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


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
