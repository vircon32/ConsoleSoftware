#include "TextureTitle.h"
#include "TextureStory.h"
#include "TextureGameplay.h"
#include "FontSmall.h"
#include "FontLarge.h"


void DefineTextureRegions()
{
    // define regions for level backgrounds
    for( int Texture = TextureBGLevel1; Texture <= TextureBGLevel3; Texture++ )
    {
        select_texture( Texture );
        select_region( RegionLevelBackground );
        define_region_topleft( 0,0,  639,359 );
    }
    
    // define regions for text font textures
    FontSmall_CreateFont();
    FontLarge_CreateFont();
    
    // now call automated region definitions, created by the editor
    DefineRegions_TextureTitle();
    DefineRegions_TextureStory();
    DefineRegions_TextureGameplay();
    
    // redefine regions for vegetation so that
    // they are no longer a square tile
    select_region( FirstRegionTileset + 65 );
    define_region( 1,244,  120,403,  41,364 );
    select_region( FirstRegionTileset + 66 );
    define_region( 124,244,  243,363,  164,324 );
    select_region( FirstRegionTileset + 67 );
    define_region( 247,244,  326,323,  267,284 );
    select_region( FirstRegionTileset + 68 );
    define_region( 124,367,  243,405,  164,366 );
}
