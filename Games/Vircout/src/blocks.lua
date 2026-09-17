block_active = {}
block_type = {}
block_hp = {}
breakable_blocks_remaining = 0

function block_index(col, row)
    return row * BLOCK_COLS + col + 1
end

function clear_blocks()
    local row = 0
    breakable_blocks_remaining = 0

    while row < BLOCK_ROWS do
        local col = 0
        while col < BLOCK_COLS do
            local index = block_index(col, row)
            block_active[index] = false
            block_type[index] = BLOCK_EMPTY
            block_hp[index] = 0
            col = col + 1
        end
        row = row + 1
    end
end

function configure_block(index, tile)
    block_type[index] = tile

    if tile == BLOCK_NORMAL then
        block_active[index] = true
        block_hp[index] = BLOCK_NORMAL_HP
        breakable_blocks_remaining = breakable_blocks_remaining + 1
    elseif tile == BLOCK_HARD then
        block_active[index] = true
        block_hp[index] = BLOCK_HARD_HP
        breakable_blocks_remaining = breakable_blocks_remaining + 1
    elseif tile == BLOCK_UNBREAKABLE then
        block_active[index] = true
        block_hp[index] = 0
    else
        block_active[index] = false
        block_type[index] = BLOCK_EMPTY
        block_hp[index] = 0
    end
end

function get_level_tile(level_number, col, row)
    if level_number == 1 then
        return tilemap.get(LEVEL1, col, row)
    elseif level_number == 2 then
        return tilemap.get(LEVEL2, col, row)
    elseif level_number == 3 then
        return tilemap.get(LEVEL3, col, row)
    elseif level_number == 4 then
        return tilemap.get(LEVEL4, col, row)
    elseif level_number == 5 then
        return tilemap.get(LEVEL5, col, row)
    elseif level_number == 6 then
        return tilemap.get(LEVEL6, col, row)
    elseif level_number == 7 then
        return tilemap.get(LEVEL7, col, row)
    else
        return tilemap.get(LEVEL8, col, row)
    end
end

function load_level(level_number)
    local row = 0
    breakable_blocks_remaining = 0

    while row < BLOCK_ROWS do
        local col = 0
        while col < BLOCK_COLS do
            local index = block_index(col, row)
            configure_block(index, get_level_tile(level_number, col, row))
            col = col + 1
        end
        row = row + 1
    end
end

function draw_block(index, x, y)
    local kind = block_type[index]

    if kind == BLOCK_NORMAL then
        spr(REGION_BLOCK_NORMAL, x, y)
    elseif kind == BLOCK_HARD then
        if block_hp[index] >= BLOCK_HARD_HP then
            spr(REGION_BLOCK_HARD, x, y)
        else
            spr(REGION_BLOCK_HARD_DAMAGED, x, y)
        end
    elseif kind == BLOCK_UNBREAKABLE then
        spr(REGION_BLOCK_UNBREAKABLE, x, y)
    end
end

function draw_blocks()
    local row = 0
    local y = BLOCK_ORIGIN_Y

    while row < BLOCK_ROWS do
        local col = 0
        local x = BLOCK_ORIGIN_X

        while col < BLOCK_COLS do
            local index = block_index(col, row)
            if block_active[index] then
                draw_block(index, x, y)
            end
            x = x + BLOCK_PITCH_X
            col = col + 1
        end

        y = y + BLOCK_PITCH_Y
        row = row + 1
    end
end

-- Shared damage entry point for balls and laser shots.
-- bx/by are supplied by the collision loop so we do not need an extra pair of
-- per-block position tables in RAM (or their table lookups every frame).
function hit_block(index, bx, by)
    if not block_active[index] then
        return false
    end

    local kind = block_type[index]
    local center_x = bx + BLOCK_WIDTH / 2
    local center_y = by + BLOCK_HEIGHT / 2

    if kind == BLOCK_UNBREAKABLE then
        spawn_effect(EFFECT_BLOCK_UNBREAKABLE, center_x, center_y)
        audio_metal_hit()
        return false
    end

    block_hp[index] = block_hp[index] - 1
    if block_hp[index] > 0 then
        -- Only hard blocks can survive a hit.
        spawn_effect(EFFECT_BLOCK_HARD_HIT, center_x, center_y)
        audio_hard_hit()
        return false
    end

    if kind == BLOCK_NORMAL then
        spawn_effect(EFFECT_BLOCK_NORMAL, center_x, center_y)
        audio_normal_break()
    else
        spawn_effect(EFFECT_BLOCK_HARD_BREAK, center_x, center_y)
        audio_hard_break()
    end

    block_active[index] = false
    breakable_blocks_remaining = breakable_blocks_remaining - 1

    if kind == BLOCK_NORMAL then
        score = score + BLOCK_NORMAL_SCORE
    elseif kind == BLOCK_HARD then
        score = score + BLOCK_HARD_SCORE
    end

    maybe_spawn_item(
        bx + (BLOCK_WIDTH - ITEM_SIZE) / 2,
        by
    )

    if breakable_blocks_remaining <= 0 then
        ball_launched = false
        clear_shots()
        audio_level_clear()
        set_state(STATE_LEVEL_CLEAR)
    end

    return true
end

-- Convert a screen X coordinate to a nearby grid column without using any
-- table lookup or math-library call. The loop has at most 11 very cheap
-- comparisons, after which collision tests only inspect a 3x3 neighbourhood.
function block_grid_col_at(px)
    local col = 0
    local boundary = BLOCK_ORIGIN_X + BLOCK_PITCH_X

    while col < BLOCK_COLS - 1 do
        if px < boundary then
            return col
        end
        col = col + 1
        boundary = boundary + BLOCK_PITCH_X
    end

    return BLOCK_COLS - 1
end

-- Same mapping for rows (at most 6 comparisons).
function block_grid_row_at(py)
    local row = 0
    local boundary = BLOCK_ORIGIN_Y + BLOCK_PITCH_Y

    while row < BLOCK_ROWS - 1 do
        if py < boundary then
            return row
        end
        row = row + 1
        boundary = boundary + BLOCK_PITCH_Y
    end

    return BLOCK_ROWS - 1
end

-- Collision hot path. Blocks form a fixed regular grid, so scanning all 84
-- cells for every moving object is unnecessary. First reject balls that are
-- outside the complete block field; otherwise inspect only the cell under the
-- ball centre and its immediate neighbours (maximum 9 candidate blocks).
function collide_ball_with_blocks_values(x, y, vx, vy, old_x, old_y)
    if x + BALL_SIZE <= BLOCK_ORIGIN_X then
        return x, y, vx, vy
    end
    if x >= BLOCK_GRID_RIGHT then
        return x, y, vx, vy
    end
    if y + BALL_SIZE <= BLOCK_ORIGIN_Y then
        return x, y, vx, vy
    end
    if y >= BLOCK_GRID_BOTTOM then
        return x, y, vx, vy
    end

    local center_col = block_grid_col_at(x + BALL_SIZE / 2)
    local center_row = block_grid_row_at(y + BALL_SIZE / 2)
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
                local overlaps = true

                if x + BALL_SIZE <= bx then
                    overlaps = false
                elseif x >= bx + BLOCK_WIDTH then
                    overlaps = false
                elseif y + BALL_SIZE <= by then
                    overlaps = false
                elseif y >= by + BLOCK_HEIGHT then
                    overlaps = false
                end

                if overlaps then
                    local old_right = old_x + BALL_SIZE
                    local old_bottom = old_y + BALL_SIZE
                    local block_right = bx + BLOCK_WIDTH
                    local block_bottom = by + BLOCK_HEIGHT

                    if vy > 0 and old_bottom <= by then
                        y = by - BALL_SIZE
                        vy = -absolute_value(vy)
                    elseif vy < 0 and old_y >= block_bottom then
                        y = block_bottom
                        vy = absolute_value(vy)
                    elseif vx > 0 and old_right <= bx then
                        x = bx - BALL_SIZE
                        vx = -absolute_value(vx)
                    elseif vx < 0 and old_x >= block_right then
                        x = block_right
                        vx = absolute_value(vx)
                    else
                        local penetration_top = y + BALL_SIZE - by
                        local penetration_bottom = block_bottom - y
                        local penetration_left = x + BALL_SIZE - bx
                        local penetration_right = block_right - x
                        local smallest = penetration_top
                        local face = 0

                        if penetration_bottom < smallest then
                            smallest = penetration_bottom
                            face = 1
                        end
                        if penetration_left < smallest then
                            smallest = penetration_left
                            face = 2
                        end
                        if penetration_right < smallest then
                            face = 3
                        end

                        if face == 0 then
                            y = by - BALL_SIZE
                            vy = -absolute_value(vy)
                        elseif face == 1 then
                            y = block_bottom
                            vy = absolute_value(vy)
                        elseif face == 2 then
                            x = bx - BALL_SIZE
                            vx = -absolute_value(vx)
                        else
                            x = block_right
                            vx = absolute_value(vx)
                        end
                    end

                    hit_block(index, bx, by)
                    return x, y, vx, vy
                end
            end

            col = col + 1
        end

        row = row + 1
    end

    return x, y, vx, vy
end
