; --------------------------------------------------  
; just a fast, safe way to enable character movement
_enable_char_movement:
  push R0
  mov R0, true
  mov CharIsMoving, R0
  pop R0
  ret

  
; --------------------------------------------------  
; analyzes the input from a gamepad direction; if
; pressed it will enable char movement; it returns
; true if direction was just pressed on this frame
; * receives 1 argument:
;   > R0 = direction value to analyze
; * returns result in R0
%define DirectionValue R0

_analyze_pressed_direction:
 
  push R1
  
  ; enable char movement if direction is pressed
  mov R1, DirectionValue
  igt R1, 0
  jf R1, _analyze_pressed_direction_unpressed
  call _enable_char_movement
  
  ; return true if it was pressed this same frame
  ieq DirectionValue, 1
  jmp _analyze_pressed_direction_end
  
  ; otherwise return false
  _analyze_pressed_direction_unpressed:
  mov DirectionValue, false
  
  _analyze_pressed_direction_end:
  pop R1
  ret

%undef DirectionValue
 
 
; --------------------------------------------------  
; reads gamepad direction and updates current
; character facing direction and moving flag
_update_char_direction:
  
  ; initialize character to not moving
  ; unless some direction is pressed
  mov R0, false
  mov CharIsMoving, R0
  
  ; set left direction if it was pressed this frame
  _read_direction_left:
  
    in R0, INP_GamepadLeft
    call _analyze_pressed_direction
    jf R0, _read_direction_right
    mov R0, -1
    mov DirectionX, R0
    mov R0, 0
    mov DirectionY, R0
    jmp _read_direction_end
    
  ; set right direction if it was pressed this frame
  _read_direction_right:
  
    in R0, INP_GamepadRight
    call _analyze_pressed_direction
    jf R0, _read_direction_up
    mov R0, 1
    mov DirectionX, R0
    mov R0, 0
    mov DirectionY, R0
    jmp _read_direction_end
  
  ; set up direction if it was pressed this frame
  _read_direction_up:
  
    in R0, INP_GamepadUp
    call _analyze_pressed_direction
    jf R0, _read_direction_down
    mov R0, 0
    mov DirectionX, R0
    mov R0, -1
    mov DirectionY, R0
    jmp _read_direction_end
  
  ; set down direction if it was pressed this frame
  _read_direction_down:
  
    in R0, INP_GamepadDown
    call _analyze_pressed_direction
    jf R0, _read_direction_end
    mov R0, 0
    mov DirectionX, R0
    mov R0, 1
    mov DirectionY, R0
    
  ; if we jumped directly here, the last
  ; character facing direction was not
  ; changed but the character is not moving
  _read_direction_end:
  ret

  
; --------------------------------------------------  
; simulates character's movement for 1 frame
_move_character:
  
  ; do nothing if character is not moving
  mov R0, CharIsMoving
  jf R0, _move_character_end
  
  ; CharacterX += CharacterSpeed * DirectionX
  mov R1, DirectionX
  imul R1, CharacterSpeed
  mov R0, CharacterX
  iadd R0, R1
  mov CharacterX, R0
  
  ; CharacterY += CharacterSpeed * DirectionY
  mov R1, DirectionY
  imul R1, CharacterSpeed
  mov R0, CharacterY
  iadd R0, R1
  mov CharacterY, R0
  
  _move_character_end:
  ret
