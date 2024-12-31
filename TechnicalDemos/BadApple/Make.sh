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
compile BadApple.c -o obj/BadApple.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/BadApple.asm -o obj/BadApple.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon TextureBadApple.png -o obj/TextureBadApple.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon MusicBadApple.wav -o obj/SoundMusicBadApple.vsnd || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom BadApple.xml -o "bin/Bad Apple.v32" || abort_build

echo
echo BUILD SUCCESSFUL
