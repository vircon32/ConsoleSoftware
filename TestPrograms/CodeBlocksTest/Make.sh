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
compile CodeBlocksTest.c -o obj/CodeBlocksTest.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/CodeBlocksTest.asm -o obj/CodeBlocksTest.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-ColorsWithAlpha.png -o obj/Texture-ColorsWithAlpha.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon Sound-SoftSynth.wav -o obj/Sound-SoftSynth.vsnd || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom CodeBlocksTest.xml -o bin/CodeBlocksTest.v32 || abort_build

echo
echo BUILD SUCCESSFUL
