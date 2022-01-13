// =============================================================================
//   READING TIME INFORMATION
// =============================================================================


// returned number is the number of frames elapsed from
// the last time the console was switched on or reset
int get_frame_counter()
{
    asm
    {
        "in R0, TIM_FrameCounter"
    }
}


// =============================================================================
//   TIMING CONTROL FUNCTIONS
// =============================================================================


// waits for the current frame to end
// (CPU blocks until next frame begins)
void end_frame()
{
    asm{ "wait" }
}

// -----------------------------------------------------------------------------

// waits for the specified number of frames
// (parameter is not in seconds as in regular C)
void sleep( int frames )
{
    int initial_frames = get_frame_counter();
    int final_frames = initial_frames + frames;
    
    while( get_frame_counter() < final_frames )
      end_frame();
}
