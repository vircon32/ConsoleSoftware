; ----------------------------------------------------------
; writes a text string in current drawing position,
; making that position the top-left of the text;
; it uses the bios text font and has no support for
; display multiline text.
; * receives 1 argument:
;   > R0: address of string to write, i.e. address of first character
_write_string:
%define CurrentCharAddress R0
  
  ; preserve used registers other than the argument R0
  ; (since the caller may be using them itself)
  push R1
  push R2
  
  ; select the bios texture to use its text font
  out GPU_SelectedTexture, TextureBios
  
  _write_string_loop:
    
    ; write current character
    mov R1, [CurrentCharAddress]
    out GPU_SelectedRegion, R1
    out GPU_Command, GPUCommand_DrawRegion
    
    ; advance to the right (bios font width is 10 pixels)
    in R2, GPU_DrawingPointX
    iadd R2, 10
    out GPU_DrawingPointX, R2
    
    ; advance to the next string position
    iadd CurrentCharAddress, 1
    
    ; if current position is 0, string ended
    ; otherwise continue writing characters
    mov R2, [CurrentCharAddress]
    jt R2, _write_string_loop
  
  ; restore saved registers in reverse order
  pop R2
  pop R1
  
  ret
  
%undef CurrentCharAddress

; ----------------------------------------------------------
; writes a number at the current drawing position,
; making that position the top-left of the text;
; this version supports the full range of 32-bit
; integers, and prints only the needed digits with
; no initial zeros unlike the version in move basic
; * receives 1 argument:
;   > R0 = number to write (integer)
%define Number R0

; additionally we define these internal roles for
; registers in the subroutine:
%define DrawingX R1
%define DigitBase R2
%define WriteZeroes R3
  
_write_number:

  ; preserve used registers other than the argument R0
  ; (since the caller may be using them itself)
  push R1
  push R2
  push R3
  push R4
  
  ; select the bios texture to use its text font
  out GPU_SelectedTexture, TextureBios
  
  ; treat number 0 separately: our algorithm won't
  ; detect any digit fot this case, but we still
  ; have to write the '0' for units
  jt Number, _write_number_initialize
  out GPU_SelectedRegion, '0'
  out GPU_Command, GPUCommand_DrawRegion
  jmp _write_number_end
  
  _write_number_initialize:
  
  ; initialize values before loop; set our initial base
  ; to the largest possible digit in a 32-bit integer;
  ; we won't write zeroes until a non-zero digit is found
  in DrawingX, GPU_DrawingPointX
  mov DigitBase, 1000000000
  mov WriteZeroes, false
  
  ; before entering the loop, place the minus sign if negative
  mov R4, Number
  ilt R4, 0
  jf R4, _write_number_loop
  
  ; write minus sign
  out GPU_SelectedRegion, '-'
  out GPU_Command, GPUCommand_DrawRegion
  
  ; advance to the right (bios font width is 10 pixels)
  iadd DrawingX, 10
  out GPU_DrawingPointX, DrawingX
  
  ; consider the positive value from now on
  isgn Number
  
  _write_number_loop:
  
    ; determine current digit = (Number/Base) mod 10
    mov R4, Number
    idiv R4, DigitBase
    imod R4, 10
    
    ; if we find a non-zero digit, enable printing next zeroes
    jf R4, _write_number_digit_zero
    mov WriteZeroes, true
    
    ; for zeroes check it we should print them
    _write_number_digit_zero:
    jf WriteZeroes, _write_number_next_digit
    
    _write_number_draw:
    
    ; draw character region for current digit
    iadd R4, '0'
    out GPU_SelectedRegion, R4
    out GPU_Command, GPUCommand_DrawRegion
    
    ; advance to the right (bios font width is 10 pixels)
    iadd DrawingX, 10
    out GPU_DrawingPointX, DrawingX
    
    _write_number_next_digit:
    
    ; reduce the base (i.e. go to next digit)
    idiv DigitBase, 10
    
    ; keep going until we go past 1 (base=0)
    jt DigitBase, _write_number_loop
  
  _write_number_end:
  
  ; restore saved registers in reverse order
  pop R4
  pop R3
  pop R2
  pop R1
  
  ret

%undef Number
%undef DrawingX
%undef DigitBase
%undef WriteZeroes

; ----------------------------------
_title_position_x:
  string "Position X = "

_title_position_y:
  string "Position Y = "

; ----------------------------------------------------------
; writes current character X and Y on the screen,
; at the top-left corner; uses the bios font and
; adds a previous title for each variable
_write_char_position:

  ; write initial text for character X
  out GPU_DrawingPointX, 10
  out GPU_DrawingPointY, 10
  mov R0, _title_position_x       ; R0 = start address of the text (points to it, rather than loading the first value)
  call _write_string
  
  ; write character X coordinate
  mov R0, CharacterX
  call _write_number
  
  ; write initial text for character Y
  out GPU_DrawingPointX, 10
  out GPU_DrawingPointY, 30
  mov R0, _title_position_y
  call _write_string
  
  ; write character Y coordinate
  mov R0, CharacterY
  call _write_number
  
  ret
