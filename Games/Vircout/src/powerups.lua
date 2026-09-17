item_active = {}
item_type = {}
item_x = {}
item_y = {}
active_item_count = 0

shot_active = {}
shot_x = {}
shot_y = {}
active_shot_count = 0

laser_enabled = false
laser_cooldown = 0
paddle_wide = false
paddle_width = PADDLE_NORMAL_WIDTH

function clear_items()
    local i = 1
    while i <= MAX_ITEMS do
        item_active[i] = false
        i = i + 1
    end
    active_item_count = 0
end

function clear_shots()
    local i = 1
    while i <= MAX_SHOTS do
        shot_active[i] = false
        i = i + 1
    end
    active_shot_count = 0
end

function reset_powerups()
    laser_enabled = false
    laser_cooldown = 0
    paddle_wide = false
    paddle_width = PADDLE_NORMAL_WIDTH
    clear_items()
    clear_shots()
end

-- Map the signed RNG register into a non-negative 31-bit range without using
-- abs(), which has an awkward edge case at -2147483648.
function random_nonnegative()
    local value = ioports.rng.value
    if value < 0 then
        value = value + 2147483648
    end
    return value
end

function spawn_item(kind, x, y)
    local i = 1
    while i <= MAX_ITEMS do
        if not item_active[i] then
            item_active[i] = true
            item_type[i] = kind
            item_x[i] = x
            item_y[i] = y
            active_item_count = active_item_count + 1
            return true
        end
        i = i + 1
    end
    return false
end

function maybe_spawn_item(x, y)
    -- Never allow more than two falling items at once. Besides keeping the
    -- screen readable, this also avoids spending RNG/CPU on a spawn that
    -- cannot be represented.
    if active_item_count >= MAX_ITEMS then
        return
    end

    -- Use the full RNG range instead of modulo on its low bits. Some simple
    -- PRNGs have visible patterns in their low bits, which can cause drops to
    -- arrive in clusters when using % 100 or % 3 directly.
    local drop_roll = random_nonnegative()
    if drop_roll >= ITEM_DROP_THRESHOLD then
        return
    end

    -- Read the RNG again for the item type. Split the whole 31-bit range into
    -- three almost equal bands, again avoiding low-bit modulo.
    local type_roll = random_nonnegative()
    local kind = ITEM_WIDE
    if type_roll < 715827883 then
        kind = ITEM_LASER
    elseif type_roll < 1431655766 then
        kind = ITEM_MULTIBALL
    end

    spawn_item(kind, x, y)
end

function item_overlaps_paddle(i)
    if item_x[i] + ITEM_SIZE <= paddle_x then
        return false
    end
    if item_x[i] >= paddle_x + paddle_width then
        return false
    end
    if item_y[i] + ITEM_SIZE <= PADDLE_Y then
        return false
    end
    if item_y[i] >= PADDLE_Y + PADDLE_HEIGHT then
        return false
    end
    return true
end

function apply_wide_paddle()
    -- Preserve the paddle centre while changing its width, so the visual
    -- growth happens symmetrically instead of extending only to the right.
    local paddle_center = paddle_x + paddle_width / 2

    paddle_wide = true
    paddle_width = PADDLE_WIDE_WIDTH
    paddle_x = paddle_center - paddle_width / 2

    -- Near a wall the centred position may no longer fit. Shift only as much
    -- as needed to keep the entire widened paddle inside the playfield.
    if paddle_x < PLAYFIELD_LEFT then
        paddle_x = PLAYFIELD_LEFT
    end

    if paddle_x > PLAYFIELD_RIGHT - paddle_width then
        paddle_x = PLAYFIELD_RIGHT - paddle_width
    end
end

function deactivate_item(i)
    if item_active[i] then
        item_active[i] = false
        active_item_count = active_item_count - 1
    end
end

function collect_item(i)
    local kind = item_type[i]
    deactivate_item(i)
    score = score + ITEM_SCORE
    audio_item_pickup()

    if kind == ITEM_LASER then
        laser_enabled = true
    elseif kind == ITEM_MULTIBALL then
        activate_multiball()
    elseif kind == ITEM_WIDE then
        apply_wide_paddle()
    end
end

function update_items()
    -- Normal gameplay has no falling item most frames. Avoid scanning 8 slots
    -- unless at least one item actually exists.
    if active_item_count == 0 then
        return
    end

    local i = 1
    while i <= MAX_ITEMS do
        if item_active[i] then
            item_y[i] = item_y[i] + ITEM_FALL_SPEED

            if item_overlaps_paddle(i) then
                collect_item(i)
            elseif item_y[i] > SCREEN_HEIGHT then
                deactivate_item(i)
            end
        end
        i = i + 1
    end
end

function draw_items()
    if active_item_count == 0 then
        return
    end

    local i = 1
    while i <= MAX_ITEMS do
        if item_active[i] then
            if item_type[i] == ITEM_LASER then
                spr(REGION_ITEM_LASER, item_x[i], item_y[i])
            elseif item_type[i] == ITEM_MULTIBALL then
                spr(REGION_ITEM_MULTIBALL, item_x[i], item_y[i])
            elseif item_type[i] == ITEM_WIDE then
                spr(REGION_ITEM_WIDE, item_x[i], item_y[i])
            end
        end
        i = i + 1
    end
end

function activate_shot(x, y)
    local i = 1
    while i <= MAX_SHOTS do
        if not shot_active[i] then
            shot_active[i] = true
            shot_x[i] = x
            shot_y[i] = y
            active_shot_count = active_shot_count + 1
            return true
        end
        i = i + 1
    end
    return false
end

function fire_lasers()
    if not laser_enabled then
        return
    end
    if laser_cooldown > 0 then
        return
    end

    local left_x = paddle_x + 8
    local right_x = paddle_x + paddle_width - 8 - SHOT_WIDTH
    local shot_y_start = PADDLE_Y - SHOT_HEIGHT

    local fired_left = activate_shot(left_x, shot_y_start)
    local fired_right = activate_shot(right_x, shot_y_start)

    if fired_left or fired_right then
        audio_laser_fire()
        laser_cooldown = LASER_COOLDOWN_FRAMES
    end
end

function deactivate_shot(i)
    if shot_active[i] then
        shot_active[i] = false
        active_shot_count = active_shot_count - 1
    end
end

function update_one_shot(i)
    local sx = shot_x[i]
    local sy = shot_y[i] - SHOT_SPEED
    shot_y[i] = sy

    if sy + SHOT_HEIGHT < PLAYFIELD_TOP then
        deactivate_shot(i)
        return
    end

    -- The block field is a fixed grid. Most of a shot's lifetime happens
    -- below it, so reject that case immediately. Once inside the block field,
    -- inspect only a 3x3 neighbourhood instead of all 84 cells.
    if sx + SHOT_WIDTH <= BLOCK_ORIGIN_X then
        return
    end
    if sx >= BLOCK_GRID_RIGHT then
        return
    end
    if sy + SHOT_HEIGHT <= BLOCK_ORIGIN_Y then
        return
    end
    if sy >= BLOCK_GRID_BOTTOM then
        return
    end

    local center_col = block_grid_col_at(sx + SHOT_WIDTH / 2)
    local center_row = block_grid_row_at(sy + SHOT_HEIGHT / 2)
    local first_col = center_col - 1
    local last_col = center_col + 1
    local first_row = center_row - 1
    local last_row = center_row + 1

    if first_col < 0 then first_col = 0 end
    if last_col >= BLOCK_COLS then last_col = BLOCK_COLS - 1 end
    if first_row < 0 then first_row = 0 end
    if last_row >= BLOCK_ROWS then last_row = BLOCK_ROWS - 1 end

    local row = first_row
    while row <= last_row do
        local col = first_col
        local by = BLOCK_ORIGIN_Y + row * BLOCK_PITCH_Y

        while col <= last_col do
            local index = block_index(col, row)
            if block_active[index] then
                local bx = BLOCK_ORIGIN_X + col * BLOCK_PITCH_X
                if sx + SHOT_WIDTH > bx and sx < bx + BLOCK_WIDTH and
                   sy + SHOT_HEIGHT > by and sy < by + BLOCK_HEIGHT then
                    deactivate_shot(i)
                    hit_block(index, bx, by)
                    return
                end
            end
            col = col + 1
        end
        row = row + 1
    end
end

function update_shots()
    if laser_cooldown > 0 then
        laser_cooldown = laser_cooldown - 1
    end

    -- Do not scan the projectile pool during ordinary gameplay when no shot is
    -- on screen. Cooldown still advances above when needed.
    if active_shot_count == 0 then
        return
    end

    local i = 1
    while i <= MAX_SHOTS do
        if shot_active[i] then
            update_one_shot(i)
            if game_state ~= STATE_PLAYING then
                return
            end
        end
        i = i + 1
    end
end

function draw_shots()
    if active_shot_count == 0 then
        return
    end

    local i = 1
    while i <= MAX_SHOTS do
        if shot_active[i] then
            spr(REGION_SHOT, shot_x[i], shot_y[i])
        end
        i = i + 1
    end
end
