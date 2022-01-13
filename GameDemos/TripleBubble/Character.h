// character "class"
struct Character
{
    // launcher where it operates
    // and the one it will move to
    Launcher* CurrentLauncher;
    Launcher* NextLauncher;
    
    // relative positions of other launchers
    Launcher* LauncherAtLeft;
    Launcher* LauncherAtRight;
    
    // position on screen
    Vector2D DrawPosition;
    
    // player configuration
    int PlayerGamepad;
    bool ActionsEnabled;
    
    // scoring properties
    Points* PointsObject;
    int ComboCounter;
    
    // current state
    int State;
    int ElapsedFrames;
    int RemainingShootFrames;
    
    // animation state
    int CurrentRegion;
};


// ---------------------------------------------------------
//     CHARACTER METHODS
// ---------------------------------------------------------


void Character_ChangeState( Character* C, int NewState )
{
    int OldState = C->State;
    C->State = NewState;
    C->ElapsedFrames = 0;
    
    // events triggered in some transitions
    if( NewState == Character_Shoot )
      C->RemainingShootFrames = 600;
}

// ---------------------------------------------------------

void Character_Reset( Character* C )
{
    Character_ChangeState( C, Character_Initialize );
}

// ---------------------------------------------------------

void Character_Create( Character* C, int Gamepad, Launcher* CurrentLauncher, Launcher* AtLeft,
                       Launcher* AtRight, Points* PointsObject )
{
    // link to player gamepad
    C->PlayerGamepad = Gamepad;
    
    // link to launcher
    CurrentLauncher->HasCharacter = true;
    C->CurrentLauncher = CurrentLauncher;
    
    // links to other launchers
    C->NextLauncher = NULL;
    C->LauncherAtLeft = AtLeft;
    C->LauncherAtRight = AtRight;
    
    // position given by launcher
    C->DrawPosition.x = CurrentLauncher->LaunchPosition.x - 38;
    C->DrawPosition.y = CurrentLauncher->BottomY;
    
    // link to point data
    C->PointsObject = PointsObject;
    
    // reset character
    Character_ChangeState( C, Character_Initialize );
}

// ---------------------------------------------------------

void Character_ProcessAimInput( Character* C )
{
    Launcher* L = C->CurrentLauncher;
    Playfield* P = L->OwnerField;
    float LastAngle = L->ArrowAngle;
    L->IsMoving = false;
    
    // do nothing if the launcher state does not allow aiming
    if( L->State != Launcher_Aim && L->State != Launcher_Shoot )
    {
        stop_channel( ChannelLauncherMove );
        return;
    }
    
    // do nothing if the playfield state does not allow aiming
    if( P->State != Playfield_Wait )
    {
        stop_channel( ChannelLauncherMove );
        return;
    }
    
    select_gamepad( C->PlayerGamepad );
    
    // change angle
    if( gamepad_left() > 0 )
      L->ArrowAngle -= 0.02;
    
    if( gamepad_right() > 0 )
      L->ArrowAngle += 0.02;
    
    // limit angle
    if( L->ArrowAngle > L->MaxAngle )
      L->ArrowAngle = L->MaxAngle;
    
    if( L->ArrowAngle < L->MinAngle )
      L->ArrowAngle = L->MinAngle;
    
    // update movement state
    L->IsMoving = (L->ArrowAngle != LastAngle);
    
    // ensure launcher move sound is playing
    if( L->IsMoving )
    {
        if( get_channel_state( ChannelLauncherMove ) != channel_playing )
          play_sound_in_channel( SoundLauncherMove, ChannelLauncherMove );
    }
    
    else
    {
        stop_channel( ChannelLauncherMove );
    }
}

// ---------------------------------------------------------

void Character_ProcessShootInput( Character* C, bool ForcedShoot )
{
    Launcher* L = C->CurrentLauncher;
    Playfield* P = L->OwnerField;
    
    // do nothing if the launcher state does not allow shooting
    if( L->State != Launcher_Aim )
      return;
    
    // do nothing if the playfield state does not allow aiming
    if( P->State != Playfield_Wait )
      return;
    
    select_gamepad( C->PlayerGamepad );
    if( gamepad_button_a() == 1 || ForcedShoot )
    {
        L->CurrentBubble.Velocity.x = BubbleSpeed * cos( L->ArrowAngle );
        L->CurrentBubble.Velocity.y = BubbleSpeed * sin( L->ArrowAngle );
        Launcher_ChangeState( L, Launcher_Shoot );
        Character_ChangeState( C, Character_Shoot );
        
        // link launcher scoring to this player
        L->CurrentShotPoints = C->PointsObject;
        L->CurrentComboCounter = &C->ComboCounter;
    }
}

// ---------------------------------------------------------

void Character_ProcessLeaveInput( Character* C )
{
    // check gamepad
    select_gamepad( C->PlayerGamepad );
    
    if( C->LauncherAtLeft && gamepad_button_l() == 1 )
    {
        Character_ChangeState( C, Character_LeaveLauncher );
        C->NextLauncher = C->LauncherAtLeft;
        C->LauncherAtLeft = C->LauncherAtRight;
        C->LauncherAtRight = C->CurrentLauncher;
        play_sound( SoundChangeLauncher );
        
    }
    
    else if( C->LauncherAtRight && gamepad_button_r() == 1 )
    {
        Character_ChangeState( C, Character_LeaveLauncher );
        C->NextLauncher = C->LauncherAtRight;
        C->LauncherAtRight = C->LauncherAtLeft;
        C->LauncherAtLeft = C->CurrentLauncher;
        play_sound( SoundChangeLauncher );
    }
}

// ---------------------------------------------------------

void Character_DetectEvents( Character* C, bool PlayfieldOnly )
{
    // detect state changes from the launcher
    Launcher* L = C->CurrentLauncher;
    
    if( !PlayfieldOnly )
    {
        if( L->State == Launcher_BubblePlaced )
          if( C->State != Character_BubblePlaced )
          Character_ChangeState( C, Character_BubblePlaced );
        
        if( L->State == Launcher_BubbleBurst )
          if( C->State != Character_BubbleBurst )
            Character_ChangeState( C, Character_BubbleBurst );
    }
    
    // detect state changes from the playfield
    // (with higher priority)
    Playfield* P = L->OwnerField;
    
    if( P->State == Playfield_CeilingDrop )
      if( C->State != Character_CeilingDrop )
        Character_ChangeState( C, Character_CeilingDrop );
    
    if( P->State == Playfield_LevelClear )
      if( C->State != Character_LevelClear )
        Character_ChangeState( C, Character_LevelClear );
    
    if( P->State == Playfield_Death )
      if( C->State != Character_Death )
        Character_ChangeState( C, Character_Death );
}

// ---------------------------------------------------------

void Character_DrawAimGuide( Character* C )
{
    select_texture( TextureCharacter1 + C->PlayerGamepad );
    select_region( RegionPlayerAimGuide );
    Launcher* L = C->CurrentLauncher;
    
    // calculate the whole trajectory
    Launcher_CalculateAimTrajectory( L );
    
    // draw each ray
    for( int i = 0; i < 3; ++i )
    {
        Ray* CurrentRay = &L->AimTrajectory[ i ];
        
        if( !CurrentRay->Active )
          break;
        
        if( CurrentRay->End == CurrentRay->Start )
          continue;
        
        float Distance = DistanceBetween( &CurrentRay->Start, &CurrentRay->End );
        float Angle = AngleBetween( &CurrentRay->Start, &CurrentRay->End );
        
        // force dot line to end in a whole dot
        int DotLineWidth = Distance;
        DotLineWidth = (DotLineWidth / 13) * 13;
        int DotLineOffset = (GlobalElapsedFrames/2) % 13;
        
        // do not draw very short lines
        if( DotLineWidth < 12 )
          continue;
        
        // first ray is drawn without the start
        // to avoid drawing behind the arrow
        if( i == 0 )
          define_region( 14,1,  14+DotLineWidth-1,9,  18-DotLineOffset,5 );
        
        else
          define_region( 66,1,  66+DotLineWidth-1,9,  70-DotLineOffset,5 );
        
        set_drawing_angle( Angle );
        draw_region_rotated_at( CurrentRay->Start.x, CurrentRay->Start.y );
    }
}


// ---------------------------------------------------------
//   CHARACTER: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Character_RunState_Initialize( Character* C )
{
    C->ActionsEnabled = false;
    C->ElapsedFrames = 0;
    C->ComboCounter = 0;
    
    // transition immediately
    // but do not enable actions yet
    Character_ChangeState( C, Character_Aim );
    
    // begin idle animation
    C->CurrentRegion = FirstRegionPlayerIdle;
}

// ---------------------------------------------------------

void Character_RunState_Aim( Character* C )
{
    // process player inputs
    if( C->ActionsEnabled )
    {
        Character_ProcessAimInput( C );
        Character_ProcessShootInput( C, false );
        
        // allow changing launcher only when there
        // is no bubble being shot, or other events
        if( C->CurrentLauncher->State == Launcher_Aim )
          Character_ProcessLeaveInput( C );
    }
    
    // run aim animation
    if( C->CurrentLauncher->IsMoving )
    {
        int CharacterAngle = -C->CurrentLauncher->ArrowAngle*10;
        int AnimationFrame = CharacterAngle % 5;
        C->CurrentRegion = FirstRegionPlayerAim + AnimationFrame;
    }
    
    // run idle animation
    else
    {
        int AnimationFrame = (C->ElapsedFrames/12) % 6;
        C->CurrentRegion = FirstRegionPlayerIdle + AnimationFrame;
    }
    
    // enforce shoot time limit
    C->RemainingShootFrames--;
    
    if( C->ActionsEnabled && C->RemainingShootFrames <= 0 )
      Character_ProcessShootInput( C, true );
    
    // play sound on countdown
    if( C->RemainingShootFrames > 0 && C->RemainingShootFrames <= 240 )
      if( !(C->RemainingShootFrames % 60) )
        if( C->ActionsEnabled )
          play_sound( SoundCountdown );
    
    // otherwise, do not make any transitions
    // (other objects will cause them)
    Character_DetectEvents( C, false );
}

// ---------------------------------------------------------

void Character_RunState_Shoot( Character* C )
{
    // process player inputs
    if( C->ActionsEnabled )
      Character_ProcessAimInput( C );
    
    // run shoot animation in ping-pong mode
    int AnimationFrame = C->ElapsedFrames / 4;
    
    int RegionIncrement = AnimationFrame;
    if( AnimationFrame > 2 ) RegionIncrement = 4 - min( 4, AnimationFrame );
    
    C->CurrentRegion = FirstRegionPlayerShoot + RegionIncrement;
    
    // when finished, go back to aim state
    if( AnimationFrame > 4 )
      Character_ChangeState( C, Character_Aim );
    
    Character_DetectEvents( C, false );
}

// ---------------------------------------------------------

void Character_RunState_BubbleBurst( Character* C )
{
    // run burst animation
    int AnimationFrame = (C->ElapsedFrames / 6) % 2;
    C->CurrentRegion = FirstRegionPlayerBubbleBurst + AnimationFrame;
    
    // when finished, go back to aim state
    if( C->ElapsedFrames > 23 )
      Character_ChangeState( C, Character_Aim );  
    
    Character_DetectEvents( C, true );
}

// ---------------------------------------------------------

void Character_RunState_BubblePlaced( Character* C )
{
    // run place animation in a loop
    int AnimationFrame = (C->ElapsedFrames / 7) % 2;
    C->CurrentRegion = FirstRegionPlayerBubblePlaced + AnimationFrame;
    
    // when finished, go back to aim state
    if( C->ElapsedFrames > 13 )
      Character_ChangeState( C, Character_Aim );  
    
    Character_DetectEvents( C, true );
}

// ---------------------------------------------------------

void Character_RunState_CeilingDrop( Character* C )
{
    // run drop animation in a loop
    int AnimationFrame = (C->ElapsedFrames / 4) % 2;
    C->CurrentRegion = FirstRegionPlayerCeilingDrop + AnimationFrame;
    
    // when finished, go back to aim state
    if( C->ElapsedFrames > 30 )
      Character_ChangeState( C, Character_Aim );  
    
    Character_DetectEvents( C, true );
}

// ---------------------------------------------------------

void Character_RunState_LevelClear( Character* C )
{
    // run clear animation
    int AnimationFrame = C->ElapsedFrames / 6;
    C->CurrentRegion = FirstRegionPlayerLevelClear + min( 5, AnimationFrame );
    
    // do NOT go back to aim state
}

// ---------------------------------------------------------

void Character_RunState_Death( Character* C )
{
    // run death animation
    int AnimationFrame = C->ElapsedFrames / 8;
    C->CurrentRegion = FirstRegionPlayerDeath + min( 3, AnimationFrame );
    
    // do NOT go back to aim state
}

// ---------------------------------------------------------

void Character_RunState_LeaveLauncher( Character* C )
{
    // run leave animation
    int AnimationFrame = C->ElapsedFrames / 3;
    C->CurrentRegion = FirstRegionPlayerLeave + min( 2, AnimationFrame );
    
    // when finished, apply new launcher and go to enter state
    if( C->ElapsedFrames >= 9 )
    {
        // change: move bubbles to the new launcher
        C->NextLauncher->CurrentBubble.Type = C->CurrentLauncher->CurrentBubble.Type;
        C->NextLauncher->NextBubble.Type = C->CurrentLauncher->NextBubble.Type;
        C->CurrentLauncher->CurrentBubble.Type = 0;
        C->CurrentLauncher->NextBubble.Type = 0;
        
        // also move other properties, like the "show trajectory" toggle
        C->NextLauncher->AimTrajectoryEnabled = C->CurrentLauncher->AimTrajectoryEnabled;
        
        // normal previous code
        Character_ChangeState( C, Character_EnterLauncher );
        C->CurrentLauncher->HasCharacter = false;
        C->CurrentLauncher = C->NextLauncher;
        C->CurrentLauncher->HasCharacter = true;
        C->NextLauncher = NULL;
        C->DrawPosition.x = C->CurrentLauncher->LaunchPosition.x - 38;
        C->DrawPosition.y = C->CurrentLauncher->BottomY;
    }
}

// ---------------------------------------------------------

void Character_RunState_EnterLauncher( Character* C )
{
    // run enter animation
    int AnimationFrame = C->ElapsedFrames / 3;
    C->CurrentRegion = FirstRegionPlayerLeave + 2 - min( 2, AnimationFrame );
    
    // when finished, go to aim state
    if( C->ElapsedFrames >= 9 )
      Character_ChangeState( C, Character_Aim );
}

// ---------------------------------------------------------

void Character_RunStateMachine( Character* C )
{
    if( C->State == Character_Aim )
      Character_RunState_Aim( C );
    
    else if( C->State == Character_Shoot )
      Character_RunState_Shoot( C );
    
    else if( C->State == Character_BubbleBurst )
      Character_RunState_BubbleBurst( C );
    
    else if( C->State == Character_BubblePlaced )
      Character_RunState_BubblePlaced( C );
    
    else if( C->State == Character_CeilingDrop )
      Character_RunState_CeilingDrop( C );
    
    else if( C->State == Character_LevelClear )
      Character_RunState_LevelClear( C );
    
    else if( C->State == Character_Death )
      Character_RunState_Death( C );
    
    else if( C->State == Character_LeaveLauncher )
      Character_RunState_LeaveLauncher( C );
    
    else if( C->State == Character_EnterLauncher )
      Character_RunState_EnterLauncher( C );
    
    else if( C->State == Character_Initialize )
      Character_RunState_Initialize( C );
    
    // count time in present state
    C->ElapsedFrames++;
}


// ---------------------------------------------------------
//   CHARACTER: DRAWING FUNCTIONS
// ---------------------------------------------------------


void Character_Draw( Character* C )
{
    select_texture( TextureCharacter1 + C->PlayerGamepad );
    select_region( C->CurrentRegion );
    draw_region_at( C->DrawPosition.x, C->DrawPosition.y );
    
    Launcher* L = C->CurrentLauncher;
    
    if( L->AimTrajectoryEnabled )
      if( L->State == Launcher_Aim )
        if( C->State == Character_Aim )
          Character_DrawAimGuide( C );
    
    // draw countdown
    if( C->ActionsEnabled )
      if( C->CurrentLauncher->State == Launcher_Aim )
        if( C->CurrentLauncher->OwnerField->State == Playfield_Wait )
          if( C->RemainingShootFrames > 0 && C->RemainingShootFrames < 240 )
            {
                select_texture( TextureGameplay );
                select_region( FirstRegionCountdown + C->RemainingShootFrames / 60 );
                
                // draw with fade-out effect
                int Alpha = 15 + 4*(C->RemainingShootFrames % 60);
                set_multiply_color( make_color_rgba(255,255,255,Alpha) );
                draw_region_at( C->CurrentLauncher->LaunchPosition.x, C->CurrentLauncher->LaunchPosition.y - 100 );
                set_multiply_color( color_white );
            }
}
