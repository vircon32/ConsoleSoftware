// include Vircon libraries
#include "audio.h"
#include "video.h"
#include "time.h"
#include "string.h"

// include project libraries
#include "ErrorInfo.h"


// ---------------------------------------------------------
//     GENERAL DEFINITIONS
// ---------------------------------------------------------


// BIOS-required regions; these
// may safely be used by programs
#define first_region_font      0
#define region_white_pixel   256

// other non-required regions, used
// to draw the logo and error screens
#define region_console       300
#define region_cartridge     301
#define region_down_arrow    302
#define region_white_square  303

// colors for error screens
#define error_colors_background   0xFF8D4130
#define error_colors_title        color_yellow
#define error_colors_description  color_white
#define error_colors_values       0xFF8080FF


// ---------------------------------------------------------
//     SUPPORT FUNCTIONS
// ---------------------------------------------------------


void draw_message_screen( error_message* message )
{
    clear_screen( error_colors_background );
    set_multiply_color( color_white );
    select_texture( -1 );
    
    // write title
    set_multiply_color( error_colors_title );
    print_at( 49, 37, message->title );
    
    // draw horizontal line
    select_region( region_white_square );
    set_drawing_scale( 640/2, 1 );
    draw_region_zoomed_at( 0, 60 );
    set_drawing_scale( 1, 1 );
    
    // write description
    set_multiply_color( error_colors_description );
    print_at( 49, 95, message->description );
}

// ---------------------------------------------------------

void print_hex_value( int x, int y, int* name, int value )
{
    // convert the number to hex, always
    // showing the full 8 hex digits
    int[ 16+1 ] hex_characters = "0123456789ABCDEF";
    int[ 8+1 ] hex_string;
    
    for( int Digit = 7; Digit >= 0; Digit-- )
    {
        hex_string[ Digit ] = hex_characters[ value & 15 ];
        value >>= 4;
    }
    
    hex_string[ 8 ] = 0;
    
    // join all text parts
    int[ 60 ] text;
    strcpy( text, name );
    strcat( text, " = 0x" );
    strcat( text, hex_string );
    
    // print the text
    print_at( x, y, text );
}

// ---------------------------------------------------------

bool cartridge_connected()
{
    asm
    {
        "in R0, CAR_Connected"
    }
}

// ---------------------------------------------------------

void request_cartridge()
{
    // write a custom message as if it was an error
    error_message no_cartridge_message =
    {
        "NO CARTRIDGE FOUND",
        "To play a game, please power off\n"
        "your console and insert a game\n"
        "cartridge compatible with Vircon32."        
    };
    
    draw_message_screen( &no_cartridge_message );
    
    // draw console diagram
    set_multiply_color( color_white );
    select_region( region_console );
    draw_region_at( 400, 207 );
    
    select_region( region_cartridge );
    draw_region_at( 469, 76 );
    
    select_region( region_down_arrow );
    draw_region_at( 497, 149 );
    
    // ensure everything gets drawn
    end_frame();
}


// ---------------------------------------------------------
//     ERROR HANDLER FUNCTION
// ---------------------------------------------------------


void error_handler()
{
    // do not initialize these!
    // or else R0 will be overwritten
    int error_code;
    int instruction_pointer;
    int instruction;
    int immediate_value;
    
    // save registers to variables
    asm
    {
        "mov {error_code}, R0"
        "mov {instruction_pointer}, R1"
        "mov {instruction}, R2"
        "mov {immediate_value}, R3"
    }
    
    // ensure everything gets drawn
    end_frame();
    
    // all possible error messages; do NOT store this as
    // global variables, since it may collide with globals
    // defined in the programs running from the cartridge
    error_message[ 12 ] error_messages =
    {
        {
            "ERROR: INVALID MEMORY READ",
            "Program attempted to read from a memory address\n"
            "that does not exist or is in a write-only device."
        },
        {
            "ERROR: INVALID MEMORY WRITE",
            "Program attempted to write on a memory address\n"
            "that does not exist or is in a read-only device."
        },
        {
            "ERROR: INVALID PORT READ",
            "Program attempted to read from a port number\n"
            "that does not exist or is set as write-only."
        },
        {
            "ERROR: INVALID PORT WRITE",
            "Program attempted to write on a port number\n"
            "that does not exist or is set as read-only."
        },
        {
            "ERROR: STACK OVERFLOW",
            "Program pushed too many values in the stack\n"
            "and available RAM memory was exhausted."
        },
        {
            "ERROR: STACK UNDERFLOW",
            "Program popped too many values from the stack\n"
            "and all data stored in stack was exhausted."
        },
        {
            "ERROR: DIVISION BY ZERO",
            "Program attempted to perform a division or\n"
            "modulus operation where the divisor was zero."
        },
        {
            "ERROR: ARC COSINE OUT OF RANGE",
            "Program attempted to perform an arc cosine\n"
            "operation when the argument was not in [-1,+1]."
        },
        {
            "ERROR: ARC TANGENT NOT DEFINED",
            "Program attempted to perform an arc tangent\n"
            "operation when both of the arguments were 0."
        },
        {
            "ERROR: LOGARITHM OUT OF RANGE",
            "Program attempted to perform a logarithm\n"
            "operation when the argument is not positive."
        },
        {
            "ERROR: POWER HAS NO REAL SOLUTION",
            "Program attempted to perform a power operation\n"
            "when base was negative and exponent non integer."
        },
        {
            "UNKNOWN ERROR",
            "Program caused a hardware error with an error\n"
            "code that was not recognized by the BIOS."
        }
    };
    
    // write the appropriate message for this error code
    if( error_code >= 0 && error_code < (int)error_unknown )
      draw_message_screen( &error_messages[ error_code ] );
    
    else
      draw_message_screen( &error_messages[ error_unknown ] );
    
    // now print the related hex values
    set_multiply_color( error_colors_values );
    print_hex_value( 49, 160, "Instruction Pointer", instruction_pointer );
    print_hex_value( 49, 180, "Instruction", instruction );
    print_hex_value( 49, 200, "Immediate Value", immediate_value );
    
    // stop any sound
    stop_all_channels();
}


// ---------------------------------------------------------
//     MAIN FUNCTION
// ---------------------------------------------------------

void main( void )
{
    // very small wait before starting,
    // to ensure a black screen is seen
    clear_screen( color_black );
    sleep( 15 );
    
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    select_texture( -1 );
    
    // all characters of the text font
    define_region_matrix( first_region_font,  1,1,  10,20,  1,1,  32,8,  0 );
    
    // white pixel
    select_region( region_white_pixel );
    define_region_topleft( 315,169,  315,169 );
    
    // console
    select_region( region_console );
    define_region_topleft( 1,164,  199,299 );
    
    // cartridge
    select_region( region_cartridge );
    define_region_topleft( 203,164,  284,237 );
    
    // down arrow
    select_region( region_down_arrow );
    define_region_topleft( 288,164,  311,223 );
    
    // white square
    select_region( region_white_square );
    define_region_topleft( 315,164,  316,165 );
    
    // ------------------------------------
    // PART 7: JUMP TO CARTRIDGE
    // ------------------------------------
    
    // if no cartrige is connected, show an alert screen and stop
    if( !cartridge_connected() )
    {
        request_cartridge();
        asm{ "hlt" }
    }
    
    // ensure that any video parameters we might have used
    // are restored to their expected defaults at startup
    set_multiply_color( color_white );
    select_region( 0 );
    
    // jump to first position in cartridge program rom
    asm{ "jmp 0x20000000" }
}
