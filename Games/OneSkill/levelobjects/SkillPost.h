// ---------------------------------------------------------
//   SKILL POST
// ---------------------------------------------------------


struct SkillPost
{
    Box EventBox;
    bool ShowingSign;
};

// ---------------------------------------------------------

void SkillPost_Create( SkillPost* SP, int TileX, int TileY )
{
    SP->EventBox.Position.x = TileWidth  * TileX;
    SP->EventBox.Position.y = TileHeight * TileY;
    SP->EventBox.Hotspot.x = -10;
    SP->EventBox.Hotspot.y = 0;
    SP->EventBox.Width  = 20;
    SP->EventBox.Height = TileHeight;
}

// ---------------------------------------------------------

void SkillPost_Reset( SkillPost* SP )
{
    SP->ShowingSign = false;
}

// ---------------------------------------------------------

void SkillPost_Update( SkillPost* SP, Player* P )
{
    SP->ShowingSign = false;
    
    // check overlap with player
    if( Box_OverlapWith( &P->ShapeBox, &SP->EventBox ) )
    {
        // skill change only allowed on ground
        if( !P->ShapeBox.Contacts.Bottom ) return;
        SP->ShowingSign = true;
        
        // if player pressed up, go to skill change scene
        if( gamepad_up() == 1 )
        {
            SkillWheelCenterX = SP->EventBox.Position.x + 20;
            SkillWheelCenterY = SP->EventBox.Position.y - 20 + 1;
            
            GameScene = Scene_ChangeSkill;
            GameState = ChangeSkill_Initialize;
        }
    }
}

// ---------------------------------------------------------

void SkillPost_Draw( SkillPost* SP, Vector2D* LevelTopLeftOnScreen )
{
    // draw the post itself
    float PostX = SP->EventBox.Position.x + LevelTopLeftOnScreen->x;
    float PostY = SP->EventBox.Position.y + 40 + LevelTopLeftOnScreen->y;
    select_region( RegionSkillPost );
    draw_region_at( round(PostX+0.4), round(PostY+0.4) );
    
    // draw button sign when skill change is accessible
    if( SP->ShowingSign )
    {
        // make the sign blink
        if( GameScene != Scene_Gameplay )
          return;
        
        if( (get_frame_counter() / 20) % 2 )
          return;
        
        float SignX = PostX + 20;
        float SignY = PostY - 90;
        select_region( RegionSignUp );
        draw_region_at( round(SignX+0.4), round(SignY+0.4) );
    }
}
