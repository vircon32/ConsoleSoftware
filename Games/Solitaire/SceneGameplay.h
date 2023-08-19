// ---------------------------------------------------------
//   GAMEPLAY: AUXILIARY FUNCTIONS
// ---------------------------------------------------------


void PickFromCursor()
{
    // save original pick position
    PickCursor = CurrentCursor;
    Cursor_GetDrawPosition( &PickCursor, &PickedHandX, &PickedHandY );
    
    // send contents to transition stack
    if( CurrentCursor.CurrentLadder >= 0 )
      CardStack_TransferSubstack
      ( &LadderStacks[ CurrentCursor.CurrentLadder ].FaceUpStack,
        &MovingStack,
        CurrentCursor.PositionInLadder
      );
        
    else if( CurrentCursor.CurrentSuit >= 0 )
      CardStack_TransferTop( &SuitStacks[ CurrentCursor.CurrentSuit ], &MovingStack );
    
    else
    {
        CardStack_TransferTop( &DrawnStack.FaceUpStack, &MovingStack );
        
        // update state of the displaced top cards
        // (we just picked the rightmost card)
        if( DrawnStack.CardsMovedRight > 0 )
          DrawnStack.CardsMovedRight--;
    }
}

// ---------------------------------------------------------

void DropToCursor( CursorPosition* DropPosition )
{
    if( DropPosition->CurrentSuit >= 0 )
      CardStack_TransferTop( &MovingStack, &SuitStacks[ DropPosition->CurrentSuit ] );
    
    else if( DropPosition->CurrentLadder >= 0 )
    {
        CardStack_TransferSubstack
        (   &MovingStack,
            &LadderStacks[ DropPosition->CurrentLadder ].FaceUpStack,
            0  // always drop the whole hand
        );
    }
    
    else
    {
        CardStack_TransferTop( &MovingStack, &DrawnStack.FaceUpStack );
        
        // update state of the displaced top cards
        // (we just unpicked the rightmost card)
        if( DrawnStack.CardsMovedRight < (DrawnCards - 1) )
          if( DrawnStack.FaceUpStack.NumberOfCards > (DrawnStack.CardsMovedRight+1) )
            DrawnStack.CardsMovedRight++;
    }
}


// ---------------------------------------------------------
//   GAMEPLAY: STATE MANAGEMENT
// ---------------------------------------------------------


int Gameplay_ElapsedFrames;
int Gameplay_DealtLadders;
int Gameplay_FlippedLadder;
Card Gameplay_FlippedCard;

// ---------------------------------------------------------

void Gameplay_ChangeState( int NewState )
{
    GameState = NewState;
    Gameplay_ElapsedFrames = 0;
    
    // some transitions have specific actions
    if( NewState == Gameplay_ReplaceDrawDeck )
    {
        play_sound( SoundCardSlide );
        
        // transfer all drawn cards to the moving stack
        CardStack_TransferSubstack( &DrawnStack.FaceUpStack, &MovingStack, 0 );
    }
    
    else if( NewState == Gameplay_Drawing )
    {
        play_sound( SoundDraw );
        
        // send up to N top cards to the moving stack
        // (depending on the game mode)
        int CardsToDraw = min( DrawnCards, DeckStack.NumberOfCards );
        
        for( int i = 0; i < CardsToDraw; i++ )
          CardStack_TransferTop( &DeckStack, &MovingStack );
    }
    
    else if( NewState == Gameplay_Waiting )
    {
        // re-check actions every time we go back to
        // waiting (only state where actions are allowed)
        bool HandHasCards = (HandStack.NumberOfCards > 0);
        ActionsAllowed[ DrawNext ] = !HandHasCards;
        ActionsAllowed[ PickHand ] = !HandHasCards;
        ActionsAllowed[ UndoPick ] = HandHasCards;
        ActionsAllowed[ DropHand ] = HandHasCards;
        ActionsAllowed[ QuitGame ] = true;
    }
    
    else if( NewState == Gameplay_DealingLadders )
    {
        Gameplay_DealtLadders = 0;
        play_sound( SoundDealLadders );
    }
      
    else if( NewState == Gameplay_FlippingLadders )
      Gameplay_DealtLadders = 0;
    
    else if( NewState == Gameplay_Moving )
      play_sound( SoundMenuMove );
    
    else if( NewState == Gameplay_Picking )
    {
        PickFromCursor();
        play_sound( SoundCardSlide );
    }
    
    else if( NewState == Gameplay_Dropping )
    {
        CardStack_TransferSubstack( &HandStack, &MovingStack, 0 );
        play_sound( SoundCardSlide );
    }
    
    else if( NewState == Gameplay_Unpicking )
    {
        CardStack_TransferSubstack( &HandStack, &MovingStack, 0 );
        play_sound( SoundMenuCancel );
    }
    
    else if( NewState == Gameplay_FlipLadderTop )
      play_sound( SoundCardFlip );
    
    else if( NewState == Gameplay_WrongMove )
      play_sound( SoundWrongMove );
}


// ---------------------------------------------------------
//   GAMEPLAY: DRAWING FUNCTIONS
// ---------------------------------------------------------


void Gameplay_DrawScene()
{
    // draw background
    select_region( RegionTableMat );
    draw_region_at( 0, 0 );
    
    // draw the drawing deck, if it has any cards
    if( DeckStack.NumberOfCards > 0 )
      Card_DrawBack( 134, 10 );
    
    // draw the already drawn cards
    DrawDrawnStack();
    
    // draw all suit stacks
    DrawSuitStacks();
    
    // draw all ladder stacks
    DrawLadderStacks();
    
    // draw allowed actions
    // (left for each scene to configure)
    DrawAllowedActions();
    
    // draw hand GUI and contents
    DrawHand();
    
    // draw hand picked cards at their
    // original place with a shadow effect
    if( HandStack.NumberOfCards > 0 )
    {
        set_multiply_color( make_color_rgb( 128,128,255 ) );
        DrawStackFaceUp( &HandStack, PickedHandX, PickedHandY );
        set_multiply_color( color_white );
    }
}


// ---------------------------------------------------------
//   GAMEPLAY: STATE-SPECIFIC UPDATES
// ---------------------------------------------------------


void Gameplay_RunState_Initialize()
{
    // initialize the game
    ResetDeck();
    ShuffleDeck();
    ResetActions();
    
    // place cursor at first ladder's top
    CurrentCursor.CurrentSuit = -1;
    CurrentCursor.CurrentLadder = 0;
    CurrentCursor.PositionInLadder = 0;
    
    // transition immediately
    Gameplay_ChangeState( Gameplay_FadeIn );
}

// ---------------------------------------------------------

void Gameplay_RunState_FadeIn()
{
    // draw scene
    Gameplay_DrawScene();
    
    // fade-in effect
    int BlackLevel = max( 0, 255 - 255 * Gameplay_ElapsedFrames / 60 );
    clear_screen( make_color_rgba( 0,0,0,BlackLevel ) );
    
    // change state when finished
    if( Gameplay_ElapsedFrames >= 60 )
      Gameplay_ChangeState( Gameplay_DealingLadders );   
}

// ---------------------------------------------------------

void Gameplay_RunState_DealingLadders()
{
    // draw scene
    Gameplay_DrawScene();
    
    // determine positions for dealt card movement
    CardStack* ThisLadderStack = &LadderStacks[ Gameplay_DealtLadders ].FaceDownStack;
    int CardsInThisLadder = ThisLadderStack->NumberOfCards;
    int FinalX = LadderStacksFirstX + Gameplay_DealtLadders * LadderStacksStepX;
    int FinalY = LadderStacksY + CardsInThisLadder * FaceDownStackStepY;
    
    // draw the card currently being dealt
    float MotionProportion = Gameplay_ElapsedFrames / 4.0;
    int DrawX = DeckStackX + (FinalX - DeckStackX) * fmin( MotionProportion, 1.0 );
    int DrawY = DeckStackY + (FinalY - DeckStackY) * fmin( MotionProportion, 1.0 );
    Card_DrawBack( DrawX, DrawY );
    
    // count the dealt cards in this ladder
    if( Gameplay_ElapsedFrames > 4 )
    {
        Gameplay_ElapsedFrames = 0;
        
        // actually transfer the card
        CardStack_TransferTop( &DeckStack, ThisLadderStack );
        CardsInThisLadder++;
        
        // check if we are finished
        if( CardsInThisLadder >= (Gameplay_DealtLadders + 1) )
          Gameplay_DealtLadders++;
    }
    
    // if all ladders were fully dealt this part is done
    if( Gameplay_DealtLadders >= NumberOfLadders )
      Gameplay_ChangeState( Gameplay_FlippingLadders );
}

// ---------------------------------------------------------

void Gameplay_RunState_FlippingLadders()
{
    // draw scene
    Gameplay_DrawScene();
    
    LadderCardStack* ThisLadder = &LadderStacks[ Gameplay_DealtLadders ];
    
    // take the last card from the ladder
    if( Gameplay_ElapsedFrames == 1 )
      CardStack_Pop( &ThisLadder->FaceDownStack, &Gameplay_FlippedCard );
    
    // draw the card currently being flipped
    int CardX = LadderStacksFirstX + Gameplay_DealtLadders * LadderStacksStepX;
    int CardY = LadderStacksY + Gameplay_DealtLadders * FaceDownStackStepY;
    Card_DrawFlipping( &Gameplay_FlippedCard, CardX, CardY, Gameplay_ElapsedFrames / 10.0 );
    
    // play flip sound
    if( Gameplay_ElapsedFrames == 1 )
      play_sound( SoundCardFlip );
        
    // change stack when finished
    if( Gameplay_ElapsedFrames >= 10 )
    {
        // actually transfer the card to the face up stack
        CardStack_Push( &ThisLadder->FaceUpStack, &Gameplay_FlippedCard );
        
        // reset for next stack
        Gameplay_ElapsedFrames = 0;
        Gameplay_DealtLadders++;
    }
    
    // if all ladders were flipped this part is done
    if( Gameplay_DealtLadders >= NumberOfLadders )
    {
        // play gameplay music
        play_sound_in_channel( MusicGameplay, ChannelMusic );
    
        Gameplay_ChangeState( Gameplay_Waiting );
    }
}

// ---------------------------------------------------------

void Gameplay_RunState_Waiting()
{
    // draw scene
    Gameplay_DrawScene();
    
    // draw cursor animation
    DrawCursor( (Gameplay_ElapsedFrames / 12) % 2 );
    
    // obtain the current movement direction
    int CursorDeltaX = 0;
    int CursorDeltaY = 0;
    
    if( gamepad_left() == 1 )  CursorDeltaX = -1;
    if( gamepad_up()   == 1 )  CursorDeltaY = -1;
    
    if( gamepad_right() == 1 ) CursorDeltaX = 1;
    if( gamepad_down()  == 1 ) CursorDeltaY = 1;
    
    // when requested, move the cursor
    if( CursorDeltaX || CursorDeltaY )
    {
        // determine cursor destination
        if( CursorDeltaX < 0 )  Cursor_SetDestinationLeft();
        if( CursorDeltaX > 0 )  Cursor_SetDestinationRight();
        if( CursorDeltaY < 0 )  Cursor_SetDestinationUp();
        if( CursorDeltaY > 0 )  Cursor_SetDestinationDown();
        
        // now change state for the transition
        Gameplay_ChangeState( Gameplay_Moving );
        return;
    }
    
    // to check actions we will need this
    bool HandHasCards = (HandStack.NumberOfCards > 0);
    
    // button X draws the next cards
    if( gamepad_button_x() == 1 )
    {
        if( IsDrawAllowed() )
        {
            if( DeckStack.NumberOfCards > 0 )
              Gameplay_ChangeState( Gameplay_Drawing );
            else
              Gameplay_ChangeState( Gameplay_ReplaceDrawDeck );
        }
        else
        {
            // play error sound?
        }
        
        return;
    }
    
    // button B undoes the last pick, returning
    // the cards in hand to their previous position
    if( gamepad_button_b() == 1 )
    {
        if( HandHasCards )
          Gameplay_ChangeState( Gameplay_Unpicking );
        
        return;
    }
    
    // button A may either pick or drop
    // depending on whether we have cards in hand
    if( gamepad_button_a() == 1 )
    {
        if( HandHasCards )
        {
            // special case: when dropping cards to
            // their pick position treat it as unpick
            if( CurrentCursor == PickCursor )
            {
                Gameplay_ChangeState( Gameplay_Unpicking );
                return;
            }
            
            // otherwise check drop position
            if( IsDropAllowed() )
              Gameplay_ChangeState( Gameplay_Dropping );
            else
              Gameplay_ChangeState( Gameplay_WrongMove );
        }
        
        else
        {
            if( IsPickAllowed() )
              Gameplay_ChangeState( Gameplay_Picking );
            else
              Gameplay_ChangeState( Gameplay_WrongMove );
        }
        
        return;
    }
    
    // button Start will show the quit game dialog
    if( gamepad_button_start() == 1 )
    {
        pause_channel( ChannelMusic );
        GameScene = Scene_QuitGame;
        GameState = QuitGame_Initialize;
        return;
    }
}

// ---------------------------------------------------------

void Gameplay_RunState_Moving()
{
    // draw scene
    Gameplay_DrawScene();
    
    // perform cursor transition
    int InitialX, InitialY;
    Cursor_GetDrawPosition( &CurrentCursor, &InitialX, &InitialY );
    
    int FinalX, FinalY;
    Cursor_GetDrawPosition( &NextCursor, &FinalX, &FinalY );
    
    // always draw full-card cursor without animating
    int DrawX = InitialX + (FinalX - InitialX) * Gameplay_ElapsedFrames / 10;
    int DrawY = InitialY + (FinalY - InitialY) * Gameplay_ElapsedFrames / 10;
    select_region( FirstRegionCursorCard );
    draw_region_at( DrawX, DrawY );
    
    // when finished, actually update cursor
    if( Gameplay_ElapsedFrames >= 10 )
    {
        CurrentCursor = NextCursor;
        Gameplay_ChangeState( Gameplay_Waiting );
    }
}

// ---------------------------------------------------------

void Gameplay_RunState_ReplaceDrawDeck()
{
    // draw scene
    Gameplay_DrawScene();
    
    // part 1: drawn cards disappear
    if( Gameplay_ElapsedFrames <= 10 )
    {
        int DrawY = DrawnStackY - Gameplay_ElapsedFrames * 11;
        Card_DrawFront( CardStack_Top( &MovingStack ), DrawnStackFirstX, DrawY );
    }
    
    // part 2: new deck appears
    else
    {
        int DrawY = DeckStackY - 110 + (Gameplay_ElapsedFrames - 10) * 11;
        Card_DrawBack( DeckStackX, DrawY );
    }
    
    // when finished, actually replace deck
    if( Gameplay_ElapsedFrames >= 20 )
    {
        while( MovingStack.NumberOfCards > 0 )
          CardStack_TransferTop( &MovingStack, &DeckStack );
        
        Gameplay_ChangeState( Gameplay_Drawing );
    }
}

// ---------------------------------------------------------

void Gameplay_RunState_Drawing()
{
    // draw scene
    Gameplay_DrawScene();
    
    // draw the cards with a flipping
    for( int i = 0; i < MovingStack.NumberOfCards; i++ )
    {
        float MotionProportion = Gameplay_ElapsedFrames / 20.0;
        int FinalX = DrawnStackFirstX + i * DrawnStackStepX;
        int DrawX = DeckStackX + (FinalX - DeckStackX) * MotionProportion;
        Card_DrawFlipping( &MovingStack.Cards[ i ], DrawX, DrawnStackY, MotionProportion );
    }
    
    // when finished, actually draw cards
    if( Gameplay_ElapsedFrames >= 20 )
    {
        int CardsBeingDrawn = MovingStack.NumberOfCards;
        DrawnStack.CardsMovedRight = CardsBeingDrawn - 1;
        CardStack_TransferSubstack( &MovingStack, &DrawnStack.FaceUpStack, 0 );
        
        // go back
        Gameplay_ChangeState( Gameplay_Waiting );
    }
}

// ---------------------------------------------------------

void Gameplay_RunState_FlipLadderTop()
{
    // draw scene
    Gameplay_DrawScene();
    
    // draw the card currently being flipped
    int CardX = LadderStacksFirstX + LadderStacksStepX * Gameplay_FlippedLadder;
    int CardY = LadderStacksY + FaceDownStackStepY * LadderStacks[ Gameplay_FlippedLadder ].FaceDownStack.NumberOfCards;
    Card_DrawFlipping( &Gameplay_FlippedCard, CardX, CardY, Gameplay_ElapsedFrames / 20.0 );
    
    // when finished, actually flip card
    if( Gameplay_ElapsedFrames >= 20 )
    {
        CardStack_Push( &LadderStacks[ Gameplay_FlippedLadder ].FaceUpStack, &Gameplay_FlippedCard );
        Gameplay_ChangeState( Gameplay_Waiting );
    }
}

// ---------------------------------------------------------

void Gameplay_RunState_Picking()
{
    // draw scene
    Gameplay_DrawScene();
    
    // during transition draw shadow picked cards too
    set_multiply_color( make_color_rgb( 128,128,255 ) );
    DrawStackFaceUp( &MovingStack, PickedHandX, PickedHandY );
    set_multiply_color( color_white );
    
    // draw picked cards in motion
    int DrawX = PickedHandX + (HandStackX - PickedHandX) * Gameplay_ElapsedFrames / 10;
    int DrawY = PickedHandY + (HandStackY - PickedHandY) * Gameplay_ElapsedFrames / 10;
    DrawStackFaceUp( &MovingStack, DrawX, DrawY );
    
    // when finished, actually pick cards
    if( Gameplay_ElapsedFrames >= 10 )
    {
        CardStack_TransferSubstack( &MovingStack, &HandStack, 0 );
        play_sound( SoundCardDrop );
        Gameplay_ChangeState( Gameplay_Waiting );
    }
}

// ---------------------------------------------------------

void Gameplay_RunState_Unpicking()
{
    // draw scene
    Gameplay_DrawScene();
    
    // during transition draw shadow picked cards too
    set_multiply_color( make_color_rgb( 128,128,255 ) );
    DrawStackFaceUp( &MovingStack, PickedHandX, PickedHandY );
    set_multiply_color( color_white );
    
    // draw dropped cards in motion
    int DrawX = HandStackX + (PickedHandX - HandStackX) * Gameplay_ElapsedFrames / 10;
    int DrawY = HandStackY + (PickedHandY - HandStackY) * Gameplay_ElapsedFrames / 10;
    DrawStackFaceUp( &MovingStack, DrawX, DrawY );
    
    // when finished, actually drop cards
    if( Gameplay_ElapsedFrames >= 10 )
    {
        DropToCursor( &PickCursor );
        play_sound( SoundCardDrop );
        Gameplay_ChangeState( Gameplay_Waiting );
        
        // also make sure the cursor is not left
        // in a non-existing ladder position
        if( CurrentCursor.CurrentLadder >= 0 )
        {
            int LadderCards = LadderStacks[ CurrentCursor.CurrentLadder ].FaceUpStack.NumberOfCards;
            
            if( CurrentCursor.PositionInLadder >= LadderCards )
              CurrentCursor.PositionInLadder = max( 0, LadderCards-1 );
        }
    }
}

// ---------------------------------------------------------

void Gameplay_RunState_Dropping()
{
    // draw scene
    Gameplay_DrawScene();
    
    // draw dropped cards in motion
    int FinalX, FinalY;
    Cursor_GetDrawPosition( &CurrentCursor, &FinalX, &FinalY );
    
    int DrawX = HandStackX + (FinalX - HandStackX) * Gameplay_ElapsedFrames / 10;
    int DrawY = HandStackY + (FinalY - HandStackY) * Gameplay_ElapsedFrames / 10;
    DrawStackFaceUp( &MovingStack, DrawX, DrawY );
    
    // when finished, actually drop cards
    if( Gameplay_ElapsedFrames >= 10 )
    {
        DropToCursor( &CurrentCursor );
        
        // after a drop to ladders, check if we need to flip cards
        for( int Ladder = 0; Ladder < NumberOfLadders; Ladder++ )
          if( LadderStacks[ Ladder ].FaceUpStack.NumberOfCards == 0 )
            if( LadderStacks[ Ladder ].FaceDownStack.NumberOfCards > 0 )
            {
                Gameplay_ChangeState( Gameplay_FlipLadderTop );
                Gameplay_FlippedLadder = Ladder;
                CardStack_Pop( &LadderStacks[ Ladder ].FaceDownStack, &Gameplay_FlippedCard );
                return;
            }
        
        // after a drop to suits, check if we have won the game
        if( GameHasEnded() )
        {
            GameScene = Scene_Win;
            GameState = Win_Initialize;
            return;
        }
        
        play_sound( SoundCardDrop );
        Gameplay_ChangeState( Gameplay_Waiting );
    }
}

// ---------------------------------------------------------

void Gameplay_RunState_WrongMove()
{
    // darken the scene
    Gameplay_DrawScene();
    DrawCursor( 0 );
    clear_screen( make_color_rgba( 0,0,0,128 ) );
    
    // show the correct dialog
    if( HandStack.NumberOfCards > 0 )
      select_region( RegionDialogWrongDrop );
    else
      select_region( RegionDialogWrongPick );
      
    draw_region_at( 363, screen_height/2 );
    
    // wait for user to press a button
    if( gamepad_button_a() == 1 || gamepad_button_start() == 1 )
      if( Gameplay_ElapsedFrames >= 20 )
      {
          play_sound( SoundMenuCancel );
          Gameplay_ChangeState( Gameplay_Waiting );
      }
}


// ---------------------------------------------------------
//   GAMEPLAY: GENERAL UPDATE FUNCTION
// ---------------------------------------------------------


void Gameplay_RunStateMachine()
{
    switch( GameState )
    {
        case Gameplay_Initialize:
          Gameplay_RunState_Initialize();
          break;
        case Gameplay_FadeIn:
          Gameplay_RunState_FadeIn();
          break;
        case Gameplay_DealingLadders:
          Gameplay_RunState_DealingLadders();
          break;
        case Gameplay_FlippingLadders:
          Gameplay_RunState_FlippingLadders();
          break;
        case Gameplay_Waiting:
          Gameplay_RunState_Waiting();
          break;
        case Gameplay_Moving:
          Gameplay_RunState_Moving();
          break;
        case Gameplay_ReplaceDrawDeck:
          Gameplay_RunState_ReplaceDrawDeck();
          break;
        case Gameplay_Drawing:
          Gameplay_RunState_Drawing();
          break;
        case Gameplay_FlipLadderTop:
          Gameplay_RunState_FlipLadderTop();
          break;
        case Gameplay_Picking:
          Gameplay_RunState_Picking();
          break;
        case Gameplay_Unpicking:
          Gameplay_RunState_Unpicking();
          break;
        case Gameplay_Dropping:
          Gameplay_RunState_Dropping();
          break;
        case Gameplay_WrongMove:
          Gameplay_RunState_WrongMove();
          break;
    }
    
    // count the frames in current state
    Gameplay_ElapsedFrames++;
}
