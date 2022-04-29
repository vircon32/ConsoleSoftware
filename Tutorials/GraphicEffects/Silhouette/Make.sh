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
compile Tutorial-Silhouette.c -o obj/Tutorial-Silhouette.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Tutorial-Silhouette.asm -o obj/Tutorial-Silhouette.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon TextureSilhouette.png -o obj/TextureSilhouette.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Tutorial-Silhouette.xml -o bin/Tutorial-Silhouette.v32 || abort_build

echo
echo BUILD SUCCESSFUL
