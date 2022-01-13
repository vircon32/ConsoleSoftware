// =============================================================================
//   RGBA COLOR REPRESENTATION
// =============================================================================


// colors are given by their RGBA value,
// but since Vircon's in-word byte order
// is little endian, their representation
// in written form actually has to be ABGR
// (same as in most current computers)
#define color_black      0xFF000000
#define color_white      0xFFFFFFFF
#define color_red        0xFF0000FF
#define color_yellow     0xFF00FFFF

// -----------------------------------------------------------------------------

// assumes full opacity for alpha
// valid range is 0 (black) to 255 (white)
int make_gray( int brightness )
{
    return 0xFF000000 | (brightness << 16) | (brightness << 8) | brightness;
}


// =============================================================================
//   GPU SELECTION FUNCTIONS
// =============================================================================


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


// =============================================================================
//   DEFINITION OF GPU TEXTURE REGIONS
// =============================================================================


// applies to the currently selected region
void define_region( int min_x, int min_y, int max_x, int max_y, int hotspot_x, int hotspot_y )
{
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
        "mov R0, {hotspot_x}"
        "out GPU_RegionHotSpotX, R0"
        "mov R0, {hotspot_y}"
        "out GPU_RegionHotSpotY, R0"
    }
}

// -----------------------------------------------------------------------------

// applies to the currently selected region
// sets the hostpot at its top left, i.e. (min_x, min_y)
void define_region_topleft( int min_x, int min_y, int max_x, int max_y )
{
    asm
    {
        "mov R0, {min_x}"
        "out GPU_RegionMinX, R0"
        "out GPU_RegionHotSpotX, R0"
        "mov R0, {min_y}"
        "out GPU_RegionMinY, R0"
        "out GPU_RegionHotSpotY, R0"
        "mov R0, {max_x}"
        "out GPU_RegionMaxX, R0"
        "mov R0, {max_y}"
        "out GPU_RegionMaxY, R0"
    }
}

// -----------------------------------------------------------------------------

// defines regions with consecutive ids from first_id
// for each region, sets the hostpot at the top left
// gap between regions is assumed to be the same in x and y
void define_region_matrix( int first_id, int min_x, int min_y, int width, int height, int elements_x, int elements_y, int gap )
{
    int current_id = first_id;
    int texture_x = min_x;
    int texture_y = min_y;
    
    for( int matrix_y = 0; matrix_y < elements_y; ++matrix_y )
    {
        for( int matrix_x = 0; matrix_x < elements_x; ++matrix_x )
        {
            // define this region
            select_region( current_id );
            define_region_topleft( texture_x, texture_y, texture_x + width - 1, texture_y + height - 1 );
            current_id += 1;
            
            // advance in X
            texture_x += width + gap;
        }
        
        // advance in Y
        texture_y += height + gap;
        
        // reset X to 0
        texture_x = min_x;
    }
}


// =============================================================================
//   SETTING GPU PARAMETERS
// =============================================================================


// the effect of this color is the same as in
// OpenGL's glColor (white is neutral)
void set_multiply_color( int color )
{
    asm
    {
        "mov R0, {color}"
        "out GPU_MultiplyColor, R0"
    }
}

// -----------------------------------------------------------------------------

// regions will be drawn so that their hot spot
// is placed at the current drawing point on screen
void set_drawing_point( int drawing_x, int drawing_y )
{
    asm
    {
        "mov R0, {drawing_x}"
        "out GPU_DrawingPointX, R0"
        "mov R0, {drawing_y}"
        "out GPU_DrawingPointY, R0"
    }
}

// -----------------------------------------------------------------------------

// negative scale factors are allowed, which have
// the effect of flipping the image along x and y
void set_drawing_scale( float scale_x, float scale_y )
{
    asm
    {
        "mov R0, {scale_x}"
        "out GPU_DrawingScaleX, R0"
        "mov R0, {scale_y}"
        "out GPU_DrawingScaleY, R0"
    }
}


// =============================================================================
//   REQUESTING GPU COMMANDS
// =============================================================================


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
// uses the current drawing point
void draw_region()
{
    asm
    {
        "out GPU_Command, GPUCommand_DrawRegion"
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

// -----------------------------------------------------------------------------

// draws the currently selected region with current scale
// version that specifies drawing point
void draw_region_zoomed_at( int drawing_x, int drawing_y )
{
    asm
    {
        "mov R0, {drawing_x}"
        "out GPU_DrawingPointX, R0"
        "mov R0, {drawing_y}"
        "out GPU_DrawingPointY, R0"
        "out GPU_Command, GPUCommand_DrawRegionZoomed"
    }
}
