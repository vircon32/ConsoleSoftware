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
compile Pseudo3DRacer.c -o obj/Pseudo3DRacer.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Pseudo3DRacer.asm -o obj/Pseudo3DRacer.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon TextureRoad.png -o obj/TextureRoad.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon SoundRoad.wav   -o obj/SoundRoad.vsnd   || abort_build
wav2vircon SoundEngine.wav -o obj/SoundEngine.vsnd || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Pseudo3DRacer.xml -o "bin/Pseudo-3D Racer (Demo).v32" || abort_build

echo
echo BUILD SUCCESSFUL
