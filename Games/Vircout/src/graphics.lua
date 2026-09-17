function print_centered(y, text)
    local text_width = #text * FONT_CHAR_WIDTH
    local x = (SCREEN_WIDTH - text_width) / 2
    print(x, y, text)
end

function format_score(value)
    if value < 10 then
        return "0000" .. value
    elseif value < 100 then
        return "000" .. value
    elseif value < 1000 then
        return "00" .. value
    elseif value < 10000 then
        return "0" .. value
    elseif value < 100000 then
        return "" .. value
    end
    return "99999"
end

function draw_solid_rect(x, y, width, height, color)
    spr(REGION_SOLID, x, y, width, height, 0, color)
end

function draw_playfield_borders()
    draw_solid_rect(0, 0, SCREEN_WIDTH, PLAYFIELD_TOP, 0xFF000000)
    draw_solid_rect(0, PLAYFIELD_TOP, PLAYFIELD_LEFT, SCREEN_HEIGHT - PLAYFIELD_TOP, 0xFF000000)
    draw_solid_rect(PLAYFIELD_RIGHT, PLAYFIELD_TOP, SCREEN_WIDTH - PLAYFIELD_RIGHT, SCREEN_HEIGHT - PLAYFIELD_TOP, 0xFF000000)
    spr(REGION_SOLID, -2, -2)
end

function define_region(region, min_x, min_y, max_x, max_y)
    ioports.gpu.region = region
    ioports.gpu.minX = min_x
    ioports.gpu.minY = min_y
    ioports.gpu.maxX = max_x
    ioports.gpu.maxY = max_y
    ioports.gpu.hotX = min_x
    ioports.gpu.hotY = min_y
end

function graphics_init()
    ioports.gpu.texture = sprites

    define_region(REGION_PADDLE, 0, 0, 79, 11)
    define_region(REGION_BALL, 82, 0, 91, 9)
    define_region(REGION_BLOCK_NORMAL, 96, 0, 139, 15)
    define_region(REGION_SOLID, 142, 0, 142, 0)
    define_region(REGION_BLOCK_HARD, 146, 0, 189, 15)
    define_region(REGION_BLOCK_HARD_DAMAGED, 192, 0, 235, 15)
    define_region(REGION_BLOCK_UNBREAKABLE, 238, 0, 281, 15)

    define_region(REGION_PADDLE_WIDE, 284, 0, 411, 11)
    define_region(REGION_ITEM_LASER, 414, 0, 429, 15)
    define_region(REGION_ITEM_MULTIBALL, 432, 0, 447, 15)
    define_region(REGION_ITEM_WIDE, 450, 0, 465, 15)
    define_region(REGION_SHOT, 468, 0, 471, 11)
    define_region(REGION_CANNON, 232, 18, 241, 33)

    -- Four 56x28 frames per short impact/highlight animation.
    -- The artwork inside each frame can be smaller (paddle/wall sparks) or
    -- nearly block-sized (block explosions/flash), but all share one canvas
    -- so their centers line up consistently with the collision point.
    define_region(REGION_FX_NORMAL_1, 0, 20, 55, 47)
    define_region(REGION_FX_NORMAL_2, 58, 20, 113, 47)
    define_region(REGION_FX_NORMAL_3, 116, 20, 171, 47)
    define_region(REGION_FX_NORMAL_4, 174, 20, 229, 47)

    define_region(REGION_FX_HARD_HIT_1, 0, 50, 55, 77)
    define_region(REGION_FX_HARD_HIT_2, 58, 50, 113, 77)
    define_region(REGION_FX_HARD_HIT_3, 116, 50, 171, 77)
    define_region(REGION_FX_HARD_HIT_4, 174, 50, 229, 77)

    define_region(REGION_FX_HARD_BREAK_1, 0, 80, 55, 107)
    define_region(REGION_FX_HARD_BREAK_2, 58, 80, 113, 107)
    define_region(REGION_FX_HARD_BREAK_3, 116, 80, 171, 107)
    define_region(REGION_FX_HARD_BREAK_4, 174, 80, 229, 107)

    define_region(REGION_FX_METAL_1, 0, 110, 55, 137)
    define_region(REGION_FX_METAL_2, 58, 110, 113, 137)
    define_region(REGION_FX_METAL_3, 116, 110, 171, 137)
    define_region(REGION_FX_METAL_4, 174, 110, 229, 137)

    define_region(REGION_FX_PADDLE_1, 0, 140, 55, 167)
    define_region(REGION_FX_PADDLE_2, 58, 140, 113, 167)
    define_region(REGION_FX_PADDLE_3, 116, 140, 171, 167)
    define_region(REGION_FX_PADDLE_4, 174, 140, 229, 167)

    define_region(REGION_FX_WALL_1, 0, 170, 55, 197)
    define_region(REGION_FX_WALL_2, 58, 170, 113, 197)
    define_region(REGION_FX_WALL_3, 116, 170, 171, 197)
    define_region(REGION_FX_WALL_4, 174, 170, 229, 197)

    -- GPU regions belong to the texture that is selected when they are
    -- defined. REGION_FULLSCREEN therefore has to be created separately
    -- in every 640x360 background texture.
    ioports.gpu.texture = title_texture
    define_region(REGION_FULLSCREEN, 0, 0, 639, 359)

    ioports.gpu.texture = gameplay_texture
    define_region(REGION_FULLSCREEN, 0, 0, 639, 359)

    ioports.gpu.texture = ending_texture
    define_region(REGION_FULLSCREEN, 0, 0, 639, 359)

    -- All ordinary gameplay regions live in the sprite atlas. Leave it
    -- selected as the default texture for the rest of the game.
    ioports.gpu.texture = sprites
end

function draw_fullscreen_texture(texture)
    ioports.gpu.texture = texture
    spr(REGION_FULLSCREEN, 0, 0)
    ioports.gpu.texture = sprites
end

function draw_title_background()
    draw_fullscreen_texture(title_texture)
end

function draw_gameplay_background()
    draw_fullscreen_texture(gameplay_texture)
end

function draw_ending_art()
    draw_fullscreen_texture(ending_texture)
end

function draw_hud()
    print(HUD_LIVES_X, 9, "LIVES " .. lives)
    print(HUD_SCORE_X, 9, "SCORE " .. format_score(score))
end

function draw_paddle()
    if paddle_wide then
        spr(REGION_PADDLE_WIDE, paddle_x, PADDLE_Y)
    else
        spr(REGION_PADDLE, paddle_x, PADDLE_Y)
    end

    -- When laser is active, overlay the same cannon sprite on each firing
    -- line. fire_lasers() launches at paddle_x+8 and paddle_width-8-SHOT_WIDTH;
    -- centering an 8 px cannon on each 4 px shot gives a -2 px offset.
    if laser_enabled then
        local left_cannon_x = paddle_x + 8 - 3
        local right_shot_x = paddle_x + paddle_width - 8 - SHOT_WIDTH
        local right_cannon_x = right_shot_x - 3
        local cannon_y = PADDLE_Y - 5

        spr(REGION_CANNON, left_cannon_x, cannon_y)
        spr(REGION_CANNON, right_cannon_x, cannon_y)
    end
end

function draw_playfield()
    draw_gameplay_background()
    draw_blocks()
    draw_items()
    draw_shots()
    draw_paddle()
    draw_balls()
    draw_effects()
    draw_hud()
end
