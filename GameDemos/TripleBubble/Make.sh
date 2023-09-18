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
compile Main.c -o obj/TripleBubble.asm -v || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/TripleBubble.asm -o obj/TripleBubble.vbin || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom TripleBubble.xml -o "bin/Triple Bubble (Demo).v32" || abort_build

echo.
echo BUILD SUCCESSFUL
