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
compile Main.c -o obj/Main.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Main.asm -o obj/Main.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon TextureChip8.png -o obj/TextureChip8.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon SoundBeep.wav -o obj/SoundBeep.vsnd  || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Chip8Emulator.xml -o "bin/Chip-8 Emulator.v32" || abort_build

echo
echo BUILD SUCCESSFUL
