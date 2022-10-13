// include Vircon libraries
#include "time.h"
#include "video.h"
#include "audio.h"
#include "math.h"
#include "input.h"
#include "misc.h"
#include "string.h"
#include "memcard.h"

// include this project files
#include "Definitions.h"
#include "Chip8.c"
#include "Chip8Instructions.c"
#include "Chip8Assembly.c"


// ---------------------------------------------------------
//   AUXILIARY FUNCTIONS
// ---------------------------------------------------------


void DrawChip8Status()
{
    // draw on/off leds
    select_region( FirstRegionLED + Chip8.Running );
    draw_region_at( 269, 314 );
    
    select_region( FirstRegionLED + Chip8.SuperChip8Enabled );
    draw_region_at( 311, 314 );
    
    // write current CPU speed (only if running)
    select_region( RegionDisplayOff );
    draw_region_at( 318, 340 );
    
    if( !Chip8.Running )
      return;
    
    int CPUSpeed = Chip8.CyclesPerTick * frames_per_second;
    int DrawX = 341;
    
    do
    {
        // write current digit
        int Digit = CPUSpeed % 10;
        select_region( RegionZero + Digit );
        draw_region_at( DrawX, 342 );
        
        // iterate
        CPUSpeed /= 10;
        DrawX -= 7;
    }
    while( CPUSpeed > 0 );
    
    // display the current control mapping
    select_region( FirstRegionControlsPad1 + CurrentControlMapping );
    draw_region_at( 57, 288 );
    
    select_region( FirstRegionControlsPad2 + CurrentControlMapping );
    draw_region_at( 457, 288 );
}


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    select_texture( TextureChip8 );
    
    select_region( RegionBackground );
    define_region_topleft( 0,0,  639,359 );
    
    select_region( RegionWhitePixel );
    define_region_topleft( 654,42,  654,42 );
    
    select_region( RegionDisplayOff );
    define_region_topleft( 318,340,  348,354 );
    
    select_region( RegionLargePixel );
    define_region_topleft( 643,35,  650,42 );
    
    select_region( RegionSmallPixel );
    define_region_topleft( 654,35,  657,38 );
    
    define_region_matrix( FirstRegionLED,  643,1,  657,15,  643,1,  1,2,  1 );
    define_region_matrix( RegionZero,  643,46,  648,56,  643,46,  2,5,  1 );
    define_region_matrix( FirstRegionControlsPad1,  1,363,  201,433,  1,363,  2,2,  1 );
    define_region_matrix( FirstRegionControlsPad2,  407,363,  532,433,  407,363,  2,2,  1 );
    
    
    // ---------------------------------------
    // PART 2: CONFIGURE SOUNDS AND CHANNELS
    // ---------------------------------------
    
    // configure our only sound
    select_sound( SoundBeep );
    set_sound_loop( true );
    
    // ------------------------------------
    // PART 3: GAME INITIALIZATIONS
    // ------------------------------------
    
    // configure the console
    srand( get_time() );
    
    // draw the background
    select_region( RegionBackground );
    draw_region_at( 0, 0 );
    
    // set initial emulator state
    Chip8_Reset();
    
    // ------------------------------------
    // PART 4: MAIN LOOP
    // ------------------------------------
    
    while( true )
    {
        // - - - - - - - - - - - - - - - - - - - - - - -
        // process emulator-level controls in gamepad 1
        select_gamepad( 0 );
        
        // reset chip-8 emulator
        if( gamepad_button_start() == 1 )
        {
            Chip8_Reset();
            continue;
        }
        
        // - - - - - - - - - - - - - - - - - - - - - - -
        // process emulator-level controls in gamepad 2
        select_gamepad( 1 );
        
        // cycle through the 4 control mappings
        if( gamepad_button_start() == 1 )
        {
            CurrentControlMapping++;
            CurrentControlMapping &= 3;
        }
        
        // cycle through the 4 screen color schemes
        if( gamepad_right() == 1 )
        {
            CurrentColorScheme++;
            CurrentColorScheme &= 3;
        }
        
        // increase CPU speed (maximum: 1500 Hz)
        if( gamepad_up() == 1 )
          if( Chip8.CyclesPerTick < 25 )
            Chip8.CyclesPerTick++;
        
        // reduce CPU speed (minimum: 300 Hz)
        if( gamepad_down() == 1 )
          if( Chip8.CyclesPerTick > 5 )
            Chip8.CyclesPerTick--;
        
        // - - - - - - - - - - - - - - - - - - - - - - -
        // on errors, report their message
        // and don't try to run the machine
        if( Chip8.ErrorHappened )
        {
            // first fill the screen background
            set_multiply_color( color_black );
            select_region( RegionWhitePixel );
            set_drawing_scale( 512, 256 );
            draw_region_zoomed_at( 64, 13 );
            
            // write the message
            set_multiply_color( color_white );
            print_at( 74, 23, Chip8.ErrorMessage );
        }
        
        // - - - - - - - - - - - - - - - - - - - - - - -
        // if chip-8 is on, emulate it
        else if( Chip8.Running )
        {
            // detect when super chip toggles to enabled
            int WasSuperChip = Chip8.SuperChip8Enabled;
            
            // operations done once per tick
            Chip8_UpdateTimers();
            Chip8_UpdateKeys();
            
            // run chip-8 CPU for a whole frame
            // (equivalent to 1 chip-8 tick)
            for( int FrameCycles = 0; FrameCycles < Chip8.CyclesPerTick; ++FrameCycles )
            {
                Chip8_RunInstruction();
                
                if( Chip8.ErrorHappened )
                  break;
            }
            
            // if super chip was just enabled, increase default CPU speed
            if( !WasSuperChip && Chip8.SuperChip8Enabled)
              Chip8.CyclesPerTick = 20;
            
            // update screen
            Chip8_DrawScreen();
        }
        
        // - - - - - - - - - - - - - - - - - - - - - - -
        // display chip8 status on screen
        DrawChip8Status();
        
        // wait until next frame
        end_frame();
    }
}
