// ---------------------------------------------------------
//   VISUAL HIGHLIGHT FOR GAME EVENTS
// ---------------------------------------------------------


struct Highlight
{
    Vector2D Position;
    float ScaleFactor;
    int ElapsedFrames;
    bool Active;
};

// ---------------------------------------------------------

void Highlight_Create( Highlight* H, Vector2D* Position, float ScaleFactor )
{
    H->Active = true;
    H->ElapsedFrames = 0;
    H->Position = *Position;
    H->ScaleFactor = ScaleFactor;
}

// ---------------------------------------------------------

void Highlight_Reset( Highlight* H )
{
    H->Active = false;
    H->ElapsedFrames = 0;
}

// ---------------------------------------------------------

void Highlight_Update( Highlight* H )
{
    // nothing to do when inactive
    if( !H->Active ) return;
    
    // advance animation
    H->ElapsedFrames ++;
    
    // deactivate when done
    if( H->ElapsedFrames >= 24 )
    {
        H->ElapsedFrames = 0;
        H->Active = false;
    }
}

// ---------------------------------------------------------

void Highlight_Draw( Highlight* H, Vector2D* LevelTopLeftOnScreen )
{
    // nothing to do when inactive
    if( !H->Active ) return;
    
    // calculate position on screen
    int HighlightX = LevelTopLeftOnScreen->x + H->Position.x;
    int HighlightY = LevelTopLeftOnScreen->y + H->Position.y;
    
    // draw current animation frame
    int AnimationFrame = min( 5, H->ElapsedFrames/4 );
    select_region( FirstRegionCircleHighlight + AnimationFrame );
    set_drawing_scale( H->ScaleFactor, H->ScaleFactor );
    draw_region_zoomed_at( HighlightX, HighlightY );
}


// ---------------------------------------------------------
//   POOL OF HIGHLIGHTS TO USE BY OTHER GAME OBJECTS
// ---------------------------------------------------------


#define MAX_HIGHLIGHTS 20
Highlight[ MAX_HIGHLIGHTS ] Highlights;

// ---------------------------------------------------------

void CreateHighlight( Vector2D* Position, float ScaleFactor )
{
    for( int i = 0; i < MAX_HIGHLIGHTS; i++ )
      if( !Highlights[ i ].Active )
        {
            Highlight_Create( &Highlights[ i ], Position, ScaleFactor );
            return;
        }
}
