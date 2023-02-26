// ---------------------------------------------------------
//   DEFINITIONS AND CONSTANTS
// ---------------------------------------------------------


// the 4 possible directions in 2D (no diagonals)
enum Direction
{
    Left,
    Right,
    Up,
    Down
};

// ---------------------------------------------------------

// definition of a generic character within the scene
struct Character
{
    // graphics
    int TextureID;
    int FirstRegionPortrait;
    int FirstRegionWalkRight;
    int FirstRegionWalkUp;
    int FirstRegionWalkDown;
    
    // display name, for dialogs
    int[ 16 ] Name;
    
    // position and shape for collisions
    collisionbox ShapeBox;
    
    // general state
    Direction FaceDirection;
    int ElapsedFrames;
    bool Walking;
};


// ---------------------------------------------------------
//   CHARACTER HANDLING FUNCTIONS
// ---------------------------------------------------------


// if we want the player to control a character we will
// pass this function the current gamepad direction with
// DeltaX and DeltaY; however we can also use this function
// with simulated inputs for automatic walking or cutscenes
void Character_Update( Character* C, int DeltaX, int DeltaY, bool EnableCollisions )
{
    Direction PreviousDirection = C->FaceDirection;
    bool PreviousWalking = C->Walking;
    
    // current directional input determines facing direction and walking state
    C->Walking = true;
    
    if     ( DeltaX < 0 ) C->FaceDirection = Left;
    else if( DeltaX > 0 ) C->FaceDirection = Right;
    else if( DeltaY < 0 ) C->FaceDirection = Up;
    else if( DeltaY > 0 ) C->FaceDirection = Down;
    else C->Walking = false;
    
    // count frames in current state
    if( PreviousDirection == C->FaceDirection
    &&  PreviousWalking == C->Walking )
      C->ElapsedFrames++;
    else
      C->ElapsedFrames = 0;
    
    // convert gamepad input to actual attempted movement
    DeltaX *= CharSpeed;
    DeltaY *= CharSpeed;
    
    // when requested, constrain movement by collisions
    if( EnableCollisions )
    {
        // perform collisions against the scene
        // (this will modulate the deltas)
        for( int i = 0; i < 6; ++i )
          collisionboxes_collide( &BlockedAreas[ i ], &C->ShapeBox, &DeltaX, &DeltaY );
        
        // now prevent character from leaving the map
        collisionbox_collide_left( &C->ShapeBox,  0, &DeltaX );
        collisionbox_collide_top ( &C->ShapeBox, 30, &DeltaY );
        collisionbox_collide_right ( &C->ShapeBox, tilemap_total_width ( &RPGMap ) - 1, &DeltaX );
        collisionbox_collide_bottom( &C->ShapeBox, tilemap_total_height( &RPGMap ) - 1, &DeltaY );
    }
    
    // now move the character as allowed by collisions
    C->ShapeBox.position_x += DeltaX;
    C->ShapeBox.position_y += DeltaY;
}

// ---------------------------------------------------------

void Character_Draw( Character* C, tilemap* M )
{
    // to draw elements in the map, like our character, we first
    // need to convert its map coordinates to screen coordinates
    int CharScreenX = C->ShapeBox.position_x;
    int CharScreenY = C->ShapeBox.position_y;
    tilemap_convert_position_to_screen( M, &CharScreenX, &CharScreenY );
    
    // draw character shadow
    select_texture( C->TextureID );
    select_region( RegionShadowPerson );
    draw_region_at( CharScreenX, CharScreenY );
    
    // select region to draw depending on char state
    bool CharMirrorX = (C->FaceDirection == Left);
    int CharImage = C->FirstRegionWalkRight;
    
    if( C->FaceDirection == Up ) CharImage = C->FirstRegionWalkUp;
    if( C->FaceDirection == Down ) CharImage = C->FirstRegionWalkDown;
    
    // when walking, select the correct animation frame
    int AnimationFrame = 0;
    
    if( C->Walking )
    {
        // adapt our 3 images to a 4-frame walk cycle
        AnimationFrame = (C->ElapsedFrames / 7) % 4;
        if( AnimationFrame == 2 ) AnimationFrame = 0;
        if( AnimationFrame == 3 ) AnimationFrame = 2;
    }
    
    // now draw the character at the converted position
    select_region( CharImage + AnimationFrame );
        
    if( CharMirrorX )
    {
        set_drawing_scale( -1, 1 );
        draw_region_zoomed_at( CharScreenX, CharScreenY );
    }
    
    else
      draw_region_at( CharScreenX, CharScreenY );
}
