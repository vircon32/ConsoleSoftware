// include corresponding header
#include "Chip8.h"


// ---------------------------------------------------------
//   AUXILIARY CHIP-8 FUNCTIONS
// ---------------------------------------------------------


int Chip8_ReadFromRAM( int Address )
{
    // mirror Address to the first 4096 positions
    return Chip8.RAM[ Address & 0xFFF ];
}

// -----------------------------------------------------------------------------

void Chip8_WriteToRAM( int Address, int Value )
{
    // mirror Address to the first 4096 positions
    Address &= 0xFFF;
    
    // prevent writing in read-only section
    if( Address > 0x1FF )
      Chip8.RAM[ Address ] = Value;
}

// -----------------------------------------------------------------------------

// careful! this function does not perform
// screen boundary checks, and just assumes
// the received (x,y) will be a valid pixel
void Chip8_FlipPixel( int x, int y )
{
    // if in low resolution, adjust pixel
    // coordinates because our screen always
    // uses high resolution internally
    if( !Chip8.HighResolutionEnabled )
    {
        x *= 2;
        y *= 2;
    }
    
    // wrap these coordinates before drawing; this
    // works because screen dimensions are a power of 2
    x &= (SC8_ScreenWidth  - 1);
    y &= (SC8_ScreenHeight - 1);
    
    // calculate the correct display memory word
    int PixelIndex = (y * SC8_ScreenWidth) + x;
    bool PixelWasSet = Chip8.DisplayRAM[ PixelIndex ];
    bool PixelNewValue = !PixelWasSet;
    
    // on high resolution, flip just that pixel
    if( Chip8.HighResolutionEnabled )
      Chip8.DisplayRAM[ PixelIndex ] = PixelNewValue;
    
    // on low resolution, work with 2x2 blocks
    else
    {
        // first row
        Chip8.DisplayRAM[ PixelIndex   ] = PixelNewValue;
        Chip8.DisplayRAM[ PixelIndex+1 ] = PixelNewValue;
        
        // second row
        PixelIndex += SC8_ScreenWidth;
        Chip8.DisplayRAM[ PixelIndex   ] = PixelNewValue;
        Chip8.DisplayRAM[ PixelIndex+1 ] = PixelNewValue;
    }
    
    // use VF as flag to indicate a collision
    if( PixelWasSet )
      Chip8.Registers[ 0xF ] = 1;
}


// ---------------------------------------------------------
//   IMPLEMENTATION OF CPU INSTRUCTIONS
// ---------------------------------------------------------


void ClearScreen()
{
	memset( Chip8.DisplayRAM, 0, sizeof(Chip8.DisplayRAM) );
}

// ---------------------------------------------------------

void ReturnFromSubroutine()
{
    Chip8.StackPointer--;
    
    if( Chip8.StackPointer < 0 )
    {
        Chip8.ErrorHappened = true;
        strcpy( Chip8.ErrorMessage, "Error: Stack underflow.\n" "Press start to reset" );
        return;
    }
    
    Chip8.ProgramCounter = Chip8.Stack[ Chip8.StackPointer ];
    Chip8.ProgramCounter &= 0xFFFF;
}

// ---------------------------------------------------------

void CallExternalSubroutine( int AddressValue )
{
    // Obsolete, not implemented (do nothing)
    // Just use the argument to silence the warning
    AddressValue;
}

// ---------------------------------------------------------

void CallSubroutine( int AddressValue )
{
    Chip8.Stack[ Chip8.StackPointer ] = Chip8.ProgramCounter;
    Chip8.StackPointer++;
    
    if( Chip8.StackPointer >= C8_StackSize )
    {
        Chip8.ErrorHappened = true;
        strcpy( Chip8.ErrorMessage, "Error: Stack overflow.\n" "Press start to reset" );
        return;
    }
    
    Chip8.ProgramCounter = AddressValue;
    Chip8.ProgramCounter &= 0xFFFF;
}

// ---------------------------------------------------------

void JumpToAddress( int AddressValue )
{
    Chip8.ProgramCounter = AddressValue;
    Chip8.ProgramCounter &= 0xFFFF;
}

// ---------------------------------------------------------

void JumpToAddressOffset( int AddressValue )
{
    Chip8.ProgramCounter = AddressValue + Chip8.Registers[ 0 ];
    Chip8.ProgramCounter &= 0xFFFF;
}

// ---------------------------------------------------------

void SkipIfEqual( int* RegisterX, int ByteValue )
{
    if( *RegisterX == ByteValue )
    {
        Chip8.ProgramCounter += 2;
        Chip8.ProgramCounter &= 0xFFFF;
    }
}

// ---------------------------------------------------------

void SkipIfNotEqual( int* RegisterX, int ByteValue )
{
    if( *RegisterX != ByteValue )
    {
        Chip8.ProgramCounter += 2;
        Chip8.ProgramCounter &= 0xFFFF;
    }
}

// ---------------------------------------------------------

void SkipIfRegistersEqual( int* RegisterX, int* RegisterY )
{
    if( *RegisterX == *RegisterY )
    {
        Chip8.ProgramCounter += 2;
        Chip8.ProgramCounter &= 0xFFFF;
    }
}

// ---------------------------------------------------------

void SkipIfRegistersNotEqual( int* RegisterX, int* RegisterY )
{
    if( *RegisterX != *RegisterY )
    {
        Chip8.ProgramCounter += 2;
        Chip8.ProgramCounter &= 0xFFFF;
    }
}

// ---------------------------------------------------------

void LoadRegister( int* RegisterX, int ByteValue ) 
{
    *RegisterX = ByteValue;
    *RegisterX &= 0xFF;
}

// ---------------------------------------------------------

void LoadIndex( int AddressValue )
{
    Chip8.Index = AddressValue;
    Chip8.Index &= 0xFFFF;
}

// ---------------------------------------------------------

void AddValueToRegister( int* RegisterX, int ByteValue )
{
    *RegisterX += ByteValue;
    *RegisterX &= 0xFF;
}

// ---------------------------------------------------------

void RegistersCopy( int* RegisterX, int* RegisterY )
{
    *RegisterX = *RegisterY;
    *RegisterX &= 0xFF;
}

// ---------------------------------------------------------

void RegistersOR( int* RegisterX, int* RegisterY )
{
    *RegisterX |= *RegisterY;
    *RegisterX &= 0xFF;
}

// ---------------------------------------------------------

void RegistersAND( int* RegisterX, int* RegisterY )
{
    *RegisterX &= *RegisterY;
    *RegisterX &= 0xFF;
}

// ---------------------------------------------------------

void RegistersXOR( int* RegisterX, int* RegisterY )
{
    *RegisterX ^= *RegisterY;
    *RegisterX &= 0xFF;
}

// ---------------------------------------------------------

void RegisterShiftLeft( int* RegisterX, int* RegisterY )
{
    // RegisterY is actually ignored!!
    // it is just a residue from the initial specification
    // Just use the argument to silence the warning
    RegisterY;
    
    // first set VF if a bit will disappear
    if( *RegisterX & 0x80 )
      Chip8.Registers[ 0xF ] = 1;
    else 
      Chip8.Registers[ 0xF ] = 0;
    
    // then do the shift
    *RegisterX <<= 1;
    *RegisterX &= 0xFF;
}

// ---------------------------------------------------------

void RegisterShiftRight( int* RegisterX, int* RegisterY )
{
    // RegisterY is actually ignored!!
    // it is just a residue from the initial specification
    // Just use the argument to silence the warning
    RegisterY;
    
    // first set VF if a bit will disappear
    if( *RegisterX & 0x01 )
      Chip8.Registers[ 0xF ] = 1;
    else 
      Chip8.Registers[ 0xF ] = 0;
    
    // then do the shift
    *RegisterX >>= 1;
    *RegisterX &= 0xFF;
}

// ---------------------------------------------------------

void RegistersAdd( int* RegisterX, int* RegisterY )
{
    // first sum
    int Result = *RegisterX;
    Result += *RegisterY;
    *RegisterX = (Result & 255);
    
    // now set the carry flag
    bool HasCarry = (Result > 255);
    Chip8.Registers[ 0xF ] = HasCarry;
}

// ---------------------------------------------------------

void RegistersSubtract( int* RegisterX, int* RegisterY )
{
    // set borrow flag first (inverted)
    bool HasBorrow = (*RegisterY > *RegisterX);
    Chip8.Registers[ 0xF ] = !HasBorrow;
    
    // now subtract
    *RegisterX -= *RegisterY;
    *RegisterX &= 0xFF;
}

// ---------------------------------------------------------

void RegistersSubtractInverted( int* RegisterX, int* RegisterY )
{
    // set borrow flag first (inverted)
    bool HasBorrow = (*RegisterX > *RegisterY);
    Chip8.Registers[ 0xF ] = !HasBorrow;
    
    // now subtract
    *RegisterX = *RegisterY - *RegisterX;
    *RegisterX &= 0xFF;
}

// ---------------------------------------------------------

void SkipIfKeyPressed( int* RegisterX )
{
    // ignore non-existent keys!
    int KeyNumber = *RegisterX;
    if( KeyNumber > 0xF ) return;
    
    // apply skip condition
    if( Chip8.KeyIsPressed[ KeyNumber ] )
    {
        Chip8.ProgramCounter += 2;
        Chip8.ProgramCounter &= 0xFFFF;
    }
}

// ---------------------------------------------------------

void SkipIfKeyNotPressed( int* RegisterX )
{
    // ignore non-existent keys!
    int KeyNumber = *RegisterX;
    if( KeyNumber > 0xF ) return;
    
    // apply skip condition
    if( !Chip8.KeyIsPressed[ KeyNumber ] )
    {
        Chip8.ProgramCounter += 2;
        Chip8.ProgramCounter &= 0xFFFF;
    }
}

// ---------------------------------------------------------

void WaitAndStoreKeypress( int* RegisterX )
{
    // store the first found keypress
    for( int i = 0; i < 16; i++ )
      if( Chip8.KeyIsPressed[ i ] )
      {
          *RegisterX = i;
          return;
      }
    
    // if not found, keep waiting!
    // (ProgramCounter had already been auto-advanced)
    Chip8.ProgramCounter -= 2;
    Chip8.ProgramCounter &= 0xFFFF;
}

// ---------------------------------------------------------

void DrawSprite( int* RegisterX, int* RegisterY, int NibbleValue )
{
    // on high resolution, it is possible to draw a
    // 16x16 sprite by indicating height = 0; in that
    // case, redirect to the specialized function
    if( Chip8.HighResolutionEnabled && NibbleValue == 0 )
    {
        DrawSprite16x16( RegisterX, RegisterY );
        return;
    }
    
    // when used on low resolution, height = 0 indicates 16 pixels high
    if( NibbleValue == 0 )
      NibbleValue = 16;
    
    // received register values are the sprite top-left
    int TopLeftX = *RegisterX;
    int TopLeftY = *RegisterY;
    
    // initially set VF to 0 to indicate no collision
    Chip8.Registers[ 0xF ] = 0;
    
    // iterate along X and Y
    int PixelY = TopLeftY;
    
    for( int i = 0; i < NibbleValue; i++ )
    {
        int PixelX = TopLeftX;
        int SpriteRow = Chip8_ReadFromRAM( Chip8.Index + i );
        
        // most significant bit is drawn at the left!
        for( int j = 7; j >= 0; j-- )
        {
            // screen pixel will be flipped if sprite pixel is set;
            // otherwise, nothing will be done (screen remains the same)
            int SpriteBitMask = 1 << j;
            bool SpritePixelActive = SpriteRow & SpriteBitMask;
            
            if( SpritePixelActive )
              Chip8_FlipPixel( PixelX, PixelY );
            
            PixelX++;
        }
        
        PixelY++;
    }
}

// ---------------------------------------------------------

void DrawSprite16x16( int* RegisterX, int* RegisterY )
{
	Chip8.SuperChip8Enabled = true;
    
    // received register values are the sprite top-left
    int TopLeftX = *RegisterX;
    int TopLeftY = *RegisterY;
    
    // initially set VF to 0 to indicate no collision
    Chip8.Registers[ 0xF ] = 0;
    
    // iterate along X and Y
    int PixelY = TopLeftY;
    
    for( int i = 0; i < 16; i++ )
    {
        // LEFT BYTE
        int PixelX = TopLeftX;
        int SpriteRow = Chip8_ReadFromRAM( Chip8.Index + 2*i );
        
        for( int j = 7; j >= 0; j-- )
        {
            // screen pixel will be flipped if sprite pixel is set;
            // otherwise, nothing will be done (screen remains the same)
            int SpriteBitMask = 1 << j;
            bool SpritePixelActive = SpriteRow & SpriteBitMask;
            
            if( SpritePixelActive )
              Chip8_FlipPixel( PixelX, PixelY );
            
            PixelX++;
        }
        
        // RIGHT BYTE (PixelX has already incremented)
        SpriteRow = Chip8_ReadFromRAM( Chip8.Index + 2*i + 1 );
        
        for( int j = 7; j >= 0; j-- )
        {
            // screen pixel will be flipped if sprite pixel is set;
            // otherwise, nothing will be done (screen remains the same)
            int SpriteBitMask = 1 << j;
            bool SpritePixelActive = SpriteRow & SpriteBitMask;
            
            if( SpritePixelActive )
              Chip8_FlipPixel( PixelX, PixelY );
            
            PixelX++;
        }
        
        PixelY++;
    }
}

// ---------------------------------------------------------

void SetDisplayResolution( bool HighResolution )
{
	Chip8.SuperChip8Enabled = true;
	Chip8.HighResolutionEnabled = HighResolution;
}

// ---------------------------------------------------------

void ScrollDisplayLeft( int Pixels )
{
	Chip8.SuperChip8Enabled = true;
    int CopiedLinePixels = SC8_ScreenWidth - Pixels;
    
    for( int y = 0; y < SC8_ScreenHeight; ++y )
    {
        // copy pixels leftwards, starting from the left
        memcpy( &Chip8.DisplayRAM[ SC8_ScreenWidth * y ], &Chip8.DisplayRAM[ SC8_ScreenWidth*y + Pixels ], CopiedLinePixels );
        
        // fill the right columns with black
        memset( &Chip8.DisplayRAM[ SC8_ScreenWidth * (y+1) - Pixels ], 0, Pixels );
    }
}

// ---------------------------------------------------------

void ScrollDisplayRight( int Pixels )
{
	Chip8.SuperChip8Enabled = true;
    
    // We can't just use memcpy here since it
    // copies in left to right order, and that
    // may overwrite some pixels before they
    // are read to be copied. To prevent that
    // we can use an intermediate buffer.
    bool[ SC8_ScreenWidth ] AuxiliaryScanline;
    int CopiedLinePixels = SC8_ScreenWidth - Pixels;
    
    for( int y = 0; y < SC8_ScreenHeight; ++y )
    {
        // copy pixels leftwards in 2 steps, using auxiliary buffer
        memcpy( AuxiliaryScanline, &Chip8.DisplayRAM[ SC8_ScreenWidth * y ], CopiedLinePixels );
        memcpy( &Chip8.DisplayRAM[ SC8_ScreenWidth*y + Pixels ], AuxiliaryScanline, CopiedLinePixels );
        
        // fill the left columns with black
        memset( &Chip8.DisplayRAM[ SC8_ScreenWidth * (y+1) ], 0, Pixels );
    }
}

// ---------------------------------------------------------

void ScrollDisplayUp( int Pixels )
{
	Chip8.SuperChip8Enabled = true;
    int FirstEmptiedLine = SC8_ScreenHeight - Pixels;
    
    // copy pixels upwards, starting from the top
    for( int y = 0; y < FirstEmptiedLine; ++y )
      memcpy( &Chip8.DisplayRAM[ SC8_ScreenWidth * y ], &Chip8.DisplayRAM[ SC8_ScreenWidth * (y+Pixels) ], SC8_ScreenWidth );
    
    // fill the bottom lines with black
    memset( &Chip8.DisplayRAM[ SC8_ScreenWidth*FirstEmptiedLine ], 0, SC8_ScreenWidth * Pixels );
}

// ---------------------------------------------------------

void ScrollDisplayDown( int Pixels )
{
	Chip8.SuperChip8Enabled = true;
    int LastCopiedLine = SC8_ScreenHeight - Pixels;
    
    // copy pixels downwards, starting from the bottom
    for( int y = LastCopiedLine; y >= 0; --y )
      memcpy( &Chip8.DisplayRAM[ SC8_ScreenWidth * y ], &Chip8.DisplayRAM[ SC8_ScreenWidth * (y-Pixels) ], SC8_ScreenWidth );
    
    // fill the top lines with black
    memset( Chip8.DisplayRAM, 0, SC8_ScreenWidth * Pixels );
}

// ---------------------------------------------------------

void ReadDelayTimer( int* RegisterX )
{
    *RegisterX = Chip8.DelayTimer;
    *RegisterX &= 0xFF;
}

// ---------------------------------------------------------

void SetDelayTimer( int* RegisterX )
{
    Chip8.DelayTimer = *RegisterX;
    Chip8.DelayTimer &= 0xFF;
}

// ---------------------------------------------------------

void SetSoundTimer( int* RegisterX )
{
    Chip8.SoundTimer = *RegisterX;
    Chip8.SoundTimer &= 0xFF;
}

// ---------------------------------------------------------

void AddToIndexRegister( int* RegisterX )
{
    Chip8.Index += *RegisterX;
    Chip8.Index &= 0xFFFF;
}

// ---------------------------------------------------------

void PointIndexToLoResDigitSprite( int* RegisterX )
{
    // each digit takes 5 bytes
    // from initial position 0x0000
    Chip8.Index = (*RegisterX) * 5;
    Chip8.Index &= 0xFFFF;
}

// ---------------------------------------------------------

void PointIndexToHiResDigitSprite( int* RegisterX )
{
	Chip8.SuperChip8Enabled = true;
    
    // each digit takes 10 bytes
    // from initial position 0x0080
    Chip8.Index = 80 + (*RegisterX) * 10;
    Chip8.Index &= 0xFFFF;
}

// ---------------------------------------------------------

void StoreRegisterAsBCD( int* RegisterX )
{
    // decompose decimal digits
    int Value = *RegisterX;
    int Digit1 = Value % 10;
    
    Value = (Value-Digit1) / 10;
    int Digit10 = Value % 10;
    
    Value = (Value-Digit10) / 10;
    int Digit100 = Value;
    
    // now store in successive positions
    Chip8_WriteToRAM( Chip8.Index + 0, Digit100 );
    Chip8_WriteToRAM( Chip8.Index + 1, Digit10 );
    Chip8_WriteToRAM( Chip8.Index + 2, Digit1 );
}

// ---------------------------------------------------------

void SaveFirstRegistersToIndex( int LastRegister )
{
    // save each register to adjacent addresses
    for( int i = 0; i <= LastRegister; i++ )
      Chip8_WriteToRAM( Chip8.Index + i, Chip8.Registers[ i ] );
    
    // increase index value only for
    // regular chip-8 (skipped in super)
    if( !Chip8.SuperChip8Enabled )
    {
        Chip8.Index += LastRegister + 1;
        Chip8.Index &= 0xFFFF;
    }
}

// ---------------------------------------------------------

void LoadFirstRegistersFromIndex( int LastRegister )
{
    // load each register from adjacent addresses
    for( int i = 0; i <= LastRegister; i++ )
      Chip8.Registers[ i ] = Chip8_ReadFromRAM( Chip8.Index + i );
    
    // increase index value only for
    // regular chip-8 (skipped in super)
    if( !Chip8.SuperChip8Enabled )
    {
        Chip8.Index += LastRegister + 1;
        Chip8.Index &= 0xFFFF;
    }
}

// ---------------------------------------------------------

void SaveFirstRegistersToFlags( int LastRegister )
{
    Chip8.SuperChip8Enabled = true;
    
    // save each register to adjacent addresses
    for( int i = 0; i <= LastRegister; i++ )
      Chip8.FlagRegisters[ i ] = Chip8.Registers[ i ];
}

// ---------------------------------------------------------

void LoadFirstRegistersFromFlags( int LastRegister )
{
    Chip8.SuperChip8Enabled = true;
    
    // load each register from adjacent addresses
    for( int i = 0; i <= LastRegister; i++ )
      Chip8.Registers[ i ] = Chip8.FlagRegisters[ i ];
}

// ---------------------------------------------------------

void RandomWithBinaryAnd( int* RegisterX, int ByteValue )
{
    int RandomValue = rand() % 256;
    *RegisterX = RandomValue & ByteValue;
}

// ---------------------------------------------------------

void ExitProgram()
{
    Chip8.SuperChip8Enabled = true;
    Chip8.Running = false;
}
