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
compile Test-CPUErrors.c -o obj/Test-CPUErrors.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Test-CPUErrors.asm -o obj/Test-CPUErrors.vbin || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Test-CPUErrors.xml -o "bin/Test - CPU Errors.v32" || abort_build

echo
echo BUILD SUCCESSFUL
