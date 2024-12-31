// include Vircon32 libraries
#include "video.h"
#include "audio.h"
#include "time.h"


// ---------------------------------------------------------
//   DEFINITIONS
// ---------------------------------------------------------

// declare cartridge contents
enum CartridgeTextures
{
    TextureBadApple
};

enum CartridgeSounds
{
    MusicBadApple
};

// give names to our texture regions
enum BadAppleRegions
{
    RegionBackground,
    RegionBlackLine
};

// some general definitions
#define VIDEO_START_X 80

// definitions for the encoder
#define NEW_LINE_CODE   0xFFFF       // lines are started with { 0xFFFF, <line_y> }
#define END_FRAME_CODE  0xFFFFFFFF   // this code indicates current frame has ended
#define END_VIDEO_CODE  0xFFFFEEEE   // this code indicates there are no more frames

// declare external data, loaded from file
// (size in words = size of file / 4 bytes per word)
embedded int[ 27355456 / 4 ] EncodedFrames = "EncodedFrames.bin";


// ---------------------------------------------------------
//   FRAME DECODING FUNCTIONS
// ---------------------------------------------------------


void DrawBlackSegment( int ScreenX1, int ScreenX2, int ScreenY )
{
    define_region_topleft( ScreenX1, 361, ScreenX2, 361 );
    draw_region_at( ScreenX1, ScreenY );
}

// ---------------------------------------------------------

// decodes and draws the next frame; after that
// it will update the pointer for the caller
void DrawFrame( int** CurrentWordPointer )
{
    int* CurrentWord = *CurrentWordPointer;
    int CurrentLineY = 0;
    select_region( RegionBlackLine );
    
    // keep decoding until the frame ends
    while( *CurrentWord != END_FRAME_CODE )
    {
        // decode word into its 2 16-bit components
        int Value1 = *CurrentWord >> 16;
        int Value2 = *CurrentWord & 0xFFFF;
        
        // CASE 1: this upper value sets a new line
        if( Value1 == NEW_LINE_CODE )
          CurrentLineY = Value2;
        
        // CASE 2: other values draw a horizontal black segment
        else
        {
            int StartX = VIDEO_START_X + Value1;
            int FinalX = VIDEO_START_X + Value2;
            DrawBlackSegment( StartX, FinalX, CurrentLineY );
        }
        
        // advance to next word
        CurrentWord++;
    }
    
    // go to next frame and update the caller's pointer
    CurrentWord++;
    *CurrentWordPointer = CurrentWord;
}


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{   
    // -----------------------------
    // PART 1: INITIALIZE
    
    // select our only texture
    select_texture( TextureBadApple );
    
    // define the background region in that texture
    select_region( RegionBackground );
    define_region_topleft( 0, 0, screen_width-1, screen_height-1 );
    
    // start playing the song
    play_sound( MusicBadApple );
    
    // -----------------------------
    // PART 2: LOOP TO DISPLAY ALL FRAMES
    
    int *CurrentWord = &EncodedFrames[ 0 ];
    
    while( *CurrentWord != END_VIDEO_CODE )
    {
        // draw the background
        select_region( RegionBackground );
        draw_region_at( 0, 0 );
        
        // decode and draw the next frame
        DrawFrame( &CurrentWord );
        
        // wait 2 frames: Vircon32 runs at 60 fps,
        // but this video only has 30 images per second
        end_frame();
        end_frame();
    }
}
