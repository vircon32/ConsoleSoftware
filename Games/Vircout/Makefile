# VIRCOUT 1.0
GAME         := vircout
ROM_NAME     := Vircout
V32LUA       ?= v32lua
ASSEMBLER    ?= assemble
PACKER       ?= packrom
TEXTURE_TOOL ?= png2vircon
SOUND_TOOL   ?= wav2vircon
PYTHON       ?= python3

DEBUG_FLAGS :=
ASM_DEBUG_FLAGS :=

SOURCES := $(GAME).lua \
           src/constants.lua \
           src/state.lua \
           src/graphics.lua \
           src/audio.lua \
           src/powerups.lua \
           src/effects.lua \
           src/blocks.lua \
           src/gameplay.lua \
           src/screens.lua \
           levels/level1.csv \
           levels/level2.csv \
           levels/level3.csv \
           levels/level4.csv \
           levels/level5.csv \
           levels/level6.csv \
           levels/level7.csv \
           levels/level8.csv

TEXTURES := assets/sprites.vtex assets/title.vtex assets/gameplay.vtex assets/ending.vtex
SOUNDS := audio/music_title.vsnd \
          audio/music_gameplay.vsnd \
          audio/music_ending.vsnd \
          audio/sfx_paddle.vsnd \
          audio/sfx_wall.vsnd \
          audio/sfx_normal.vsnd \
          audio/sfx_hard_hit.vsnd \
          audio/sfx_hard_break.vsnd \
          audio/sfx_metal.vsnd \
          audio/sfx_item.vsnd \
          audio/sfx_laser.vsnd \
          audio/sfx_ball_lost.vsnd \
          audio/sfx_level_clear.vsnd \
          audio/sfx_game_over.vsnd \
          audio/sfx_cheat.vsnd

.PHONY: all asm vbin vtex vsnd v32 debug regen-audio regen-graphics clean

all: v32

debug: DEBUG_FLAGS := -g
debug: ASM_DEBUG_FLAGS := -g program
debug: clean v32

regen-audio:
	$(PYTHON) tools/make_audio.py

regen-graphics:
	$(PYTHON) tools/make_graphics.py

asm: obj/$(GAME).asm

obj/$(GAME).asm: $(SOURCES) assets/sprites.png assets/title.png assets/gameplay.png assets/ending.png
	@mkdir -p obj bin
	$(V32LUA) $(DEBUG_FLAGS) -o obj/$(GAME).asm $(GAME).lua
	@# v32lua emits the cartridge XML next to the ASM output.
	@# Move it to the project root, matching the official demos.
	mv obj/$(GAME).xml .

vbin: obj/$(GAME).vbin

obj/$(GAME).vbin: obj/$(GAME).asm
	$(ASSEMBLER) $(ASM_DEBUG_FLAGS) -o obj/$(GAME).vbin obj/$(GAME).asm

vtex: $(TEXTURES)

assets/%.vtex: assets/%.png
	$(TEXTURE_TOOL) $< -o $@

vsnd: $(SOUNDS)

audio/%.vsnd: audio/%.wav
	$(SOUND_TOOL) -o $@ $<

v32: bin/$(ROM_NAME).v32

bin/$(ROM_NAME).v32: obj/$(GAME).vbin $(TEXTURES) $(SOUNDS)
	$(PACKER) $(GAME).xml -o bin/$(ROM_NAME).v32

clean:
	rm -f bin/*.v32 obj/* assets/*.vtex audio/*.vsnd *.xml
