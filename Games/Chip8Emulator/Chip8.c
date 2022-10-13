// include corresponding header
#include "Chip8.h"


// ---------------------------------------------------------
//   MAIN EMULATOR FUNCTIONS
// ---------------------------------------------------------


void Chip8_Reset()
{
    // clear all registers
    memset( Chip8.Registers, 0, C8_NumberOfRegisters );
    memset( Chip8.FlagRegisters, 0, C8_NumberOfRegisters );
    
    // set initial configuration
    Chip8.Running = true;
    Chip8.SuperChip8Enabled = false;
    Chip8.HighResolutionEnabled = false;
    Chip8.CyclesPerTick = 9;  // 540 Hz
    
    // clear the screen
    memset( Chip8.DisplayRAM, 0, sizeof(Chip8.DisplayRAM) );
    
    // clear RAM
    memset( Chip8.RAM, 0, sizeof(Chip8.RAM) );
    
    // copy character ROMs to RAM
    memcpy( &Chip8.RAM[ 0 ], C8_CharacterROM, sizeof(C8_CharacterROM) );
    memcpy( &Chip8.RAM[ sizeof(C8_CharacterROM) ], SC8_CharacterROM, sizeof(SC8_CharacterROM) );
    
    // initialize the program pointer
    Chip8.ProgramCounter = 0x200;
    
    // set a null initial state
    Chip8.StackPointer = 0;
    Chip8.Index = 0;
    Chip8.SoundTimer = 0;
    Chip8.DelayTimer = 0;
    
    // clear any previous errors
    Chip8.ErrorHappened = false;
    strcpy( Chip8.ErrorMessage, "" );
    
    // reset all I/O
    memset( Chip8.KeyIsPressed, false, C8_NumberOfKeys );
    stop_all_channels();
    Chip8_DrawScreen();
    
    // if there is a ROM, copy it to RAM
    if( !Chip8_LoadROM() )
      return;
    
    // force a new frame for better
    // timing of subsequent first frame
    end_frame();
}

// ---------------------------------------------------------

// returns a boolean indicating success/failure
bool Chip8_LoadROM()
{
    // check if there is a memory card inserted
    if( !card_is_connected() )
    {
        strcpy( Chip8.ErrorMessage, "Please insert a memory card with a Chip-8 ROM\n" "and press start to reset" );
        Chip8.ErrorHappened = true;
        return false;
    }
    
    // ensure card signature match
    game_signature Chip8Signature;
    memset( Chip8Signature, 0, sizeof(game_signature) );
    strcpy( Chip8Signature, "CHIP-8 ROM" );
    
    if( !card_signature_matches( &Chip8Signature ) )
    {
        strcpy( Chip8.ErrorMessage, "Inserted memory card is not a compatible Chip-8\n" "ROM. Try again with a valid card and press start\n" "to reset" );
        Chip8.ErrorHappened = true;
        return false;
    }
    
    // enforce the maximum ROM size
    int ROMSize;
    card_read_data( &ROMSize, sizeof( game_signature ), 1 );
    
    if( ROMSize > C8_MaxROMSize )
    {
        strcpy( Chip8.ErrorMessage, "Inserted Chip-8 ROM is too large to fit in RAM\n" "memory. Try again with a smaller ROM and press\n" "start to reset" );
        Chip8.ErrorHappened = true;
        return false;
    }
    
    // null or negative size is also an error
    if( ROMSize <= 0 )
    {
        strcpy( Chip8.ErrorMessage, "Inserted Chip-8 ROM does not list a correct size.\n" "Try with a correct ROM and press start to reset" );
        Chip8.ErrorHappened = true;
        return false;
    }
    
    // copy the ROM contents to RAM
    card_read_data( &Chip8.RAM[ 0x200 ], sizeof( game_signature ) + 1, ROMSize );
    return true;
}

// ---------------------------------------------------------

void Chip8_DrawScreen()
{
    // first fill the screen background
    set_multiply_color( ColorsScreenBackground[ CurrentColorScheme ] );
    select_region( RegionWhitePixel );
    set_drawing_scale( 512, 256 );
    draw_region_zoomed_at( 64, 13 );
    
    // set current pixel color
    set_multiply_color( ColorsScreenPixels[ CurrentColorScheme ] );
    
    // common variables
    bool* Pixel = Chip8.DisplayRAM;
    int DrawingX = 64;
    int DrawingY = 13;
    
    // case 1: high resolution
    if( Chip8.SuperChip8Enabled )
    {
        // use the assembly version for more speed;
        // otherwise looping through 8192 positions
        // will not be fast enough to do every frame
        Chip8_DrawScreen_HighResolution();
    }
    
    // case 2: low resolution
    else
    {
        // draw only pixels set to white
        select_region( RegionLargePixel );
        
        for( int y = 0; y < C8_ScreenHeight; ++y )
        {
            DrawingX = 64;
            
            for( int x = 0; x < C8_ScreenWidth; ++x )
            {
                if( *Pixel )
                  draw_region_at( DrawingX, DrawingY );
                
                Pixel += 2;
                DrawingX += 8;
            }
            
            Pixel += SC8_ScreenWidth;
            DrawingY += 8;
        }
    }
    
    // restore drawing colors
    set_multiply_color( color_white );
}

// ---------------------------------------------------------

void Chip8_UpdateKeys()
{
    // update keys in gamepad 1
    // using our configurable mappings
    select_gamepad( 0 );
    
    Chip8.KeyIsPressed[ KeysPad1Up     [ CurrentControlMapping ] ] = (gamepad_up()    > 0);
    Chip8.KeyIsPressed[ KeysPad1Left   [ CurrentControlMapping ] ] = (gamepad_left()  > 0);
    Chip8.KeyIsPressed[ KeysPad1Right  [ CurrentControlMapping ] ] = (gamepad_right() > 0);
    Chip8.KeyIsPressed[ KeysPad1Down   [ CurrentControlMapping ] ] = (gamepad_down()  > 0);
    
    Chip8.KeyIsPressed[ KeysPad1ButtonA[ CurrentControlMapping ] ] = (gamepad_button_a() > 0);
    Chip8.KeyIsPressed[ KeysPad1ButtonB[ CurrentControlMapping ] ] = (gamepad_button_b() > 0);
    Chip8.KeyIsPressed[ KeysPad1ButtonX[ CurrentControlMapping ] ] = (gamepad_button_x() > 0);
    Chip8.KeyIsPressed[ KeysPad1ButtonY[ CurrentControlMapping ] ] = (gamepad_button_y() > 0);
    
    Chip8.KeyIsPressed[ KeysPad1ButtonL[ CurrentControlMapping ] ] = (gamepad_button_l() > 0);
    Chip8.KeyIsPressed[ KeysPad1ButtonR[ CurrentControlMapping ] ] = (gamepad_button_r() > 0);
    
    // update keys in gamepad 2
    // using our configurable mappings
    select_gamepad( 1 );
    
    Chip8.KeyIsPressed[ KeysPad2ButtonA[ CurrentControlMapping ] ] = (gamepad_button_a() > 0);
    Chip8.KeyIsPressed[ KeysPad2ButtonB[ CurrentControlMapping ] ] = (gamepad_button_b() > 0);
    Chip8.KeyIsPressed[ KeysPad2ButtonX[ CurrentControlMapping ] ] = (gamepad_button_x() > 0);
    Chip8.KeyIsPressed[ KeysPad2ButtonY[ CurrentControlMapping ] ] = (gamepad_button_y() > 0);
    
    Chip8.KeyIsPressed[ KeysPad2ButtonL[ CurrentControlMapping ] ] = (gamepad_button_l() > 0);
    Chip8.KeyIsPressed[ KeysPad2ButtonR[ CurrentControlMapping ] ] = (gamepad_button_r() > 0);
}

// ---------------------------------------------------------

void Chip8_UpdateTimers()
{
    // auto-decrement timers towards zero
    if( Chip8.SoundTimer > 0 ) Chip8.SoundTimer--;
    if( Chip8.DelayTimer > 0 ) Chip8.DelayTimer--;
    
    // play or stop sound based on the sound timer value
    bool SoundIsPlaying = (get_channel_state( ChannelBeep ) == channel_playing);
    
    if( Chip8.SoundTimer > 0 && !SoundIsPlaying )
      play_sound_in_channel( SoundBeep, ChannelBeep );
    
    if( Chip8.SoundTimer == 0 && SoundIsPlaying )
      stop_channel( ChannelBeep );
}

// ---------------------------------------------------------

void Chip8_RunInstruction()
{
    // load next instruction (2 bytes)
    Chip8.InstructionBytes[ 0 ] = Chip8.RAM[ Chip8.ProgramCounter ];
    Chip8.InstructionBytes[ 1 ] = Chip8.RAM[ Chip8.ProgramCounter + 1 ];
      
    // instruction 0x0000 usually means we
    // reached empty memory, so stop machine
    if( !Chip8.InstructionBytes[ 0 ] && !Chip8.InstructionBytes[ 1 ] )
    {
        Chip8.Running = false;
        return;
    }
    
    // increase program counter
    Chip8.ProgramCounter += 2;
    
    // Chip-8 instructions are mostly based on nibbles
    // so decompose the 2 bytes into their 4 nibbles
    int[ 4 ] InstructionNibbles;
    InstructionNibbles[ 0 ] = (Chip8.InstructionBytes[ 0 ] >> 4) & 15;
    InstructionNibbles[ 1 ] =  Chip8.InstructionBytes[ 0 ] & 15;
    InstructionNibbles[ 2 ] = (Chip8.InstructionBytes[ 1 ] >> 4) & 15;
    InstructionNibbles[ 3 ] =  Chip8.InstructionBytes[ 1 ] & 15;
    
    // "decode" the opcode and its possible operands
    int OpCode = InstructionNibbles[ 0 ];
    int ByteValue    = Chip8.InstructionBytes[ 1 ];
    int NibbleValue  = InstructionNibbles[ 3 ];
    int AddressValue = (InstructionNibbles[ 1 ] << 8) | Chip8.InstructionBytes[ 1 ];
    
    int* RegisterX = &Chip8.Registers[ InstructionNibbles[ 1 ] ];
    int* RegisterY = &Chip8.Registers[ InstructionNibbles[ 2 ] ];
    
    // first check for instructions 00XX
    if( Chip8.InstructionBytes[ 0 ] == 0x00 )
    {
        // super chip 8 instructions (with parameters)
        switch( InstructionNibbles[ 2 ] )
        {
            case 0xB:  ScrollDisplayUp( NibbleValue );    return;
            case 0xC:  ScrollDisplayDown( NibbleValue );  return;
        }
        
        switch( Chip8.InstructionBytes[ 1 ] )
        {
            // regular chip 8 instructions
            case 0xE0:  ClearScreen();                  return;
            case 0xEE:  ReturnFromSubroutine();         return;
            
            // super chip 8 instructions (fixed)
            case 0xFB:  ScrollDisplayRight( 4 );        return;
            case 0xFC:  ScrollDisplayLeft( 4 );         return;
            case 0xFD:  ExitProgram();                  return;
            case 0xFE:  SetDisplayResolution( false );  return;
            case 0xFF:  SetDisplayResolution( true );   return;
        }
    }
    
    // now check the non-partitioned opcodes
    // (given by just the first nibble)
    switch( OpCode )
    {
        case 0x0:  CallExternalSubroutine( AddressValue );           return;
        case 0x1:  JumpToAddress( AddressValue );                    return;
        case 0x2:  CallSubroutine( AddressValue );                   return;
        case 0x3:  SkipIfEqual( RegisterX, ByteValue );              return;
        case 0x4:  SkipIfNotEqual( RegisterX, ByteValue );           return;
        case 0x6:  LoadRegister( RegisterX, ByteValue );             return;
        case 0x7:  AddValueToRegister( RegisterX, ByteValue );       return;
        case 0xA:  LoadIndex( AddressValue );                        return;
        case 0xB:  JumpToAddressOffset( AddressValue );              return;
        case 0xC:  RandomWithBinaryAnd( RegisterX, ByteValue );      return;
        case 0xD:  DrawSprite( RegisterX, RegisterY, NibbleValue );  return;
    }
    
    // now check for opcodes with multiple options
    if( OpCode == 0x5 )
    {
        if( NibbleValue == 0 )
        {
            SkipIfRegistersEqual( RegisterX, RegisterY );
            return;
        }
    }
    
    if( OpCode == 0x8 )
    {
        // this opcode selects is operation from its last nibble
        switch( NibbleValue )
        {
            case 0x0: RegistersCopy( RegisterX, RegisterY );              return;
            case 0x1: RegistersOR( RegisterX, RegisterY );                return;
            case 0x2: RegistersAND( RegisterX, RegisterY );               return;
            case 0x3: RegistersXOR( RegisterX, RegisterY );               return;
            case 0x4: RegistersAdd( RegisterX, RegisterY );               return;
            case 0x5: RegistersSubtract( RegisterX, RegisterY );          return;
            case 0x6: RegisterShiftRight( RegisterX, RegisterY );         return;
            case 0x7: RegistersSubtractInverted( RegisterX, RegisterY );  return;
            case 0xE: RegisterShiftLeft( RegisterX, RegisterY );          return;
        }
    }
    
    if( OpCode == 0x9 )
    {
        if( NibbleValue == 0 )
        {
            SkipIfRegistersNotEqual( RegisterX, RegisterY );
            return;
        }
    }
    
    if( OpCode == 0xE )
    {
        // this opcode selects is operation from its second byte
        switch( Chip8.InstructionBytes[ 1 ] )
        {
            case 0x9E: SkipIfKeyPressed( RegisterX );     return;
            case 0xA1: SkipIfKeyNotPressed( RegisterX );  return;
        }
    }
    
    if( OpCode == 0xF )
    {
        // this opcode selects is operation from its second byte
        switch( Chip8.InstructionBytes[ 1 ] )
        {
            case 0x07: ReadDelayTimer( RegisterX );                return;
            case 0x0A: WaitAndStoreKeypress( RegisterX );          return;
            case 0x15: SetDelayTimer( RegisterX );                 return;
            case 0x18: SetSoundTimer( RegisterX );                 return;
            case 0x1E: AddToIndexRegister( RegisterX );            return;
            case 0x29: PointIndexToLoResDigitSprite( RegisterX );  return;
            case 0x33: StoreRegisterAsBCD( RegisterX );            return;
            
            // careful! These 2 actually need the number 'X' itself
            // (they don't read the number from the register X)
            case 0x55: SaveFirstRegistersToIndex( InstructionNibbles[ 1 ] );    return;
            case 0x65: LoadFirstRegistersFromIndex( InstructionNibbles[ 1 ] );  return;
            
            // super chip 8 instructions (same as before with last 2)
            case 0x30: PointIndexToHiResDigitSprite( RegisterX );               return;
            case 0x75: SaveFirstRegistersToFlags( InstructionNibbles[ 1 ] );    return;
            case 0x85: LoadFirstRegistersFromFlags( InstructionNibbles[ 1 ] );  return;
        }
    }
    
    // for any non-recognized instruction, raise an error
    strcpy( Chip8.ErrorMessage, "Error: Invalid instruction found" );
    Chip8.ErrorHappened = true;
}
