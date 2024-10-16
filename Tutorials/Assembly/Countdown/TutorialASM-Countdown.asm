; THIS PROGRAM SHOWS A SIMPLE 5-SECOND COUNTDOWN
; ===============================================

; execution begins here; since our main program is further
; down, jump to it or else the CPU will run unintended code
JMP _program

; ------------ DECLARATIONS ---------------
; define constants
%define StartSeconds 5               ; start countdown at 5
%define FramesPerSecond 60           ; this console works at constant 60 fps
%define Seconds R0                   ; register R0 will store the current seconds

; declare data
_digit_characters:
  integer '0','1','2','3','4','5'    ; sequence of characters for the countdown

; ------------ SUBROUTINES ---------------
_wait_1_second:
  MOV R1, FramesPerSecond     ; R1 = frames remaining (don't use R0!)
_start_wait_loop:
  WAIT                        ; wait for end of frame
  ISUB R1, 1                  ; now there is 1 less frame to wait
  MOV R2, R1                  ; comparing overwrites, so make a copy
  IGT R2, 0                   ; are frames remaining (i.e. R2) > 0 ? ...
  JT R2, _start_wait_loop     ; ... if so, keep in the loop
  RET                         ; otherwise we are done, go back

_print_seconds:
  OUT GPU_Command, GPUCommand_ClearScreen   ; clear screen (default color is black)
  MOV R1, _digit_characters                 ; R1 points to '0' (don't use R0!)
  IADD R1, Seconds                          ; R1 points to the character for Seconds
  MOV R1, [R1]                              ; load in R1 the pointed value in memory
  OUT GPU_SelectedRegion, R1                ; select our character in BIOS texture
  OUT GPU_Command, GPUCommand_DrawRegion    ; draw it at the current drawing point
  RET                                       ; we are done, go back

; ------------ MAIN PROGRAM ---------------
_program:
  MOV Seconds, StartSeconds    ; initialize our seconds count
  OUT GPU_DrawingPointX, 320   ; set screen X to draw our numbers
  OUT GPU_DrawingPointY, 180   ; set screen Y to draw our numbers
_start_main_loop:
  CALL _print_seconds          ; call subroutine to print seconds
  CALL _wait_1_second          ; call subroutine to wait 1 second
  ISUB Seconds, 1              ; now there is 1 less second remaining
  MOV R1, Seconds              ; comparing overwrites, so make a copy
  IGE R1, 0                    ; are seconds remaining (i.e. R1) >= 0? ...
  JT R1, _start_main_loop      ; ... if so, keep in the loop
  HLT                          ; otherwise end program, countdown ended
