// include Vircon libraries
#include "time.h"
#include "video.h"
#include "audio.h"
#include "math.h"
#include "input.h"

// include this project files
#include "Definitions.h"
#include "TexturePiano.h"


// ---------------------------------------------------------
//   GLOBAL VARIABLES
// ---------------------------------------------------------


// our general state
int SelectedNote = 12;                // from the left; range is 0 to 23
int SoundingNote = 0;                 // from the left; range is 0 to 23
int VolumeLevel = 100;                // in percentage; range is 0% to 200%
bool NotePressed = false;             // true while we press button A
int ChannelState = channel_stopped;   // channel state displayed by the buttons

// basic definitions for our piano
#define OctaveNotes 12
#define OctaveWhiteNotes 7

// graphic definitions for our piano
int PianoBaseX = 131;
int PianoBaseY = 230;
int OctaveWidth = 189;
int WhiteKeyWidth = 27;

// properties for each note within an octave
bool[ OctaveNotes ] BlackOctaveNotes = { false, true, false, true, false, false, true, false, true, false, true, false };
int[ OctaveNotes ] OctaveNoteXOffsets = { 0, 13, 27, 45, 54, 81, 93, 108, 125, 135, 157, 162 };


// ---------------------------------------------------------
//   SCENE DRAWING FUNCTIONS
// ---------------------------------------------------------


void DrawWhiteNotes()
{
    select_region( RegionWhiteKey );
    int x = PianoBaseX;
    
    for( int i = 0; i < (2 * OctaveWhiteNotes); i++ )
    {
        draw_region_at( x, PianoBaseY );
        x += WhiteKeyWidth;
    }    
}

// ---------------------------------------------------------

void DrawBlackNotes()
{
    select_region( RegionBlackKey );
    
    for( int i = 0; i < OctaveNotes; i++ )
    {
        if( !BlackOctaveNotes[ i ] )
          continue;
        
        draw_region_at( PianoBaseX + OctaveNoteXOffsets[ i ], PianoBaseY );
        draw_region_at( PianoBaseX + OctaveWidth + OctaveNoteXOffsets[ i ], PianoBaseY );
    }
}

// ---------------------------------------------------------

void DrawPressedNote()
{
    // decompose absolute note in octave + note
    int Octave = SoundingNote / OctaveNotes;
    int NoteInOctave = SoundingNote % OctaveNotes;
    
    if( BlackOctaveNotes[ NoteInOctave ] )
      select_region( RegionBlackKeyPressed );
    else
      select_region( RegionWhiteKeyPressed );
    
    int x = PianoBaseX + Octave * OctaveWidth;
    draw_region_at( x + OctaveNoteXOffsets[ NoteInOctave ], PianoBaseY );
}

// ---------------------------------------------------------

void DrawScene()
{
    clear_screen( 0xFF5C6C8D );
    
    // draw panels
    select_region( RegionControls );
    draw_region_at( 58, 57 );
    
    select_region( RegionStatePanel );
    draw_region_at( 343, 57 );
    
    // draw piano layer 1 (white notes)
    DrawWhiteNotes();
    int PressedNoteInOctave = (SoundingNote) % OctaveNotes;
    
    if( NotePressed )
      if( !BlackOctaveNotes[ PressedNoteInOctave ] )
        DrawPressedNote();
    
    // draw piano layer 2 (black notes)
    DrawBlackNotes();
    
    if( NotePressed )
      if( BlackOctaveNotes[ PressedNoteInOctave ] )
        DrawPressedNote();
    
    // draw arrow indicating selected note
    select_region( RegionArrow );
    draw_region_at( 128 + 16 * SelectedNote, 225 );
    
    // draw volume level
    select_region( RegionVolumeSlider );
    draw_region_at( 367 + VolumeLevel / 2, 139 );
    
    // draw playback state
    if( ChannelState == channel_stopped )
    {
        select_region( FirstRegionPlayButtons );
        draw_region_at( 351, 65 );
    }
    
    else if( ChannelState == channel_paused )
    {
        select_region( FirstRegionPlayButtons + 1 );
        draw_region_at( 399, 65 );
    }
    
    else if( ChannelState == channel_playing )
    {
        select_region( FirstRegionPlayButtons + 2 );
        draw_region_at( 447, 65 );
    }
}


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    DefineRegions_TexturePiano();
    
    // ---------------------------------------
    // PART 2: CONFIGURE SOUNDS AND CHANNELS
    // ---------------------------------------
    
    // configure our only sound
    select_sound( SoundPiano );
    set_sound_loop( true );
    set_sound_loop_start( 9081 );
    set_sound_loop_end( 15655 );
    
    // we will only play in this channel
    select_channel( ChannelPiano );
    set_channel_volume( 1 );
    set_global_volume( 0.5 );
    
    // all controls are on gamepad 1
    select_gamepad( 0 );
    
    // ------------------------------------
    // PART 4: MAIN LOOP
    // ------------------------------------
    
    while( true )
    {
        // process controls for note change
        if( gamepad_left() == 1 )
          if( SelectedNote > 0 )
            SelectedNote--;
        
        if( gamepad_right() == 1 )
          if( SelectedNote < 23 )
            SelectedNote++;
        
        // process controls for volume
        if( gamepad_down() == 1 )
          if( VolumeLevel > 0 )
          {
              VolumeLevel -= 20;
              set_channel_volume( VolumeLevel / 100.0 );
          }
        
        if( gamepad_up() == 1 )
          if( VolumeLevel < 200 )
          {
              VolumeLevel += 20;
              set_channel_volume( VolumeLevel / 100.0 );
          }
        
        // process controls for note playback
        if( gamepad_button_a() == 1 )
        {
            play_sound_in_channel( SoundPiano, ChannelPiano );
            SoundingNote = SelectedNote;
            NotePressed = true;
            
            // set pitch as N semitones offset from our
            // sample's base note (which is middle C)
            set_channel_speed( pow( 2, (SoundingNote-OctaveNotes)/12.0 ) );
        }
        else if( gamepad_button_a() == -1 )
        {
            set_channel_loop( false );
            NotePressed = false;
        }
        
        // process controls for pause
        ChannelState = get_channel_state( ChannelPiano );
        
        if( gamepad_button_b() == 1 )
        {
            if( ChannelState == channel_playing )
              pause_channel( ChannelPiano );
            
            else if( ChannelState == channel_paused )
              play_channel( ChannelPiano );
        }
        
        // update the scene on screen
        DrawScene();
        
        // wait until next frame
        end_frame();
    }
}
