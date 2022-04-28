#!/bin/bash

# Since program changes much more often than the rest,
# these scripts will separately process images, sounds
# and tilemaps so that we can choose whether to build
# everything (MakeAll.sh) or just update the previous
# build with any changes done to the code (Make.sh).
# For a first time build, MakeAll.sh must be used.

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
tiled2vircon maps/LevelTitle.tmx -o obj/ || abort_build

# levels with a leading 0 (1 to 9)
for ((i=1;i<=9;i++))
do
  tiled2vircon maps/Level0$i.tmx -o obj/ || abort_build
done

# rest of levels (10 to 30)
for ((i=10;i<=30;i++))
do
  tiled2vircon maps/Level$i.tmx -o obj/ || abort_build
done

echo
echo Convert the PNG textures
echo --------------------------
png2vircon textures/TextureGame.png     -o obj/TextureGame.vtex     || abort_build
png2vircon textures/TextureTextFont.png -o obj/TextureTextFont.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon music/MusicTitle.wav      -o obj/MusicTitle.vsnd      || abort_build
wav2vircon music/MusicWorld1.wav     -o obj/MusicWorld1.vsnd     || abort_build
wav2vircon music/MusicWorld2.wav     -o obj/MusicWorld2.vsnd     || abort_build
wav2vircon music/MusicWorld3.wav     -o obj/MusicWorld3.vsnd     || abort_build
wav2vircon music/MusicWorldClear.wav -o obj/MusicWorldClear.vsnd || abort_build
wav2vircon music/MusicCredits.wav    -o obj/MusicCredits.vsnd    || abort_build
wav2vircon music/MusicEnding.wav     -o obj/MusicEnding.vsnd     || abort_build
wav2vircon music/MusicGameOver.wav   -o obj/MusicGameOver.vsnd   || abort_build

wav2vircon sounds/SoundJump.wav     -o obj/SoundJump.vsnd     || abort_build
wav2vircon sounds/SoundLand.wav     -o obj/SoundLand.vsnd     || abort_build
wav2vircon sounds/SoundCoin.wav     -o obj/SoundCoin.vsnd     || abort_build
wav2vircon sounds/SoundOnOff.wav    -o obj/SoundOnOff.vsnd    || abort_build
wav2vircon sounds/SoundSpring.wav   -o obj/SoundSpring.vsnd   || abort_build
wav2vircon sounds/SoundGetKey.wav   -o obj/SoundGetKey.vsnd   || abort_build
wav2vircon sounds/SoundOpenDoor.wav -o obj/SoundOpenDoor.vsnd || abort_build
wav2vircon sounds/SoundGoal.wav     -o obj/SoundGoal.vsnd     || abort_build
wav2vircon sounds/SoundDeath.wav    -o obj/SoundDeath.vsnd    || abort_build
wav2vircon sounds/SoundPause.wav    -o obj/SoundPause.vsnd    || abort_build
wav2vircon sounds/SoundStart.wav    -o obj/SoundStart.vsnd    || abort_build
wav2vircon sounds/SoundHurry.wav    -o obj/SoundHurry.vsnd    || abort_build

# Now call the make script to build the program and pack the ROM
./Make.sh
