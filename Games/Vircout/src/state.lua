game_state = STATE_TITLE
state_frame = 0

lives = STARTING_LIVES
score = 0
current_level = 1

-- Entropy accumulated while the player waits on the title screen. User input
-- timing is our closest equivalent to srand(time()) on a deterministic console.
rng_title_entropy = 1

-- Secret-code state. Once enabled, 20-life mode remains active until the
-- cartridge is restarted.
konami_progress = 0
cheat_lives_enabled = false
cheat_start_timer = 0

function set_state(new_state)
    game_state = new_state
    state_frame = 0
end

function start_new_game()
    audio_play_game_music()

    -- Seed the hardware RNG from player timing, then discard the first values.
    -- The hardware generator is deterministic for a given seed, and its first
    -- outputs can be strongly correlated for nearby seeds. Mixing the title
    -- wait time and warming the generator prevents the first broken block from
    -- repeatedly seeing the same part of the sequence.
    local seed = ioports.tim.frames + rng_title_entropy * 257
    while seed >= 2147483647 do
        seed = seed - 2147483647
    end
    ioports.rng.seed = seed

    local warmup = 0
    while warmup < 8 do
        local discard = ioports.rng.value
        warmup = warmup + 1
    end

    if cheat_lives_enabled then
        lives = CHEAT_STARTING_LIVES
    else
        lives = STARTING_LIVES
    end

    score = 0
    current_level = 1
    load_level(current_level)
    reset_playfield()
    set_state(STATE_READY)
end

function start_next_level()
    audio_play_game_music()
    current_level = current_level + 1
    load_level(current_level)
    reset_playfield()
    set_state(STATE_READY)
end
