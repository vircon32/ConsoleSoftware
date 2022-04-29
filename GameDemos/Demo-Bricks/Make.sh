#!/bin/bash

# define an abort function to call on error
abort_build()
{
    echo
    echo BUILD FAILED
    exit 1
}

# create obj and bin folders if non exiting, since
# the development tools will not create them themselves
mkdir -p obj
mkdir -p bin

echo
echo Import embedded data files
echo --------------------------
tiled2vircon maps/TileMapBricks.tmx  -o obj/ || abort_build
tiled2vircon maps/TileMapScenery.tmx -o obj/ || abort_build

echo
echo Compile the C code
echo --------------------------
compile Demo-Bricks.c -o obj/Demo-Bricks.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Demo-Bricks.asm -o obj/Demo-Bricks.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-Bricks.png -o obj/Texture-Bricks.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon Music-Gameplay.wav -o obj/Music-Gameplay.vsnd || abort_build

wav2vircon Sound-PaddleBounce.wav      -o obj/Sound-PaddleBounce.vsnd      || abort_build
wav2vircon Sound-WallBounce.wav        -o obj/Sound-WallBounce.vsnd        || abort_build
wav2vircon Sound-RegularBlock.wav      -o obj/Sound-RegularBlock.vsnd      || abort_build
wav2vircon Sound-NonBreakableBlock.wav -o obj/Sound-NonBreakableBlock.vsnd || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Demo-Bricks.xml -o bin/Demo-Bricks.v32 || abort_build

echo
echo BUILD SUCCESSFUL
