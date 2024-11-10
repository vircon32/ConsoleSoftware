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
assemble TutorialASM-MoveBasic.asm -o obj/TutorialASM-MoveBasic.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon TextureRobot.png -o obj/TextureRobot.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom TutorialASM-MoveBasic.xml -o bin/TutorialASM-MoveBasic.v32 || abort_build

echo
echo BUILD SUCCESSFUL