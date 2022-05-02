define CartridgeROMFirstAddress 0x20000000

; ------------------------------------
__hardware_error_vector:
  call __error_handler
  wait
  hlt

; ------------------------------------
__main:
  
  ; check if there is a cartridge inserted
  in R0, CAR_Connected
  ieq R0, 0
  jf R0, __run_cartridge
  
  ; if not, just show a red screen and stop
  out GPU_ClearColor, 0xFF0000FF
  out GPU_Command, GPUCommand_ClearScreen
  wait
  hlt
  
  ; now safely jump to the start of cartridge
__run_cartridge:
  jmp CartridgeROMFirstAddress
  

; ------------------------------------
__error_handler:

  ; just show a blue screen and stop
  out GPU_ClearColor, 0xFFFF0000
  out GPU_Command, GPUCommand_ClearScreen
  ret
