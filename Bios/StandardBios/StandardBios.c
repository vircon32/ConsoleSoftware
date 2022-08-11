// include Vircon libraries
#include "audio.h"
#include "video.h"
#include "time.h"
#include "string.h"
#include "misc.h"


// ---------------------------------------------------------
//     GENERAL DEFINITIONS
// ---------------------------------------------------------


// BIOS-required regions; these
// may safely be used by programs
#define first_region_font      0
#define region_white_pixel   256

// other non-required regions, used
// to draw the logo and error screens
#define region_logo_letters  300
#define region_logo_line     301
#define region_subtitle      302
#define region_console       303
#define region_gamepad       304
#define region_cartridge     305
#define region_memory_card   306
#define region_large_arrow   307
#define region_small_arrow   308
#define region_white_square  309

// an auxiliary region that will keep
// being redefined for logo animation
#define region_logo_temp     400

// colors for error screens
#define error_colors_background  0xFF8D4130
#define error_colors_title       color_yellow
#define error_colors_message     color_white
#define error_colors_values      0xFF8080FF


// ---------------------------------------------------------
//     SUPPORT FUNCTIONS
// ---------------------------------------------------------


void draw_message_screen( int* title, int* message )
{
    clear_screen( error_colors_background );
    set_multiply_color( color_white );
    select_texture( -1 );
    
    // write title
    set_multiply_color( error_colors_title );
    print_at( 49, 37, title );
    
    // draw horizontal line
    select_region( region_white_square );
    set_drawing_scale( 640/2, 1 );
    draw_region_zoomed_at( 0, 60 );
    set_drawing_scale( 1, 1 );
    
    // write message
    set_multiply_color( error_colors_message );
    print_at( 49, 95, message );
}

// ---------------------------------------------------------

void print_hex_value( int x, int y, int* name, int value )
{
    // convert the number to hex
    int[ 10 ] hex_string;
    itoa( value, hex_string, 16 );
    
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
    draw_message_screen
    (
        "NO CARTRIDGE FOUND",
        "To play a game, please power off\n"
        "your console and insert a game\n"
        "cartridge compatible with Vircon32."
    );
    
    // draw console diagram
    set_multiply_color( color_white );
    select_region( region_console );
    draw_region_at( 400, 207 );
    
    select_region( region_cartridge );
    draw_region_at( 469, 76 );
    
    select_region( region_large_arrow );
    draw_region_at( 497, 149 );
    
    // ensure everything gets drawn
    end_frame();
}


// ---------------------------------------------------------
//     HARDWARE ERROR CODES
// ---------------------------------------------------------


#define error_memory_read       0
#define error_memory_write      1
#define error_port_read         2
#define error_port_write        3
#define error_stack_overflow    4
#define error_stack_underflow   5
#define error_division          6
#define error_arc_cosine        7
#define error_arc_tangent_2     8
#define error_logarithm         9
#define error_power            10
#define error_unknown          11


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
    
    // determine messages for this error code
    int* error_title = NULL;
    int* error_message = NULL;
    
    if( error_code == error_memory_read )
    {
        error_title = "ERROR: INVALID MEMORY READ";
        
        error_message = "Program attempted to read from a memory address\n"
                        "that does not exist or is in a write-only device.";
    }
    
    else if( error_code == error_memory_write )
    {
        error_title = "ERROR: INVALID MEMORY WRITE";
        
        error_message = "Program attempted to write on a memory address\n"
                        "that does not exist or is in a read-only device.";
    }
    
    else if( error_code == error_port_read )
    {
        error_title = "ERROR: INVALID PORT READ";
        
        error_message = "Program attempted to read from a port number\n"
                        "that does not exist or is set as write-only.";
    }
    
    else if( error_code == error_port_write )
    {
        error_title = "ERROR: INVALID PORT WRITE";
        
        error_message = "Program attempted to write on a port number\n"
                        "that does not exist or is set as read-only.";
    }
    
    else if( error_code == error_stack_overflow )
    {
        error_title = "ERROR: STACK OVERFLOW";
        
        error_message = "Program pushed too many values in the stack\n"
                        "and available RAM memory was exhausted.";
    }
    
    else if( error_code == error_stack_underflow )
    {
        error_title = "ERROR: STACK UNDERFLOW";
        
        error_message = "Program popped too many values from the stack\n"
                        "and all data stored in stack was exhausted.";
    }
    
    else if( error_code == error_division )
    {
        error_title = "ERROR: DIVISION BY ZERO";
        
        error_message = "Program attempted to perform a division or\n"
                        "modulus operation where the divisor was zero.";
    }
    
    else if( error_code == error_arc_cosine )
    {
        error_title = "ERROR: ARC COSINE OUT OF RANGE";
        
        error_message = "Program attempted to perform an arc cosine\n"
                        "operation when the argument was not in [-1,+1].";
    }
    
    else if( error_code == error_arc_tangent_2 )
    {
        error_title = "ERROR: ARC TANGENT NOT DEFINED";
        
        error_message = "Program attempted to perform an arc tangent\n"
                        "operation when both of the arguments were 0.";
    }
    
    else if( error_code == error_logarithm )
    {
        error_title = "ERROR: LOGARITHM OUT OF RANGE";
        
        error_message = "Program attempted to perform a logarithm\n"
                        "operation when the argument is not positive.";
    }
    
    else if( error_code == error_power )
    {
        error_title = "ERROR: POWER HAS NO REAL SOLUTION";
        
        error_message = "Program attempted to perform a power operation\n"
                        "when base was negative and exponent non integer.";
    }
    
    else
    {
        error_title = "UNKNOWN ERROR";
        
        error_message = "Program caused a hardware error with an error\n"
                        "code that was not recognized by the BIOS.";
    }
    
    // draw the basic screen
    draw_message_screen( error_title, error_message );
    
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
    // wait a bit before starting
    sleep( 30 );
    
    // play logo sound
    play_sound_in_channel( -1, 0 );
    
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    select_texture( -1 );
    
    // all characters of the text font
    define_region_matrix( first_region_font,  1,22,  10,41,  1,22,  32,8,  0 );
    
    // white pixel
    select_region( region_white_pixel );
    define_region_topleft( 469,29,  469,29 );
    
    // logo letters
    select_region( region_logo_letters );
    define_region( 1,183,  278,238,  140,238 );
    
    // logo line
    select_region( region_logo_line );
    define_region( 1,240,  278,247,  140,232 );
    
    // subtitle
    select_region( region_subtitle );
    define_region( 1,1,  440,20,  221,20 );
    
    // console
    select_region( region_console );
    define_region_topleft( 280,191,  478,326 );
    
    // cartridge
    select_region( region_cartridge );
    define_region_topleft( 322,116,  403,189 );
    
    // large arrow
    select_region( region_large_arrow );
    define_region_topleft( 444,26,  467,85 );
    
    // white square
    select_region( region_white_square );
    define_region_topleft( 469,26,  470,27 );
    
    // ------------------------------------
    // PART 2: FORM LOGO LINE
    // ------------------------------------
    
    int elapsed_frames = 0;
    set_multiply_color( color_white );
    set_drawing_point( 320, 178 );
    
    while( true )
    {
        // draw background
        clear_screen( color_black );
        
        // define a copied line that grows each frame
        select_region( region_logo_temp );
        define_region( 140-4.6*elapsed_frames,240,  140+4.6*elapsed_frames,247,  140,232 );
        draw_region();
        
        // control timing
        end_frame();
        ++elapsed_frames;
        
        if( elapsed_frames >= 30 )
          break;
    }
    
    // ------------------------------------
    // PART 3: FORM LOGO LETTERS
    // ------------------------------------
    
    elapsed_frames = 0;
    
    while( true )
    {
        // draw background
        clear_screen( color_black );
        
        // draw the full line
        select_region( region_logo_line );
        draw_region();
        
        // define a copied logo that grows each frame
        select_region( region_logo_temp );
        int MaxY = 183 + 1.38*elapsed_frames;  // 1.35
        define_region( 1,183,  278,MaxY,  140,MaxY );
        draw_region();
        
        // control timing
        end_frame();
        ++elapsed_frames;
        
        if( elapsed_frames >= 40 )
          break;
    }
    
    // ------------------------------------
    // PART 4: FADE IN SUBTITLE
    // ------------------------------------
    
    elapsed_frames = 0;
    
    while( true )
    {
        // draw background
        clear_screen( color_black );
        
        // draw complete logo
        set_multiply_color( color_white );
        set_drawing_point( 320, 178 );
        select_region( region_logo_line );
        draw_region();
        select_region( region_logo_letters );
        draw_region();
        
        // set the fade in
        int gray_level = 255 * elapsed_frames / 30;
        set_multiply_color( make_gray( gray_level ) );
        
        // draw the subtitle
        set_drawing_point( 320, 270 );
        select_region( region_subtitle );
        draw_region();
        
        // control timing
        end_frame();
        ++elapsed_frames;
        
        if( elapsed_frames >= 30 )
          break;
    }
    
    // ------------------------------------
    // PART 5: PAUSE FOR A MOMENT
    // ------------------------------------
    
    sleep( 50 );
    
    // ------------------------------------
    // PART 6: FADE OUT TO BLACK
    // ------------------------------------
    
    elapsed_frames = 0;
    
    while( true )
    {
        // draw background
        set_multiply_color( color_white );
        clear_screen( color_black );
        
        // set the fade out
        int gray_level = 255 * (40-elapsed_frames) / 40;
        set_multiply_color( make_gray( gray_level ) );
        
        // draw complete logo
        set_drawing_point( 320, 178 );
        select_region( region_logo_line );
        draw_region();
        select_region( region_logo_letters );
        draw_region();
        
        // draw the subtitle
        select_region( region_subtitle );
        draw_region_at( 320, 270 );
        
        // control timing
        end_frame();
        ++elapsed_frames;
        
        if( elapsed_frames >= 40 )
          break;
    }
    
    // ------------------------------------
    // PART 7: JUMP TO CARTRIDGE
    // ------------------------------------
    
    // small wait before jump
    clear_screen( color_black );
    sleep( 30 );
    
    // if no cartrige is connected, show an alert screen and stop
    if( !cartridge_connected() )
    {
        request_cartridge();
        exit();
    }
    
    // ensure that any video parameters we might have used
    // are restored to their expected defaults at startup
    set_multiply_color( color_white );
    select_region( 0 );
    
    // jump to first position in cartridge program rom
    asm{ "jmp 0x20000000" }
}
