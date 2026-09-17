-- Screen states.
STATE_TITLE          = 0
STATE_READY          = 1
STATE_PLAYING        = 2
STATE_DYING          = 3
STATE_GAME_OVER      = 4
STATE_LEVEL_CLEAR    = 5
STATE_FADE_OUT       = 6
STATE_ENDING         = 7
STATE_DYING_FADE     = 8
STATE_GAME_OVER_FADE = 9
STATE_ENDING_FADE    = 10
STATE_PAUSE          = 11

-- Vircon32 native input button IDs.
BUTTON_LEFT  = 0
BUTTON_RIGHT = 1
BUTTON_UP    = 2
BUTTON_DOWN  = 3
BUTTON_START = 4
BUTTON_A     = 5
BUTTON_B     = 6

-- Screen / HUD.
SCREEN_WIDTH    = 640
SCREEN_HEIGHT   = 360
HUD_HEIGHT      = 36
FONT_CHAR_WIDTH = 10

-- Visible gameplay bounds. Left/right coincide exactly with the outer block-grid edges.
PLAYFIELD_LEFT  = 32
PLAYFIELD_RIGHT = 604
PLAYFIELD_TOP   = HUD_HEIGHT

HUD_LIVES_X = 40
HUD_SCORE_X = 488

-- Paddle.
PADDLE_NORMAL_WIDTH = 80
PADDLE_WIDE_WIDTH   = 128
PADDLE_HEIGHT       = 12
PADDLE_Y            = 330
PADDLE_SPEED        = 5

-- Balls.
MAX_BALLS       = 3
BALL_SIZE       = 10

-- Global multiplier applied whenever a new ball velocity is assigned.
-- 1.0 preserves the original Vircout movement exactly.
BALL_SPEED_MULTIPLIER = 1.0

-- Base ball velocities. Keep these unscaled: BALL_SPEED_MULTIPLIER is applied
-- at the points where a fresh velocity is assigned.
BALL_START_VX = 2.4
BALL_START_VY = -3.2

BALL_MULTIBALL_VX = 2.60
BALL_MULTIBALL_VY = -3.04

-- Paddle rebound velocity pairs, from the centre of the paddle to its edges.
BALL_BOUNCE_CENTER_VX = 0.80
BALL_BOUNCE_CENTER_VY = -3.92

BALL_BOUNCE_MID_VX = 1.80
BALL_BOUNCE_MID_VY = -3.57

BALL_BOUNCE_WIDE_VX = 2.60
BALL_BOUNCE_WIDE_VY = -3.04

BALL_BOUNCE_EDGE_VX = 3.20
BALL_BOUNCE_EDGE_VY = -2.40

BALL_ATTACH_GAP = 2

-- Blocks. The 12-column layout is 576 px wide and centered in 640 px.
BLOCK_COLS     = 12
BLOCK_ROWS     = 7
BLOCK_WIDTH    = 44
BLOCK_HEIGHT   = 16
BLOCK_GAP_X    = 4
BLOCK_GAP_Y    = 4
BLOCK_PITCH_X  = BLOCK_WIDTH + BLOCK_GAP_X
BLOCK_PITCH_Y  = BLOCK_HEIGHT + BLOCK_GAP_Y
BLOCK_ORIGIN_X = 32
BLOCK_ORIGIN_Y = 60
BLOCK_GRID_RIGHT  = BLOCK_ORIGIN_X + (BLOCK_COLS - 1) * BLOCK_PITCH_X + BLOCK_WIDTH
BLOCK_GRID_BOTTOM = BLOCK_ORIGIN_Y + (BLOCK_ROWS - 1) * BLOCK_PITCH_Y + BLOCK_HEIGHT

BLOCK_EMPTY       = 0
BLOCK_NORMAL      = 1
BLOCK_HARD        = 2
BLOCK_UNBREAKABLE = 3

BLOCK_NORMAL_HP = 1
BLOCK_HARD_HP   = 2

BLOCK_NORMAL_SCORE = 50
BLOCK_HARD_SCORE   = 100

-- Falling items.
ITEM_NONE      = 0
ITEM_LASER     = 1
ITEM_MULTIBALL = 2
ITEM_WIDE      = 3
MAX_ITEMS      = 2
ITEM_SIZE       = 16
ITEM_FALL_SPEED = 1.5
ITEM_DROP_PERCENT = 12
-- 12% of the non-negative 31-bit RNG range. Using a full-range threshold
-- avoids relying on the RNG low bits via modulo.
ITEM_DROP_THRESHOLD = 257698038
ITEM_SCORE = 20

-- Laser shots.
MAX_SHOTS       = 8
SHOT_WIDTH      = 4
SHOT_HEIGHT     = 12
SHOT_SPEED      = 6
LASER_COOLDOWN_FRAMES = 40

-- Game rules.
STARTING_LIVES       = 5
CHEAT_STARTING_LIVES = 20

-- Konami code: Up, Up, Down, Down, Left, Right, Left, Right, B, A, Start.
KONAMI_CODE = {
    BUTTON_UP,
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_B,
    BUTTON_A,
    BUTTON_START
}
KONAMI_CODE_LENGTH = 11
CHEAT_CONFIRM_FRAMES = 30

-- Timings, in frames (Vircon32 normally runs at 60 FPS).
READY_TOTAL_FRAMES      = 120
READY_BLINK_FRAMES      = 15
DYING_TOTAL_FRAMES      = 75
DYING_BLINK_FRAMES      = 10
GAME_OVER_HOLD_FRAMES   = 150
LEVEL_CLEAR_HOLD_FRAMES = 90
FADE_TOTAL_FRAMES       = 32
ENDING_INPUT_DELAY      = 120
TOTAL_LEVELS            = 8

-- GPU region IDs.
REGION_PADDLE             = 1
REGION_BALL               = 2
REGION_BLOCK_NORMAL       = 3
REGION_SOLID              = 4
REGION_BLOCK_HARD         = 5
REGION_BLOCK_HARD_DAMAGED = 6
REGION_BLOCK_UNBREAKABLE  = 7
REGION_PADDLE_WIDE        = 8
REGION_ITEM_LASER         = 9
REGION_ITEM_MULTIBALL     = 10
REGION_ITEM_WIDE          = 11
REGION_SHOT               = 12

-- Short impact/highlight animations. Each effect owns 4 consecutive regions.
REGION_FX_NORMAL_1          = 13
REGION_FX_NORMAL_2          = 14
REGION_FX_NORMAL_3          = 15
REGION_FX_NORMAL_4          = 16
REGION_FX_HARD_HIT_1        = 17
REGION_FX_HARD_HIT_2        = 18
REGION_FX_HARD_HIT_3        = 19
REGION_FX_HARD_HIT_4        = 20
REGION_FX_HARD_BREAK_1      = 21
REGION_FX_HARD_BREAK_2      = 22
REGION_FX_HARD_BREAK_3      = 23
REGION_FX_HARD_BREAK_4      = 24
REGION_FX_METAL_1           = 25
REGION_FX_METAL_2           = 26
REGION_FX_METAL_3           = 27
REGION_FX_METAL_4           = 28
REGION_FX_PADDLE_1          = 29
REGION_FX_PADDLE_2          = 30
REGION_FX_PADDLE_3          = 31
REGION_FX_PADDLE_4          = 32
REGION_FX_WALL_1            = 33
REGION_FX_WALL_2            = 34
REGION_FX_WALL_3            = 35
REGION_FX_WALL_4            = 36
REGION_FULLSCREEN            = 37
REGION_CANNON                = 38

-- Fixed compact pool for short, stationary visual effects.
MAX_EFFECTS = 8
EFFECT_WIDTH = 56
EFFECT_HEIGHT = 28
EFFECT_FRAME_HOLD = 3
EFFECT_TOTAL_FRAMES = 12

EFFECT_BLOCK_NORMAL      = 1
EFFECT_BLOCK_HARD_HIT    = 2
EFFECT_BLOCK_HARD_BREAK  = 3
EFFECT_BLOCK_UNBREAKABLE = 4
EFFECT_PADDLE_HIT        = 5
EFFECT_WALL_HIT          = 6

-- Laser cannon overlay. The same 8x14 sprite is used on both paddle sizes.
CANNON_WIDTH  = 10
CANNON_HEIGHT = 16
