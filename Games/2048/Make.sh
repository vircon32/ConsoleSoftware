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
png2vircon Texture2048.png -o obj/Texture2048.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon music/MusicTitle.wav    -o obj/MusicTitle.vsnd    || abort_build
wav2vircon music/MusicGameplay.wav -o obj/MusicGameplay.vsnd || abort_build

wav2vircon sounds/SoundTitleTiles.wav    -o obj/SoundTitleTiles.vsnd    || abort_build
wav2vircon sounds/SoundTitleLogo.wav     -o obj/SoundTitleLogo.vsnd     || abort_build
wav2vircon sounds/SoundTitleStart.wav    -o obj/SoundTitleStart.vsnd    || abort_build
wav2vircon sounds/SoundQuitGame.wav      -o obj/SoundQuitGame.vsnd      || abort_build
wav2vircon sounds/SoundMenuMove.wav      -o obj/SoundMenuMove.vsnd      || abort_build
wav2vircon sounds/SoundMenuAccept.wav    -o obj/SoundMenuAccept.vsnd    || abort_build
wav2vircon sounds/SoundMenuCancel.wav    -o obj/SoundMenuCancel.vsnd    || abort_build
wav2vircon sounds/SoundMoveBoard.wav     -o obj/SoundMoveBoard.vsnd     || abort_build
wav2vircon sounds/SoundTileCollision.wav -o obj/SoundTileCollision.vsnd || abort_build
wav2vircon sounds/SoundTileAdded.wav     -o obj/SoundTileAdded.vsnd     || abort_build
wav2vircon sounds/SoundNewHighest.wav    -o obj/SoundNewHighest.vsnd    || abort_build
wav2vircon sounds/SoundWin.wav           -o obj/SoundWin.vsnd           || abort_build
wav2vircon sounds/SoundLose.wav          -o obj/SoundLose.vsnd          || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom 2048.xml -o bin/2048.v32 || abort_build

echo
echo BUILD SUCCESSFUL
