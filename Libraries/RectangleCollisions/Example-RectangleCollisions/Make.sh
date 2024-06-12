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
compile Example-RectangleCollisions.c -o obj/Example-RectangleCollisions.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Example-RectangleCollisions.asm -o obj/Example-RectangleCollisions.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon TextureRectangles.png -o obj/TextureRectangles.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Example-RectangleCollisions.xml -o bin/Example-RectangleCollisions.v32 || abort_build

echo
echo BUILD SUCCESSFUL
