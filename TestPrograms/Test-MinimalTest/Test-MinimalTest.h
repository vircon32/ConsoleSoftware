// =============================================================================
//   MINIMAL VIDEO FUNCTIONS
// =============================================================================


// colors we will use
#define color_purple  0xFFFF00FF

// -----------------------------------------------------------------------------

// texture ids in cartridge start from 0, but
// the BIOS texture at id = -1 is also selectable
void select_texture( int texture_id )
{
    asm
    {
        "mov R0, {texture_id}"
        "out GPU_SelectedTexture, R0"
    }
}

// -----------------------------------------------------------------------------

// region id refers to the currently selected texture
void select_region( int region_id )
{
    asm
    {
        "mov R0, {region_id}"
        "out GPU_SelectedRegion, R0"
    }
}

// -----------------------------------------------------------------------------

// applies to the currently selected region;
// sets the hostpot at its center
void define_region_center( int min_x, int min_y, int max_x, int max_y )
{
    int center_x = (min_x + max_x) / 2;
    int center_y = (min_y + max_y) / 2;
    
    asm
    {
        "mov R0, {min_x}"
        "out GPU_RegionMinX, R0"
        "mov R0, {min_y}"
        "out GPU_RegionMinY, R0"
        "mov R0, {max_x}"
        "out GPU_RegionMaxX, R0"
        "mov R0, {max_y}"
        "out GPU_RegionMaxY, R0"
        "mov R0, {center_x}"
        "out GPU_RegionHotSpotX, R0"
        "mov R0, {center_y}"
        "out GPU_RegionHotSpotY, R0"
    }
}

// -----------------------------------------------------------------------------

void clear_screen( int color )
{
    asm
    {
        "mov R0, {color}"
        "out GPU_ClearColor, R0"
        "out GPU_Command, GPUCommand_ClearScreen"
    }
}

// -----------------------------------------------------------------------------

// draws the currently selected region
// version that specifies drawing point
void draw_region_at( int drawing_x, int drawing_y )
{
    asm
    {
        "mov R0, {drawing_x}"
        "out GPU_DrawingPointX, R0"
        "mov R0, {drawing_y}"
        "out GPU_DrawingPointY, R0"
        "out GPU_Command, GPUCommand_DrawRegion"
    }
}


// =============================================================================
//   MINIMAL AUDIO FUNCTIONS
// =============================================================================


void play_sound_in_channel( int sound_id, int channel_id )
{
    asm
    {
        "mov R0, {channel_id}"
        "out SPU_SelectedChannel, R0"
        "mov R0, {sound_id}"
        "out SPU_ChannelAssignedSound, R0"
        "out SPU_Command, SPUCommand_PlaySelectedChannel"
    }
}
