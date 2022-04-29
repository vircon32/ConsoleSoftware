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
compile Tutorial04-ConstantWidthFont.c -o obj/Tutorial04-ConstantWidthFont.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Tutorial04-ConstantWidthFont.asm -o obj/Tutorial04-ConstantWidthFont.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-Font11x16.png -o obj/Texture-Font11x16.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Tutorial04-ConstantWidthFont.xml -o bin/Tutorial04-ConstantWidthFont.v32 || abort_build

echo
echo BUILD SUCCESSFUL
