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
compile Test-DateAndTime.c -o obj/Test-DateAndTime.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Test-DateAndTime.asm -o obj/Test-DateAndTime.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-Clock.png -o obj/Texture-Clock.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon Sound-TicTac.wav -o obj/Sound-TicTac.vsnd || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Test-DateAndTime.xml -o bin/Test-DateAndTime.v32 || abort_build

echo
echo BUILD SUCCESSFUL
