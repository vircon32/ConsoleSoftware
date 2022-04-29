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
compile Tutorial06-TextAlignment.c -o obj/Tutorial06-TextAlignment.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Tutorial06-TextAlignment.asm -o obj/Tutorial06-TextAlignment.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-Font22x32.png -o obj/Texture-Font22x32.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Tutorial06-TextAlignment.xml -o bin/Tutorial06-TextAlignment.v32 || abort_build

echo
echo BUILD SUCCESSFUL
