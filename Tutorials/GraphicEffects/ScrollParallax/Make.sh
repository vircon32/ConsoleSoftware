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
compile Tutorial-ScrollParallax.c -o obj/Tutorial-ScrollParallax.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Tutorial-ScrollParallax.asm -o obj/Tutorial-ScrollParallax.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon TextureScrollParallax.png -o obj/TextureScrollParallax.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Tutorial-ScrollParallax.xml -o bin/Tutorial-ScrollParallax.v32 || abort_build

echo
echo BUILD SUCCESSFUL
