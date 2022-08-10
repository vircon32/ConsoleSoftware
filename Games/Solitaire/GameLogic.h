// ---------------------------------------------------------
//   GAME RELATED DEFINITIONS
// ---------------------------------------------------------


// general deck-level definitions
#define NumberOfSuits 4
#define CardsInSuit (10+3)
#define CardsInDeck (NumberOfSuits * CardsInSuit)

// other game definitions
#define NumberOfLadders 7
#define NumberOfActions 5


// ---------------------------------------------------------
//   CARDS
// ---------------------------------------------------------


// same order as drawn in our texture
// so we can quickly locate each card
enum Suits
{
    Diamonds,
    Clubs,
    Hearts,
    Spades
};

// ---------------------------------------------------------

struct Card
{
    int Suit;    // store as int instead of enum for flexibility
    int Value;   // from 0 (ace) to 12 (king)
};

// ---------------------------------------------------------

bool Card_IsRed( Card* C )
{
    return (C->Suit == Hearts || C->Suit == Diamonds);
}


// ---------------------------------------------------------
//   STACKS OF CARDS
// ---------------------------------------------------------


// regular stack, implemented as a manual list / dynamic array
struct CardStack
{
    Card[ CardsInDeck ] Cards;
    int NumberOfCards;
};

// ---------------------------------------------------------

// special stacks
struct DrawnCardStack
{
    CardStack FaceUpStack;
    int CardsMovedRight;    // either 0 (all left), 1 (1 at middle) or 2 (1 at middle, 1 at right)
};

struct LadderCardStack
{
    CardStack FaceDownStack;
    CardStack FaceUpStack;
};

// ---------------------------------------------------------

// declare our stacks as globals
CardStack[ NumberOfSuits ] SuitStacks;  // it will be drawn in a special way! (just top card, if any)
CardStack DeckStack;   // it will be drawn in a special way! (just a card back, if any)
CardStack HandStack;
CardStack MovingStack;

// special stacks
LadderCardStack[ NumberOfLadders ] LadderStacks;
DrawnCardStack DrawnStack;  // it will be drawn in a special way! (just top 3 cards??)


// ---------------------------------------------------------
//   STACK METHODS & LOGIC
// ---------------------------------------------------------


Card* CardStack_Top( CardStack* S )
{
    return &S->Cards[ S->NumberOfCards - 1 ];
}

// ---------------------------------------------------------

void CardStack_Pop( CardStack* S, Card* PoppedCard )
{
    *PoppedCard = *CardStack_Top( S );
    S->NumberOfCards--;
}

// ---------------------------------------------------------

void CardStack_Push( CardStack* S, Card* PushedCard )
{
    S->NumberOfCards++;
    *CardStack_Top( S ) = *PushedCard;
}

// ---------------------------------------------------------

void CardStack_TransferTop( CardStack* Source, CardStack* Destination )
{
    Card PoppedSourceTop;
    CardStack_Pop( Source, &PoppedSourceTop );
    CardStack_Push( Destination, &PoppedSourceTop );
}

// ---------------------------------------------------------

void CardStack_TransferSubstack( CardStack* Source, CardStack* Destination, int FirstPositionToTransfer )
{
    // copy the cards in order
    for( int Position = FirstPositionToTransfer; Position < Source->NumberOfCards; ++Position )
      CardStack_Push( Destination, &Source->Cards[ Position ] );
    
    // remove cards from source by just adjusting stack size
    Source->NumberOfCards = FirstPositionToTransfer;
}


// ---------------------------------------------------------
//   GAME-WIDE LOGIC
// ---------------------------------------------------------


void ResetDeck()
{
    // make all cards be in the deck stack
    DeckStack.NumberOfCards = CardsInDeck;
    
    // none of the other stacks must have any cards
    for( int Suit = 0; Suit < NumberOfSuits; Suit++ )
      SuitStacks[ Suit ].NumberOfCards = 0;
    
    HandStack.NumberOfCards = 0;
    MovingStack.NumberOfCards = 0;
    
    for( int Ladder = 0; Ladder < NumberOfLadders; Ladder++ )
    {
        LadderStacks[ Ladder ].FaceDownStack.NumberOfCards = 0;
        LadderStacks[ Ladder ].FaceUpStack.NumberOfCards = 0;
    }
    
    DrawnStack.FaceUpStack.NumberOfCards = 0;
    DrawnStack.CardsMovedRight = 0;
    
    // now generate all cards in default order
    for( int Suit = 0; Suit < NumberOfSuits; Suit++ )
      for( int Value = 0; Value < CardsInSuit; Value++ )
      {
          DeckStack.Cards[ CardsInSuit * Suit + Value ].Suit = Suit;
          DeckStack.Cards[ CardsInSuit * Suit + Value ].Value = Value;
      }
}

// ---------------------------------------------------------

void ShuffleDeck()
{
    Card SwapCard;
    
    // we will make 200 card swaps within the deck
    for( int i = 0; i < 200; i++ )
    {
        int Position1 = rand() % CardsInDeck;
        int Position2 = rand() % CardsInDeck;
        
        SwapCard = DeckStack.Cards[ Position1 ];
        DeckStack.Cards[ Position1 ] = DeckStack.Cards[ Position2 ];
        DeckStack.Cards[ Position2 ] = SwapCard;
    }
}

// ---------------------------------------------------------

bool GameHasEnded()
{
    // game ends when the 4 suit stacks have all their cards
    for( int Suit = 0; Suit < NumberOfSuits; Suit++ )
      if( SuitStacks[ Suit ].NumberOfCards < CardsInSuit )
        return false;
    
    return true;
}

// ---------------------------------------------------------

bool LadderStackAcceptsCard( int LadderIndex, Card* NextCard )
{
    // CASE 1: for an empty ladder, any king is allowed
    if( LadderStacks[ LadderIndex ].FaceUpStack.NumberOfCards == 0 )
      return (NextCard->Value == 12);
    
    // CASE 2: otherwise follow the alternating color ladder
    Card* TopCard = CardStack_Top( &LadderStacks[ LadderIndex ].FaceUpStack );
    
    // validate color
    if( Card_IsRed( TopCard ) == Card_IsRed( NextCard ) )
      return false;
    
    // validate value
    return (NextCard->Value == (TopCard->Value - 1));
}

// ---------------------------------------------------------

bool SuitStackAcceptsCard( int Suit, Card* NextCard )
{
    // validate suit
    if( NextCard->Suit != Suit )
      return false;
    
    // validate value
    return (NextCard->Value == SuitStacks[ Suit ].NumberOfCards);
}


// ---------------------------------------------------------
//   CURSOR POSITION MANAGEMENT
// ---------------------------------------------------------


struct CursorPosition
{
    int CurrentLadder;    // -1 when not at ladder stacks
    int CurrentSuit;      // -1 when not at suit stacks
    int PositionInLadder; // 0 at ladder face up base, increases downwards
};

CursorPosition CurrentCursor;  // actual current state
CursorPosition NextCursor;     // used for transitions
CursorPosition PickCursor;     // stored for undo pick
int PickedHandX, PickedHandY;

// ---------------------------------------------------------

bool Cursor_AtTopPosition( CursorPosition* P )
{
    if( P->CurrentLadder < 0 )
      return true;
    
    int CardsInLadder = LadderStacks[ P->CurrentLadder ].FaceUpStack.NumberOfCards;
    return P->PositionInLadder >= (CardsInLadder - 1);
}

// ---------------------------------------------------------

void Cursor_SetDestinationLeft()
{
    // start with the same position as a base
    NextCursor = CurrentCursor;
    
    // CASE 1: Cursor is at ladder stacks
    if( CurrentCursor.CurrentLadder >= 0 )
    {
        // advance left and wrap around
        NextCursor.CurrentLadder--;
        
        if( NextCursor.CurrentLadder < 0 )
          NextCursor.CurrentLadder = NumberOfLadders - 1;
        
        // CASE 1-A: no cards in hand
        if( HandStack.NumberOfCards <= 0 )
        {
            // adjust height to next ladder's number of cards
            int NextLadderCards = LadderStacks[ NextCursor.CurrentLadder ].FaceUpStack.NumberOfCards;
            
            if( CurrentCursor.PositionInLadder >= NextLadderCards )
              NextCursor.PositionInLadder = max( 0, NextLadderCards - 1 );
            
            return;
        }
        
        // CASE 1-B: card has 1 or more cards
        else
        {
            // cursor will be at the 1st free position
            int NextLadderCards = LadderStacks[ NextCursor.CurrentLadder ].FaceUpStack.NumberOfCards;
            NextCursor.PositionInLadder = NextLadderCards;
            return;
        }
    }
    
    // CASE 2: Cursor is at suit stacks
    if( CurrentCursor.CurrentSuit >= 0 )
    {
        // just advance left; if we reach
        // -1 we will end at the drawn stack's top
        NextCursor.CurrentSuit--;
        return;
    }
    
    // CASE 3: cursor is at the drawn stack's top card
    else
    {
        // wrap around to last suit stack
        NextCursor.CurrentSuit = NumberOfSuits - 1;
        return;
    }
}

// ---------------------------------------------------------

void Cursor_SetDestinationRight()
{
    // start with the same position as a base
    NextCursor = CurrentCursor;
    
    // CASE 1: Cursor is at ladder stacks
    if( CurrentCursor.CurrentLadder >= 0 )
    {
        // advance left and wrap around
        NextCursor.CurrentLadder++;
        
        if( NextCursor.CurrentLadder >= NumberOfLadders )
          NextCursor.CurrentLadder = 0;
        
        // CASE 1-A: no cards in hand
        if( HandStack.NumberOfCards <= 0 )
        {
            int NextLadderCards = LadderStacks[ NextCursor.CurrentLadder ].FaceUpStack.NumberOfCards;
            
            // adjust height to next ladder's number of cards
            if( CurrentCursor.PositionInLadder >= NextLadderCards )
              NextCursor.PositionInLadder = max( 0, NextLadderCards - 1 );
            
            return;
        }
        
        // CASE 1-B: card has 1 or more cards
        else
        {
            // cursor will be at the 1st free position
            int NextLadderCards = LadderStacks[ NextCursor.CurrentLadder ].FaceUpStack.NumberOfCards;
            NextCursor.PositionInLadder = NextLadderCards;
            return;
        }
    }
    
    // CASE 2: Cursor is at suit stacks
    if( CurrentCursor.CurrentSuit >= 0 )
    {
        // advance right and wrap
        // around to drawn stack's top
        NextCursor.CurrentSuit++;
        
        if( NextCursor.CurrentSuit >= NumberOfSuits )
          NextCursor.CurrentSuit = -1;
        
        return;
    }
    
    // CASE 3: cursor is at the drawn stack's top card
    else
    {
        // go to first suit stack
        NextCursor.CurrentSuit = 0;
        return;
    }
}

// ---------------------------------------------------------

void Cursor_SetDestinationUp()
{
    // start with the same position as a base
    NextCursor = CurrentCursor;
    
    // CASE 1: Cursor is at ladder stacks
    if( CurrentCursor.CurrentLadder >= 0 )
    {
        // with cards in hand, always go to upper positions
        if( HandStack.NumberOfCards > 0 )
        {
            NextCursor.CurrentLadder = -1;
            
            if( CurrentCursor.CurrentLadder > 2 )
              NextCursor.CurrentSuit = CurrentCursor.CurrentLadder - 3;
            else
              NextCursor.CurrentSuit = -1;
            
            return;
        }
        
        // advance up
        NextCursor.PositionInLadder--;
        
        // if ladder ends go to upper positions
        if( NextCursor.PositionInLadder < 0 )
        {
            NextCursor.CurrentLadder = -1;
            
            if( CurrentCursor.CurrentLadder > 2 )
              NextCursor.CurrentSuit = CurrentCursor.CurrentLadder - 3;
            else
              NextCursor.CurrentSuit = -1;
        }
        
        return;
    }
    
    // CASE 2: Cursor is at suit stacks
    if( CurrentCursor.CurrentSuit >= 0 )
    {
        // go to the end of the ladder below
        NextCursor.CurrentSuit = -1;
        NextCursor.CurrentLadder = CurrentCursor.CurrentSuit + 3;
        
        int LadderCards = LadderStacks[ NextCursor.CurrentLadder ].FaceUpStack.NumberOfCards;
        NextCursor.PositionInLadder = max( 0, LadderCards - 1 );
        
        // but, with cards in hand, go
        // instead to the first free position
        if( HandStack.NumberOfCards > 0 )
          NextCursor.PositionInLadder = LadderCards;
          
        return;
    }
    
    // CASE 3: cursor is at the drawn stack's top card
    else
    {
        // go to end of 3rd ladder
        NextCursor.CurrentLadder = 2;
        int LadderCards = LadderStacks[ 2 ].FaceUpStack.NumberOfCards;
        NextCursor.PositionInLadder = max( 0, LadderCards - 1 );
        
        // but, with cards in hand, go
        // instead to the first free position
        if( HandStack.NumberOfCards > 0 )
          NextCursor.PositionInLadder = LadderCards;
        
        return;
    }
}

// ---------------------------------------------------------

void Cursor_SetDestinationDown()
{
    // start with the same position as a base
    NextCursor = CurrentCursor;
    
    // CASE 1: Cursor is at ladder stacks
    if( CurrentCursor.CurrentLadder >= 0 )
    {
        // with cards in hand, always go to upper positions
        if( HandStack.NumberOfCards > 0 )
        {
            NextCursor.CurrentLadder = -1;
            
            if( CurrentCursor.CurrentLadder > 2 )
              NextCursor.CurrentSuit = CurrentCursor.CurrentLadder - 3;
            else
              NextCursor.CurrentSuit = -1;
            
            return;
        }
        
        // advance down
        NextCursor.PositionInLadder++;
        
        // if ladder ends wrap to upper positions
        int LadderCards = LadderStacks[ CurrentCursor.CurrentLadder ].FaceUpStack.NumberOfCards;
        
        if( NextCursor.PositionInLadder >= LadderCards )
        {
            NextCursor.CurrentLadder = -1;
            
            if( CurrentCursor.CurrentLadder > 2 )
              NextCursor.CurrentSuit = CurrentCursor.CurrentLadder - 3;
            else
              NextCursor.CurrentSuit = -1;
        }
        
        return;
    }
    
    // CASE 2: Cursor is at suit stacks
    if( CurrentCursor.CurrentSuit >= 0 )
    {
        // go to the base of the ladder below
        NextCursor.CurrentSuit = -1;
        NextCursor.CurrentLadder = CurrentCursor.CurrentSuit + 3;
        NextCursor.PositionInLadder = 0;
        
        // but, with cards in hand, go
        // instead to the first free position
        if( HandStack.NumberOfCards > 0 )
        {
            int LadderCards = LadderStacks[ NextCursor.CurrentLadder ].FaceUpStack.NumberOfCards;
            NextCursor.PositionInLadder = LadderCards;
        }
        
        return;
    }
    
    // CASE 3: cursor is at the drawn stack's top card
    else
    {
        // go to base of 3rd ladder
        NextCursor.CurrentLadder = 2;
        NextCursor.PositionInLadder = 0;
        
        // but, with cards in hand, go
        // instead to the first free position
        if( HandStack.NumberOfCards > 0 )
        {
            int LadderCards = LadderStacks[ 2 ].FaceUpStack.NumberOfCards;
            NextCursor.PositionInLadder = LadderCards;
        }
        
        return;
    }
}


// ---------------------------------------------------------
//   USER ACTIONS
// ---------------------------------------------------------


// usable as: FirstRegionActions + <Action>
enum Actions
{
    PickHand,
    UndoPick,
    DropHand,
    DrawNext,
    QuitGame
};

// global indicators of allowed actions
// (used mainly for drawing, not for control)
bool[ NumberOfActions ] ActionsAllowed;

// ---------------------------------------------------------

void ResetActions()
{
    // all actions are initially non allowed
    // until game logic is applied to allow them
    for( int Action = 0; Action < NumberOfActions; Action++ )
      ActionsAllowed[ Action ] = false;
}

// ---------------------------------------------------------

bool IsDrawAllowed()
{
    // can't draw between pick and drop
    if( HandStack.NumberOfCards > 0 )
      return false;
    
    // there have to be cards to draw
    int CardsToDraw = DeckStack.NumberOfCards;
    int CardsDrawn = DrawnStack.FaceUpStack.NumberOfCards;
    return (CardsToDraw + CardsDrawn > 0);
}

// ---------------------------------------------------------

bool IsPickAllowed()
{
    if( HandStack.NumberOfCards > 0 )
      return false;
    
    // now check pick position
    if( CurrentCursor.CurrentSuit >= 0 )
    {
        // picking from suit stacks is always
        // allowed when there are cards to pick
        return (SuitStacks[ CurrentCursor.CurrentSuit ].NumberOfCards > 0);
    }
    
    else if( CurrentCursor.CurrentLadder >= 0 )
    {
        // picking from ladder stacks is always
        // allowed when there are cards to pick
        return (LadderStacks[ CurrentCursor.CurrentLadder ].FaceUpStack.NumberOfCards > 0);
    }
    
    else
    {
        // picking from drawn cards is always
        // allowed when there are cards to pick
        return (DrawnStack.FaceUpStack.NumberOfCards > 0);
    }
}

// ---------------------------------------------------------

bool IsDropAllowed()
{
    if( HandStack.NumberOfCards <= 0 )
      return false;
    
    // now check drop position
    if( CurrentCursor.CurrentSuit >= 0 )
    {
        // cannot drop more than 1 card
        if( HandStack.NumberOfCards > 1 )
          return false;
        
        // dropping 1 card at suit stacks is
        // allowed if suit and order is kept
        return SuitStackAcceptsCard( CurrentCursor.CurrentSuit, &HandStack.Cards[ 0 ] );
    }
    
    else if( CurrentCursor.CurrentLadder >= 0 )
    {
        // we have to be at the ladder's first
        // free position (can'r drop in-between)
        if( CurrentCursor.PositionInLadder < LadderStacks[ CurrentCursor.CurrentLadder ].FaceUpStack.NumberOfCards )
          return false;
        
        // dropping any number of cards at ladders
        // is allowed allowed if color and order is kept
        return LadderStackAcceptsCard( CurrentCursor.CurrentLadder, &HandStack.Cards[ 0 ] );
    }
    
    else
    {
        // dropping at drawn cards is never allowed
        return false;
    }
}
