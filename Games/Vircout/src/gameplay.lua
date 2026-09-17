paddle_x = 0

ball_x = {}
ball_y = {}
ball_vx = {}
ball_vy = {}
balls_active_count = 0
ball_launched = false

function absolute_value(value)
    if value < 0 then
        return -value
    end
    return value
end

function clear_balls()
    balls_active_count = 0
end

function reset_playfield()
    reset_powerups()
    clear_effects()
    paddle_x = PLAYFIELD_LEFT + (PLAYFIELD_RIGHT - PLAYFIELD_LEFT - paddle_width) / 2
    ball_launched = false
    clear_balls()
    balls_active_count = 1
    ball_vx[1] = BALL_START_VX * BALL_SPEED_MULTIPLIER
    ball_vy[1] = BALL_START_VY * BALL_SPEED_MULTIPLIER
    attach_ball_to_paddle()
end

function attach_ball_to_paddle()
    ball_x[1] = paddle_x + (paddle_width - BALL_SIZE) / 2
    ball_y[1] = PADDLE_Y - BALL_SIZE - BALL_ATTACH_GAP
end

function update_paddle()
    if btn(BUTTON_LEFT) then
        paddle_x = paddle_x - PADDLE_SPEED
    end

    if btn(BUTTON_RIGHT) then
        paddle_x = paddle_x + PADDLE_SPEED
    end

    if paddle_x < PLAYFIELD_LEFT then
        paddle_x = PLAYFIELD_LEFT
    end

    if paddle_x > PLAYFIELD_RIGHT - paddle_width then
        paddle_x = PLAYFIELD_RIGHT - paddle_width
    end
end

function launch_ball()
    if not ball_launched then
        ball_launched = true
        balls_active_count = 1
        ball_vx[1] = BALL_START_VX * BALL_SPEED_MULTIPLIER
        ball_vy[1] = BALL_START_VY * BALL_SPEED_MULTIPLIER
    end
end

function activate_multiball()
    if not ball_launched or balls_active_count <= 0 then
        return
    end

    local target_x = ball_x[1]
    local target_y = ball_y[1]

    if balls_active_count < 2 then
        balls_active_count = 2
        ball_x[2] = target_x
        ball_y[2] = target_y
        ball_vx[2] = -BALL_MULTIBALL_VX * BALL_SPEED_MULTIPLIER
        ball_vy[2] = BALL_MULTIBALL_VY * BALL_SPEED_MULTIPLIER
    end

    if balls_active_count < 3 then
        balls_active_count = 3
        ball_x[3] = target_x
        ball_y[3] = target_y
        ball_vx[3] = BALL_MULTIBALL_VX * BALL_SPEED_MULTIPLIER
        ball_vy[3] = BALL_MULTIBALL_VY * BALL_SPEED_MULTIPLIER
    end
end

-- Paddle collision works entirely on cached scalar values and returns the
-- corrected values. This keeps the normal one-ball path close to milestone 5
-- instead of repeatedly indexing Lua tables during collision tests.
function collide_ball_with_paddle_values(x, y, vx, vy, old_x, old_y)
    if x + BALL_SIZE <= paddle_x then
        return x, y, vx, vy
    end
    if x >= paddle_x + paddle_width then
        return x, y, vx, vy
    end
    if y + BALL_SIZE <= PADDLE_Y then
        return x, y, vx, vy
    end
    if y >= PADDLE_Y + PADDLE_HEIGHT then
        return x, y, vx, vy
    end

    local old_right = old_x + BALL_SIZE
    local old_bottom = old_y + BALL_SIZE
    local paddle_right = paddle_x + paddle_width

    if vy > 0 and old_bottom <= PADDLE_Y then
        local ball_center = x + BALL_SIZE / 2
        local paddle_center = paddle_x + paddle_width / 2
        local offset = (ball_center - paddle_center) / (paddle_width / 2)
        local direction = 1
        local amount = offset

        if amount < 0 then
            direction = -1
            amount = -amount
        elseif amount == 0 and vx < 0 then
            direction = -1
        end

        if amount < 0.20 then
            vx = direction * BALL_BOUNCE_CENTER_VX * BALL_SPEED_MULTIPLIER
            vy = BALL_BOUNCE_CENTER_VY * BALL_SPEED_MULTIPLIER
        elseif amount < 0.50 then
            vx = direction * BALL_BOUNCE_MID_VX * BALL_SPEED_MULTIPLIER
            vy = BALL_BOUNCE_MID_VY * BALL_SPEED_MULTIPLIER
        elseif amount < 0.80 then
            vx = direction * BALL_BOUNCE_WIDE_VX * BALL_SPEED_MULTIPLIER
            vy = BALL_BOUNCE_WIDE_VY * BALL_SPEED_MULTIPLIER
        else
            vx = direction * BALL_BOUNCE_EDGE_VX * BALL_SPEED_MULTIPLIER
            vy = BALL_BOUNCE_EDGE_VY * BALL_SPEED_MULTIPLIER
        end

        y = PADDLE_Y - BALL_SIZE
        spawn_effect(EFFECT_PADDLE_HIT, x + BALL_SIZE / 2, PADDLE_Y)
        audio_paddle_hit()
        return x, y, vx, vy
    end

    if vx > 0 and old_right <= paddle_x then
        vx = -absolute_value(vx)
        x = paddle_x - BALL_SIZE
        spawn_effect(EFFECT_PADDLE_HIT, paddle_x, y + BALL_SIZE / 2)
        audio_paddle_hit()
        return x, y, vx, vy
    end

    if vx < 0 and old_x >= paddle_right then
        vx = absolute_value(vx)
        x = paddle_right
        spawn_effect(EFFECT_PADDLE_HIT, paddle_right, y + BALL_SIZE / 2)
        audio_paddle_hit()
        return x, y, vx, vy
    end

    local penetration_top = y + BALL_SIZE - PADDLE_Y
    local penetration_left = x + BALL_SIZE - paddle_x
    local penetration_right = paddle_right - x

    if vy > 0 and penetration_top <= penetration_left and penetration_top <= penetration_right then
        local ball_center = x + BALL_SIZE / 2
        local paddle_center = paddle_x + paddle_width / 2
        local offset = (ball_center - paddle_center) / (paddle_width / 2)
        local direction = 1
        local amount = offset

        if amount < 0 then
            direction = -1
            amount = -amount
        elseif amount == 0 and vx < 0 then
            direction = -1
        end

        if amount < 0.20 then
            vx = direction * BALL_BOUNCE_CENTER_VX * BALL_SPEED_MULTIPLIER
            vy = BALL_BOUNCE_CENTER_VY * BALL_SPEED_MULTIPLIER
        elseif amount < 0.50 then
            vx = direction * BALL_BOUNCE_MID_VX * BALL_SPEED_MULTIPLIER
            vy = BALL_BOUNCE_MID_VY * BALL_SPEED_MULTIPLIER
        elseif amount < 0.80 then
            vx = direction * BALL_BOUNCE_WIDE_VX * BALL_SPEED_MULTIPLIER
            vy = BALL_BOUNCE_WIDE_VY * BALL_SPEED_MULTIPLIER
        else
            vx = direction * BALL_BOUNCE_EDGE_VX * BALL_SPEED_MULTIPLIER
            vy = BALL_BOUNCE_EDGE_VY * BALL_SPEED_MULTIPLIER
        end
        y = PADDLE_Y - BALL_SIZE
        spawn_effect(EFFECT_PADDLE_HIT, x + BALL_SIZE / 2, PADDLE_Y)
        audio_paddle_hit()
    elseif penetration_left < penetration_right then
        vx = -absolute_value(vx)
        x = paddle_x - BALL_SIZE
        spawn_effect(EFFECT_PADDLE_HIT, paddle_x, y + BALL_SIZE / 2)
        audio_paddle_hit()
    else
        vx = absolute_value(vx)
        x = paddle_right
        spawn_effect(EFFECT_PADDLE_HIT, paddle_right, y + BALL_SIZE / 2)
        audio_paddle_hit()
    end

    return x, y, vx, vy
end

function begin_ball_loss()
    if game_state ~= STATE_DYING then
        lives = lives - 1
        clear_balls()
        audio_ball_lost()
        set_state(STATE_DYING)
    end
end

-- Remove one ball while keeping the active pool contiguous. This means all
-- ball loops run only balls_active_count iterations (normally exactly one).
function remove_ball(index)
    local last = balls_active_count

    if index < last then
        ball_x[index] = ball_x[last]
        ball_y[index] = ball_y[last]
        ball_vx[index] = ball_vx[last]
        ball_vy[index] = ball_vy[last]
    end

    balls_active_count = last - 1
end

function update_one_ball(i)
    -- Cache table values once. The collision hot paths below use scalars only.
    local x = ball_x[i]
    local y = ball_y[i]
    local vx = ball_vx[i]
    local vy = ball_vy[i]
    local old_x = x
    local old_y = y

    x = x + vx
    y = y + vy

    if x < PLAYFIELD_LEFT then
        x = PLAYFIELD_LEFT
        vx = -vx
        spawn_effect(EFFECT_WALL_HIT, PLAYFIELD_LEFT, y + BALL_SIZE / 2)
        audio_wall_hit()
    end

    if x + BALL_SIZE > PLAYFIELD_RIGHT then
        x = PLAYFIELD_RIGHT - BALL_SIZE
        vx = -vx
        spawn_effect(EFFECT_WALL_HIT, PLAYFIELD_RIGHT, y + BALL_SIZE / 2)
        audio_wall_hit()
    end

    if y < PLAYFIELD_TOP then
        y = PLAYFIELD_TOP
        vy = -vy
        spawn_effect(EFFECT_WALL_HIT, x + BALL_SIZE / 2, PLAYFIELD_TOP)
        audio_wall_hit()
    end

    x, y, vx, vy = collide_ball_with_blocks_values(x, y, vx, vy, old_x, old_y)

    if game_state ~= STATE_PLAYING then
        ball_x[i] = x
        ball_y[i] = y
        ball_vx[i] = vx
        ball_vy[i] = vy
        return true
    end

    x, y, vx, vy = collide_ball_with_paddle_values(x, y, vx, vy, old_x, old_y)

    if y > SCREEN_HEIGHT then
        return false
    end

    ball_x[i] = x
    ball_y[i] = y
    ball_vx[i] = vx
    ball_vy[i] = vy
    return true
end

function update_balls()
    if not ball_launched then
        attach_ball_to_paddle()
        return
    end

    local i = 1
    while i <= balls_active_count do
        if update_one_ball(i) then
            i = i + 1
        else
            remove_ball(i)
        end

        if game_state ~= STATE_PLAYING then
            return
        end
    end

    if balls_active_count == 0 then
        begin_ball_loss()
    end
end

function draw_balls()
    local i = 1
    while i <= balls_active_count do
        spr(REGION_BALL, ball_x[i], ball_y[i])
        i = i + 1
    end
end
