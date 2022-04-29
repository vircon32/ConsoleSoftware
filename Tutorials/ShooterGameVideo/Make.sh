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
compile Tutorial-Shooter.c -o obj/Tutorial-Shooter.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Tutorial-Shooter.asm -o obj/Tutorial-Shooter.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Textures/TextureBackground.png  -o obj/TextureBackground.vtex  || abort_build
png2vircon Textures/TextureGameObjects.png -o obj/TextureGameObjects.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
wav2vircon Sounds/MusicGameplay.wav  -o obj/MusicGameplay.vsnd  || abort_build
wav2vircon Sounds/SoundShoot.wav     -o obj/SoundShoot.vsnd     || abort_build
wav2vircon Sounds/SoundExplosion.wav -o obj/SoundExplosion.vsnd || abort_build
wav2vircon Sounds/SoundDeath.wav     -o obj/SoundDeath.vsnd     || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Tutorial-Shooter.xml -o bin/Tutorial-Shooter.v32 || abort_build

echo
echo BUILD SUCCESSFUL
