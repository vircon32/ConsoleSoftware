; names all ROM textures
; (bios is -1, cartridge uses consecutive values from 0)
%define TextureBios -1
%define TextureRobot 0

; regions for our only texture (can be any number from 0 to 4095)
; here we want all robot regions to be consecutive and following
; the same order as in the texture, so that we can easily define
; them in a loop; for ease of use, there is an unused extra
; region at the matrix center that; we should never draw it
%define RegionBackground 0
%define FirstRegionRobot 1

; top-left position of our robot regions in the texture
%define RobotRegionsX 645
%define RobotRegionsY 1

; ----------------------------------------------------------
; define our static background region
_define_background_region:
  
  ; select this texture and region
  out GPU_SelectedTexture, TextureRobot
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
; all our robot regions have the same size
; and same relative position of the hotspot
; so we define each of them them just from
; their top-left pixel, for brevity
; * receives 2 arguments:
;   > R0 = X coordinate of region's top-left pixel
;   > R1 = Y coordinate of region's top-left pixel
%define ReferenceX R0
%define ReferenceY R1

_define_robot_region:

  ; preserve these registers, since the parent
  ; subroutine to define all robot regions
  ; will keep operating on them
  push ReferenceX
  push ReferenceY
  
  ; define region top-left
  out GPU_RegionMinX, ReferenceX
  out GPU_RegionMinY, ReferenceY
  
  ; define region bottom-right
  iadd ReferenceX, 65
  iadd ReferenceY, 80
  out GPU_RegionMaxX, ReferenceX
  out GPU_RegionMaxY, ReferenceY
  
  ; define region hotspot
  isub ReferenceX, 34
  isub ReferenceY, 24
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
; In our texture, all robot regions are the same
; size and are laid out in a row; this subroutine
; makes use of that to iterate on X and define all
; 8 robot regions
%define TopLeftX R0
%define TopLeftY R1
%define RegionID R2

_define_all_robot_regions:
  
  ; select robot texture
  out GPU_SelectedTexture, TextureRobot
  
  ; initialize the loop on Y
  mov TopLeftY, RobotRegionsY
  mov RegionID, FirstRegionRobot
  
  _define_all_robot_regions_loop_y:
  
    ; initialize the loop on X
    mov TopLeftX, RobotRegionsX
    mov R4, RegionID  ; first region of this x loop
    
    _define_all_robot_regions_loop_x:
    
      ; define current region
      out GPU_SelectedRegion, RegionID
      call _define_robot_region
      
      ; for next iteration, increment region ID and advance to the right
      iadd RegionID, 1
      iadd TopLeftX, 67  ; width of robot regions + 1 pixel separation
      
      ; repeat loop on X if we did not define the 3 regions
      mov R3, RegionID
      isub R3, R4
      ilt R3, 3
      jt R3, _define_all_robot_regions_loop_x
    
    ; advance down on next row of regions
    iadd TopLeftY, 82  ; height of robot regions + 1 pixel separation
      
    ; repeat if we did not define the rows of regions
    mov R3, RegionID
    isub R3, FirstRegionRobot
    ilt R3, 9    ; 3 rows x 3 regions/row
    jt R3, _define_all_robot_regions_loop_y
    
  ret

%undef TopLeftX
%undef TopLeftY
%undef RegionID
