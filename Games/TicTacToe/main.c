// include Vircon libraries
#include "time.h"
#include "video.h"
#include "audio.h"
#include "math.h"
#include "input.h"
#include "misc.h"
#include "string.h"
// includes outside SDK
#include "Definitions.h"
#include "TextureRegions.h"
#include "GameLogic.h"
#include "SceneTitle.h"
#include "SceneGame.h"

void main(void)
{
    // Initialize Textures
    DefineTextureRegions();

    // Initialize Logic
    GameState = Title_Initialize;
    
    while(1) {
        if (GameState == Title_Initialize) Title_RunStateMachine(); else Game_RunStateMachine();
        end_frame();
    }
}