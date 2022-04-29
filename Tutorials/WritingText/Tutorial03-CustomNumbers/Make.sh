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
compile Tutorial03-CustomNumbers.c -o obj/Tutorial03-CustomNumbers.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Tutorial03-CustomNumbers.asm -o obj/Tutorial03-CustomNumbers.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-Numbers.png -o obj/Texture-Numbers.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Tutorial03-CustomNumbers.xml -o bin/Tutorial03-CustomNumbers.v32 || abort_build

echo
echo BUILD SUCCESSFUL
