// "class"
struct Launcher
{
    // playfield where it is contained
    Playfield* OwnerField;
    
    // position on screen
    Vector2D LaunchPosition;
    int BottomY;
    
    // aim orientation
    float ArrowAngle;
    float InitialAngle;
    float MinAngle, MaxAngle;
    
    // stored bubbles
    Bubble CurrentBubble;
    Bubble NextBubble;
    
    // position of bubble placed
    Vector2D AddedBubbleCenter;
    
    // positions for bubbles burst
    BubbleArray BurstBubbles;
    
    // animation for points
    Points* CurrentShotPoints;
    int* CurrentComboCounter;
    
    // aim line to predict shot
    bool AimTrajectoryEnabled;
    Ray[4] AimTrajectory;
    
    // current state
    int State;
    int OwningState;
    int ElapsedFrames;
    bool IsMoving;
    bool HasCharacter;
};


// ---------------------------------------------------------
//     LAUNCHER METHODS
// ---------------------------------------------------------


void Launcher_SetNextBubble( Launcher* L )
{
    L->CurrentBubble = L->NextBubble;
    //L->CurrentBubble.Center = L->LaunchPosition;
    
    L->NextBubble.Center.x = L->LaunchPosition.x + 39;
    L->NextBubble.Center.y = L->LaunchPosition.y + 39;
    L->NextBubble.Velocity.x = 0;
    L->NextBubble.Velocity.y = 0;
    L->NextBubble.Type = Playfield_GetNewBubbleType( L->OwnerField );
}

// ---------------------------------------------------------

// returns true if bubble movement has ended
bool Launcher_UpdateBubbleMovement( Launcher* L )
{
    Bubble* TargetBubble = &L->CurrentBubble;
    Playfield* Field = L->OwnerField;
    
    // simulate this frame
    TargetBubble->Center.x += TargetBubble->Velocity.x;
    TargetBubble->Center.y += TargetBubble->Velocity.y;
    
    // collide with walls
    if( TargetBubble->Center.x <= Field->BubbleMinX )
    {
        TargetBubble->Center.x = Field->BubbleMinX;
        TargetBubble->Velocity.x = -TargetBubble->Velocity.x;
        
        play_sound( SoundBubbleBounce );
    }
    
    if( TargetBubble->Center.x >= Field->BubbleMaxX )
    {
        TargetBubble->Center.x = Field->BubbleMaxX;
        TargetBubble->Velocity.x = -TargetBubble->Velocity.x;
        
        play_sound( SoundBubbleBounce );
    }
    
    // collide with the level
    return Playfield_CollideBubble( Field, TargetBubble );
}

// ---------------------------------------------------------

void Launcher_CalculateAimTrajectory( Launcher* L )
{
    // initialize
    Ray* CurrentRay = &L->AimTrajectory[ 0 ];
    CurrentRay->Start.x = L->LaunchPosition.x;
    CurrentRay->Start.y = L->LaunchPosition.y;
    
    CurrentRay->Velocity.x = BubbleSpeed * cos( L->ArrowAngle );
    CurrentRay->Velocity.y = BubbleSpeed * sin( L->ArrowAngle );
    
    // set 2nd and 3rd rays as initially inactive
    L->AimTrajectory[ 0 ].Active = true;
    L->AimTrajectory[ 1 ].Active = false;
    L->AimTrajectory[ 2 ].Active = false;
    
    // cast rays
    while( Playfield_CastRay( L->OwnerField, CurrentRay ) )
    {
        // next ray starts at current ray end
        Ray* NextRay = CurrentRay + 1;
        NextRay->Start = CurrentRay->End;
        
        // reflect speed for next ray
        // (the only option is a wall bounce)
        NextRay->Velocity.x = -CurrentRay->Velocity.x;
        NextRay->Velocity.y = +CurrentRay->Velocity.y;
        
        // iterate
        CurrentRay = NextRay;
    }
}


// ---------------------------------------------------------
//   LAUNCHER: STATE MANAGEMENT
// ---------------------------------------------------------


void Launcher_ChangeState( Launcher* L, int NewState )
{
    int OldState = L->State;
    L->State = NewState;
    L->ElapsedFrames = 0;
    
    // some transitions trigger events
    if( NewState == Launcher_Shoot )
    {
        L->CurrentBubble.Velocity.x = BubbleSpeed * cos( L->ArrowAngle );
        L->CurrentBubble.Velocity.y = BubbleSpeed * sin( L->ArrowAngle );
        
        play_sound( SoundBubbleShoot );
    }
    
    if( NewState == Launcher_BubbleBurst )
    {
        Launcher_SetNextBubble( L );
        play_sound( SoundBubblesBurst );
        
        // copy burst bubbles positions
        memcpy( L->BurstBubbles, L->OwnerField->ConnectedBubbles, sizeof(BubbleArray) );
        
        // determine scored points
        // and create their animations
        (*L->CurrentComboCounter)++;
        Points_Create( L->CurrentShotPoints, L->OwnerField, *(L->CurrentComboCounter) );
    }
    
    if( NewState == Launcher_BubblePlaced )
    {
        Launcher_SetNextBubble( L );
        play_sound( SoundBubblePlaced );
        
        // copy placed bubble position
        L->AddedBubbleCenter = L->OwnerField->AddedBubbleCenter;
        
        // reset combo counter
        (*L->CurrentComboCounter) = 0;
    }
    
    if( NewState == Launcher_Aim )
    {
        // check for level clear and ceiling drop
        // (with clear having the highest priority)
        if( OldState == Launcher_BubbleBurst )
        {
            if( !Playfield_CheckLevelClear( L->OwnerField ) )
              Playfield_CheckCeilingDrop( L->OwnerField );
        }
        
        // check for death and ceiling drop
        if( OldState == Launcher_BubblePlaced )
        {
            if( !Playfield_CheckDeath( L->OwnerField ) )
              Playfield_CheckCeilingDrop( L->OwnerField );
        }
    }
}

// ---------------------------------------------------------

void Launcher_Create( Launcher* L, Playfield* OwnerField, int OffsetX )
{
    // base configuration
    L->OwnerField = OwnerField;
    L->AimTrajectoryEnabled = true;
    
    // position
    L->BottomY = OwnerField->MaxY;
    L->LaunchPosition.y = L->BottomY - 53;
    L->LaunchPosition.x = (OwnerField->MinX + OwnerField->MaxX) / 2 + OffsetX;
    
    // orientation
    L->MinAngle = -9*pi/10;
    L->MaxAngle = -pi/10;
    L->InitialAngle = -pi/2;
    
    // initially, set no character
    L->HasCharacter = false;
}

// ---------------------------------------------------------

void Launcher_Reset( Launcher* L )
{
    Launcher_ChangeState( L, Launcher_Initialize );
}


// ---------------------------------------------------------
//   LAUNCHER: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Launcher_RunState_Initialize( Launcher* L )
{
    // set initial state
    L->ArrowAngle = L->InitialAngle;
    L->IsMoving = false;
    
    // create launcher bubbles
    L->CurrentBubble.Type = 1 + rand() % 3;
    L->CurrentBubble.Center.x = L->LaunchPosition.x;
    L->CurrentBubble.Center.y = L->LaunchPosition.y;
    L->CurrentBubble.Velocity.x = 0;
    L->CurrentBubble.Velocity.y = 0;
    
    L->NextBubble.Type = 1 + rand() % 3;
    L->NextBubble.Center.x = L->LaunchPosition.x + 39;
    L->NextBubble.Center.y = L->LaunchPosition.y + 39;
    L->NextBubble.Velocity.x = 0;
    L->NextBubble.Velocity.y = 0;
    
    // reset points
    L->CurrentShotPoints = NULL;
    L->CurrentComboCounter = NULL;
    
    // transition immediately
    Launcher_ChangeState( L, Launcher_Aim );
    
    if( L->HasCharacter )
      L->OwningState = LauncherOwning_Used;
    else
      L->OwningState = LauncherOwning_Unused;
    
    // do not reset aim enable flag
    // (this will be controlled by the scene)
}

// ---------------------------------------------------------

void Launcher_RunState_Aim( Launcher* L )
{
    // do nothing??
}

// ---------------------------------------------------------

void Launcher_RunState_Shoot( Launcher* L )
{
    // update bubble movement
    if( Launcher_UpdateBubbleMovement( L ) )
    {
        if( L->OwnerField->LastShotConnectedBubbles > 2 )
          Launcher_ChangeState( L, Launcher_BubbleBurst );
        else
          Launcher_ChangeState( L, Launcher_BubblePlaced );
    }
}

// ---------------------------------------------------------

void Launcher_RunState_BubbleBurst( Launcher* L )
{
    // move new current bubble
    int Offset = max( 0, 39 - 3*L->ElapsedFrames );
    L->CurrentBubble.Center.x = L->LaunchPosition.x + Offset;
    L->CurrentBubble.Center.y = L->LaunchPosition.y + Offset;
    
    // update animation
    
    // at the end of the animation, change state
    if( L->ElapsedFrames >= 23 )
      Launcher_ChangeState( L, Launcher_Aim );
}

// ---------------------------------------------------------

void Launcher_RunState_BubblePlaced( Launcher* L )
{
    // move new current bubble
    int Offset = max( 0, 39 - 3*L->ElapsedFrames );
    L->CurrentBubble.Center.x = L->LaunchPosition.x + Offset;
    L->CurrentBubble.Center.y = L->LaunchPosition.y + Offset;
    
    // at the end of the animation, change state
    if( L->ElapsedFrames >= 13 )
      Launcher_ChangeState( L, Launcher_Aim );
}

// ---------------------------------------------------------

void Launcher_RunState_Unused( Launcher* L )
{
    // do nothing??
}

// ---------------------------------------------------------

void Launcher_RunStateMachine( Launcher* L )
{
    if( L->State == Launcher_Aim )
      Launcher_RunState_Aim( L );
    
    else if( L->State == Launcher_Shoot )
      Launcher_RunState_Shoot( L );
    
    else if( L->State == Launcher_BubblePlaced )
      Launcher_RunState_BubblePlaced( L );
    
    else if( L->State == Launcher_BubbleBurst )
      Launcher_RunState_BubbleBurst( L );
    
    else if( L->State == Launcher_Initialize )
      Launcher_RunState_Initialize( L );
    
    // count time in present state
    L->ElapsedFrames++;
}


// ---------------------------------------------------------
//   LAUNCHER: DRAWING FUNCTIONS
// ---------------------------------------------------------


void DrawPointsBurst( Vector2D* Position, int Points )
{
    int[ 8 ] PointsString;
    itoa( Points, PointsString, 10 );
    
    Vector2D DigitPosition = *Position;
    DigitPosition.y += 35;
    int* Digit = PointsString;
    
    while( *Digit )
    {
        select_region( RegionBurstZero + *Digit - '0' );
        draw_region_at( DigitPosition.x, DigitPosition.y );
        DigitPosition.x += 17;
        Digit++;
    }
}

// ---------------------------------------------------------

void DrawPointsFall( Vector2D* Position, int Points )
{
    int[ 8 ] PointsString;
    itoa( Points, PointsString, 10 );
    
    Vector2D DigitPosition = *Position;
    DigitPosition.y += 35 + 26;
    int* Digit = PointsString;
    
    // first, write the plus symbol
    select_region( RegionFallPlus );
    draw_region_at( DigitPosition.x, DigitPosition.y );
    DigitPosition.x += 13;
    
    // now write the number
    while( *Digit )
    {
        select_region( RegionFallZero + *Digit - '0' );
        draw_region_at( DigitPosition.x, DigitPosition.y );
        DigitPosition.x += 13;
        Digit++;
    }
}

// ---------------------------------------------------------

void DrawComboCounter( Vector2D* Position, int Counter )
{
    int[ 8 ] CounterString;
    itoa( Counter, CounterString, 10 );
    
    Vector2D DigitPosition = *Position;
    DigitPosition.y += 35 + 49;
    int* Digit = CounterString;
    
    // first, write the combo indicator
    select_region( RegionComboText );
    draw_region_at( DigitPosition.x, DigitPosition.y );
    DigitPosition.x += 75;
    
    // now write the number
    while( *Digit )
    {
        select_region( RegionComboZero + *Digit - '0' );
        draw_region_at( DigitPosition.x, DigitPosition.y );
        DigitPosition.x += 12;
        Digit++;
    }
}

// ---------------------------------------------------------

void Launcher_Draw( Launcher* L )
{
    // set shadow for unmanaged launchers
    if( !L->HasCharacter )
      set_multiply_color( color_gray );
    
    // draw front wheel
    select_region( RegionFrontWheel );
    set_drawing_angle( -2*(L->ArrowAngle + pi/2) );
    draw_region_rotated_at( L->LaunchPosition.x, L->LaunchPosition.y+33 );
    
    // draw side wheel
    int CrankAngle = -L->ArrowAngle*30;
    int CrankFrame = CrankAngle % 5;
    select_region( FirstRegionSideWheel + CrankFrame );
    draw_region_at( L->LaunchPosition.x - 27, L->BottomY );
    
    // draw arrow
    select_region( RegionLauncherArrow );
    set_drawing_angle( L->ArrowAngle + pi/2 );
    draw_region_rotated_at( L->LaunchPosition.x, L->LaunchPosition.y );
    
    // only show these when the launcher has a character
    if( L->HasCharacter )
    {
        // draw banner
        select_region( RegionNextBanner );
        draw_region_at( L->LaunchPosition.x + 53, L->BottomY );
        
        // draw next bubble
        select_region( FirstRegionBubbles1 + L->NextBubble.Type );
        draw_region_at( L->NextBubble.Center.x, L->NextBubble.Center.y );
        
        // draw current bubble
        select_region( FirstRegionBubbles1 + L->CurrentBubble.Type );
        draw_region_at( L->CurrentBubble.Center.x, L->CurrentBubble.Center.y );
    }
    
    // disable any shadow
    set_multiply_color( color_white );
    
    // draw bubble place animation
    if( L->State == Launcher_BubblePlaced )
    {
        int AnimationFrame = min( 6, L->ElapsedFrames / 2 );
        select_region( FirstRegionBubbleHighlight + AnimationFrame );
        draw_region_at( L->AddedBubbleCenter.x, L->AddedBubbleCenter.y );
    }
    
    // draw bubble place animation
    else if( L->State == Launcher_BubbleBurst )
    {
        int AnimationFrame = min( 5, L->ElapsedFrames / 4 );
        select_region( FirstRegionBubbleBurst + AnimationFrame );
        Playfield* P = L->OwnerField;
        
        for( int y = 0; y < P->BubblesInY; ++y )
          for( int x = 0; x < P->BubblesInX; ++x )
            if( L->BurstBubbles[y][x] == -1 )
            {
                int CenterX = P->BubbleMinX + BubbleDistanceX*x;
                int CenterY = P->BubbleMinY + BubbleDistanceY*y;
                
                if( y % 2 )
                  CenterX += BubbleDistanceX/2;
                
                draw_region_at( CenterX, CenterY );
            }
    }
}
