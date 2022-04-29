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
compile Test-RandomNumbers.c -o obj/Test-RandomNumbers.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Test-RandomNumbers.asm -o obj/Test-RandomNumbers.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-DiceStats.png -o obj/Texture-DiceStats.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon Sound-DiceRoll.wav -o obj/Sound-DiceRoll.vsnd || abort_build
wav2vircon Sound-PageComplete.wav -o obj/Sound-PageComplete.vsnd || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Test-RandomNumbers.xml -o bin/Test-RandomNumbers.v32 || abort_build

echo
echo BUILD SUCCESSFUL
