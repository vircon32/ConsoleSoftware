// ---------------------------------------------------------
//   BIRD ENEMY
// ---------------------------------------------------------


enum BirdStates
{
    Bird_Inactive,
    Bird_Flying,
    Bird_Dying,
    Bird_Dead
};

// ---------------------------------------------------------

struct Bird
{
    // physics
    Box ShapeBox;
    
    // initial state
    Vector2D StartPosition;
    int InitialHealth;
    
    // state variables
    BirdStates State;
    int FacingDirectionX;
    int FramesSinceLastHit;
    int Health;
};

// ---------------------------------------------------------

void Bird_Create( Bird* B, int TileX, int TileY )
{
    B->ShapeBox.Width = 62;
    B->ShapeBox.Height = 33;
    B->ShapeBox.Hotspot.x = 31;
    B->ShapeBox.Hotspot.y = 33;
    B->StartPosition.x = TileWidth * TileX + TileWidth / 2;
    B->StartPosition.y = TileHeight * (TileY + 1) - 2;
    B->InitialHealth = 4;
}

// ---------------------------------------------------------

void Bird_Reset( Bird* B )
{
    B->State = Bird_Inactive;
    Box_FullStop( &B->ShapeBox );
    B->ShapeBox.Position = B->StartPosition;
    B->FacingDirectionX = -1;
    
    B->Health = B->InitialHealth;
    B->FramesSinceLastHit = 60;
}

// ---------------------------------------------------------

void Bird_DetectImpacts( Bird* B, Player* P )
{
    B->FramesSinceLastHit++;
    
    // detect impacts from player bullets
    for( int i = 0; i < 10; i++ )
    {
        if( !PistolShots[ i ].Active || PistolShots[ i ].IsBreaking )
          continue;
        
        if( Box_OverlapWith( &PistolShots[ i ].ShapeBox, &B->ShapeBox ) )
        {
            // destroy player shot
            PistolShots[ i ].IsBreaking = true;
            PistolShots[ i ].ElapsedFrames = 0;
            
            // reduce bird health
            B->Health--;
            
            // kill bird when no health remains
            if( B->Health <= 0 )
            {
                B->State = Bird_Dying;
                
                Vector2D HighlightPosition = B->ShapeBox.Position;
                HighlightPosition.y -= 16;
                CreateHighlight( &HighlightPosition );
                play_sound( SoundBirdDeath );
            }
            
            play_sound( SoundEnemyHit );
            B->FramesSinceLastHit = 0;
            break;
        }
    }
    
    // kill the player on impact
    if( Box_OverlapWith( &P->ShapeBox, &B->ShapeBox ) )
      P->IsDead = true;
}

// ---------------------------------------------------------

void Bird_CollideWithPlayer( Bird* B, Player* P )
{
    if( B->State == Bird_Dead || B->State == Bird_Inactive )
      return;
    
    // check that both boxes overlap along the X axis
    if( Box_Left( &P->ShapeBox ) < Box_Right( &B->ShapeBox ) )
    if( Box_Left( &B->ShapeBox ) < Box_Right( &P->ShapeBox ) )
    {
        // check if player will go below bird's top this frame
        if( Box_Bottom( &P->ShapeBox ) <= Box_Top( &B->ShapeBox ) )
        if( (Box_Bottom( &P->ShapeBox ) + P->ShapeBox.DeltaPosition.y) >= Box_Top( &B->ShapeBox ) )
        {
            P->ShapeBox.Contacts.Bottom = true;
            P->ShapeBox.DeltaPosition.y = 0;
            P->ShapeBox.Velocity.y = 0;
            P->ShapeBox.Position.y = Box_Top( &B->ShapeBox );
            
            // adjust player X speed
            if( B->State == Bird_Flying )
              P->ShapeBox.DeltaPosition.x += 1.5 * B->FacingDirectionX;
        }
    }
}

// ---------------------------------------------------------

bool Bird_DetectFrontWall( Bird* B )
{
    Vector2D FrontPoint = B->ShapeBox.Position;
    FrontPoint.x += 35 * B->FacingDirectionX;
    FrontPoint.y -= 18;
    
    if( point_overlaps_tilemap( &FrontPoint, &MapLevel ) )
      return true;
}

// ---------------------------------------------------------

bool Bird_IsWithinActiveDistance( Bird* B, Player* P )
{
    float DistanceX = fabs( P->ShapeBox.Position.x - B->ShapeBox.Position.x );
    float DistanceY = fabs( P->ShapeBox.Position.y - B->ShapeBox.Position.y );
    
    return (DistanceX < (screen_width+70) && DistanceY < (screen_height+50));
}

// ---------------------------------------------------------

void Bird_Update( Bird* B, Player* P )
{
    switch( B->State )
    {
        case Bird_Dead:
          return;
        
        case Bird_Dying:
        {
            B->ShapeBox.Velocity.y += Gravity * frame_time;
            B->ShapeBox.Position.y += B->ShapeBox.Velocity.y * frame_time;
            return;
        }
        
        case Bird_Inactive:
        {
            // activate in initial state when player gets close enough
            if( Bird_IsWithinActiveDistance( B, P ) )
            {
                Bird_Reset( B );
                B->State = Bird_Flying;
            }
            
            break;
        }
        
        case Bird_Flying:
        {
            // check if the bird needs to turn around
            if( Bird_DetectFrontWall( B ) )
            {
                B->FacingDirectionX *= -1;
                break;
            }
            
            // check if it flied too far from its starting position
            float FlownDistanceX = fabs( B->ShapeBox.Position.x - B->StartPosition.x );
            
            if( FlownDistanceX >= (4 * TileWidth) )
            {
                B->ShapeBox.Position.x -= 1.5 * B->FacingDirectionX;
                B->FacingDirectionX *= -1;
                break;
            }
            
            // continue flying forward
            B->ShapeBox.Position.x += 1.5 * B->FacingDirectionX;
            break;
        }
        
        default: break;
    }
    
    // now, for active states we also check if
    // the bird is killed by a player shot,
    // or if it kills the player by contact
    if( B->State != Bird_Inactive )
      Bird_DetectImpacts( B, P );
    
    // deactivate the bird if it remains alive
    // but too far from the player to be seen
    if( B->State != Bird_Inactive )
      if( !Bird_IsWithinActiveDistance( B, P ) )
        B->State = Bird_Inactive;
}

// ---------------------------------------------------------

void Bird_Draw( Bird* B, Vector2D* LevelTopLeftOnScreen )
{
    // don't draw inactive or dead birds
    if( B->State == Bird_Inactive || B->State == Bird_Dead )
      return;
    
    int BirdX = LevelTopLeftOnScreen->x + B->ShapeBox.Position.x;
    int BirdY = LevelTopLeftOnScreen->y + B->ShapeBox.Position.y;
    set_drawing_scale( B->FacingDirectionX, 1 );
    
    // draw the bird dying
    if( B->State == Bird_Dying )
    {
        // draw dying bird
        select_region( RegionBirdDeath );
        draw_region_zoomed_at( BirdX, BirdY );
        
        // if it exits the screen, it finishes dying
        if( BirdY >= (screen_width + 45) )
          B->State = Bird_Dead;
        
        return;
    }
    
    // draw flapping animation
    int ReferenceX = fabs( B->ShapeBox.Position.x );
    int AnimationFrame = (ReferenceX / 12) % 2;
    select_region( FirstRegionBirdFlap + AnimationFrame );
    draw_region_zoomed_at( BirdX, BirdY );
    
    // when bird is being hit, highlight its body
    if( B->FramesSinceLastHit < 4 )
    {
        set_blending_mode( blending_add );
        draw_region_zoomed();
        set_blending_mode( blending_alpha );
    }
}
