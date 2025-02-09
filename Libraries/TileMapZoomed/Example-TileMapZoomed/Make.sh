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
compile Example-TileMapZoomed.c -o obj/Example-TileMapZoomed.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Example-TileMapZoomed.asm -o obj/Example-TileMapZoomed.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon TextureTiles.png -o obj/TextureTiles.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Example-TileMapZoomed.xml -o bin/Example-TileMapZoomed.v32 || abort_build

echo
echo BUILD SUCCESSFUL
