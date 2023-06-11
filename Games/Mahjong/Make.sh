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
echo Import the Tiled tile maps
echo --------------------------
tiled2vircon maps/MapTurtle2.tmx    -o obj/ || abort_build
tiled2vircon maps/MapPyramids2.tmx  -o obj/ || abort_build
tiled2vircon maps/MapCastle2.tmx    -o obj/ || abort_build
tiled2vircon maps/MapBridge2.tmx    -o obj/ || abort_build

echo
echo Compile the C code
echo --------------------------
compile Main.c -o obj/Mahjong.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Mahjong.asm -o obj/Mahjong.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon textures/TextureMahjong.png  -o obj/TextureMahjong.vtex  || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon music/MusicTitle.wav         -o obj/MusicTitle.vsnd        || abort_build
wav2vircon music/MusicGameplay.wav      -o obj/MusicGameplay.vsnd     || abort_build

wav2vircon sounds/SoundTitleLogo.wav    -o obj/SoundTitleLogo.vsnd    || abort_build
wav2vircon sounds/SoundPressStart.wav   -o obj/SoundPressStart.vsnd   || abort_build
wav2vircon sounds/SoundMenuMove.wav     -o obj/SoundMenuMove.vsnd     || abort_build
wav2vircon sounds/SoundMenuAccept.wav   -o obj/SoundMenuAccept.vsnd   || abort_build
wav2vircon sounds/SoundMenuCancel.wav   -o obj/SoundMenuCancel.vsnd   || abort_build
wav2vircon sounds/SoundWin.wav          -o obj/SoundWin.vsnd          || abort_build
wav2vircon sounds/SoundQuitGame.wav     -o obj/SoundQuitGame.vsnd     || abort_build
wav2vircon sounds/SoundSelectTile.wav   -o obj/SoundSelectTile.vsnd   || abort_build
wav2vircon sounds/SoundUndoSelect.wav   -o obj/SoundUndoSelect.vsnd   || abort_build
wav2vircon sounds/SoundMatchTiles.wav   -o obj/SoundMatchTiles.vsnd   || abort_build
wav2vircon sounds/SoundUnmatchTiles.wav -o obj/SoundUnmatchTiles.vsnd || abort_build
wav2vircon sounds/SoundHighlight.wav    -o obj/SoundHighlight.vsnd    || abort_build
wav2vircon sounds/SoundTileCollide.wav  -o obj/SoundTileCollide.vsnd  || abort_build
wav2vircon sounds/SoundWrongMove.wav    -o obj/SoundWrongMove.vsnd    || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Mahjong.xml -o bin/Mahjong.v32 || abort_build

echo
echo BUILD SUCCESSFUL
