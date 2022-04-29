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
compile Tutorial02-PlayerInput.c -o obj/Tutorial02-PlayerInput.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Tutorial02-PlayerInput.asm -o obj/Tutorial02-PlayerInput.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-RobotLab.png -o obj/Texture-RobotLab.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Tutorial02-PlayerInput.xml -o bin/Tutorial02-PlayerInput.v32 || abort_build

echo
echo BUILD SUCCESSFUL
