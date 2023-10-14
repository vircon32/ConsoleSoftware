// ---------------------------------------------------------
//   GOLEM ENEMY
// ---------------------------------------------------------


enum GolemStates
{
    Golem_Inactive,
    Golem_MovingHands,
    Golem_MovingBody,
    Golem_Dying,
    Golem_Dead
};

// ---------------------------------------------------------

struct Golem
{
    // physics
    Box ShapeBox;
    
    // initial state
    Vector2D StartPosition;
    int InitialHealth;
    
    // state variables
    GolemStates State;
    int FacingDirectionX;
    int HandsOffsetX;
    int FramesSinceLastHit;
    int Health;
};

// ---------------------------------------------------------

void Golem_Create( Golem* G, int TileX, int TileY )
{
    G->ShapeBox.Width = 67;
    G->ShapeBox.Height = 65;
    G->ShapeBox.Hotspot.x = 33;
    G->ShapeBox.Hotspot.y = 65;
    G->StartPosition.x = TileWidth * TileX + TileWidth / 2;
    G->StartPosition.y = TileHeight * (TileY + 1);
    G->InitialHealth = 8;
}

// ---------------------------------------------------------

void Golem_Reset( Golem* G )
{
    G->State = Golem_Inactive;
    Box_FullStop( &G->ShapeBox );
    G->ShapeBox.Position = G->StartPosition;
    G->HandsOffsetX = -35;
    G->FacingDirectionX = -1;
    
    G->Health = G->InitialHealth;
    G->FramesSinceLastHit = 60;
}

// ---------------------------------------------------------

void Golem_DetectImpacts( Golem* G, Player* P )
{
    G->FramesSinceLastHit++;
    
    // detect impacts from player bullets
    for( int i = 0; i < 10; i++ )
    {
        if( !PistolShots[ i ].Active || PistolShots[ i ].IsBreaking )
          continue;
        
        if( Box_OverlapWith( &PistolShots[ i ].ShapeBox, &G->ShapeBox ) )
        {
            // destroy player shot
            PistolShots[ i ].IsBreaking = true;
            PistolShots[ i ].ElapsedFrames = 0;
            
            // reduce golem health
            G->Health--;
            
            // kill golem when no health remains
            if( G->Health <= 0 )
            {
                G->State = Golem_Dying;
                
                Vector2D HighlightPosition = G->ShapeBox.Position;
                HighlightPosition.y -= 32;
                CreateHighlight( &HighlightPosition );
                play_sound( SoundGolemDeath );
            }
            
            play_sound( SoundEnemyHit );
            G->FramesSinceLastHit = 0;
            break;
        }
    }
    
    
    // kill the player on impact
    if( Box_OverlapWith( &P->ShapeBox, &G->ShapeBox ) )
      P->IsDead = true;
}

// ---------------------------------------------------------

void Golem_CollideWithPlayer( Golem* G, Player* P )
{
    if( G->State == Golem_Dead || G->State == Golem_Inactive )
      return;
    
    // check that both boxes overlap along the X axis
    if( Box_Left( &P->ShapeBox ) < Box_Right( &G->ShapeBox ) )
    if( Box_Left( &G->ShapeBox ) < Box_Right( &P->ShapeBox ) )
    {
        // check if player will go below golem's top this frame
        if( Box_Bottom( &P->ShapeBox ) <= Box_Top( &G->ShapeBox ) )
        if( (Box_Bottom( &P->ShapeBox ) + P->ShapeBox.DeltaPosition.y) >= Box_Top( &G->ShapeBox ) )
        {
            P->ShapeBox.Contacts.Bottom = true;
            P->ShapeBox.DeltaPosition.y = 0;
            P->ShapeBox.Velocity.y = 0;
            P->ShapeBox.Position.y = Box_Top( &G->ShapeBox );
            
            // adjust player X speed
            if( G->State == Golem_MovingBody )
              P->ShapeBox.DeltaPosition.x += 1 * G->FacingDirectionX;
        }
    }
}

// ---------------------------------------------------------

bool Golem_DetectFrontWall( Golem* G )
{
    // the golem is tall, so we need to check
    // for 2 heights of tiles in front of it
    Vector2D LowFrontPoint = G->ShapeBox.Position;
    LowFrontPoint.x += 38 * G->FacingDirectionX;
    LowFrontPoint.y -= 20;
    
    if( point_overlaps_tilemap( &LowFrontPoint, &MapLevel ) )
      return true;
    
    Vector2D HighFrontPoint = LowFrontPoint;
    HighFrontPoint.y -= 40;
    return point_overlaps_tilemap( &HighFrontPoint, &MapLevel );
}

// ---------------------------------------------------------

bool Golem_DetectFrontFall( Golem* G )
{
    Vector2D FloorPoint = G->ShapeBox.Position;
    FloorPoint.x += 38 * G->FacingDirectionX;
    FloorPoint.y += 20;
    return !point_overlaps_tilemap( &FloorPoint, &MapLevel );
}

// ---------------------------------------------------------

bool Golem_IsWithinActiveDistance( Golem* G, Player* P )
{
    float DistanceX = fabs( P->ShapeBox.Position.x - G->ShapeBox.Position.x );
    float DistanceY = fabs( P->ShapeBox.Position.y - G->ShapeBox.Position.y );
    
    return (DistanceX < (screen_width+70) && DistanceY < (screen_height+70));
}

// ---------------------------------------------------------

void Golem_Update( Golem* G, Player* P )
{
    switch( G->State )
    {
        case Golem_Dead:
          return;
        
        case Golem_Dying:
        {
            G->ShapeBox.Velocity.y += Gravity * frame_time;
            G->ShapeBox.Position.y += G->ShapeBox.Velocity.y * frame_time;
            return;
        }
        
        case Golem_Inactive:
        {
            // activate in initial state when player gets close enough
            if( Golem_IsWithinActiveDistance( G, P ) )
            {
                Golem_Reset( G );
                G->State = Golem_MovingHands;
            }
            
            break;
        }
        
        case Golem_MovingHands:
        {
            // continue moving the hands
            G->HandsOffsetX += 2;
            
            // check if it is turn for the body
            if( G->HandsOffsetX >= -10 )
            {
                G->State = Golem_MovingBody;
                
                // at each hand impulse play the walk sound
                play_sound( SoundGolemWalk );
            }
            
            break;
        }
        
        case Golem_MovingBody:
        {
            // check if the golem needs to turn around
            if( Golem_DetectFrontFall( G ) || Golem_DetectFrontWall( G ) )
            {
                G->FacingDirectionX *= -1;
                G->State = Golem_MovingHands;
                G->HandsOffsetX <= -35;
                break;
            }
            
            // continue moving the body
            G->ShapeBox.Position.x += 1 * G->FacingDirectionX;
            G->HandsOffsetX -= 1;
            
            // check if it is turn for the hands
            if( G->HandsOffsetX <= -35 )
              G->State = Golem_MovingHands;
            
            break;
        }
        
        default: break;
    }
    
    // now, for active states we also check if
    // the golem is killed by a player shot,
    // or if it kills the player by contact
    if( G->State != Golem_Inactive )
      Golem_DetectImpacts( G, P );
    
    // deactivate the golem if it remains alive
    // but too far from the player to be seen
    if( G->State != Golem_Inactive )
      if( !Golem_IsWithinActiveDistance( G, P ) )
        G->State = Golem_Inactive;
}

// ---------------------------------------------------------

void Golem_Draw( Golem* G, Vector2D* LevelTopLeftOnScreen )
{
    // don't draw inactive or dead golems
    if( G->State == Golem_Inactive || G->State == Golem_Dead )
      return;
    
    int GolemX = LevelTopLeftOnScreen->x + G->ShapeBox.Position.x;
    int GolemY = LevelTopLeftOnScreen->y + G->ShapeBox.Position.y;
    set_drawing_scale( G->FacingDirectionX, 1 );
    
    // draw the golem dying
    if( G->State == Golem_Dying )
    {
        // draw dying golem
        select_region( RegionGolemDeath );
        draw_region_zoomed_at( GolemX, GolemY );
        
        // if it exits the screen, it finishes dying
        if( GolemY >= (screen_width + 70) )
          G->State = Golem_Dead;
        
        return;
    }
    
    // draw back hand
    select_region( RegionGolemHand );
    draw_region_zoomed_at( GolemX + G->FacingDirectionX * (G->HandsOffsetX + 25), GolemY );
    
    // draw body
    select_region( RegionGolem );
    draw_region_zoomed_at( GolemX, GolemY );
    
    // when golem is being hit, highlight its body
    if( G->FramesSinceLastHit < 4 )
    {
        set_blending_mode( blending_add );
        draw_region_zoomed();
        set_blending_mode( blending_alpha );
    }
    
    select_region( RegionGolemHand );
    draw_region_zoomed_at( GolemX + G->FacingDirectionX * G->HandsOffsetX, GolemY );
}
