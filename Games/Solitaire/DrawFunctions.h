// ---------------------------------------------------------
//   DEFINITIONS FOR GRAPHICS
// ---------------------------------------------------------


#define DeckStackX  134
#define DeckStackY   10

#define DrawnStackY        10
#define DrawnStackFirstX  211
#define DrawnStackStepX    24

#define SuitStacksY        10
#define SuitStacksFirstX  365
#define SuitStacksStepX    77

#define LadderStacksY       126
#define LadderStacksFirstX  134
#define LadderStacksStepX    77

#define HandStackX   43
#define HandStackY  228

#define ActionsStepY  46

#define FaceUpStackStepY    17
#define FaceDownStackStepY   9


// ---------------------------------------------------------
//   DRAWING SINGLE CARDS
// ---------------------------------------------------------


void Card_DrawFront( Card* C, int x, int y )
{
    select_region( FirstRegionCardFronts + C->Suit * CardsInSuit + C->Value );
    draw_region_at( x, y );
}

// ---------------------------------------------------------

void Card_DrawBack( int x, int y )
{
    select_region( RegionCardBack );
    draw_region_at( x, y );
}

// ---------------------------------------------------------

// this will draw a card rotated around its vertical
// center axis; the angle is in radians (0 = facing camera)
void Card_DrawRotated( Card* C, int DrawX, int DrawY, float Angle )
{
    // determine normal
    float NormalZ = cos( Angle );
    float NormalX = sin( Angle );
    
    // no specific reason to choose a cubic to
    // modulate lighting; just tried different
    // factors and thought this looked the best
    float LightingFactor = fabs(NormalX * NormalX * NormalX);
    
    // when facing down invert normal
    // for lighting of the opposite side
    bool FacingDown = (NormalZ < 0);
    if( FacingDown ) NormalX = -NormalX;
    
    // CASE 1: if the rotated card would be too thin
    // just draw its side (no light or shadow applied)
    if( fabs( NormalX ) > 0.997 )
    {
        select_region( RegionCardSide );
        draw_region_at( DrawX, DrawY );;
        return;
    }
    
    // from now the drawing scale will be common
    set_drawing_scale( fabs( NormalZ ), 1 );
    
    // draw card edges; this uses 2 times the angle
    // as a tric to easily divide the motion into left
    // and right halves, showing each edge when it should
    if( sin( Angle * 2 ) < 0 )
    {
        select_region( RegionCardEdgeDark );
        draw_region_zoomed_at( DrawX + 1, DrawY );
    }
    else
    {
        select_region( RegionCardEdgeLight );
        draw_region_zoomed_at( DrawX - 1, DrawY );
    }
    
    // apply darken effect when the card side
    // seen by the camera is facing right
    // (this must be applied BEFORE drawing the card)
    if( NormalX > 0.1 )
      set_multiply_color( make_gray( 60.0 + 195.0 * (1.0-LightingFactor) ) );
    
    // CASE 2: draw card back
    if( FacingDown )
    {
        select_region( RegionCardBack );
        draw_region_zoomed_at( DrawX, DrawY );
    }
    
    // CASE 3: draw card front
    else
    {
        select_region( FirstRegionCardFronts + C->Suit * CardsInSuit + C->Value );
        draw_region_zoomed_at( DrawX, DrawY );
    }
    
    // apply highlight effect when the card
    // side seen by the camera is facing left
    // (this must be applied AFTER drawing the card)
    if( NormalX < -0.1 )
    {
        select_region( RegionCardLight );
        set_multiply_color( make_color_rgba( 255, 255, 255, 180.0 * LightingFactor ) );
        draw_region_zoomed_at( DrawX, DrawY );
    }
    
    // disable any used color multiply effect
    set_multiply_color( color_white );
}

// ---------------------------------------------------------

// The flipping animation (face down -> face down) not only
// rotates the card, but also simulates how the card would have
// to be lifted from the table, and casts the shadow under it;
// The effect is timed to progess evenly proportion (0.0 to 1.0)
void Card_DrawFlipping( Card* C, int DrawX, int DrawY, float MotionProportion )
{
    // determine rotation angle
    float Angle = pi * (1.0 + MotionProportion);
    float NormalZ = cos( Angle );
    
    // determine height
    float Height = abs( 35 * sin( Angle ) );
    float OffsetX = Height / 2;
    float OffsetY = Height / 6;
    
    // draw card shadow with less opacity the
    // higher up the card is from the table
    select_region( RegionCardShadow );
    set_drawing_scale( fabs( NormalZ ), 1 );
    set_multiply_color( make_color_rgba( 255,255,255, 255.0 * fabs( NormalZ ) ) );
    draw_region_zoomed_at( DrawX, DrawY );
    
    // draw the card at the given angle
    set_multiply_color( color_white );
    Card_DrawRotated( C, DrawX - OffsetX, DrawY - OffsetY, Angle );
}


// ---------------------------------------------------------
//   DRAWING CARD STACKS
// ---------------------------------------------------------


void DrawStackFaceUp( CardStack* S, int BaseX, int BaseY )
{
    int DrawY = BaseY;
    
    for( int Position = 0; Position < S->NumberOfCards; ++Position )
    {
        Card* CurrentCard = &S->Cards[ Position ];
        Card_DrawFront( CurrentCard, BaseX, DrawY );
        DrawY += FaceUpStackStepY;
    }
}

// ---------------------------------------------------------

void DrawStackFaceDown( CardStack* S, int BaseX, int BaseY )
{
    int DrawY = BaseY;
    select_region( RegionCardBack );
    
    for( int Position = 0; Position < S->NumberOfCards; ++Position )
    {
        draw_region_at( BaseX, DrawY );
        DrawY += FaceDownStackStepY;
    }
}

// ---------------------------------------------------------

void DrawDrawnStack()
{
    // some sanity checks
    int DrawnCards = DrawnStack.FaceUpStack.NumberOfCards;
    
    if( DrawnCards <= 0 )
      return;
    
    if( DrawnStack.CardsMovedRight > DrawnCards )
      DrawnStack.CardsMovedRight = DrawnCards;
    
    switch( DrawnStack.CardsMovedRight )
    {
        case 2:
            Card_DrawFront( &DrawnStack.FaceUpStack.Cards[ DrawnCards - 3 ], 211, 10 );
            Card_DrawFront( &DrawnStack.FaceUpStack.Cards[ DrawnCards - 2 ], 235, 10 );
            Card_DrawFront( &DrawnStack.FaceUpStack.Cards[ DrawnCards - 1 ], 259, 10 );
            break;
            
        case 1:
            Card_DrawFront( &DrawnStack.FaceUpStack.Cards[ DrawnCards - 2 ], 211, 10 );
            Card_DrawFront( &DrawnStack.FaceUpStack.Cards[ DrawnCards - 1 ], 235, 10 );
            break;
            
        default:
            Card_DrawFront( &DrawnStack.FaceUpStack.Cards[ DrawnCards - 1 ], 211, 10 );
            break;
    }
}

// ---------------------------------------------------------

void DrawSuitStacks()
{
    int DrawX = SuitStacksFirstX;
    int DrawY = SuitStacksY;
    
    for( int Suit = 0; Suit < NumberOfSuits; Suit++ )
    {
        // only the top card of each stack can be seen
        if( SuitStacks[ Suit ].NumberOfCards > 0 )
          Card_DrawFront( CardStack_Top( &SuitStacks[ Suit ] ), DrawX, DrawY );
        
        DrawX += SuitStacksStepX;
    }
}

// ---------------------------------------------------------

void DrawLadderStacks()
{
    int DrawX = LadderStacksFirstX;
    
    for( int Ladder = 0; Ladder < NumberOfLadders; Ladder++ )
    {
        // draw the face down stack
        int DrawY = LadderStacksY;
        DrawStackFaceDown( &LadderStacks[ Ladder ].FaceDownStack, DrawX, DrawY );
        
        // draw the face up stack
        int CardsFacingDown = LadderStacks[ Ladder ].FaceDownStack.NumberOfCards;
        DrawY += CardsFacingDown * FaceDownStackStepY;
        DrawStackFaceUp( &LadderStacks[ Ladder ].FaceUpStack, DrawX, DrawY );
        
        DrawX += LadderStacksStepX;
    }
}


// ---------------------------------------------------------
//   DRAWING THE CURSOR
// ---------------------------------------------------------


void Cursor_GetDrawPosition( CursorPosition* P, int* DrawX, int* DrawY )
{
    // CASE 1: Cursor is at ladder stacks
    if( P->CurrentLadder >= 0 )
    {
        *DrawX = LadderStacksFirstX + P->CurrentLadder * LadderStacksStepX;
        
        int FaceDownCards = LadderStacks[ P->CurrentLadder ].FaceDownStack.NumberOfCards;
        *DrawY = LadderStacksY + FaceDownCards * FaceDownStackStepY;
        *DrawY += P->PositionInLadder * FaceUpStackStepY;
        return;
    }
    
    // CASE 2: Cursor is at suit stacks
    if( P->CurrentSuit >= 0)
    {
        *DrawX = SuitStacksFirstX + P->CurrentSuit * SuitStacksStepX;
        *DrawY = SuitStacksY;
        return;
    }
    
    // CASE 3: cursor is at the drawn stack's top card
    *DrawY = DrawnStackY;
    *DrawX = DrawnStackFirstX + DrawnStack.CardsMovedRight * DrawnStackStepX;
    
    // special case 3-A: when empty, it looks better
    // to have the cursor at the left draw position
    if( DrawnStack.FaceUpStack.NumberOfCards == 0 )
      *DrawX = DrawnStackFirstX;
    
    // special case 3-B: when a drawn card is picked
    // it looks better to have the cursor at that place
    else if( HandStack.NumberOfCards > 0)
    {
        if( PickCursor.CurrentLadder < 0 )
          if( PickCursor.CurrentSuit < 0 )
          {
              *DrawX = PickedHandX;
              *DrawY = PickedHandY;
          }
    }
}

// ---------------------------------------------------------

void DrawCursor( int AnimationFrame )
{
    // find out where it is located
    int DrawX, DrawY;
    Cursor_GetDrawPosition( &CurrentCursor, &DrawX, &DrawY );
    
    // check if it is a top card or not
    int FirstRegion = FirstRegionCursorCard;
    
    if( !Cursor_AtTopPosition( &CurrentCursor ) )
      FirstRegion = FirstRegionCursorStack;
    
    // draw the requested animation frame
    select_region( FirstRegion + AnimationFrame );
    draw_region_at( DrawX, DrawY );
}


// ---------------------------------------------------------
//   DRAWING USER ACTIONS
// ---------------------------------------------------------


void DrawAllowedActions()
{
    int DrawY = 0;
    
    for( int i = 0; i < 5; i++ )
      if( ActionsAllowed[ i ] )
      {
          select_region( FirstRegionActions + i );
          draw_region_at( 0, DrawY );
          DrawY += ActionsStepY;
      }
}


// ---------------------------------------------------------
//   DRAWING PLAYER HAND
// ---------------------------------------------------------


void DrawHand()
{
    // draw hand GUI
    select_region( RegionHandInfo );
    draw_region_at( 0, 170 );
    
    // write info on picked cards,
    // highlighting in color cyan
    int CardsInHand = HandStack.NumberOfCards;
    set_multiply_color( color_cyan );
    
    if( CardsInHand <= 0 )
    {
        select_region( RegionTextNothing );
        draw_region_at( 6, 198 );
    }
    
    else if( CardsInHand == 1 )
    {
        select_region( RegionText1Card );
        draw_region_at( 14, 198 );
    }
    
    else if( CardsInHand < 10 )
    {
        select_region( FirstRegionDigits + CardsInHand );
        draw_region_at( 10, 198 );
        
        select_region( RegionTextNCards );
        draw_region_at( 27, 198 );
    }
    
    else
    {
        select_region( FirstRegionDigits + 1 );
        draw_region_at( 4, 198 );
        
        select_region( FirstRegionDigits + (CardsInHand-10) );
        draw_region_at( 15, 198 );
        
        select_region( RegionTextNCards );
        draw_region_at( 32, 198 );
    }
    
    // draw cards in hand
    set_multiply_color( color_white );
    DrawStackFaceUp( &HandStack, HandStackX, HandStackY );
}
