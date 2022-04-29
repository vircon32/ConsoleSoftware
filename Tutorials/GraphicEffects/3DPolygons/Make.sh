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
compile Tutorial-3DPolygons.c -o obj/Tutorial-3DPolygons.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Tutorial-3DPolygons.asm -o obj/Tutorial-3DPolygons.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Texture3DPolygons.png -o obj/Texture3DPolygons.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Tutorial-3DPolygons.xml -o bin/Tutorial-3DPolygons.v32 || abort_build

echo
echo BUILD SUCCESSFUL
