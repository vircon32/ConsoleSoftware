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
compile Test-MemoryCard.c -o obj/Test-MemoryCard.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Test-MemoryCard.asm -o obj/Test-MemoryCard.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-GameScreen.png -o obj/Texture-GameScreen.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Test-MemoryCard.xml -o bin/Test-MemoryCard.v32 || abort_build

echo
echo BUILD SUCCESSFUL
