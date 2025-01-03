; names all ROM textures
; (consecutive values from 0)
%define TextureBall 0

; names for all ROM sounds
; (consecutive values from 0)
%define SoundBounce 0

; regions for our only texture
; (can be any number from 0 to 4095)
%define RegionBall 0

; definitions for our physics
; for these definitions use floats, not integers!
; we will use them in float operations, remember
; there are no automatic conversions as in C and
; they would get interpreted incorrectly otherwise,
; producing garbage values
%define BallRadius       30.0
%define InitialCenterX  320.0
%define InitialCenterY  180.0
%define InitialSpeedX     2.0
%define InitialSpeedY    -2.0

; store global variables in these RAM positions
; (all are interpreted as float)
%define BallCenterX  [ 0 ]
%define BallCenterY  [ 1 ]
%define BallSpeedX   [ 2 ]
%define BallSpeedY   [ 3 ]

; define region for the ball, with hotspot at center
_define_region_ball:
  out GPU_SelectedTexture, TextureBall
  out GPU_SelectedRegion, RegionBall
  out GPU_RegionMinX, 0
  out GPU_RegionMinY, 0
  out GPU_RegionMaxX, 60
  out GPU_RegionMaxY, 60
  out GPU_RegionHotspotX, 30
  out GPU_RegionHotspotY, 30
  ret
