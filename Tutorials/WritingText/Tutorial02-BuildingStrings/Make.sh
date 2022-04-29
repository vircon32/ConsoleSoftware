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
compile Tutorial02-BuildingStrings.c -o obj/Tutorial02-BuildingStrings.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Tutorial02-BuildingStrings.asm -o obj/Tutorial02-BuildingStrings.vbin || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Tutorial02-BuildingStrings.xml -o bin/Tutorial02-BuildingStrings.v32 || abort_build

echo
echo BUILD SUCCESSFUL
