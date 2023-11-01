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
compile Example-BoxCollisions.c -o obj/Example-BoxCollisions.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Example-BoxCollisions.asm -o obj/Example-BoxCollisions.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon TextureCollisions.png -o obj/TextureCollisions.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Example-BoxCollisions.xml -o bin/Example-BoxCollisions.v32 || abort_build

echo
echo BUILD SUCCESSFUL
