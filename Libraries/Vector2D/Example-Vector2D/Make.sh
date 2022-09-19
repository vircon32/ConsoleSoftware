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
compile Example-Vector2D.c -o obj/Example-Vector2D.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Example-Vector2D.asm -o obj/Example-Vector2D.vbin || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Example-Vector2D.xml -o bin/Example-Vector2D.v32 || abort_build

echo
echo BUILD SUCCESSFUL
