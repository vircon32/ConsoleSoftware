; THIS PROGRAM SHOWS A CHARACTER YOU CAN MOVE IN 4 DIRECTIONS
; =============================================================

; execution begins here; since our main program is further
; down, jump to it or else the CPU will run unintended code
jmp _program

; ------------ DECLARATIONS ---------------

; define constants
%define CharacterSpeed 2       ; character moves at 2 pixels/frame = 120 pixels/second

; define variables with their addresses in memory
%define CharacterX   [ 0 ]   ; character's X coordinate on screen (integer)
%define CharacterY   [ 1 ]   ; character's Y coordinate on screen (integer)
%define DirectionX   [ 2 ]   ; character's facing direction on X: -1 = left, 0 = neutral, +1 = right (integer)
%define DirectionY   [ 3 ]   ; character's facing direction on Y: -1 = up  , 0 = neutral, +1 = down (integer)
%define CharIsMoving [ 4 ]  ; true when character is moving, false when stopped (boolean)
%define FramesMoving [ 5 ]  ; number of frames the character has been moving (integer)

; ------------ INCLUDES ---------------

; include other source files
%include "DefineRegions.asm"     ; subroutines to define character regions from the texture
%include "WritePosition.asm"     ; subroutines to write character position on screen with text
%include "Movement.asm"          ; subroutines to determine character direction and movement

; ------------ AUXILIARY SUBROUTINES ---------------

; counts number of frames the character has been moving
; continuously and stores it in our variable; we will
; use this to decide which animation image to show
_count_moving_frames:
  
  ; reset motion frames when not moving
  push R0
  mov R0, CharIsMoving
  jf R0, _count_moving_frames_reset
  
  ; when moving add 1 frame to the count
  mov R0, FramesMoving
  iadd R0, 1
  mov FramesMoving, R0
  jmp _count_moving_frames_end
  
  _count_moving_frames_reset:
  mov R0, 0
  mov FramesMoving, R0
  
  _count_moving_frames_end:
  pop R0
  ret


; determines the animation to use for the character
; based on its facing direction, and sets R0 to the
; first region of that animation
_choose_char_animation:
  
  ; first choose from vertical or horizontal
  mov R0, DirectionY
  jf R0, _choose_char_animation_horizontal
  
  ; choose from up or down
  ilt R0, 0
  jf R0, _choose_char_animation_down
  mov R0, FirstRegionCharacterUp
  jmp _choose_char_animation_end
  
  _choose_char_animation_down:
  mov R0, FirstRegionCharacterDown
  jmp _choose_char_animation_end
  
  ; left or right share the same animation
  _choose_char_animation_horizontal:
  mov R0, FirstRegionCharacterRight
  
  _choose_char_animation_end:
  ret

; ------------ MAIN PROGRAM ---------------

_program:
  
  ; PART 1: initialize the scene
  ; - - - - - - - - - - - - - - - -
  
  ; define all texture regions
  call _define_background_region
  
  mov R0, 1
  mov R1, 366
  out GPU_SelectedRegion, FirstRegionCharacterDown
  call _define_char_animation
  
  mov R0, 142
  mov R1, 366
  out GPU_SelectedRegion, FirstRegionCharacterUp
  call _define_char_animation
  
  mov R0, 283
  mov R1, 366
  out GPU_SelectedRegion, FirstRegionCharacterRight
  call _define_char_animation
  
  ; character starts at screen center
  mov R0, 320
  mov CharacterX, R0
  mov R0, 180
  mov CharacterY, R0
  
  ; character starts facing down
  mov R0, 0
  mov DirectionX, R0
  mov R0, 1
  mov DirectionY, R0
  
  ; character begins not moving
  mov R0, false
  mov R0, CharIsMoving
  mov R0, 0
  mov R0, FramesMoving
  
  ; PART 2: enter main loop to update every frame
  ; - - - - - - - - - - - - - - - - - - - - - - - -
  _main_loop_start:
    
    ; PART 2.1: update the scene
    ; - - - - - - - - - - - - - - -
    
    ; read inputs and update our character based on them
    call _update_char_direction
    call _move_character
    
    ; PART 2.2: draw the scene
    ; - - - - - - - - - - - - - - -
    
    ; draw background
    out GPU_SelectedTexture, TextureMars
    out GPU_SelectedRegion, RegionBackground
    out GPU_DrawingPointX, 0
    out GPU_DrawingPointY, 0
    out GPU_Command, GPUCommand_DrawRegion
    
    ; determine which character animation to draw
    ; (after this R0 is set to its first region ID)
    call _choose_char_animation
    
    ; determine which animation image to draw;
    ; we will count the number of frames the char
    ; has been moving and determine a region ID as:
    ; region = 1st animation region + ((moving frames / 8) mod 4
    ; (because animations loop their 4 images and
    ; we make each image be on screen for 8 frames)
    call _count_moving_frames
    mov R1, FramesMoving
    idiv R1, 8
    imod R1, 4
    iadd R0, R1
    out GPU_SelectedRegion, R0
    
    ; draw character at its current position
    mov R0, CharacterX
    out GPU_DrawingPointX, R0
    mov R0, CharacterY
    out GPU_DrawingPointY, R0
    
    ; check if direction is left
    mov R0, DirectionX
    ilt R0, 0
    jt R0, _draw_char_mirrored
    
    ; draw character with no mirroring
    out GPU_Command, GPUCommand_DrawRegion
    jmp _draw_char_end
    
    ; if facing left draw mirrored on X
    _draw_char_mirrored:
    out GPU_DrawingScaleX, -1.0   ; careful!! must be -1.0 (float), not -1 (integer)
    out GPU_DrawingScaleY,  1.0   ; careful!! must be  1.0 (float), not  1 (integer)
    out GPU_Command, GPUCommand_DrawRegionZoomed
    jmp _draw_char_end
    
    _draw_char_end:
    
    ; write character coordinates
    call _write_char_position
    
    ; we are done for the current frame
    ; so wait to regulate program speed
    wait
    
    ; we will never exit, continue forever
    jmp _main_loop_start
  