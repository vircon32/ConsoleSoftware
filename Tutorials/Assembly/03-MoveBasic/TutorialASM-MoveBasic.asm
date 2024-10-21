; THIS PROGRAM SHOWS A ROBOT YOU CAN MOVE WITH THE D-PAD
; ========================================================

; execution begins here; since our main program is further
; down, jump to it or else the CPU will run unintended code
jmp _program

; ------------ DECLARATIONS ---------------

; define constants
%define RobotSpeed 2       ; robot moves at 2 pixels/frame = 120 pixels/second

; define variables with their addresses in memory
; (all of them are integers)
%define RobotX     [ 0 ]   ; robot's X coordinate on screen
%define RobotY     [ 1 ]   ; robot's Y coordinate on screen
%define DirectionX [ 2 ]   ; robot's facing direction on X: -1 = left, 0 = neutral, +1 = right
%define DirectionY [ 3 ]   ; robot's facing direction on Y: -1 = up  , 0 = neutral, +1 = down
%define PressedX   [ 4 ]   ; gamepad's pressed direction on X: -1 = left, 0 = neutral, +1 = right
%define PressedY   [ 5 ]   ; gamepad's pressed direction on Y: -1 = up  , 0 = neutral, +1 = down

; ------------ INCLUDES ---------------

; include other source files
%include "DefineRegions.asm"     ; subroutines to define robot regions from the texture
%include "WritePosition.asm"     ; subroutines to write robot position on screen with text

; ------------ SUBROUTINES ---------------

; read gamepad direction and update our variables
_read_direction:
  
  ; initialize pressed direction to neutral
  mov R0, 0
  mov PressedX, R0
  mov PressedY, R0
  
  ; if left is pressed PressedX = -1
  _read_direction_left:
  
    in R0, INP_GamepadLeft
    igt R0, 0
    jf R0, _read_direction_right
    mov R0, -1
    mov PressedX, R0
  
  ; if right is pressed PressedX = 1
  _read_direction_right:
  
    in R0, INP_GamepadRight
    igt R0, 0
    jf R0, _read_direction_up
    mov R0, 1
    mov PressedX, R0
  
  ; if up is pressed PressedY = -1
  _read_direction_up:
  
    in R0, INP_GamepadUp
    igt R0, 0
    jf R0, _read_direction_down
    mov R0, -1
    mov PressedY, R0
  
  ; if down is pressed PressedY = 1
  _read_direction_down:
  
    in R0, INP_GamepadDown
    igt R0, 0
    jf R0, _read_direction_end
    mov R0, 1
    mov PressedY, R0
  
  _read_direction_end:
  ret


; update current facing direction for the robot
_update_robot_direction:
  
  ; read our variables
  mov R0, PressedX
  mov R1, PressedY
  
  ; set robot direction only if some direction was pressed
  mov R2, R0
  jt R2, _update_robot_direction_write
  mov R2, R1
  jt R2, _update_robot_direction_write
  jmp _update_robot_direction_end
  
  ; now update robot direction
  _update_robot_direction_write:
  mov DirectionX, R0
  mov DirectionY, R1
  
  _update_robot_direction_end:
  ret


; simulate robot's movement for 1 frame
_move_robot:
  
  ; RobotX += RobotSpeed * PressedX
  mov R1, PressedX
  imul R1, RobotSpeed
  mov R0, RobotX
  iadd R0, R1
  mov RobotX, R0
  
  ; RobotY += RobotSpeed * PressedX
  mov R1, PressedY
  imul R1, RobotSpeed
  mov R0, RobotY
  iadd R0, R1
  mov RobotY, R0
  
  ret

; ------------ MAIN PROGRAM ---------------

_program:
  
  ; PART 1: initialize the scene
  ; - - - - - - - - - - - - - - - -
  
  ; define all texture regions
  call _define_background_region
  call _define_all_robot_regions
  
  ; robot starts at screen center
  mov R0, 320
  mov RobotX, R0
  mov R0, 180
  mov RobotY, R0
  
  ; robot starts facing down
  mov R0, 0
  mov DirectionX, R0
  mov R0, 1
  mov DirectionY, R0
  
  ; PART 2: enter main loop to update every frame
  ; - - - - - - - - - - - - - - - - - - - - - - - -
  _main_loop_start:
    
    ; PART 2.1: update the scene
    ; - - - - - - - - - - - - - - -
    
    ; read inputs and update our robot based on them
    call _read_direction
    call _update_robot_direction
    call _move_robot
    
    ; PART 2.2: draw the scene
    ; - - - - - - - - - - - - - - -
    
    ; draw background
    out GPU_SelectedTexture, TextureRobot
    out GPU_SelectedRegion, RegionBackground
    out GPU_DrawingPointX, 0
    out GPU_DrawingPointY, 0
    out GPU_Command, GPUCommand_DrawRegion
    
    ; determine robot region to draw as:
    ; FirstRegionRobot + 3 * (DirectionY+1) + (DirectionX+1)
    mov R0, DirectionX
    iadd R0, 1
    
    mov R1, DirectionY
    iadd R1, 1
    imul R1, 3
    
    iadd R0, R1
    iadd R0, FirstRegionRobot
    out GPU_SelectedRegion, R0
    
    ; draw robot at its current position
    mov R0, RobotX
    out GPU_DrawingPointX, R0
    mov R0, RobotY
    out GPU_DrawingPointY, R0
    out GPU_Command, GPUCommand_DrawRegion
    
    ; write robot coordinates
    call _write_robot_position
    
    ; we are done for the current frame
    ; so wait to regulate program speed
    wait
    
    ; we will never exit, continue forever
    jmp _main_loop_start
  