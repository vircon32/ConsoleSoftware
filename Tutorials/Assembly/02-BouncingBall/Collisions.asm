; auxiliary subroutine that plays our only sound
_play_bounce_sound:
  out SPU_SelectedChannel, 0
  out SPU_ChannelAssignedSound, SoundBounce
  out SPU_Command, SPUCommand_PlaySelectedChannel
  ret
  
; ----------------------------------------------------------
; bounce the ball when hitting a left boundary
; * receives 1 argument:
;   > R0 = X coordinate of the boundary (float)
%define BoundaryMinX R0

_bounce_left:
  
  ; minimum center X = BoundaryMinX + BallRadius
  fadd BoundaryMinX, BallRadius
  
  ; if not BallCenterX <= minimum X, there is no bounce
  mov R1, BallCenterX
  fle R1, BoundaryMinX
  jf R1, _bounce_left_end
  
  ; limit X to the boundary
  mov BallCenterX, BoundaryMinX
  
  ; reflect velocity on X by changing sign
  mov R1, BallSpeedX
  fsgn R1
  mov BallSpeedX, R1
  
  ; play the sound
  call _play_bounce_sound
  
  _bounce_left_end:
  ret

%undef BoundaryMinX

; ----------------------------------------------------------
; bounce the ball when hitting a right boundary
; * receives 1 argument:
;   > R0 = X coordinate of the boundary (float)
%define BoundaryMaxX R0

_bounce_right:
  
  ; maximum center X = BoundaryMaxX - BallRadius
  fsub BoundaryMaxX, BallRadius
  
  ; if not BallCenterX >= maximum X, there is no bounce
  mov R1, BallCenterX
  fge R1, BoundaryMaxX
  jf R1, _bounce_right_end
  
  ; limit X to the boundary
  mov BallCenterX, BoundaryMaxX
  
  ; reflect velocity on X by changing sign
  mov R1, BallSpeedX
  fsgn R1
  mov BallSpeedX, R1
  
  ; play the sound
  call _play_bounce_sound
  
  _bounce_right_end:
  ret

%undef BoundaryMaxX

; ----------------------------------------------------------
; bounce the ball when hitting a top boundary
; * receives 1 argument:
;   > R0 = Y coordinate of the boundary (float)
%define BoundaryMinY R0

_bounce_top:
  
  ; minimum center Y = BoundaryMinY + BallRadius
  fadd BoundaryMinY, BallRadius
  
  ; if not BallCenterY <= minimum Y, there is no bounce
  mov R1, BallCenterY
  fle R1, BoundaryMinY
  jf R1, _bounce_top_end
  
  ; limit Y to the boundary
  mov BallCenterY, BoundaryMinY
  
  ; reflect velocity on Y by changing sign
  mov R1, BallSpeedY
  fsgn R1
  mov BallSpeedY, R1
  
  ; play the sound
  call _play_bounce_sound
  
  _bounce_top_end:
  ret

%undef BoundaryMinY

; ----------------------------------------------------------
; bounce the ball when hitting a bottom boundary
; * receives 1 argument:
;   > R0 = Y coordinate of the boundary (float)
%define BoundaryMaxY R0

_bounce_bottom:
  
  ; maximum center Y = BoundaryMaxY - BallRadius
  fsub BoundaryMaxY, BallRadius
  
  ; if not BallCenterY >= maximum Y, there is no bounce
  mov R1, BallCenterY
  fge R1, BoundaryMaxY
  jf R1, _bounce_bottom_end
  
  ; limit Y to the boundary
  mov BallCenterY, BoundaryMaxY
  
  ; reflect velocity on Y by changing sign
  mov R1, BallSpeedY
  fsgn R1
  mov BallSpeedY, R1
  
  ; play the sound
  call _play_bounce_sound
  
  _bounce_bottom_end:
  ret

%undef BoundaryMaxY
