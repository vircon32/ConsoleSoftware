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
compile Example-TextFonts.c -o obj/Example-TextFonts.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Example-TextFonts.asm -o obj/Example-TextFonts.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon TextureFont22x32.png    -o obj/TextureFont22x32.vtex    || abort_build
png2vircon TextureNumbers15x21.png -o obj/TextureNumbers15x21.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Example-TextFonts.xml -o bin/Example-TextFonts.v32 || abort_build

echo
echo BUILD SUCCESSFUL
