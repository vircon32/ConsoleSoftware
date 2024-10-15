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
echo Assemble the ASM code
echo --------------------------
assemble TutorialASM-Countdown.asm -o obj/TutorialASM-Countdown.vbin || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom TutorialASM-Countdown.xml -o bin/TutorialASM-Countdown.v32 || abort_build

echo
echo BUILD SUCCESSFUL
