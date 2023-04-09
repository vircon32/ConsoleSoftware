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
echo Import embedded data files
echo --------------------------
tiled2vircon maps/TitleMap.tmx    -o obj/ || abort_build
tiled2vircon maps/PieceModels.tmx -o obj/ || abort_build

echo
echo Compile the C code
echo --------------------------
compile Main.c -o obj/Tetris.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Tetris.asm -o obj/Tetris.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon textures/TextureTitle.png     -o obj/TextureTitle.vtex     || abort_build
png2vircon textures/TextureGameplay.png  -o obj/TextureGameplay.vtex  || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon music/MusicTitle.wav         -o obj/MusicTitle.vsnd        || abort_build
wav2vircon music/MusicGameplay.wav      -o obj/MusicGameplay.vsnd     || abort_build

wav2vircon sounds/SoundPressStart.wav   -o obj/SoundPressStart.vsnd   || abort_build
wav2vircon sounds/SoundMenuMove.wav     -o obj/SoundMenuMove.vsnd     || abort_build
wav2vircon sounds/SoundMenuAccept.wav   -o obj/SoundMenuAccept.vsnd   || abort_build
wav2vircon sounds/SoundMenuCancel.wav   -o obj/SoundMenuCancel.vsnd   || abort_build
wav2vircon sounds/SoundGameOver.wav     -o obj/SoundGameOver.vsnd     || abort_build
wav2vircon sounds/SoundQuitGame.wav     -o obj/SoundQuitGame.vsnd     || abort_build
wav2vircon sounds/SoundPieceMove.wav    -o obj/SoundPieceMove.vsnd    || abort_build
wav2vircon sounds/SoundPieceRotate.wav  -o obj/SoundPieceRotate.vsnd  || abort_build
wav2vircon sounds/SoundPieceDrop.wav    -o obj/SoundPieceDrop.vsnd    || abort_build
wav2vircon sounds/SoundPieceLock.wav    -o obj/SoundPieceLock.vsnd    || abort_build
wav2vircon sounds/SoundClear1Line.wav   -o obj/SoundClear1Line.vsnd   || abort_build
wav2vircon sounds/SoundClear2Lines.wav  -o obj/SoundClear2Lines.vsnd  || abort_build
wav2vircon sounds/SoundClear3Lines.wav  -o obj/SoundClear3Lines.vsnd  || abort_build
wav2vircon sounds/SoundClear4Lines.wav  -o obj/SoundClear4Lines.vsnd  || abort_build
wav2vircon sounds/SoundBoardFall.wav    -o obj/SoundBoardFall.vsnd    || abort_build
wav2vircon sounds/SoundLevelAdvance.wav -o obj/SoundLevelAdvance.vsnd || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Tetris.xml -o bin/Tetris.v32 || abort_build

echo
echo BUILD SUCCESSFUL
