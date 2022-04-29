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
echo Import the Tiled tile maps
echo --------------------------
tiled2vircon maps/ClassicLevels.tmx -o obj/ || abort_build
tiled2vircon maps/TripleLevels.tmx  -o obj/ || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon textures/TextureTitle.png      -o obj/TextureTitle.vtex      || abort_build
png2vircon textures/TextureMenus.png      -o obj/TextureMenus.vtex      || abort_build
png2vircon textures/TextureGameplay.png   -o obj/TextureGameplay.vtex   || abort_build
png2vircon textures/TextureCutscenes.png  -o obj/TextureCutscenes.vtex  || abort_build
png2vircon textures/TextureCharacter1.png -o obj/TextureCharacter1.vtex || abort_build
png2vircon textures/TextureCharacter2.png -o obj/TextureCharacter2.vtex || abort_build
png2vircon textures/TextureCharacter3.png -o obj/TextureCharacter3.vtex || abort_build
png2vircon textures/TextureWorld1.png     -o obj/TextureWorld1.vtex     || abort_build
png2vircon textures/TextureWorld2.png     -o obj/TextureWorld2.vtex     || abort_build
png2vircon textures/TextureWorld3.png     -o obj/TextureWorld3.vtex     || abort_build
png2vircon textures/TextureWorld4.png     -o obj/TextureWorld4.vtex     || abort_build
png2vircon textures/TextureWorld5.png     -o obj/TextureWorld5.vtex     || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon music/MusicWorld1.wav -o obj/MusicWorld1.vsnd || abort_build
wav2vircon music/MusicWorld2.wav -o obj/MusicWorld2.vsnd || abort_build
wav2vircon music/MusicWorld3.wav -o obj/MusicWorld3.vsnd || abort_build
wav2vircon music/MusicWorld4.wav -o obj/MusicWorld4.vsnd || abort_build
wav2vircon music/MusicWorld5.wav -o obj/MusicWorld5.vsnd || abort_build

wav2vircon sounds/SoundTitleBubble.wav    -o obj/SoundTitleBubble.vsnd    || abort_build
wav2vircon sounds/SoundTitleFanfare.wav   -o obj/SoundTitleFanfare.vsnd   || abort_build
wav2vircon sounds/SoundTitleStart.wav     -o obj/SoundTitleStart.vsnd     || abort_build
wav2vircon sounds/SoundMenuMove.wav       -o obj/SoundMenuMove.vsnd       || abort_build
wav2vircon sounds/SoundMenuAccept.wav     -o obj/SoundMenuAccept.vsnd     || abort_build
wav2vircon sounds/SoundMenuCancel.wav     -o obj/SoundMenuCancel.vsnd     || abort_build
wav2vircon sounds/SoundLauncherMove.wav   -o obj/SoundLauncherMove.vsnd   || abort_build
wav2vircon sounds/SoundBubbleShoot.wav    -o obj/SoundBubbleShoot.vsnd    || abort_build
wav2vircon sounds/SoundBubbleBounce.wav   -o obj/SoundBubbleBounce.vsnd   || abort_build
wav2vircon sounds/SoundBubblePlaced.wav   -o obj/SoundBubblePlaced.vsnd   || abort_build
wav2vircon sounds/SoundBubblesBurst.wav   -o obj/SoundBubblesBurst.vsnd   || abort_build
wav2vircon sounds/SoundBurstFanfare.wav   -o obj/SoundBurstFanfare.vsnd   || abort_build
wav2vircon sounds/SoundChangeLauncher.wav -o obj/SoundChangeLauncher.vsnd || abort_build
wav2vircon sounds/SoundCountdown.wav      -o obj/SoundCountdown.vsnd      || abort_build
wav2vircon sounds/SoundCeilingDrop.wav    -o obj/SoundCeilingDrop.vsnd    || abort_build
wav2vircon sounds/SoundPause.wav          -o obj/SoundPause.vsnd          || abort_build
wav2vircon sounds/SoundLevelClear.wav     -o obj/SoundLevelClear.vsnd     || abort_build
wav2vircon sounds/SoundDeath.wav          -o obj/SoundDeath.vsnd          || abort_build
wav2vircon sounds/SoundReady.wav          -o obj/SoundReady.vsnd          || abort_build
wav2vircon sounds/SoundClear.wav          -o obj/SoundClear.vsnd          || abort_build
wav2vircon sounds/SoundFailed.wav         -o obj/SoundFailed.vsnd         || abort_build
wav2vircon sounds/SoundCombo.wav          -o obj/SoundCombo.vsnd          || abort_build
wav2vircon sounds/SoundComboNice.wav      -o obj/SoundComboNice.vsnd      || abort_build
wav2vircon sounds/SoundComboGreat.wav     -o obj/SoundComboGreat.vsnd     || abort_build

# Now call the make script to build the program and pack the ROM
./Make.sh
