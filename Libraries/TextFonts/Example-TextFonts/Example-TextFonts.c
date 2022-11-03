// *****************************************************************************
    // include standard Vircon headers
    #include "time.h"
    #include "video.h"
    
    // include additional headers
    #include "../textfont.h"
    #include "../numberfont.h"
// *****************************************************************************


// ---------------------------------------------------------
//   DEFINITIONS
// ---------------------------------------------------------


// names for textures
#define TextureFullFont 0
#define TextureNumberFont 1

// texture regions for full font texture
#define FirstRegionFullFont 0

// texture regions for number font texture
#define RegionMinus 0
#define RegionDot 1
#define RegionZero 2


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    // - - - - - - - - - - - - - - - - - - -
    // 1.1: define regions for full font
    
    select_texture( TextureFullFont );
    
    // First we define define 128 consecutive regions (i.e. standard ASCII only)
    // with the same size and hotspot position, as for a constant-width font
    define_region_matrix( FirstRegionFullFont,  0,0,  21,31,  0,31,  16,8,  0 );
    
    // then we redefine some characters to have different widths
    // (note that, for this font, upper and lowercase letters are the same)
    select_region( FirstRegionFullFont + 'M' );
    define_region( 22,0,  46,31,  22,31 );
    select_region( FirstRegionFullFont + 'm' );
    define_region( 22,0,  46,31,  22,31 );
    
    select_region( FirstRegionFullFont + 'W' );
    define_region( 66,0,  90,31,  66,31 );
    select_region( FirstRegionFullFont + 'w' );
    define_region( 66,0,  90,31,  66,31 );
    
    select_region( FirstRegionFullFont + 'I' );
    define_region( 110,0,  121,31,  110,31 );
    select_region( FirstRegionFullFont + 'i' );
    define_region( 110,0,  121,31,  110,31 );
    
    select_region( FirstRegionFullFont + ' ' );
    define_region( 0,64,  15,95,  0,95 );
    
    // - - - - - - - - - - - - - - - - - - -
    // 1.2: define regions for number font
    
    select_texture( TextureNumberFont );
    define_region_matrix( RegionMinus,  1,1,  15,21,  1,21,  12,1,  1 );
    
    // ------------------------------------
    // PART 2: DEFINE OUR CUSTOM FONTS
    // ------------------------------------
    
    textfont FontLetters;
    numberfont FontNumbers;
    
    // - - - - - - - - - - - - - - - - - - -
    // 2.1: define full font
    
    // define dimensions for our characters
    FontLetters.character_height = 31;
    FontLetters.use_variable_width = true;
    textfont_read_region_widths( &FontLetters );
    
    // 2 pixels overlap between characters, 15 pixels between lines
    FontLetters.character_separation = -2;
    FontLetters.line_separation = 15;
    
    // define texture and regions for our characters
    FontLetters.texture_id = TextureFullFont;
    FontLetters.character_zero_region_id = FirstRegionFullFont;
    
    // - - - - - - - - - - - - - - - - - - -
    // 2.2: define number font
    
    // define dimensions for our characters
    FontNumbers.character_width = 15;
    FontNumbers.character_height = 21;
    
    // 2 pixels between characters
    FontNumbers.character_separation = 2;
    
    // define texture and regions for our characters
    FontNumbers.texture_id = TextureNumberFont;
    FontNumbers.region_id_zero = RegionZero;
    FontNumbers.region_id_minus = RegionMinus;
    FontNumbers.region_id_dot = RegionDot;
    
    // ------------------------------------
    // PART 3: USE OUR FONTS
    // ------------------------------------
    
    // fill screen with a visible color
    clear_screen( color_blue );
    
    // with our full font print text in all alignments;
    // also, the text in the middle is multi-line
    textfont_print_from_left
    (
        &FontLetters,
        5, 32 + 5,
        "Left aligned"
    );
    
    textfont_print_centered
    (
        &FontLetters,
        screen_width/2, screen_height/2 - 15/2,
        "Centered at middle" "\n" "with 2 lines!"
    );
    
    textfont_print_from_right
    (
        &FontLetters,
        screen_width - 5, screen_height - 5,
        "Right aligned"
    );
    
    // with our number font print an integer using 4
    // fixed digits, and a float aligned to the right
    numberfont_print_int_fixed
    (
        &FontNumbers,
        5, screen_height - 5,
        57,  // our integer
        4    // use 4 digits
    );
    
    numberfont_print_float_from_right
    (
        &FontNumbers,
        screen_width - 5, 21 + 5,
        -3.1416  // our float
    );
    
    // wait until next frame to ensure
    // everything gets drawn on screen
    end_frame();
    
    // (our program will now end, but our drawn
    // screen contents will keep being shown)
}
