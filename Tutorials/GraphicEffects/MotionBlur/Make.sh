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
compile Tutorial-MotionBlur.c -o obj/Tutorial-MotionBlur.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Tutorial-MotionBlur.asm -o obj/Tutorial-MotionBlur.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon TextureMotionBlur.png -o obj/TextureMotionBlur.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Tutorial-MotionBlur.xml -o bin/Tutorial-MotionBlur.v32 || abort_build

echo
echo BUILD SUCCESSFUL
