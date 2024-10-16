; THIS PROGRAM SHOWS A BALL BOUNCING ON SCREEN EDGES
; ====================================================

; execution begins here; since our main program is further
; down, jump to it or else the CPU will run unintended code
jmp _program

; include other source files
%include "Definitions.asm"     ; general definitions: textures, sounds, constants...
%include "Collisions.asm"      ; subroutines for ball vs boundary collisions

; ------------- AUXILIARY FUNCTIONS ---------------

; calculate cosine of a value; needed because there is
; no instruction for cosine; we calculate it from sine
; * receives 1 argument:
;   > R0 = value (float)
; * provides result in R0
_calculate_cos:

  ; cos( x ) = sqrt( 1 - sin(x)^2 )
  push R1        ; preserve R1
  sin R0         ; R0 = sin(x)
  fmul R0, R0    ; R0 = sin(x)^2
  mov R1, 1.0    ; R1 = 1
  fsub R1, R0    ; R1 = 1 - sin(x)^2
  mov R0, 0.5
  pow R1, R0     ; R1 = sqrt( 1 - sin(x)^2 ) = cos(x)
  mov R0, R1     ; return result in R0
  pop R1         ; restore R1
  ret

; simulate ball movement for 1 frame
_simulate_ball:

  ; BallCenterX += BallVelocityX
  mov R0, BallCenterX
  mov R1, BallVelocityX
  fadd R0, R1
  mov BallCenterX, R0
  
  ; BallCenterY += BallVelocityY
  mov R0, BallCenterY
  mov R1, BallVelocityY
  fadd R0, R1
  mov BallCenterY, R0
  
  ret

; ------------- MAIN PROGRAM ---------------

_program:
  
  ; define our texture regions
  call _define_region_ball
  
  ; initial ball position is screen center
  mov R0, 320.0
  mov BallCenterX, R0
  mov R0, 180.0
  mov BallCenterY, R0
  
  ; BallVelocityY = sin( BallAngle )
  mov R0, BallAngle
  sin R0
  mov R1, BallSpeed
  fmul R0, R1
  mov BallVelocityY, R0
  
  ; BallVelocityY = cos( BallAngle )
  mov R0, BallAngle
  call _calculate_cos
  fmul R0, R1
  mov BallVelocityX, R0
  
  ; enter main loop to update once per frame
  _main_loop_start:
    
    ; simulate motion for this frame
    call _simulate_ball
    
    ; collide ball with screen left
    mov R0, 0.0
    mov R1, false
    call _bounce_vertical
    
    ; collide ball with screen right
    mov R0, 640.0
    mov R1, true
    call _bounce_vertical
    
    ; collide ball with screen top
    mov R0, 0.0
    mov R1, false
    call _bounce_horizontal
    
    ; collide ball with screen bottom
    mov R0, 360.0
    mov R1, true
    call _bounce_horizontal
    
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
