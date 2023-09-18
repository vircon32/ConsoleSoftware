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
compile Test-Gamepads.c -o obj/Test-Gamepads.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Test-Gamepads.asm -o obj/Test-Gamepads.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-Gamepad.png -o obj/Texture-Gamepad.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Test-Gamepads.xml -o "bin/Test - Gamepads.v32" || abort_build

echo
echo BUILD SUCCESSFUL
