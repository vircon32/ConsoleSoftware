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
echo Import embedded data files
echo --------------------------
tiled2vircon TileMap.tmx -o obj/ || abort_build

echo
echo Compile the C code
echo --------------------------
compile Test-TileMap.c -o obj/Test-TileMap.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Test-TileMap.asm -o obj/Test-TileMap.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-TileSet.png -o obj/Texture-TileSet.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Test-TileMap.xml -o "bin/Test - Tile map.v32" || abort_build

echo
echo BUILD SUCCESSFUL
