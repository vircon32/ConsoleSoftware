function title_pressed_button()
    local pressed = -1
    local count = 0

    if btnp(BUTTON_UP) then
        pressed = BUTTON_UP
        count = count + 1
    end
    if btnp(BUTTON_DOWN) then
        pressed = BUTTON_DOWN
        count = count + 1
    end
    if btnp(BUTTON_LEFT) then
        pressed = BUTTON_LEFT
        count = count + 1
    end
    if btnp(BUTTON_RIGHT) then
        pressed = BUTTON_RIGHT
        count = count + 1
    end
    if btnp(BUTTON_B) then
        pressed = BUTTON_B
        count = count + 1
    end
    if btnp(BUTTON_A) then
        pressed = BUTTON_A
        count = count + 1
    end
    if btnp(BUTTON_START) then
        pressed = BUTTON_START
        count = count + 1
    end

    if count == 1 then
        return pressed
    elseif count > 1 then
        return -2
    end

    return -1
end

function update_konami_code(button)
    if button == -2 then
        konami_progress = 0
        return false
    end

    if button < 0 then
        return false
    end

    local expected = KONAMI_CODE[konami_progress + 1]

    if button == expected then
        konami_progress = konami_progress + 1

        if konami_progress >= KONAMI_CODE_LENGTH then
            konami_progress = 0
            cheat_lives_enabled = true
            cheat_start_timer = CHEAT_CONFIRM_FRAMES
            audio_cheat_enabled()
            return true
        end

        return false
    end

    -- If an unexpected UP is pressed, keep it as the beginning of a new
    -- attempt. Any other unexpected input resets the sequence.
    if button == BUTTON_UP then
        konami_progress = 1
    else
        konami_progress = 0
    end

    return false
end

function update_title()
    -- Accumulate player-dependent timing entropy while waiting on the title.
    rng_title_entropy = rng_title_entropy + 1
    if rng_title_entropy >= 8000000 then
        rng_title_entropy = 1
    end

    if state_frame == 1 then
        konami_progress = 0
    end

    -- While the confirmation jingle is playing, keep the title frozen and
    -- start the game automatically once the short delay finishes.
    if cheat_start_timer > 0 then
        cheat_start_timer = cheat_start_timer - 1

        if cheat_start_timer == 0 then
            start_new_game()
        end

        return
    end

    local pressed = title_pressed_button()
    local completed_cheat = update_konami_code(pressed)

    -- START normally begins immediately. The START that completes the Konami
    -- code instead begins the short confirmation delay above.
    if pressed == BUTTON_START and not completed_cheat then
        start_new_game()
    end
end

function draw_title()
    draw_title_background()

    if (state_frame % 60) < 30 then
        print_centered(272, "PRESS START")
    end
end

function update_ready()
    update_paddle()
    attach_ball_to_paddle()

    if state_frame >= READY_TOTAL_FRAMES then
        set_state(STATE_PLAYING)
    end
end

function draw_ready()
    draw_playfield()
    print_centered(214, "LEVEL " .. current_level)

    if (state_frame % (READY_BLINK_FRAMES * 2)) < READY_BLINK_FRAMES then
        print_centered(246, "READY")
    end
end

function update_playing()
    -- START toggles a true gameplay pause. Check it before any gameplay
    -- update so this frame does not advance the paddle, balls, items or shots.
    if btnp(BUTTON_START) then
        set_state(STATE_PAUSE)
        return
    end

    update_paddle()

    if not ball_launched then
        attach_ball_to_paddle()
        if btnp(BUTTON_A) then
            launch_ball()
        end
    else
        -- Once the ball is in play, A becomes the laser-fire button whenever
        -- the laser power-up is active.
        if laser_enabled and btnp(BUTTON_A) then
            fire_lasers()
        end
    end

    update_balls()
    if game_state ~= STATE_PLAYING then
        return
    end

    update_shots()
    if game_state ~= STATE_PLAYING then
        return
    end

    update_items()
end

function draw_playing()
    draw_playfield()

    if not ball_launched then
        if (state_frame % 60) < 40 then
            print_centered(286, "PRESS A TO LAUNCH")
        end
    end
end

function update_pause()
    -- Everything in the playfield stays frozen. START resumes.
    if btnp(BUTTON_START) then
        set_state(STATE_PLAYING)
    end
end

function draw_pause()
    -- If update_pause resumed this frame, avoid drawing one last pause overlay.
    if game_state ~= STATE_PAUSE then
        draw_playing()
        return
    end

    draw_playfield()

    -- 50% black overlay. 0x80000000 represented as signed 32-bit.
    draw_solid_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, -2147483648)
    -- Restore the persistent GPU multiply/blend state before printing text.
    spr(REGION_SOLID, -2, -2)

    print_centered(172, "PAUSE")
end

function update_dying()
    -- Freeze all gameplay, including the paddle, while BALL LOST is shown.
    -- First hold the normal BALL LOST message. The fade is a separate state,
    -- so its alpha always starts at zero and advances one step every frame.
    if state_frame >= DYING_TOTAL_FRAMES then
        set_state(STATE_DYING_FADE)
    end
end

function draw_dying()
    draw_playfield()

    if (state_frame % (DYING_BLINK_FRAMES * 2)) < DYING_BLINK_FRAMES then
        print_centered(244, "BALL LOST")
    end
end

-- Return a black packed color whose alpha advances every frame.
-- Important: colors are carried through 32-bit signed integer paths. Runtime
-- arithmetic above 0x7FFFFFFF can collapse to 0x80000000, which made the old
-- fade stop around 50% opacity. Keep the exact same bit patterns but represent
-- the upper half as negative signed 32-bit values.
function fade_color_for_frame(frame)
    local alpha = frame * 8

    if alpha >= 255 then
        return -16777216  -- 0xFF000000
    end

    local packed = alpha * 16777216  -- alpha << 24

    if alpha >= 128 then
        packed = packed - 4294967296
    end

    return packed
end

function draw_fade_overlay()
    draw_solid_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, fade_color_for_frame(state_frame))

    -- draw_solid_rect changes persistent GPU multiply/blend state. Restore it
    -- after the overlay so the next screen cannot inherit the black tint.
    spr(REGION_SOLID, -2, -2)
end

function update_dying_fade()
    if state_frame >= FADE_TOTAL_FRAMES then
        if lives > 0 then
            reset_playfield()
            audio_play_game_music()
            set_state(STATE_READY)
        else
            audio_enter_game_over()
            set_state(STATE_GAME_OVER)
        end
    end
end

function draw_dying_fade()
    -- If update_dying_fade changed state this frame, keep the hand-off frame
    -- completely black. The next screen begins cleanly on the following frame.
    if game_state ~= STATE_DYING_FADE then
        draw_solid_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, -16777216)
        spr(REGION_SOLID, -2, -2)
        return
    end

    draw_playfield()
    draw_fade_overlay()
end

function update_game_over()
    -- Arcade-style timeout: no input is required. After the message has been
    -- visible for a fixed time, fade out and return to the title automatically.
    if state_frame >= GAME_OVER_HOLD_FRAMES then
        set_state(STATE_GAME_OVER_FADE)
    end
end

function draw_game_over_scene()
    draw_gameplay_background()
    draw_solid_rect(112, 112, 416, 130, 0xFF050812)
    spr(REGION_SOLID, -2, -2)
    draw_hud()
    print_centered(135, "GAME OVER")
    print_centered(167, "REACHED LEVEL " .. current_level)
    print_centered(199, "FINAL SCORE " .. format_score(score))
end

function draw_game_over()
    draw_game_over_scene()
end

function update_game_over_fade()
    if state_frame >= FADE_TOTAL_FRAMES then
        audio_enter_title()
        set_state(STATE_TITLE)
    end
end

function draw_game_over_fade()
    if game_state ~= STATE_GAME_OVER_FADE then
        draw_solid_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, -16777216)
        spr(REGION_SOLID, -2, -2)
        return
    end

    draw_game_over_scene()
    draw_fade_overlay()
end

function update_level_clear()
    if state_frame >= LEVEL_CLEAR_HOLD_FRAMES then
        set_state(STATE_FADE_OUT)
    end
end

function draw_level_clear()
    draw_playfield()
    print_centered(146, "LEVEL CLEAR")
    print_centered(178, "LEVEL " .. current_level)
    print_centered(210, "SCORE " .. format_score(score))
end

function update_fade_out()
    if state_frame >= FADE_TOTAL_FRAMES then
        if current_level < TOTAL_LEVELS then
            start_next_level()
        else
            audio_play_ending_music()
            set_state(STATE_ENDING)
        end
    end
end

function draw_fade_out()
    -- update_fade_out may switch state before this frame is drawn. Keep that
    -- hand-off frame fully black so the next READY/ENDING screen really
    -- starts after a completed fade instead of flashing the newly loaded level.
    if game_state ~= STATE_FADE_OUT then
        draw_solid_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, -16777216)
        spr(REGION_SOLID, -2, -2)
        return
    end

    -- Redraw the complete source frame first, then lay progressively more
    -- opaque black over it. This gives a true frame-by-frame fade rather than
    -- a few discrete opacity plateaus.
    draw_playfield()
    print_centered(146, "LEVEL CLEAR")
    print_centered(178, "LEVEL " .. current_level)
    draw_fade_overlay()
end

function update_ending()
    if state_frame >= ENDING_INPUT_DELAY then
        if btnp(BUTTON_START) then
            set_state(STATE_ENDING_FADE)
        end
    end
end

function draw_ending_background()
    draw_ending_art()
end

function draw_ending()
    draw_ending_background()
    print_centered(116, "THE END")
    print_centered(156, "CONGRATULATIONS!")
    print_centered(204, "FINAL SCORE " .. format_score(score))

    if state_frame >= ENDING_INPUT_DELAY then
        if (state_frame % 60) < 30 then
            print_centered(294, "PRESS START")
        end
    end
end


function update_ending_fade()
    if state_frame >= FADE_TOTAL_FRAMES then
        audio_enter_title()
        set_state(STATE_TITLE)
    end
end

function draw_ending_fade()
    if game_state ~= STATE_ENDING_FADE then
        draw_solid_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, -16777216)
        spr(REGION_SOLID, -2, -2)
        return
    end

    draw_ending_background()
    print_centered(116, "THE END")
    print_centered(156, "CONGRATULATIONS!")
    print_centered(204, "FINAL SCORE " .. format_score(score))
    print_centered(294, "PRESS START")
    draw_fade_overlay()
end
