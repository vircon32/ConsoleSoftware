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
compile Test-BeatEditor.c -o obj/Test-BeatEditor.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Test-BeatEditor.asm -o obj/Test-BeatEditor.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon TextureEditor.png -o obj/TextureEditor.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon SoundDrum.wav   -o obj/SoundDrum.vsnd   || abort_build
wav2vircon SoundSticks.wav -o obj/SoundSticks.vsnd || abort_build
wav2vircon SoundClap.wav   -o obj/SoundClap.vsnd   || abort_build
wav2vircon SoundBell.wav   -o obj/SoundBell.vsnd   || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Test-BeatEditor.xml -o bin/Test-BeatEditor.v32 || abort_build

echo
echo BUILD SUCCESSFUL
