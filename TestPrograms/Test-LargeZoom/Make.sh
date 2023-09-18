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
compile Test-LargeZoom.c -o obj/Test-LargeZoom.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Test-LargeZoom.asm -o obj/Test-LargeZoom.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon TextureCheckers.png -o obj/TextureCheckers.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Test-LargeZoom.xml -o "bin/Test - Large zoom.v32" || abort_build

echo
echo BUILD SUCCESSFUL
