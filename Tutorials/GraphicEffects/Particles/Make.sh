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
compile Particles.c -o obj/Particles.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Particles.asm -o obj/Particles.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon TextureParticles.png -o obj/TextureParticles.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Particles.xml -o bin/Particles.v32 || abort_build

echo
echo BUILD SUCCESSFUL
