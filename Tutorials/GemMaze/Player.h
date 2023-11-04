// ---------------------------------------------------------
//   STRUCTURE WITH OUR PLAYER STATE
// ---------------------------------------------------------


struct Player
{
    box Shape;
    vector2d StartPosition;
    float DirectionAngle;
};

// our single player object
Player Player1;


// ---------------------------------------------------------
//   FUNCTIONS TO HANDLE A PLAYER
// ---------------------------------------------------------


void Player_Create( Player* P, int TileX, int TileY )
{
    P->Shape.width  = 32;
    P->Shape.height = 32;
    P->Shape.hotspot.x = 32/2;
    P->Shape.hotspot.y = 32/2;
    P->StartPosition.x = TilesMaze.width  * (TileX + 0.5);
    P->StartPosition.y = TilesMaze.height * (TileY + 0.5);
}

// ---------------------------------------------------------

void Player_Reset( Player* P )
{
    box_full_stop( &P->Shape );
    P->Shape.position = P->StartPosition;
    P->DirectionAngle = 0;
}

// ---------------------------------------------------------

void Player_Update( Player* P )
{
    // determine movement from first gamepad
    float DeltaX, DeltaY;
    select_gamepad( 0 );
    gamepad_direction_normalized( &DeltaX, &DeltaY );
    
    // if moving, determine the angle for drawing
    if( DeltaX || DeltaY )
      P->DirectionAngle = atan2( DeltaY, DeltaX );
    
    // apply that movement to player
    P->Shape.velocity.x = DeltaX * 100;
    P->Shape.velocity.y = DeltaY * 100;
    
    // simulation step 1: unrestricted motion
    box_begin_frame( &P->Shape );
    box_simulate( &P->Shape );
    
    // simulation step 2: restrict motion with player-map collisions
    collide_box_with_tilemap( &P->Shape, &MapMaze );
    collide_box_with_tilemap_limits( &P->Shape, &MapMaze );
    
    // simulation step 3: apply restricted motion
    // (now there should be no object penetration)
    box_move( &P->Shape );
    
    // simulation step 4: adapt player speed if collisions happened
    // (bounce coeff. = 0 means no bouncing, just stop)
    box_apply_bounce( &P->Shape, 0 );
}

// ---------------------------------------------------------

void Player_Draw( Player* P )
{
    // calculate position on screen
    int PlayerX = P->Shape.position.x;
    int PlayerY = P->Shape.position.y;
    tilemap_convert_position_to_screen( &MapMaze, &PlayerX, &PlayerY );
    
    // draw player in its current direction
    select_region( RegionPlayer );
    set_drawing_angle( P->DirectionAngle );
    draw_region_rotated_at( PlayerX, PlayerY );
}
