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
echo Assemble the ASM code
echo --------------------------
assemble TutorialASM-BouncingBall.asm -o obj/TutorialASM-BouncingBall.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon TextureBall.png -o obj/TextureBall.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon SoundBounce.wav -o obj/SoundBounce.vsnd || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom TutorialASM-BouncingBall.xml -o bin/TutorialASM-BouncingBall.v32 || abort_build

echo
echo BUILD SUCCESSFUL
