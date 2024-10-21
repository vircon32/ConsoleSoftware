; name all ROM textures
; (bios is -1, cartridge uses consecutive values from 0)
%define TextureBios -1
%define TextureMars  0

; name all regions for our only texture (each can be any number
; from 0 to 4095); every character animation are 4 consecutive
; regions that are the same size and loop in a cycle
%define RegionBackground          0
%define FirstRegionCharacterDown  1
%define FirstRegionCharacterUp    5
%define FirstRegionCharacterRight 9

; ----------------------------------------------------------
; define our static background region
_define_background_region:
  
  ; select this texture and region
  out GPU_SelectedTexture, TextureMars
  out GPU_SelectedRegion, RegionBackground
  
  ; define region top-left
  out GPU_RegionMinX, 0
  out GPU_RegionMinY, 0
  
  ; define region bottom-right
  out GPU_RegionMaxX, 639  ; screen width - 1
  out GPU_RegionMaxY, 359  ; screen height - 1
  
  ; define region hotspot at top-left
  ; so that we can just draw it at (0,0)
  out GPU_RegionHotSpotX, 0
  out GPU_RegionHotSpotY, 0
  
  ret

; ----------------------------------------------------------
; all our character regions have the same size
; and same relative position of the hotspot,
; so we define each of them just from their
; top-left pixel, for brevity
; * receives 2 arguments:
;   > R0 = X coordinate of first region's top-left pixel
;   > R1 = Y coordinate of first region's top-left pixel
%define ReferenceX R0
%define ReferenceY R1

_define_char_region:

  ; preserve these registers, since the parent
  ; subroutine to define character animations
  ; will keep operating on them
  push ReferenceX
  push ReferenceY
  
  ; define region top-left
  out GPU_RegionMinX, ReferenceX
  out GPU_RegionMinY, ReferenceY
  
  ; define region bottom-right
  iadd ReferenceX, 32
  iadd ReferenceY, 73
  out GPU_RegionMaxX, ReferenceX
  out GPU_RegionMaxY, ReferenceY
  
  ; define region hotspot
  isub ReferenceX, 16
  isub ReferenceY, 5
  out GPU_RegionHotSpotX, ReferenceX
  out GPU_RegionHotSpotY, ReferenceY
  
  ; restore previous register values from
  ; the stack (always in reverse order!!)
  pop ReferenceY
  pop ReferenceX
  ret

%undef ReferenceX
%undef ReferenceY

; ----------------------------------------------------------
; In our texture, all character animations have
; their 4 regions laid out in a row; this subroutine
; makes use of that to iterate on X and define all
; 4 regions consecutively
; * receives 2 arguments:
;   > R0 = X coordinate of first region's top-left pixel
;   > R1 = Y coordinate of first region's top-left pixel
%define TopLeftX R0
%define TopLeftY R1

; additionally we define these internal roles for
; registers in the subroutine:
%define RegionID R2
%define InitialRegionID R3

_define_char_animation:
  
  ; select scenery texture
  out GPU_SelectedTexture, TextureMars
  
  ; initialize the loop
  in InitialRegionID, GPU_SelectedRegion
  mov RegionID, InitialRegionID
  
  _define_char_animation_loop:
  
    ; define current region
    out GPU_SelectedRegion, RegionID
    call _define_char_region
    
    ; for next iteration, increment region ID and advance to the right
    iadd RegionID, 1
    iadd TopLeftX, 34  ; width of character regions + 1 pixel separation
    
    ; repeat loop on X if we did not define the 4 regions
    mov R4, RegionID
    isub R4, InitialRegionID
    ilt R4, 4
    jt R4, _define_char_animation_loop
    
  ret

%undef TopLeftX
%undef TopLeftY
%undef RegionID
%undef InitialRegionID
