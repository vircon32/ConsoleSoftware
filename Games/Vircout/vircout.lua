--#title "Vircout"
--#version "1.0.0"
--#texture sprites "assets/sprites.png"
--#texture title_texture "assets/title.png"
--#texture gameplay_texture "assets/gameplay.png"
--#texture ending_texture "assets/ending.png"
--#sound MUSIC_TITLE      "audio/music_title.vsnd"
--#sound MUSIC_GAMEPLAY   "audio/music_gameplay.vsnd"
--#sound MUSIC_ENDING     "audio/music_ending.vsnd"
--#sound SFX_PADDLE       "audio/sfx_paddle.vsnd"
--#sound SFX_WALL         "audio/sfx_wall.vsnd"
--#sound SFX_NORMAL       "audio/sfx_normal.vsnd"
--#sound SFX_HARD_HIT     "audio/sfx_hard_hit.vsnd"
--#sound SFX_HARD_BREAK   "audio/sfx_hard_break.vsnd"
--#sound SFX_METAL        "audio/sfx_metal.vsnd"
--#sound SFX_ITEM         "audio/sfx_item.vsnd"
--#sound SFX_LASER        "audio/sfx_laser.vsnd"
--#sound SFX_BALL_LOST    "audio/sfx_ball_lost.vsnd"
--#sound SFX_LEVEL_CLEAR  "audio/sfx_level_clear.vsnd"
--#sound SFX_GAME_OVER    "audio/sfx_game_over.vsnd"
--#sound SFX_CHEAT        "audio/sfx_cheat.vsnd"
--#tilemap LEVEL1 "levels/level1.csv"
--#tilemap LEVEL2 "levels/level2.csv"
--#tilemap LEVEL3 "levels/level3.csv"
--#tilemap LEVEL4 "levels/level4.csv"
--#tilemap LEVEL5 "levels/level5.csv"
--#tilemap LEVEL6 "levels/level6.csv"
--#tilemap LEVEL7 "levels/level7.csv"
--#tilemap LEVEL8 "levels/level8.csv"

-- VIRCOUT 1.0

--#include "src/constants.lua"
--#include "src/state.lua"
--#include "src/graphics.lua"
--#include "src/audio.lua"
--#include "src/powerups.lua"
--#include "src/effects.lua"
--#include "src/blocks.lua"
--#include "src/gameplay.lua"
--#include "src/screens.lua"

function init()
    graphics_init()
    audio_init()
    game_state = STATE_TITLE
    state_frame = 0
    lives = STARTING_LIVES
    score = 0
    current_level = 1
    clear_blocks()
    reset_playfield()
end

function game_loop()
    state_frame = state_frame + 1

    -- Pause freezes cosmetic effect ages too, so the entire gameplay image is
    -- preserved exactly until START is pressed again.
    if game_state ~= STATE_PAUSE then
        update_effects()
    end

    if game_state == STATE_TITLE then
        update_title()
        draw_title()
    elseif game_state == STATE_READY then
        update_ready()
        draw_ready()
    elseif game_state == STATE_PLAYING then
        update_playing()
        draw_playing()
    elseif game_state == STATE_DYING then
        update_dying()
        draw_dying()
    elseif game_state == STATE_GAME_OVER then
        update_game_over()
        draw_game_over()
    elseif game_state == STATE_LEVEL_CLEAR then
        update_level_clear()
        draw_level_clear()
    elseif game_state == STATE_FADE_OUT then
        update_fade_out()
        draw_fade_out()
    elseif game_state == STATE_ENDING then
        update_ending()
        draw_ending()
    elseif game_state == STATE_DYING_FADE then
        update_dying_fade()
        draw_dying_fade()
    elseif game_state == STATE_GAME_OVER_FADE then
        update_game_over_fade()
        draw_game_over_fade()
    elseif game_state == STATE_ENDING_FADE then
        update_ending_fade()
        draw_ending_fade()
    elseif game_state == STATE_PAUSE then
        update_pause()
        draw_pause()
    end
end
