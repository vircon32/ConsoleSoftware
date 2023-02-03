// *****************************************************************************
    // include standard Vircon headers
    #include "math.h"
    #include "time.h"
    #include "input.h"
    #include "audio.h"
    #include "video.h"
    #include "misc.h"
    
    // include additional libraries
    #include "print_numbers.h"
    
    // include our project headers
    #include "Globals.c"
    #include "TextureRoad.h"
    #include "RoadObjects.c"
    #include "Mountains.c"
// *****************************************************************************


// ---------------------------------------------------------
//   MAIN FUNCTION
// ---------------------------------------------------------


void main( void )
{
    // ------------------------------------
    // PART 1: DEFINE ALL TEXTURE REGIONS
    // ------------------------------------
    
    // first create the texture regions defined in the region editor
    select_texture( TextureRoad );
    DefineRegions_TextureRoad();
    
    // now create regions that split the road in horizontal lines
    for( int i = 0; i < 512; i++ )
    {
        int TextureY = 578 - i;
        select_region( FirstRegionRoadLines + i );
        define_region( 1,TextureY,  RoadWidth,TextureY,  RoadWidth/2,TextureY );
    }
    
    // ------------------------------------
    // PART 2: CONFIGURE SOUNDS
    // ------------------------------------
    
    // enable loop for our 2 sounds
    select_sound( SoundRoad );
    set_sound_loop( true );
    
    select_sound( SoundEngine );
    set_sound_loop( true );
    
    // -----------------------------------------------
    // PART 3: INITIALIZATIONS
    // -----------------------------------------------
    
    // we will control the car with gamepad 1
    select_gamepad( 0 );
    
    // since our road perspective does not change vertically we will
    // precalculate, for each drawn road scanline in Y, their scaling
    // factor and distance to the camera in the 3D world
    for( int ScreenY = DrawnHorizonY + 1; ScreenY < screen_height; ScreenY++ )
    {
        // scale factor will just reduce linearly until 0 at true horizon Y
        LineDrawingScale[ ScreenY ] = ((ScreenY - TrueHorizonY) / 256.0) * (600.0 / 512.0);
        
        // from that scale, we can calculate the distance
        LineDistanceZ[ ScreenY ] = 300.0 / LineDrawingScale[ ScreenY ] - 256.0;
        
        // also, for very far distances, we will also apply a fade out
        // effect by drawing each line progressively more transparent
        LineMultiplyColor[ ScreenY ] = color_white;
        
        if( (ScreenY-DrawnHorizonY) < 30 )
        {
            int Opacity = 255.0 - 6 * ((DrawnHorizonY+30) - ScreenY);
            LineMultiplyColor[ ScreenY ] = make_color_rgba( 255, 255, 255, Opacity );
        }
    }
    
    // initialize road curve level to none
    CurveXFactor = 0;
    CloudsX = 0;
    
    // initialize car to not moving
    TraveledDistance = 0;
    CarSpeed = 0;
    
    // now create our 2 sets of road objects
    Objects_Initialize();
    Objects_Populate();
    
    Mountains_Initialize();
    Mountains_Populate();
    
    // begin playing road sound, but
    // make the volume 0 until car moves
    play_sound_in_channel( SoundRoad, ChannelRoad );
    set_channel_volume( 0 );
    
    // begin playing car engine sound
    // and regulate its pitch for speed 0
    play_sound_in_channel( SoundEngine, ChannelEngine );
    set_channel_volume( 0.65 );
    set_channel_speed( 0.5 );
    
    // ------------------------------------
    // PART 4: MAIN LOOP
    // ------------------------------------
    
    while( true )
    {
        // apply inputs for acceleration
        // and cap speed to +/- 2000
        if( gamepad_up() > 0 )
        {
            if( CarSpeed < 2000 )
              CarSpeed += 500 / 60.0;
        }
            
        else if( gamepad_down() > 0 )
        {
            if( CarSpeed > -2000 )
              CarSpeed -= 500 / 60.0;
        }
        
        else
        {
            CarSpeed *= 0.985;
            
            if( fabs( CarSpeed ) < 10.0 )
              CarSpeed = 0;
        }
        
        // simulate car movement
        TraveledDistance += CarSpeed / 60.0;
        
        // make car position wrap in a loop
        if( TraveledDistance >= 512000 )
          TraveledDistance -= 512000;
        
        if( TraveledDistance < 0 )
          TraveledDistance += 512000;
        
        // apply inputs for road curvature
        // and cap it to +/- 25
        if( gamepad_right() > 0 )
        {
            if( CurveXFactor < 25 )
              CurveXFactor += 15 / 60.0;
        }
        
        if( gamepad_left() > 0 )
        {
            if( CurveXFactor > -25 )
              CurveXFactor -= 15 / 60.0;
        }
        
        // simulate movement of road objects
        Objects_Update();
        Mountains_Update();
        
        // draw background
        set_drawing_scale( screen_width, 1 );
        select_region( RegionBackground );
        draw_region_zoomed_at( 0, 0 );
        
        // clouds will move when the road is curved to convey distance
        CloudsX -= 0.10 * CurveXFactor * (CarSpeed / 2000.0);
        
        // our clouds loop horizontally, so make them wrap around
        if( CloudsX < 0 )             CloudsX += screen_width;
        if( CloudsX >= screen_width ) CloudsX -= screen_width;
        
        // draw clouds over the background
        select_region( RegionClouds );
        draw_region_at( CloudsX, DrawnHorizonY );
        draw_region_at( CloudsX - screen_width, DrawnHorizonY );
        
        // draw road line by line across screen Y
        for( int ScreenY = DrawnHorizonY + 1; ScreenY < screen_height; ScreenY++ )
        {
            // apply the stored parameters for this line
            set_drawing_scale( LineDrawingScale[ ScreenY ], 1 );
            set_multiply_color( LineMultiplyColor[ ScreenY ] );
            
            // we have a stored per-line distance, but to produce movement
            // we also have to account for the car's traveled distance
            int LineDistance = LineDistanceZ[ ScreenY ] + TraveledDistance;
            
            // each line will be displaced from the center by a factor that
            // depends both on 2 factors: current road curvature (CurveXFactor)
            // and a function of the current Y against the horizon (CurveYFactor)
            float CurveYFactor = ((screen_height-1.0) - TrueHorizonY) / (ScreenY - (TrueHorizonY-10));
            float LineX = screen_width/2 + CurveXFactor * CurveYFactor;
            
            // when road is curved, adjust camera view to the opposite
            // side in order to give a better view of the road ahead
            LineX -= 6 * CurveXFactor;
            
            // choose the correct line to draw from our road texture,
            // according to the current distance on that screen Y
            select_region( FirstRegionRoadLines + (LineDistance % 512) );
            draw_region_zoomed_at( LineX, ScreenY );
        }
        
        // we will always draw the car at the center of the road,
        // so determine current road center X at car's position
        set_multiply_color( color_white );
        set_drawing_point( screen_width/2 - 4.75 * CurveXFactor, 340 );
        
        // depending on the current road curve we will draw
        // a slightly different perspective of our car to
        // make it match better with the road
        if( CurveXFactor < -15 )
        {
            select_region( RegionCarSemiside );
            set_drawing_scale( -1, 1 );
            draw_region_zoomed();
        }
        
        else if( CurveXFactor > 15 )
        {
            select_region( RegionCarSemiside );
            draw_region();
        }
        
        else
        {
            select_region( RegionCarBehind );
            draw_region();
        }
        
        // draw all objects on top of the road
        Mountains_Draw();
        Objects_Draw();
        
        // the road will make a louder sound the
        // faster the car is traveling over it
        select_channel( ChannelRoad );
        set_channel_volume( 0.3 * fabs( CarSpeed / 2000.0 ) );
        
        // the engine will make a higher pitch sound
        // the faster the car goes
        select_channel( ChannelEngine );
        set_channel_speed( 0.5 + 1.5 * fabs( CarSpeed / 2000.0 ) );
        
        // print speed on screen
        print_at( 5,5, "Km/h:" );
        print_int_at( 5, 25, fabs( CarSpeed / 10.0 ) );
        
        // wait until next frame
        end_frame();
    }
}
