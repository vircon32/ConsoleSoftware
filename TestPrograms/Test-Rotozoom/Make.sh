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
compile Test-Rotozoom.c -o obj/Test-Rotozoom.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Test-Rotozoom.asm -o obj/Test-Rotozoom.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-Background.png -o obj/Texture-Background.vtex || abort_build
png2vircon Texture-Wheel.png      -o obj/Texture-Wheel.vtex      || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Test-Rotozoom.xml -o "bin/Test - Rotozoom.v32" || abort_build

echo
echo BUILD SUCCESSFUL
