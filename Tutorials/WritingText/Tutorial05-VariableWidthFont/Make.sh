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
compile Tutorial05-VariableWidthFont.c -o obj/Tutorial05-VariableWidthFont.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Tutorial05-VariableWidthFont.asm -o obj/Tutorial05-VariableWidthFont.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-Font22x32.png -o obj/Texture-Font22x32.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Tutorial05-VariableWidthFont.xml -o bin/Tutorial05-VariableWidthFont.v32 || abort_build

echo
echo BUILD SUCCESSFUL
