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
compile Test-MathFunctions.c -o obj/Test-MathFunctions.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Test-MathFunctions.asm -o obj/Test-MathFunctions.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-MathFunctions.png -o obj/Texture-MathFunctions.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Test-MathFunctions.xml -o bin/Test-MathFunctions.v32 || abort_build

echo
echo BUILD SUCCESSFUL
