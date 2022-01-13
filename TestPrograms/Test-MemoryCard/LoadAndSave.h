// ---------------------------------------------------------
//   GRAPHIC INTERFACE DRAWING FUNCTIONS
// ---------------------------------------------------------


void DrawTextLineCentered( int CenterX, int BaselineY, int* Text )
{
    int TextWidth = bios_character_width * strlen( Text );
    print_at( CenterX - TextWidth/2, BaselineY - 16, Text );
}

// ---------------------------------------------------------

void ShowMessage( int* Text )
{
    // draw window
    select_region( RegionWindow );
    draw_region_at( 183, 116 );
    
    select_region( RegionExit );
    draw_region_at( 287, 129 );
    
    // write message
    DrawTextLineCentered( 319, 204, Text );
    
    // wait until button Start is pressed
    while( true )
    {
        if( gamepad_button_start() == 1 )
          break;
        
        end_frame();
    }
}

// ---------------------------------------------------------

void DrawCardContents()
{
    // draw the left panel
    select_region( RegionContentsText );
    draw_region_at( 187, 158 );
    
    // now check the different cases
    // and draw the right panel
    if( card_is_empty() )
      print_at( 362, 178, "EMPTY\nCARD!" );
    
    else if( !card_signature_matches( &GameSignature ) )
      print_at( 352, 178, "ANOTHER\n GAME!" );
    
    else
    {
        select_region( RegionContentsInfo );
        draw_region_at( 321, 158 );
        
        // print the score from the saved game!
        // not the current one
        SceneData SavedGame;
        card_read_data( &SavedGame, sizeof(game_signature), sizeof(SceneData) );
        
        PrintScore( 370, 197, SavedGame.Score );
    }
}


// ---------------------------------------------------------
//   SCENE LOAD/SAVE FUNCTIONS
// ---------------------------------------------------------


void LoadGameScene()
{
    // check that memory card is connected
    if( !card_is_connected() )
    {
        ShowMessage( "NO CARD CONNECTED" );
        return;
    }
    
    // check that the card is empty
    if( card_is_empty() )
    {
        ShowMessage( "CARD IS EMPTY" );
        return;
    }
    
    // check that the signature matches
    if( !card_signature_matches( &GameSignature ) )
    {
        ShowMessage( "CARD CONTENTS INVALID" );
        return;
    }
    
    // draw load window
    select_region( RegionWindow );
    draw_region_at( 183, 116 );
    
    select_region( RegionLoad );
    draw_region_at( 236, 129 );
    
    select_region( RegionCancel );
    draw_region_at( 323, 129 );
    
    select_region( RegionContentsText );
    draw_region_at( 187, 158 );
    
    // show card contents
    DrawCardContents();
    
    // wait 1 frame to force button states to update
    // (otherwise the same press can do both actions)
    end_frame();
    
    // wait until some option is pressed
    while( true )
    {
        if( gamepad_button_start() == 1 )
          return;
        
        if( gamepad_button_a() == 1 )
          break;
        
        end_frame();
    }
    
    // now we can safely load our data
    card_read_data( &GameState, sizeof(game_signature), sizeof(SceneData) );
    
    // show confirmation
    ShowMessage( "GAME LOADED" );
}

// ---------------------------------------------------------

void SaveGameScene()
{
    // check that memory card is connected
    if( !card_is_connected() )
    {
        ShowMessage( "NO CARD CONNECTED" );
        return;
    }
    
    // draw save window
    select_region( RegionWindow );
    draw_region_at( 183, 116 );
    
    select_region( RegionSave );
    draw_region_at( 236, 129 );
    
    select_region( RegionCancel );
    draw_region_at( 323, 129 );
    
    select_region( RegionContentsText );
    draw_region_at( 187, 158 );
    
    // show card contents
    DrawCardContents();
    
    // wait 1 frame to force button states to update
    // (otherwise the same press can do both actions)
    end_frame();
    
    // wait until some option is pressed
    while( true )
    {
        if( gamepad_button_start() == 1 )
          return;
        
        if( gamepad_button_b() == 1 )
          break;
        
        end_frame();
    }
    
    // now we can safely save our data
    card_write_signature( &GameSignature );
    card_write_data( &GameState, sizeof(game_signature), sizeof(SceneData) );
    
    // show confirmation
    ShowMessage( "GAME SAVED" );
}
