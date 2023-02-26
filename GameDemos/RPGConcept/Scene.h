// ---------------------------------------------------------
//   SCENE DEFINITIONS
// ---------------------------------------------------------


enum SceneStates
{
    Scene_MovePlayer,     // first, you can freely move
    Scene_MovePeasant,    // when you approach, the peasant moves
    Scene_DialogPeasant,  // peasant's dialog
    Scene_DialogPlayer    // player's dialog
};

// ---------------------------------------------------------

// state of our scene
SceneStates SceneState;

// the objects in our scene
Character Player;
Character Peasant;
Dialog DialogWindow;

// our only scene trigger
bool PeasantHasSpoken;


// ---------------------------------------------------------
//   AUXILIARY FUNCTIONS
// ---------------------------------------------------------


// this is a specialized function to read gamepad direction; it
// works the same way as the standard gamepad_get_direction(),
// but will only allows us to move in 4 directions (not allowing
// diagonals); if both axis are pressed, it will prioritize the
// most recently pressed direction
void GamepadDirectionNoDiagonals( int* DeltaX, int* DeltaY )
{
    // read movement inputs
    int Left  = gamepad_left();
    int Right = gamepad_right();
    int Up    = gamepad_up();
    int Down  = gamepad_down();
    
    // our character can only move in 4 directions (no diagonals)
    // so we will prioritize the most recently pressed direction
    int Minimum = 10000;
    int* LastDirection = NULL;
    
    if( Left > 0 && Left < Minimum )
    {
        Minimum = Left;
        LastDirection = &Left;
    }
    
    if( Right > 0 && Right < Minimum )
    {
        Minimum = Right;
        LastDirection = &Right;
    }
    
    if( Up > 0 && Up < Minimum )
    {
        Minimum = Up;
        LastDirection = &Up;
    }
    
    if( Down > 0 && Down < Minimum )
    {
        Minimum = Down;
        LastDirection = &Down;
    }
    
    // now, depending on the direction, determine movement
    if( LastDirection == &Left )
    {
        *DeltaX = -1;
        *DeltaY = 0;
        return;
    }
    
    if( LastDirection == &Right )
    {
        *DeltaX = 1;
        *DeltaY = 0;
        return;
    }
    
    if( LastDirection == &Up )
    {
        *DeltaX = 0;
        *DeltaY = -1;
        return;
    }
    
    if( LastDirection == &Down )
    {
        *DeltaX = 0;
        *DeltaY = 1;
        return;
    }
    
    // no direction is pressed
    *DeltaX = 0;
    *DeltaY = 0;
}


// ---------------------------------------------------------
//   SCENE MAIN FUNCTIONS
// ---------------------------------------------------------


// called every frame to update our game scene
void Scene_Update()
{
    // process the current state of our scene
    switch( SceneState )
    {
        case Scene_MovePlayer:
        {
            // read inputs
            int DeltaX, DeltaY;
            GamepadDirectionNoDiagonals( &DeltaX, &DeltaY );
            
            // update player with that input
            Character_Update( &Player, DeltaX, DeltaY, true );
            
            // check if peasant has spoken
            if( !PeasantHasSpoken )
            {
                // trigger cutscene when player first
                // arrives at the event trigger area
                if( collisionboxes_overlap( &Player.ShapeBox, &EventAreas[ 0 ] ) )
                //if( Player.ShapeBox.PositionX >= 635 && Player.ShapeBox.PositionY < 400 )
                {
                    PeasantHasSpoken = true;
                    Character_Update( &Player, 0, 0, false );
                    SceneState = Scene_MovePeasant;
                }
            }
            
            break;
        }
        
        case Scene_MovePeasant:
        {
            // make peasant walk left
            Character_Update( &Peasant, -1, 0, false );
            
            // stop walking at the cutscene trigger area
            if( collisionboxes_overlap( &Peasant.ShapeBox, &EventAreas[ 0 ] ) )
            {
                Dialog_Initialize( &DialogWindow, &Peasant, "Sir! I'm sorry but there has been trouble.\n" "I can't allow you in." );
                Character_Update( &Peasant, 0, 0, false );
                SceneState = Scene_DialogPeasant;
            }
            
            break;
        }
        
        case Scene_DialogPeasant:
        {
            Dialog_Update( &DialogWindow );
            
            // change state after dialog ends
            if( DialogWindow.State == Dialog_Finished )
            {
                Dialog_Initialize( &DialogWindow, &Player, "What? Damn it, this is bad luck..." );
                SceneState = Scene_DialogPlayer;
            }
            
            break;
        }
        
        case Scene_DialogPlayer:
        {
            Dialog_Update( &DialogWindow );
            
            // change state after dialog ends
            if( DialogWindow.State == Dialog_Finished )
              SceneState = Scene_MovePlayer;
            
            break;
        }
        
        default: break;
    }
    
    // in any state, keep camera on player
    RPGMap.camera_x = Player.ShapeBox.position_x;
    RPGMap.camera_y = Player.ShapeBox.position_y;
    
    // restrict camera placement so that, even when the player is
    // near map bounds, screen view will never reach out of the map
    tilemap_clip_camera_position( &RPGMap );
}

// ---------------------------------------------------------

// draws our scene based on its current state
void Scene_Draw()
{
    // draw the map itself
    tilemap_draw( &RPGMap );
    
    // draw any fixed shadows before other objects
    select_texture( TextureRPG );
    int TreePositionX = TreeX;
    int TreePositionY = TreeY;
    tilemap_convert_position_to_screen( &RPGMap, &TreePositionX, &TreePositionY );
    select_region( RegionShadowTree );
    draw_region_at( TreePositionX, TreePositionY );
    
    // draw both characters
    Character_Draw( &Peasant, &RPGMap );
    Character_Draw( &Player, &RPGMap );
    
    // draw the tree (no depth control for now!!)
    select_region( RegionTree );
    draw_region_at( TreePositionX, TreePositionY );
    
    // draw the dialog only when active
    if( DialogWindow.State != Dialog_Finished )
      Dialog_Draw( &DialogWindow );
}
