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
compile Tutorial01A-HelloWorld.c -o obj/Tutorial01A-HelloWorld.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Tutorial01A-HelloWorld.asm -o obj/Tutorial01A-HelloWorld.vbin || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Tutorial01A-HelloWorld.xml -o bin/Tutorial01A-HelloWorld.v32 || abort_build

echo
echo BUILD SUCCESSFUL
