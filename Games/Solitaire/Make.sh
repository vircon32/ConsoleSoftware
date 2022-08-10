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
png2vircon TextureSolitaire.png -o obj/TextureSolitaire.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon music/MusicTitle.wav    -o obj/MusicTitle.vsnd    || abort_build
wav2vircon music/MusicGameplay.wav -o obj/MusicGameplay.vsnd || abort_build

wav2vircon sounds/SoundTitleLogo.wav    -o obj/SoundTitleLogo.vsnd    || abort_build
wav2vircon sounds/SoundTitleStart.wav   -o obj/SoundTitleStart.vsnd   || abort_build
wav2vircon sounds/SoundMenuMove.wav     -o obj/SoundMenuMove.vsnd     || abort_build
wav2vircon sounds/SoundMenuAccept.wav   -o obj/SoundMenuAccept.vsnd   || abort_build
wav2vircon sounds/SoundMenuCancel.wav   -o obj/SoundMenuCancel.vsnd   || abort_build
wav2vircon sounds/SoundDealLadders.wav  -o obj/SoundDealLadders.vsnd  || abort_build
wav2vircon sounds/SoundDraw.wav         -o obj/SoundDraw.vsnd         || abort_build
wav2vircon sounds/SoundCardSlide.wav    -o obj/SoundCardSlide.vsnd    || abort_build
wav2vircon sounds/SoundCardDrop.wav     -o obj/SoundCardDrop.vsnd     || abort_build
wav2vircon sounds/SoundCardFlip.wav     -o obj/SoundCardFlip.vsnd     || abort_build
wav2vircon sounds/SoundWrongMove.wav    -o obj/SoundWrongMove.vsnd    || abort_build
wav2vircon sounds/SoundQuitGame.wav     -o obj/SoundQuitGame.vsnd     || abort_build
wav2vircon sounds/SoundWin.wav          -o obj/SoundWin.vsnd          || abort_build
wav2vircon sounds/SoundWinHighlight.wav -o obj/SoundWinHighlight.vsnd || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Solitaire.xml -o bin/Solitaire.v32 || abort_build

echo
echo BUILD SUCCESSFUL
