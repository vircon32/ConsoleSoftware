// *****************************************************************************
    // start include guard
    #ifndef CHIP8_H
    #define CHIP8_H
// *****************************************************************************


// ---------------------------------------------------------
//   CHIP-8 DEFINITIONS
// ---------------------------------------------------------


// constants for regular Chip-8
#define C8_ScreenWidth      64
#define C8_ScreenHeight     32
#define C8_ScreenPixels     (C8_ScreenWidth * C8_ScreenHeight)
#define C8_CyclesPerSecond  540
#define C8_CyclesPerFrame   (C8_CyclesPerSecond / frames_per_second)

// constants for Chip-8 constants
#define SC8_ScreenWidth      128
#define SC8_ScreenHeight     64
#define SC8_ScreenPixels     (SC8_ScreenWidth * SC8_ScreenHeight)
#define SC8_CyclesPerSecond  1200
#define SC8_CyclesPerFrame   (SC8_CyclesPerSecond / frames_per_second)

// constants common for both variants
#define C8_NumberOfKeys       16
#define C8_NumberOfRegisters  16
#define C8_StackSize          16
#define C8_RAMSize            4096
#define C8_MaxROMSize         (C8_RAMSize - 512)


// ---------------------------------------------------------
//   CHIP-8 "CLASS"
// ---------------------------------------------------------


struct Chip8Machine
{
    // general purpose registers
    // (though 0xF is also used as flag)
    int[ C8_NumberOfRegisters ] Registers;
    int[ C8_NumberOfRegisters ] FlagRegisters;
    
    // specific purpose registers
    int[ 2 ] InstructionBytes;
    int StackPointer;
    int Index;
    int ProgramCounter;
    
    // timer counter registers are interpreted as
    // ticks: (60 ticks = 1 second) so in Vircon32
    // 1 Chip-8 tick = 1 frame
    int SoundTimer; 
    int DelayTimer; 
    
    // all available memory
    // (also contains internal ROM)
    int[ C8_RAMSize ] RAM;
    
    // display memory is separated,
    // and not accessible directly
    bool[ SC8_ScreenPixels ] DisplayRAM;
    
    // configuration parameters
    bool Running;
    int CyclesPerTick;
    bool SuperChip8Enabled;
    bool HighResolutionEnabled;
    
    // stack is not directly addressable
    int[ C8_StackSize ] Stack;
    
    // state of keys 0-F
    bool[ C8_NumberOfKeys ] KeyIsPressed;
    
    // error detection and report
    bool ErrorHappened;
    int[ 100 ] ErrorMessage;
};


// ---------------------------------------------------------
//   GLOBAL VARIABLES
// ---------------------------------------------------------


// we will have a single instance, so just
// make it global for simplicity and speed
Chip8Machine Chip8;

// emulator-level configurations that are
// external to the chip-8 machine itself
int CurrentControlMapping = 0;
int CurrentColorScheme = 0;


// ---------------------------------------------------------
//   PROTOTYPES FOR MAIN EMULATOR FUNCTIONS
// ---------------------------------------------------------


void Chip8_Reset();
bool Chip8_LoadROM();
void Chip8_UpdateKeys();
void Chip8_UpdateTimers();
void Chip8_RunInstruction();
void Chip8_DrawScreen();
void Chip8_DrawScreen_HighResolution();


// ---------------------------------------------------------
//   PROTOTYPES FOR CPU INSTRUCTIONS
// ---------------------------------------------------------


void ClearScreen();
void ReturnFromSubroutine();
void CallExternalSubroutine( int AddressValue );
void CallSubroutine( int AddressValue );
void JumpToAddress( int AddressValue );
void JumpToAddressOffset( int AddressValue );
void SkipIfEqual( int* RegisterX, int ByteValue );
void SkipIfNotEqual( int* RegisterX, int ByteValue );
void SkipIfRegistersEqual( int* RegisterX, int* RegisterY );
void SkipIfRegistersNotEqual( int* RegisterX, int* RegisterY );
void LoadRegister( int* RegisterX, int ByteValue ) ;
void LoadIndex( int AddressValue );
void AddValueToRegister( int* RegisterX, int ByteValue );
void RegistersCopy( int* RegisterX, int* RegisterY );
void RegistersOR( int* RegisterX, int* RegisterY );
void RegistersAND( int* RegisterX, int* RegisterY );
void RegistersXOR( int* RegisterX, int* RegisterY );
void RegisterShiftLeft( int* RegisterX, int* RegisterY );
void RegisterShiftRight( int* RegisterX, int* RegisterY );
void RegistersAdd( int* RegisterX, int* RegisterY );
void RegistersSubtract( int* RegisterX, int* RegisterY );
void RegistersSubtractInverted( int* RegisterX, int* RegisterY );
void SkipIfKeyPressed( int* RegisterX );
void SkipIfKeyNotPressed( int* RegisterX );
void WaitAndStoreKeypress( int* RegisterX );
void DrawSprite( int* RegisterX, int* RegisterY, int NibbleValue );
void DrawSprite16x16( int* RegisterX, int* RegisterY );
void SetDisplayResolution( bool HighResolution );
void ScrollDisplayLeft( int Pixels );
void ScrollDisplayRight( int Pixels );
void ScrollDisplayUp( int Pixels );
void ScrollDisplayDown( int Pixels );
void ReadDelayTimer( int* RegisterX );
void SetDelayTimer( int* RegisterX );
void SetSoundTimer( int* RegisterX );
void AddToIndexRegister( int* RegisterX );
void PointIndexToLoResDigitSprite( int* RegisterX );
void PointIndexToHiResDigitSprite( int* RegisterX );
void StoreRegisterAsBCD( int* RegisterX );
void SaveFirstRegistersToIndex( int LastRegister );
void LoadFirstRegistersFromIndex( int LastRegister );
void SaveFirstRegistersToFlags( int LastRegister );
void LoadFirstRegistersFromFlags( int LastRegister );
void RandomWithBinaryAnd( int* RegisterX, int ByteValue );
void ExitProgram();


// ---------------------------------------------------------
//   INTERNAL CHARACTER ROMS
// ---------------------------------------------------------


// low resolution character ROM (16 characters of 4x5 pixels)
int[ 5 * 16 ] C8_CharacterROM =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0,   // character 0
    0x20, 0x60, 0x20, 0x20, 0x70,   // character 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,   // character 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,   // character 3
    0x90, 0x90, 0xF0, 0x10, 0x10,   // character 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,   // character 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,   // character 6
    0xF0, 0x10, 0x20, 0x40, 0x40,   // character 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,   // character 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,   // character 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,   // character A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,   // character B
    0xF0, 0x80, 0x80, 0x80, 0xF0,   // character C
    0xE0, 0x90, 0x90, 0x90, 0xE0,   // character D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,   // character E
    0xF0, 0x80, 0xF0, 0x80, 0x80    // character F
};

// ---------------------------------------------------------

// high resolution character ROM (16 characters of 8x10 pixels)
int[ 10 * 16 ] SC8_CharacterROM =
{
    0x7E, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0x7E,    // character 0
    0x18, 0x38, 0x78, 0xD8, 0x18, 0x18, 0x18, 0x18, 0xFF, 0xFF,    // character 1
    0xFE, 0xFF, 0x03, 0x03, 0x7F, 0xFE, 0xC0, 0xC0, 0xFF, 0xFF,    // character 2
    0x7E, 0xFF, 0xC3, 0x03, 0x3E, 0x3E, 0x03, 0xC3, 0xFF, 0x7E,    // character 3
    0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03,    // character 4
    0xFF, 0xFF, 0xC0, 0xC0, 0xFE, 0xFF, 0x03, 0x03, 0xFF, 0xFE,    // character 5
    0x7E, 0xFF, 0xC3, 0xC0, 0xFE, 0xFF, 0xC3, 0xC3, 0xFF, 0x7E,    // character 6
    0xFF, 0xFF, 0x03, 0x03, 0x06, 0x0C, 0x18, 0x18, 0x18, 0x18,    // character 7
    0x7E, 0xFF, 0xC3, 0xC3, 0x7E, 0x7E, 0xC3, 0xC3, 0xFF, 0x7E,    // character 8
    0x7E, 0xFF, 0xC3, 0xC3, 0xFF, 0x7F, 0x03, 0xC3, 0xFF, 0x7E,    // character 9
    0x7E, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF, 0xC3, 0xC3, 0xC3,    // character A
    0xFC, 0xFE, 0xC3, 0xC3, 0xFE, 0xFE, 0xC3, 0xC3, 0xFE, 0xFC,    // character B
    0x7E, 0xFF, 0xC3, 0xC0, 0xC0, 0xC0, 0xC0, 0xC3, 0xFF, 0x7E,    // character C
    0xF8, 0xFE, 0xC7, 0xC3, 0xC3, 0xC3, 0xC3, 0xC7, 0xFE, 0xF8,    // character D
    0xFF, 0xFF, 0xC0, 0xC0, 0xFE, 0xFE, 0xC0, 0xC0, 0xFF, 0xFF,    // character E
    0xFF, 0xFF, 0xC0, 0xC0, 0xFE, 0xFE, 0xC0, 0xC0, 0xC0, 0xC0     // character F
};


// ---------------------------------------------------------
//   OUR 4 SELECTABLE VIRCON32->CHIP-8 BUTTON MAPPINGS
// ---------------------------------------------------------


// variable mappings for pad 1
int[ 4 ] KeysPad1Up    = { 0x2, 0x2, 0x2, 0x2 };
int[ 4 ] KeysPad1Left  = { 0x4, 0x4, 0x4, 0x4 };
int[ 4 ] KeysPad1Right = { 0x6, 0x6, 0x6, 0x6 };
int[ 4 ] KeysPad1Down  = { 0x8, 0x8, 0x8, 0x8 };

int[ 4 ] KeysPad1ButtonA = { 0x5, 0x5, 0x5, 0xC };
int[ 4 ] KeysPad1ButtonB = { 0x0, 0x0, 0x0, 0xD };
int[ 4 ] KeysPad1ButtonX = { 0xC, 0x9, 0xB, 0x3 };
int[ 4 ] KeysPad1ButtonY = { 0xD, 0x7, 0xA, 0x1 };

int[ 4 ] KeysPad1ButtonL = { 0xA, 0x1, 0x1, 0xA };
int[ 4 ] KeysPad1ButtonR = { 0xB, 0x3, 0x3, 0xB };

// variable mappings for pad 2
int[ 4 ] KeysPad2ButtonA = { 0xE, 0xE, 0xE, 0xE };
int[ 4 ] KeysPad2ButtonB = { 0xF, 0xF, 0xF, 0xF };
int[ 4 ] KeysPad2ButtonX = { 0x9, 0xC, 0xC, 0x5 };
int[ 4 ] KeysPad2ButtonY = { 0x7, 0xD, 0xD, 0x0 };

int[ 4 ] KeysPad2ButtonL = { 0x1, 0xA, 0x7, 0x7 };
int[ 4 ] KeysPad2ButtonR = { 0x3, 0xB, 0x9, 0x9 };


// ---------------------------------------------------------
//   OUR 4 SELECTABLE SCREEN COLOR SCHEMES
// ---------------------------------------------------------


// colors for the screen
int[ 4 ] ColorsScreenBackground =
{
    color_black,
    make_color_rgb( 180,180,160 ),
    make_color_rgb( 184,205, 81 ),
    make_color_rgb(  76, 34, 26 )
};

int[ 4 ] ColorsScreenPixels =
{
    color_white,
    make_color_rgb(  70,  64, 78 ),
    make_color_rgb(  83, 122, 62 ),
    make_color_rgb( 230, 146, 49 )
};


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
