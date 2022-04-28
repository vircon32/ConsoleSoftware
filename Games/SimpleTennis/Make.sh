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
echo Compile the C code
echo --------------------------
compile Main.c -o obj/SimpleTennis.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/SimpleTennis.asm -o obj/SimpleTennis.vprg || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-SimpleTennis.png -o obj/Texture-SimpleTennis.vtex || abort_build
png2vircon TextureFont11x16.png -o obj/TextureFont11x16.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon SoundHitPaddle.wav -o obj/SoundHitPaddle.vsnd || abort_build
wav2vircon SoundHitWall.wav -o obj/SoundHitWall.vsnd || abort_build
wav2vircon SoundScore.wav -o obj/SoundScore.vsnd || abort_build
wav2vircon SoundGameEnd.wav -o obj/SoundGameEnd.vsnd || abort_build
wav2vircon SoundMenu.wav -o obj/SoundMenu.vsnd || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom SimpleTennis.xml -o bin/SimpleTennis.v32 || abort_build

echo
echo BUILD SUCCESSFUL
