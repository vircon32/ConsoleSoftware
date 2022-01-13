// include Vircon libraries
#include "video.h"
#include "audio.h"
#include "time.h"
#include "input.h"
#include "math.h"
#include "misc.h"


// ---------------------------------------------------------
//   DEFINITIONS AND CONSTANTS
// ---------------------------------------------------------


// names for textures
#define TextureEditor  0

// names for texture regions
#define RegionBackground    0
#define RegionActiveColumn  1
#define RegionStop          2
#define RegionPause         3
#define RegionPlay          4
#define RegionButtonL       5
#define RegionButtonR       6
#define RegionDrum          7
#define RegionSticks        8
#define RegionClap          9
#define RegionBell         10
#define RegionSelection    11
#define RegionCellMark     12

// names for sounds
#define SoundDrum    0
#define SoundSticks  1
#define SoundClap    2
#define SoundBell    3

// names for sound channels
#define ChannelDrum    0
#define ChannelSticks  1
#define ChannelClap    2
#define ChannelBell    3

// program states
#define StateStopped  0
#define StatePaused   1
#define StatePlaying  2


// ---------------------------------------------------------
//   AUXILIARY FUNCTIONS
// ---------------------------------------------------------


// cells are not evenly spaced, and several functions need
// their positions, so these helper functions calculate them
int GetCellLeft( int CellX )
{
    return 66 + (CellX/4)*141 + (CellX%4)*33;
}

int GetCellTop( int CellY )
{
    return 153 + CellY*48;
}

// for directions, we will fake an "auto-repeat"
// so that selection movement is more convenient
bool IsKeyTriggered( int KeyValue )
{
    // delay after first trigger
    if( KeyValue < 20 )
      return (KeyValue == 1);
    
    // faster repeats after that
    return !((KeyValue-20) % 6);
}


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    select_texture( TextureEditor );
    
    select_region( RegionBackground );
    define_region_topleft( 0,0,  639,359 );
    
    select_region( RegionActiveColumn );
    define_region_topleft( 641,1,  674,184 );
    
    define_region_matrix( RegionStop,  1,361,  70,430,  1,361,  3,1,  1  );
    define_region_matrix( RegionButtonL,  214,361,  253,400,  214,361,  2,1,  1  );
    define_region_matrix( RegionDrum,  296,361,  335,400,  296,361,  4,1,  1  );
    
    select_region( RegionSelection );
    define_region( 460,361,  501,408,  464,365 );
    
    select_region( RegionCellMark );
    define_region_topleft( 503,361,  536,400 );
    
    // ------------------------------------
    // PART 2: INITIAL PREPARATIONS
    // ------------------------------------
    
    int State = StateStopped;
    int PlayedFrames = 0;
    int ActiveColumn = 0;
    
    // initialize our array of cells
    bool[ 4 ][ 16 ] Cells;
    memset( &Cells[0][0], 0, sizeof(Cells) );
    
    // initialize cell selection
    int SelectedCellX = 0;
    int SelectedCellY = 0;
    
    // only the first gamepad is used
    select_gamepad( 0 );
    
    // ------------------------------------
    // PART 3: MAIN LOOP
    // ------------------------------------
    
    while( true )
    {
        // - - - - - - - - - - - - - - - - - -
        // STEP 1: CHECK INPUTS
        // - - - - - - - - - - - - - - - - - -
        
        // check inputs for the music player
        if( gamepad_button_l() == 1 )
          if( State != StateStopped )
          {
              State = StateStopped;
              PlayedFrames = 0;
              ActiveColumn = 0;
          }
            
        if( gamepad_button_r() == 1 )
        {
            if( State == StateStopped )
              State = StatePlaying;
            
            else if( State == StatePlaying )
              State = StatePaused;
              
            else
              State = StatePlaying;
        }
        
        // check inputs for track edition
        if( gamepad_button_a() == 1 )
        {
            bool* Cell = &Cells[ SelectedCellY ][ SelectedCellX ];
            *Cell = !(*Cell);
        }
        
        // for directions, we will fake an "auto-repeat"
        // so that the matrix can be traversed more easily;
        if( IsKeyTriggered( gamepad_left() ) )
          if( SelectedCellX > 0 )
            SelectedCellX--;
          
        if( IsKeyTriggered( gamepad_right() ) )
          if( SelectedCellX < 15 )
            SelectedCellX++;
          
        if( IsKeyTriggered( gamepad_up() ) )
          if( SelectedCellY > 0 )
            SelectedCellY--;
          
        if( IsKeyTriggered( gamepad_down() ) )
          if( SelectedCellY < 3 )
            SelectedCellY++;
        
        // - - - - - - - - - - - - - - - - - -
        // STEP 2: DRAW SCENE
        // - - - - - - - - - - - - - - - - - -
        
        // draw background
        select_region( RegionBackground );
        draw_region_at( 0,0 );
        
        // draw current player state
        if( State == StatePlaying )
        {
            select_region( RegionPlay );
            draw_region_at( 499,30 );
            
            select_region( RegionButtonL );
            draw_region_at( 29,46 );
            
            select_region( RegionButtonR );
            draw_region_at( 237,46 );
        }
        
        else if( State == StatePaused )
        {
            select_region( RegionPause );
            draw_region_at( 285,30 );
            
            select_region( RegionButtonL );
            draw_region_at( 29,46 );
            
            select_region( RegionButtonR );
            draw_region_at( 453,46 );
        }
        
        else
        {
            select_region( RegionStop );
            draw_region_at( 75,30 );
            
            select_region( RegionButtonR );
            draw_region_at( 453,46 );
        }
        
        // highlight active bar
        select_region( RegionActiveColumn );
        draw_region_at( GetCellLeft( ActiveColumn ), GetCellTop( 0 ) );
        
        // draw marked cells
        select_region( RegionCellMark );
        
        for( int CellY = 0; CellY < 4; CellY++ )
          for( int CellX = 0; CellX < 16; CellX++ )
            if( Cells[ CellY ][ CellX ] )
              draw_region_at( GetCellLeft( CellX ), GetCellTop( CellY ) );
        
        // draw selected cell
        select_region( RegionSelection );
        draw_region_at( GetCellLeft( SelectedCellX ), GetCellTop( SelectedCellY ) );
        
        // draw playing instruments
        for( int i = 0; i < 4; i++ )
        {
            if( get_channel_state( i ) == channel_playing )
            {
                select_region( RegionDrum + i );
                draw_region_at( 18, GetCellTop( i ) );
            }
        }
        
        // - - - - - - - - - - - - - - - - - -
        // STEP 3: APPLY PLAYER LOGIC
        // - - - - - - - - - - - - - - - - - -
        
        // play instruments at start of each column
        if( State == StatePlaying )
          if( !(PlayedFrames % 7) )
            for( int i = 0; i < 4; i++ )
            {
                if( Cells[ 0 ][ ActiveColumn ] )
                  play_sound_in_channel( SoundDrum, ChannelDrum );
                
                if( Cells[ 1 ][ ActiveColumn ] )
                  play_sound_in_channel( SoundSticks, ChannelSticks );
                  
                if( Cells[ 2 ][ ActiveColumn ] )
                  play_sound_in_channel( SoundClap, ChannelClap );
                  
                if( Cells[ 3 ][ ActiveColumn ] )
                  play_sound_in_channel( SoundBell, ChannelBell );
            }
        
        // control timing for song advance
        if( State == StatePlaying )
        {
            PlayedFrames++;
            ActiveColumn = (PlayedFrames/7) % 16;
        }
        
        end_frame();
    }
}
