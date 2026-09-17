-- Audio routing for V32 Breakout.
-- Channel 0 is reserved for music. sfx.play() without an explicit channel
-- round-robins through channels 1-15, matching the v32lua api_spu demo.

function audio_play_title_music()
    music.stop()
    music.play(MUSIC_TITLE, 0, true)
end

function audio_play_game_music()
    music.stop()
    music.play(MUSIC_GAMEPLAY, 0, true)
end

function audio_play_ending_music()
    music.stop()
    sfx.stop()
    music.play(MUSIC_ENDING, 0, false)
end

function audio_enter_game_over()
    music.stop()
    sfx.stop()
    sfx.play(SFX_GAME_OVER)
end

function audio_enter_title()
    sfx.stop()
    audio_play_title_music()
end

function audio_init()
    ioports.spu.volume = 0.65
    audio_play_title_music()
end

function audio_stop_game_music()
    music.stop()
end

function audio_cheat_enabled()
    -- Give the confirmation jingle a clean audio window before gameplay music
    -- starts. The title theme resumes only if the cartridge later returns here.
    music.stop()
    sfx.play(SFX_CHEAT)
end

function audio_paddle_hit()
    sfx.play(SFX_PADDLE)
end

function audio_wall_hit()
    sfx.play(SFX_WALL)
end

function audio_normal_break()
    sfx.play(SFX_NORMAL)
end

function audio_hard_hit()
    sfx.play(SFX_HARD_HIT)
end

function audio_hard_break()
    sfx.play(SFX_HARD_BREAK)
end

function audio_metal_hit()
    sfx.play(SFX_METAL)
end

function audio_item_pickup()
    sfx.play(SFX_ITEM)
end

function audio_laser_fire()
    sfx.play(SFX_LASER)
end

function audio_ball_lost()
    audio_stop_game_music()
    sfx.play(SFX_BALL_LOST)
end

function audio_level_clear()
    audio_stop_game_music()
    sfx.play(SFX_LEVEL_CLEAR)
end
