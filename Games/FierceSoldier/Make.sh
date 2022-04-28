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
tiled2vircon maps/Level01.tmx -o obj/ || abort_build
tiled2vircon maps/Level02.tmx -o obj/ || abort_build
tiled2vircon maps/Level03.tmx -o obj/ || abort_build

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
png2vircon textures/TextureTitle.png     -o obj/TextureTitle.vtex     || abort_build
png2vircon textures/TextureStory.png     -o obj/TextureStory.vtex     || abort_build
png2vircon textures/TextureGameplay.png  -o obj/TextureGameplay.vtex  || abort_build
png2vircon textures/TextureBGLevel1.png  -o obj/TextureBGLevel1.vtex  || abort_build
png2vircon textures/TextureBGLevel2.png  -o obj/TextureBGLevel2.vtex  || abort_build
png2vircon textures/TextureBGLevel3.png  -o obj/TextureBGLevel3.vtex  || abort_build
png2vircon textures/TextureFont11x16.png -o obj/TextureFont11x16.vtex || abort_build
png2vircon textures/TextureFont22x32.png -o obj/TextureFont22x32.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon music/MusicTitle.wav    -o obj/MusicTitle.vsnd    || abort_build
wav2vircon music/MusicStory.wav    -o obj/MusicStory.vsnd    || abort_build
wav2vircon music/MusicGameplay.wav -o obj/MusicGameplay.vsnd || abort_build

wav2vircon sounds/SoundStartGame.wav      -o obj/SoundStartGame.vsnd      || abort_build
wav2vircon sounds/SoundStoryLetter.wav    -o obj/SoundStoryLetter.vsnd    || abort_build
wav2vircon sounds/SoundJump.wav           -o obj/SoundJump.vsnd           || abort_build
wav2vircon sounds/SoundLand.wav           -o obj/SoundLand.vsnd           || abort_build
wav2vircon sounds/SoundShoot.wav          -o obj/SoundShoot.vsnd          || abort_build
wav2vircon sounds/SoundDeath.wav          -o obj/SoundDeath.vsnd          || abort_build
wav2vircon sounds/SoundLevelClear.wav     -o obj/SoundLevelClear.vsnd     || abort_build
wav2vircon sounds/SoundPause.wav          -o obj/SoundPause.vsnd          || abort_build
wav2vircon sounds/SoundEnemyShoot.wav     -o obj/SoundEnemyShoot.vsnd     || abort_build
wav2vircon sounds/SoundEnemyHit.wav       -o obj/SoundEnemyHit.vsnd       || abort_build
wav2vircon sounds/SoundEnemyDeath.wav     -o obj/SoundEnemyDeath.vsnd     || abort_build
wav2vircon sounds/SoundExplosionSmall.wav -o obj/SoundExplosionSmall.vsnd || abort_build
wav2vircon sounds/SoundExplosionLarge.wav -o obj/SoundExplosionLarge.vsnd || abort_build
wav2vircon sounds/SoundMissile.wav        -o obj/SoundMissile.vsnd        || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom FierceSoldier.xml -o bin/FierceSoldier.v32 || abort_build

echo
echo BUILD SUCCESSFUL
