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
echo Import tile map as an embedded data file
echo -------------------------------------------
tiled2vircon map/TileMapTest.tmx -o obj/ || abort_build

echo
echo Compile the C code
echo --------------------------
compile RPGConcept.c -o obj/RPGConcept.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/RPGConcept.asm -o obj/RPGConcept.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon TextureRPG.png        -o obj/TextureRPG.vtex        || abort_build
png2vircon TextureDialogFont.png -o obj/TextureDialogFont.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom RPGConcept.xml -o bin/RPGConcept.v32 || abort_build

echo
echo BUILD SUCCESSFUL
