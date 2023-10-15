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

# levels have 2 parts each
for ((i=1;i<=4;i++))
do
  tiled2vircon maps/Level$i-1.tmx -o obj/ || abort_build
  tiled2vircon maps/Level$i-2.tmx -o obj/ || abort_build
done

echo
echo Convert the PNG textures
echo --------------------------
png2vircon textures/TextureTitle.png      -o obj/TextureTitle.vtex     || abort_build
png2vircon textures/TextureGameplay.png   -o obj/TextureGameplay.vtex  || abort_build
png2vircon textures/TextureCutscenes.png  -o obj/TextureCutscenes.vtex || abort_build
png2vircon textures/TextureFont11x16.png  -o obj/TextureFont11x16.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon music/MusicTitle.wav      -o obj/MusicTitle.vsnd      || abort_build
wav2vircon music/MusicCutscenes.wav  -o obj/MusicCutscenes.vsnd  || abort_build
wav2vircon music/MusicTutorial.wav   -o obj/MusicTutorial.vsnd   || abort_build
wav2vircon music/MusicLevelDay.wav   -o obj/MusicLevelDay.vsnd   || abort_build
wav2vircon music/MusicLevelNight.wav -o obj/MusicLevelNight.vsnd || abort_build

wav2vircon sounds/SoundTitleSlide.wav    -o obj/SoundTitleSlide.vsnd    || abort_build
wav2vircon sounds/SoundTitleLogo.wav     -o obj/SoundTitleLogo.vsnd     || abort_build
wav2vircon sounds/SoundPressStart.wav    -o obj/SoundPressStart.vsnd    || abort_build
wav2vircon sounds/SoundMenuMove.wav      -o obj/SoundMenuMove.vsnd      || abort_build
wav2vircon sounds/SoundMenuAccept.wav    -o obj/SoundMenuAccept.vsnd    || abort_build
wav2vircon sounds/SoundMenuCancel.wav    -o obj/SoundMenuCancel.vsnd    || abort_build
wav2vircon sounds/SoundStoryLetter.wav   -o obj/SoundStoryLetter.vsnd   || abort_build
wav2vircon sounds/SoundPause.wav         -o obj/SoundPause.vsnd         || abort_build
wav2vircon sounds/SoundDeath.wav         -o obj/SoundDeath.vsnd         || abort_build
wav2vircon sounds/SoundEnterDoor.wav     -o obj/SoundEnterDoor.vsnd     || abort_build
wav2vircon sounds/SoundLevelClear.wav    -o obj/SoundLevelClear.vsnd    || abort_build
wav2vircon sounds/SoundJump.wav          -o obj/SoundJump.vsnd          || abort_build
wav2vircon sounds/SoundLand.wav          -o obj/SoundLand.vsnd          || abort_build
wav2vircon sounds/SoundGolemWalk.wav     -o obj/SoundGolemWalk.vsnd     || abort_build
wav2vircon sounds/SoundGolemDeath.wav    -o obj/SoundGolemDeath.vsnd    || abort_build
wav2vircon sounds/SoundBirdFlap.wav      -o obj/SoundBirdFlap.vsnd      || abort_build
wav2vircon sounds/SoundBirdDeath.wav     -o obj/SoundBirdDeath.vsnd     || abort_build
wav2vircon sounds/SoundEnemyHit.wav      -o obj/SoundEnemyHit.vsnd      || abort_build
wav2vircon sounds/SoundShoot.wav         -o obj/SoundShoot.vsnd         || abort_build
wav2vircon sounds/SoundShotDestroyed.wav -o obj/SoundShotDestroyed.vsnd || abort_build
wav2vircon sounds/SoundBoxDestroyed.wav  -o obj/SoundBoxDestroyed.vsnd  || abort_build
wav2vircon sounds/SoundSpikesSwitch.wav  -o obj/SoundSpikesSwitch.vsnd  || abort_build
wav2vircon sounds/SoundWandMagic.wav     -o obj/SoundWandMagic.vsnd     || abort_build
wav2vircon sounds/SoundDoubleJump.wav    -o obj/SoundDoubleJump.vsnd    || abort_build
wav2vircon sounds/SoundBootsLand.wav     -o obj/SoundBootsLand.vsnd     || abort_build

# Now call the make script to build the program and pack the ROM
./Make.sh
