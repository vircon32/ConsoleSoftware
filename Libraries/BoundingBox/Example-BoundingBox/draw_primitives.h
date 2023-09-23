/* *****************************************************************************
*  Vircon32 library: "draw_primitives.h"          File version: 2022/09/18     *
*  --------------------------------------------------------------------------- *
*  This header allows programs to draw basic geometric shapes without needing  *
*  to include any additional textures. It uses only the white pixel region     *
*  defined by the BIOS (requires a BIOS version 1.1). Drawn shape color will   *
*  determined by the current multiply color.                                   *
*                                                                              *
*  Keep in mind that these functions will change selected texture and region.  *
***************************************************************************** */

// *****************************************************************************
    // start include guard
    #ifndef DRAW_PRIMITIVES_H
    #define DRAW_PRIMITIVES_H
    
    // include standard Vircon headers
    #include "video.h"
    #include "math.h"
// *****************************************************************************


void draw_pixel( int x, int y )
{
    asm
    {
        // select the bios's white pixel region
        "out GPU_SelectedTexture, -1"
        "out GPU_SelectedRegion, 256"
        
        // draw it at the requested position
        "mov R0, {x}"
        "out GPU_DrawingPointX, R0"
        "mov R0, {y}"
        "out GPU_DrawingPointY, R0"
        "out GPU_Command, GPUCommand_DrawRegion"
    }
}

// ---------------------------------------------------------

void draw_horizontal_line( int x1, int y1, int x2 )
{
    // select the bios's white pixel region
    asm{ "out GPU_SelectedTexture, -1"
         "out GPU_SelectedRegion, 256" }
    
    // draw horizontal line
    set_drawing_scale( x2-x1+1, 1 );
    draw_region_zoomed_at( x1, y1 );
}

// ---------------------------------------------------------

void draw_vertical_line( int x1, int y1, int y2 )
{
    // select the bios's white pixel region
    asm{ "out GPU_SelectedTexture, -1"
         "out GPU_SelectedRegion, 256" }
    
    // draw vertical line
    set_drawing_scale( 1, y2-y1+1 );
    draw_region_zoomed_at( x1, y1 );
}

// ---------------------------------------------------------

void draw_line( int x1, int y1, int x2, int y2 )
{
    // select the bios's white pixel region
    asm{ "out GPU_SelectedTexture, -1"
         "out GPU_SelectedRegion, 256" }
    
    // careful with this case or atan2
    // will produce a hardware error
    if( x1 == x2 && y1 == y2 )
    {
        draw_region_at( x1, y1 );
        return;
    }
    
    // convert line to polar coordinates
    int delta_x = x2 - x1;
    int delta_y = y2 - y1;
    float line_distance = 1 + sqrt( delta_x * delta_x + delta_y * delta_y );
    float line_angle = atan2( delta_y, delta_x );
    
    // draw the line
    set_drawing_angle( line_angle );
    set_drawing_scale( line_distance, 1 );
    draw_region_rotozoomed_at( x1, y1 );
}

// ---------------------------------------------------------

void draw_rectangle( int x1, int y1, int x2, int y2 )
{
    // select the bios's white pixel region
    asm{ "out GPU_SelectedTexture, -1"
         "out GPU_SelectedRegion, 256" }
    
    // draw horizontal lines
    set_drawing_scale( x2-x1+1, 1 );
    draw_region_zoomed_at( x1, y1 );
    draw_region_zoomed_at( x1, y2 );
    
    // draw vertical lines
    set_drawing_scale( 1, y2-y1+1 );
    draw_region_zoomed_at( x1, y1 );
    draw_region_zoomed_at( x2, y1 );
}

// ---------------------------------------------------------

void draw_filled_rectangle( int x1, int y1, int x2, int y2 )
{
    // select the bios's white pixel region
    asm{ "out GPU_SelectedTexture, -1"
         "out GPU_SelectedRegion, 256" }
    
    // draw the filled rectangle
    // by stretching the pixel
    set_drawing_scale( x2-x1+1, y2-y1+1 );
    draw_region_zoomed_at( x1, y1 );
}

// ---------------------------------------------------------

void draw_circle( int center_x, int center_y, int radius )
{
    // select the bios's white pixel region
    asm{ "out GPU_SelectedTexture, -1"
         "out GPU_SelectedRegion, 256" }
    
    // determine number of segments
    // (simple experimental approximation)
    int segments = 4 * sqrt( radius );
    segments = max( 2, segments );
    
    // use the same line length in all segments
    float segment_angle = (2*pi) / segments;
    float segment_length = radius * segment_angle;
    set_drawing_scale( ceil(segment_length), 1 );
    
    // draw circle as a regular polygon
    int x1 = center_x + radius;
    int y1 = center_y;
    
    for( int i = 1; i <= segments; ++i )
    {
        float angle = segment_angle * i;
        int x2 = round( center_x + radius * cos( angle ) );
        int y2 = round( center_y + radius * sin( angle ) );
        
        // special case: draw segment as a single pixel
        if( x1 == x2 && y1 == y2 )
          draw_region_at( x1, y1 );
        
        else
        {
            // determine segment angle
            int delta_x = x2 - x1;
            int delta_y = y2 - y1;
            float line_angle = atan2( delta_y, delta_x );
            
            // draw the segment as a region
            set_drawing_angle( line_angle );
            draw_region_rotozoomed_at( x1, y1 );
        }
        
        // iterate
        x1 = x2;
        y1 = y2;
    }
}

// ---------------------------------------------------------

void draw_filled_circle( int center_x, int center_y, int radius )
{
    // select the bios's white pixel region
    asm{ "out GPU_SelectedTexture, -1"
         "out GPU_SelectedRegion, 256" }
    
    int min_y = center_y - radius;
    int max_y = center_y + radius;
    
    // is it out of the screen?
    if( max_y < 0 || min_y >= screen_height )
      return;
    
    // clipping
    min_y = max( min_y, 0 );
    max_y = min( max_y, screen_height - 1 );
    
    // expand radius slightly to avoid bad looking
    // circle ends at left, right, top and bottom
    float expanded_radius_2 = (radius + 0.3) * (radius + 0.3);
    
    // draw it using horizontal scanlines
    for( int y = min_y; y <= max_y; y++ )
    {
        int delta_y = y - center_y;
        int width = round( sqrt( expanded_radius_2 - delta_y*delta_y ) );
        draw_horizontal_line( center_x - width, y, center_x + width );
    }
}

// ---------------------------------------------------------

void draw_arc( int center_x, int center_y, int radius, float angle1, float angle2 )
{
    // select the bios's white pixel region
    asm{ "out GPU_SelectedTexture, -1"
         "out GPU_SelectedRegion, 256" }
    
    // determine number of segments
    // (simple experimental approximation)
    int segments = 4 * sqrt( radius );
    segments *= (angle2 - angle1) / (2 * pi);
    segments = max( 2, segments );
    
    // use the same line length in all segments
    float segment_angle = (angle2 - angle1) / segments;
    float segment_length = radius * segment_angle;
    set_drawing_scale( ceil(segment_length), 1 );
    
    // draw circle as a regular polygon
    int x1 = round( center_x + radius * cos( angle1 ) );
    int y1 = round( center_y + radius * sin( angle1 ) );
    
    for( int i = 1; i <= segments; ++i )
    {
        float angle = angle1 + segment_angle * i;
        int x2 = round( center_x + radius * cos( angle ) );
        int y2 = round( center_y + radius * sin( angle ) );
        
        // special case: draw segment as a single pixel
        if( x1 == x2 && y1 == y2 )
          draw_region_at( x1, y1 );
        
        else
        {
            // determine segment angle
            int delta_x = x2 - x1;
            int delta_y = y2 - y1;
            float line_angle = atan2( delta_y, delta_x );
            
            // draw the segment as a region
            set_drawing_angle( line_angle );
            draw_region_rotozoomed_at( x1, y1 );
        }
        
        // iterate
        x1 = x2;
        y1 = y2;
    }
}


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
