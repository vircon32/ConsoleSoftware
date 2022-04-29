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
# compilation of a BIOS requires argument -b
compile StandardBios.c -o obj/StandardBios.asm -b || abort_build

echo
echo Assemble the ASM code
echo --------------------------
# assembly of a BIOS requires argument -b
assemble obj/StandardBios.asm -o obj/StandardBios.vbin -b || abort_build

echo
echo Convert the PNG texture
echo --------------------------
png2vircon BiosTexture.png -o obj/BiosTexture.vtex || abort_build

echo
echo Convert the WAV sound
echo --------------------------
wav2vircon BiosSound.wav -o obj/BiosSound.vsnd || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom StandardBios.xml -o bin/StandardBios.v32 || abort_build

echo
echo BUILD SUCCESSFUL
