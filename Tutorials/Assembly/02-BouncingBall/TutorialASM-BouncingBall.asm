; THIS PROGRAM SHOWS A BALL BOUNCING ON SCREEN EDGES
; ====================================================

; execution begins here; since our main program is further
; down, jump to it or else the CPU will run unintended code
jmp _program

; include other source files
%include "Definitions.asm"     ; general definitions: textures, sounds, constants...
%include "Collisions.asm"      ; subroutines for ball vs boundary collisions

; ------------- AUXILIARY FUNCTIONS ---------------

; simulate ball movement for 1 frame
_simulate_ball:

  ; BallCenterX += BallSpeedX
  mov R0, BallCenterX
  mov R1, BallSpeedX
  fadd R0, R1
  mov BallCenterX, R0
  
  ; BallCenterY += BallSpeedY
  mov R0, BallCenterY
  mov R1, BallSpeedY
  fadd R0, R1
  mov BallCenterY, R0
  
  ret

; ------------- MAIN PROGRAM ---------------

_program:
  
  ; define our texture regions
  call _define_region_ball
  
  ; set initial ball position
  mov R0, InitialCenterX
  mov BallCenterX, R0
  mov R0, InitialCenterY
  mov BallCenterY, R0
  
  ; set initial ball speed
  mov R0, InitialSpeedX
  mov BallSpeedX, R0
  mov R0, InitialSpeedY
  mov BallSpeedY, R0
  
  ; enter main loop to update once per frame
  _main_loop_start:
    
    ; simulate motion for this frame
    call _simulate_ball
    
    ; collide ball with all screen edges
    mov R0, 0.0
    call _bounce_left
    
    mov R0, 640.0
    call _bounce_right
    
    mov R0, 0.0
    call _bounce_top
    
    mov R0, 360.0
    call _bounce_bottom
    
    ; clear screen to brown
    out GPU_ClearColor, 0xFF204080
    out GPU_Command, GPUCommand_ClearScreen
    
    ; draw ball at its current center position
    ; (remember we have to convert its X and Y to integers)
    out GPU_SelectedTexture, TextureBall
    out GPU_SelectedRegion, RegionBall
    mov R0, BallCenterX
    cfi R0
    out GPU_DrawingPointX, R0
    mov R0, BallCenterY
    cfi R0
    out GPU_DrawingPointY, R0
    out GPU_Command, GPUCommand_DrawRegion
    
    ; we are done for the current frame
    ; so wait to regulate program speed
    wait
    
    ; we will never exit, continue forever
    jmp _main_loop_start
