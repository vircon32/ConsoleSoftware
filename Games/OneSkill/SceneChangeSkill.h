// *****************************************************************************
    // start include guard
    #ifndef SCENE_CHANGE_SKILL_H
    #define SCENE_CHANGE_SKILL_H
    
    // include project headers
    #include "Globals.h"
// *****************************************************************************


// auxiliary function to handle skills cyclically
int NormalizeSkillNumber( int SkillNumber )
{
    if( SkillNumber < 1  ) return SkillNumber + 4;
    if( SkillNumber > 4 ) return SkillNumber - 4;
    return SkillNumber;
}


// ---------------------------------------------------------
//   CHANGE SKILL: STATE MANAGEMENT
// ---------------------------------------------------------


int ChangeSkill_ElapsedFrames;
int ChangeSkill_SelectedSkill;
int ChangeSkill_NextSkillSign;
float ChangeSkill_CurrentAngle;

// wheel properties
#define WheelRadius 78

// ---------------------------------------------------------

void ChangeSkill_ChangeState( int NewState )
{
    GameState = NewState;
    ChangeSkill_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( NewState == ChangeSkill_Rotating )
      play_sound( SoundMenuMove );
    
    else if( NewState == ChangeSkill_FadeOut )
      play_sound( SoundMenuCancel );
}


// ---------------------------------------------------------
//   CHANGE SKILL: DRAWING FUNCTIONS
// ---------------------------------------------------------


void ChangeSkill_DrawScene( int AlphaLevel )
{
    // first draw the gameplay scene underneath
    DrawCurrentRoom();
    
    // darken the background
    clear_screen( make_color_rgba( 0,0,0, AlphaLevel / 2 ) );
    
    // draw title
    set_multiply_color( make_color_rgba( 255,255,255,AlphaLevel/2 ) );
    select_region( RegionTextChangeSkill );
    draw_region_at( 5, 5 );
    
    select_texture( TextureGameplay );
    set_multiply_color( make_color_rgba( 255,255,255,AlphaLevel ) );
    
    // draw the wheel
    set_drawing_angle( ChangeSkill_CurrentAngle );
    select_region( RegionSkillCircle );
    draw_region_rotated_at( SkillWheelCenterX, SkillWheelCenterY );
    
    // draw skill icons
    float IconsRadius = WheelRadius;
    
    if( GameState == ChangeSkill_IconsAppear )
      IconsRadius = (WheelRadius / 15.0) * ChangeSkill_ElapsedFrames;
    
    for( int i = 0; i < 4; ++i )
    {
        float Angle = ChangeSkill_CurrentAngle + (pi/2.0) * (i - 1);
        float IconX = SkillWheelCenterX + IconsRadius * cos( Angle ) - 20;
        float IconY = SkillWheelCenterY + IconsRadius * sin( Angle ) - 20;
        
        int SkillNumber = NormalizeSkillNumber( ChangeSkill_SelectedSkill + i );
        select_region( FirstRegionSkillIcons + (SkillNumber-1) );
        draw_region_at( IconX, IconY );
    }
    
    // draw center icon
    select_region( RegionSkillsIconChange );
    draw_region_at( SkillWheelCenterX - 20, SkillWheelCenterY - 20 );
    
    set_multiply_color( color_white );
}

   
// ---------------------------------------------------------
//   CHANGE SKILL: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void ChangeSkill_RunState_Initialize()
{
    // use current skill, but start at
    // pistol if player has no skill yet
    ChangeSkill_SelectedSkill = Player1.Skill;
    
    if( ChangeSkill_SelectedSkill <= 0 )
      ChangeSkill_SelectedSkill = 1;
    
    ChangeSkill_NextSkillSign = 0;
    ChangeSkill_CurrentAngle = 0;
    
    // adapt wheel center from room coordinates
    // to screen coordinates
    SkillWheelCenterX += CurrentRoomMap.TopLeftOnScreen.x;
    SkillWheelCenterY += CurrentRoomMap.TopLeftOnScreen.y;
    
    // transition immediately
    ChangeSkill_ChangeState( ChangeSkill_IconsAppear );
}

// ---------------------------------------------------------

void ChangeSkill_RunState_IconsAppear()
{
    ChangeSkill_DrawScene( 255 );
    
    // after transition time change to waiting
    if( ChangeSkill_ElapsedFrames >= 15 )
      ChangeSkill_ChangeState( ChangeSkill_Waiting );
}

// ---------------------------------------------------------

void ChangeSkill_RunState_Rotating()
{
    ChangeSkill_DrawScene( 255 );
    ChangeSkill_CurrentAngle += ChangeSkill_NextSkillSign * (pi/2.0) / 15.0;
    
    // after transition time:
    if( ChangeSkill_ElapsedFrames >= 15 )
    {
        // select next skill
        ChangeSkill_CurrentAngle = 0;
        ChangeSkill_SelectedSkill = NormalizeSkillNumber( ChangeSkill_SelectedSkill - ChangeSkill_NextSkillSign );
        
        // change back to waiting
        ChangeSkill_ChangeState( ChangeSkill_Waiting );
    }
}

// ---------------------------------------------------------

void ChangeSkill_RunState_Waiting()
{
    ChangeSkill_DrawScene( 255 );
    
    // draw icon highlight
    int CurrentFrame = (ChangeSkill_ElapsedFrames / 8) % 2;
    select_region( FirstRegionSkillSelection + CurrentFrame );
    draw_region_at( SkillWheelCenterX - 20, SkillWheelCenterY - (WheelRadius + 20) );
    
    // draw skill name
    select_region( FirstRegionSkillNamesChange + (ChangeSkill_SelectedSkill-1) );
    draw_region_at( SkillWheelCenterX, SkillWheelCenterY - (WheelRadius + 30) );
    
    // select previous skill
    if( gamepad_left() == 1 )
    {
        ChangeSkill_NextSkillSign = 1;
        ChangeSkill_ChangeState( ChangeSkill_Rotating );
        return;
    }
    
    // select next skill
    if( gamepad_right() == 1 )
    {
        ChangeSkill_NextSkillSign = -1;
        ChangeSkill_ChangeState( ChangeSkill_Rotating );
        return;
    }
    
    // apply current skill
    if( gamepad_button_a() == 1 || gamepad_button_b() == 1 )
    {
        // apply skill change
        Player1.Skill = (PlayerSkills)ChangeSkill_SelectedSkill;
        
        ChangeSkill_ChangeState( ChangeSkill_FadeOut );
    }
}

// ---------------------------------------------------------

void ChangeSkill_RunState_FadeOut()
{
    // fade out to gameplay
    int AlphaLevel = max( 0, 255 - 255 * ChangeSkill_ElapsedFrames / 15 );
    ChangeSkill_DrawScene( AlphaLevel );
    
    // when finished change back to gameplay scene
    if( ChangeSkill_ElapsedFrames >= 15 )
    {
        GameScene = Scene_Gameplay;
        GameState = Gameplay_Level;
    }
}


// ---------------------------------------------------------
//   CHANGE SKILL: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void ChangeSkill_RunStateMachine()
{
    switch( GameState )
    {
        case ChangeSkill_Initialize:
          ChangeSkill_RunState_Initialize();
          break;
        
        case ChangeSkill_IconsAppear:
          ChangeSkill_RunState_IconsAppear();
          break;
        
        case ChangeSkill_Rotating:
          ChangeSkill_RunState_Rotating();
          break;
        
        case ChangeSkill_Waiting:
          ChangeSkill_RunState_Waiting();
          break;
        
        case ChangeSkill_FadeOut:
          ChangeSkill_RunState_FadeOut();
          break;
    }
    
    // count the frames in current state
    ChangeSkill_ElapsedFrames++;
}


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
