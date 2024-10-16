; auxiliary subroutine that plays our only sound
_play_bounce_sound:
  out SPU_SelectedChannel, 0
  out SPU_ChannelAssignedSound, SoundBounce
  out SPU_Command, SPUCommand_PlaySelectedChannel
  ret
  
; ----------------------------------------------------------
; bounce the ball when hitting a vertical boundary
; * receives 2 arguments:
;   > R0 = X coordinate of the boundary (float)
;   > R1 = Boundary location (boolean): false for left, true for right
%define BoundaryX R0
%define IsRightBoundary R1

_bounce_vertical:
  
  ; distinguish left or right boundary;
  ; after this check we can freely reuse R1
  jf IsRightBoundary, _bounce_vertical_left
  
  ; CASE 1: right boundary (there is a maximum x)
  
    ; maximum center X = BoundaryX - BallRadius
    fsub BoundaryX, BallRadius
    
    ; if not BallCenterX >= maximum X, there is no bounce
    mov R1, BallCenterX
    fge R1, BoundaryX
    jf R1, _bounce_vertical_end
    jmp _bounce_vertical_response   ; jump to not run the left boundary case
  
  ; CASE 2: left boundary (there is a minimum x)
  _bounce_vertical_left:
  
    ; minimum center X = BoundaryX + BallRadius
    fadd BoundaryX, BallRadius
    
    ; if not BallCenterX <= minimum X, there is no bounce
    mov R1, BallCenterX
    fle R1, BoundaryX
    jf R1, _bounce_vertical_end
    
  ; once a bounce is detected, response is common to both cases
  _bounce_vertical_response:
    
    ; limit X to the boundary
    mov BallCenterX, BoundaryX
    
    ; reflect velocity on X by changing sign
    mov R1, BallVelocityX
    fsgn R1
    mov BallVelocityX, R1
    
    ; play the sound
    call _play_bounce_sound
  
  _bounce_vertical_end:
    ret

%undef BoundaryX
%undef IsRightBoundary

; ----------------------------------------------------------
; bounce the ball when hitting a horizontal boundary
; * receives 2 arguments:
;   > R0 = Y coordinate of the boundary (float)
;   > R1 = Boundary location (boolean): false for top, true for bottom
%define BoundaryY R0
%define IsBottomBoundary R1

_bounce_horizontal:
  
  ; distinguish left or right boundary;
  ; after this check we can freely reuse R1
  jf IsBottomBoundary, _bounce_horizontal_top
  
  ; CASE 1: bottom boundary (there is a maximum y)
  
    ; maximum center Y = BoundaryY - BallRadius
    fsub BoundaryY, BallRadius
    
    ; if not BallCenterY >= maximum Y, there is no bounce
    mov R1, BallCenterY
    fge R1, BoundaryY
    jf R1, _bounce_horizontal_end
    jmp _bounce_horizontal_response   ; jump to not run the left boundary case
  
  ; CASE 2: left boundary (there is a minimum y)
  _bounce_horizontal_top:
  
    ; minimum center Y = BoundaryY + BallRadius
    fadd BoundaryY, BallRadius
    
    ; if not BallCenterY <= minimum Y, there is no bounce
    mov R1, BallCenterY
    fle R1, BoundaryY
    jf R1, _bounce_horizontal_end
    
  ; once a bounce is detected, response is common to both cases
  _bounce_horizontal_response:
    
    ; limit Y to the boundary
    mov BallCenterY, BoundaryY
    
    ; reflect velocity on Y by changing sign
    mov R1, BallVelocityY
    fsgn R1
    mov BallVelocityY, R1
    
    ; play the sound
    call _play_bounce_sound
  
  _bounce_horizontal_end:
    ret

%undef BoundaryY
%undef IsBottomBoundary
