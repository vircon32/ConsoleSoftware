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
compile Test-SoundEffects.c -o obj/Test-SoundEffects.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Test-SoundEffects.asm -o obj/Test-SoundEffects.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon TexturePiano.png -o obj/TexturePiano.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon SoundPiano.wav -o obj/SoundPiano.vsnd  || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Test-SoundEffects.xml -o "bin/Test - Sound effects.v32" || abort_build

echo
echo BUILD SUCCESSFUL
