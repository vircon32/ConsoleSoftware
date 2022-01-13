// ---------------------------------------------------------
//   ROTATING MORNING STARS
// ---------------------------------------------------------


struct MorningStar
{
    Vector2D Center;
    Box EventBox;
    float RotationCycles;
};

// ---------------------------------------------------------

void MorningStar_Create( MorningStar* M, int TileX, int TileY )
{
    M->Center.x = TileWidth  * (TileX + 0.5);
    M->Center.y = TileHeight * (TileY + 0.5);
    M->EventBox.Width  = 28;
    M->EventBox.Height = 28;
    M->EventBox.Hotspot.x = 14;
    M->EventBox.Hotspot.y = 14;
}

// ---------------------------------------------------------

void MorningStar_Reset( MorningStar* M )
{
    M->RotationCycles = 0;
    M->EventBox.Position.x = M->Center.x + 80;
    M->EventBox.Position.y = M->Center.y;
}

// ---------------------------------------------------------

void MorningStar_Update( MorningStar* M, Player* P )
{
    // advance rotation
    M->RotationCycles += (1.0/150);
    
    if( M->RotationCycles > 1 )
      M->RotationCycles -= 1;
      
    // place event box
    float RotationAngle = M->RotationCycles * (2*pi);
    M->EventBox.Position.x = M->Center.x + 80 * cos( RotationAngle );
    M->EventBox.Position.y = M->Center.y + 80 * sin( RotationAngle );
    
    // check damage to the player
    if( Box_OverlapWith( &P->ShapeBox, &M->EventBox ) )
      Gameplay_ChangeState( Gameplay_Death );
}

// ---------------------------------------------------------

void MorningStar_Draw( MorningStar* M, Vector2D* LevelTopLeftOnScreen )
{
    // draw the chain links
    float RotationAngle = M->RotationCycles * (2*pi);
    float CenterX = LevelTopLeftOnScreen->x + M->Center.x;
    float CenterY = LevelTopLeftOnScreen->y + M->Center.y;
    select_region( RegionMorningStarLink );
    
    for( int i = 0; i < 5; i++ )
    {
        float LinkX = CenterX + (13*i + 11) * cos( RotationAngle );
        float LinkY = CenterY + (13*i + 11) * sin( RotationAngle );
        draw_region_at( LinkX, LinkY );
        
    }
    
    // draw the spike ball
    float BallX = LevelTopLeftOnScreen->x + M->EventBox.Position.x;
    float BallY = LevelTopLeftOnScreen->y + M->EventBox.Position.y;
    select_region( RegionMorningStarBall );
    draw_region_at( BallX, BallY );
}
