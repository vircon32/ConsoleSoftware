#!/bin/bash

# Since program changes much more often than the rest,
# these scripts will separately process images, sounds
# and tilemaps so that we can choose whether to build
# everything (MakeAll.sh) or just update the previous
# build with any changes done to the code (Make.sh).
# For a first time build, MakeAll.sh must be used.

# define an abort function to call on error
abort_build()
{
    echo
    echo BUILD FAILED
    exit 1
}

echo
echo Compile the C code
echo --------------------------
compile Main.c -o obj/Main.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Main.asm -o obj/Main.vbin || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom BasicPlatformer.xml -o "bin/Basic Platformer.v32" || abort_build

echo
echo BUILD SUCCESSFUL
