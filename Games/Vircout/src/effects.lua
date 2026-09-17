effect_type = {}
effect_x = {}
effect_y = {}
effect_age = {}
active_effect_count = 0

function clear_effects()
    active_effect_count = 0
end

-- Effects are stored compactly: slots 1..active_effect_count are always live.
-- If the pool is full, the new cosmetic effect is simply dropped. Gameplay is
-- never allowed to pay an unbounded CPU cost for particles/highlights.
function spawn_effect(kind, center_x, center_y)
    if active_effect_count >= MAX_EFFECTS then
        return
    end

    local i = active_effect_count + 1
    active_effect_count = i
    effect_type[i] = kind
    effect_x[i] = center_x - EFFECT_WIDTH / 2
    effect_y[i] = center_y - EFFECT_HEIGHT / 2
    effect_age[i] = 0
end

function remove_effect(index)
    local last = active_effect_count
    if index < last then
        effect_type[index] = effect_type[last]
        effect_x[index] = effect_x[last]
        effect_y[index] = effect_y[last]
        effect_age[index] = effect_age[last]
    end
    active_effect_count = last - 1
end

function update_effects()
    if active_effect_count == 0 then
        return
    end

    local i = 1
    while i <= active_effect_count do
        local age = effect_age[i] + 1
        if age >= EFFECT_TOTAL_FRAMES then
            remove_effect(i)
        else
            effect_age[i] = age
            i = i + 1
        end
    end
end

function effect_base_region(kind)
    if kind == EFFECT_BLOCK_NORMAL then
        return REGION_FX_NORMAL_1
    elseif kind == EFFECT_BLOCK_HARD_HIT then
        return REGION_FX_HARD_HIT_1
    elseif kind == EFFECT_BLOCK_HARD_BREAK then
        return REGION_FX_HARD_BREAK_1
    elseif kind == EFFECT_BLOCK_UNBREAKABLE then
        return REGION_FX_METAL_1
    elseif kind == EFFECT_PADDLE_HIT then
        return REGION_FX_PADDLE_1
    else
        return REGION_FX_WALL_1
    end
end

function draw_effects()
    if active_effect_count == 0 then
        return
    end

    local i = 1
    while i <= active_effect_count do
        local base = effect_base_region(effect_type[i])
        local age = effect_age[i]
        local frame = 0

        if age >= EFFECT_FRAME_HOLD * 3 then
            frame = 3
        elseif age >= EFFECT_FRAME_HOLD * 2 then
            frame = 2
        elseif age >= EFFECT_FRAME_HOLD then
            frame = 1
        end

        spr(base + frame, effect_x[i], effect_y[i])
        i = i + 1
    end
end
