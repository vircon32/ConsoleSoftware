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
compile Tutorial-WaterReflection.c -o obj/Tutorial-WaterReflection.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Tutorial-WaterReflection.asm -o obj/Tutorial-WaterReflection.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon TextureLakeMountain.png -o obj/TextureLakeMountain.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon SoundDripping.wav -o obj/SoundDripping.vsnd || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Tutorial-WaterReflection.xml -o bin/Tutorial-WaterReflection.v32 || abort_build

echo
echo BUILD SUCCESSFUL
