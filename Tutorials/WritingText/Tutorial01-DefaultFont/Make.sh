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
compile Tutorial01-DefaultFont.c -o obj/Tutorial01-DefaultFont.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Tutorial01-DefaultFont.asm -o obj/Tutorial01-DefaultFont.vbin || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Tutorial01-DefaultFont.xml -o bin/Tutorial01-DefaultFont.v32 || abort_build

echo
echo BUILD SUCCESSFUL
