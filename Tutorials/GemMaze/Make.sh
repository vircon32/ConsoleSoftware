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
compile Main.c -o obj/Tutorial-GemMaze.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Tutorial-GemMaze.asm -o obj/Tutorial-GemMaze.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon textures/TextureGemMaze.png -o obj/TextureGemMaze.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon sounds/SoundStart.wav          -o obj/SoundStart.vsnd          || abort_build
wav2vircon sounds/SoundCollectRedGem.wav  -o obj/SoundCollectRedGem.vsnd  || abort_build
wav2vircon sounds/SoundCollectBlueGem.wav -o obj/SoundCollectBlueGem.vsnd || abort_build
wav2vircon sounds/SoundCollectTime.wav    -o obj/SoundCollectTime.vsnd    || abort_build
wav2vircon sounds/SoundDeath.wav          -o obj/SoundDeath.vsnd          || abort_build
wav2vircon sounds/SoundTimeOut.wav        -o obj/SoundTimeOut.vsnd        || abort_build
wav2vircon sounds/SoundGoal.wav           -o obj/SoundGoal.vsnd           || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Tutorial-GemMaze.xml -o bin/Tutorial-GemMaze.v32 || abort_build

echo
echo BUILD SUCCESSFUL
