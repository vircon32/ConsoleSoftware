// ---------------------------------------------------------
//   AUXILIARY FUNCTIONS TO INITIALIZE DIALOG FONT
// ---------------------------------------------------------


void FontDialog_DefineRegions()
{
    select_texture( TextureDialogFont );
    
    // First we define define 128 consecutive regions (i.e. standard ASCII only)
    // with the same size and hotspot position, as for a constant-width font
    define_region_matrix( 0,  0,0,  11,20,  0,0,  16,8,  0 );
    
    // then we redefine some characters to have larger width
    select_region( 'M' );
    define_region_topleft( 0,0,  13,20 );
    select_region( 'm' );
    define_region_topleft( 24,0,  37,20 );
    select_region( 'W' );
    define_region_topleft( 48,0,  61,20 );
    select_region( 'w' );
    define_region_topleft( 72,0,  85,20 );
}

// ---------------------------------------------------------

// note that the font is still usable without this!
// but it won't look nearly as good
void FontDialog_ConfigureCharacterWidths()
{
    FontDialog.variable_character_widths[ ' ' ] = 9;
    FontDialog.variable_character_widths[ '.' ] = 8;
    FontDialog.variable_character_widths[ ',' ] = 8;
    FontDialog.variable_character_widths[ ':' ] = 8;
    FontDialog.variable_character_widths[ ';' ] = 8;
    FontDialog.variable_character_widths[ '`' ] = 9;
    FontDialog.variable_character_widths[ '{' ] = 9;
    FontDialog.variable_character_widths[ '}' ] = 9;
    FontDialog.variable_character_widths[ '(' ] = 8;
    FontDialog.variable_character_widths[ ')' ] = 8;
    FontDialog.variable_character_widths[ '[' ] = 8;
    FontDialog.variable_character_widths[ ']' ] = 8;
    FontDialog.variable_character_widths[ '|' ] = 6;
    FontDialog.variable_character_widths[ '!' ] = 7;
    FontDialog.variable_character_widths[ '\'' ] = 7;
    FontDialog.variable_character_widths[ '1' ] = 11;
    FontDialog.variable_character_widths[ 'I' ] = 10;
    FontDialog.variable_character_widths[ 'L' ] = 11;
    FontDialog.variable_character_widths[ 'f' ] = 10;
    FontDialog.variable_character_widths[ 'i' ] = 10;
    FontDialog.variable_character_widths[ 'j' ] = 9;
    FontDialog.variable_character_widths[ 'k' ] = 11;
    FontDialog.variable_character_widths[ 'l' ] = 9;
    FontDialog.variable_character_widths[ 't' ] = 10;
}
