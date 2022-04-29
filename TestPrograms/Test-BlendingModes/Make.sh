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
compile Test-BlendingModes.c -o obj/Test-BlendingModes.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Test-BlendingModes.asm -o obj/Test-BlendingModes.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-Room.png -o obj/Texture-Room.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon Sound-LampMoving.wav -o obj/Sound-LampMoving.vsnd || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Test-BlendingModes.xml -o bin/Test-BlendingModes.v32 || abort_build

echo
echo BUILD SUCCESSFUL
