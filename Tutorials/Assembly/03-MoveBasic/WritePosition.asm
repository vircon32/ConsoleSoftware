; ----------------------------------------------------------
; writes a number at the current drawing position,
; making that position the top-left of the text;
; this is a simplified version that will always
; write 3 digits (000-999), but can also display
; negative numbers in that same range
; * receives 1 argument:
;   > R0 = number to write (integer)
%define Number R0
  
_write_number:

  ; preserve used registers other than the argument R0
  ; (since the caller may be using them itself)
  push R1
  push R2
  push R3
  
  ; select the bios texture to use its text font
  out GPU_SelectedTexture, TextureBios
  
  ; initialize values before loop
  in R1, GPU_DrawingPointX   ; R1 is the current X position to draw
  mov R2, 100                ; R2 is our base for the current digit
  
  ; before entering the loop, place the minus sign if negative
  mov R3, Number
  ilt R3, 0
  jf R3, _write_number_loop
  
  ; write minus sign
  out GPU_SelectedRegion, '-'
  out GPU_Command, GPUCommand_DrawRegion
  
  ; advance to the right (bios font width is 10 pixels)
  iadd R1, 10
  out GPU_DrawingPointX, R1
  
  ; consider the positive value from now on
  isgn Number
  
  _write_number_loop:
  
    ; draw character region for current digit
    ; (character = '0' + (Number/Base) mod 10)
    mov R3, Number
    idiv R3, R2
    imod R3, 10
    iadd R3, '0'
    out GPU_SelectedRegion, R3
    out GPU_Command, GPUCommand_DrawRegion
    
    ; advance to the right (bios font width is 10 pixels)
    iadd R1, 10
    out GPU_DrawingPointX, R1
    
    ; reduce the base (i.e. go to next digit)
    idiv R2, 10
    
    ; keep going until we go past 1 (base=0)
    jt R2, _write_number_loop
  
  ; restore saved registers in reverse order
  pop R3
  pop R2
  pop R1
  
  ret

%undef Number

; ----------------------------------------------------------
; writes current robot X and Y on the screen,
; at the top-left corner; uses the bios font
_write_robot_position:

  ; write robot X coordinate
  out GPU_DrawingPointX, 10
  out GPU_DrawingPointY, 10
  mov R0, RobotX
  call _write_number
  
  ; write robot U coordinate
  out GPU_DrawingPointX, 10
  out GPU_DrawingPointY, 30
  mov R0, RobotY
  call _write_number
  
  ret
