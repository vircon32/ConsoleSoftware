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
tiled2vircon map/TileMap-RobotLab.tmx -o obj/ || abort_build

echo
echo Compile the C code
echo --------------------------
compile Tutorial03-TileMap.c -o obj/Tutorial03-TileMap.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Tutorial03-TileMap.asm -o obj/Tutorial03-TileMap.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-RobotLab.png -o obj/Texture-RobotLab.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Tutorial03-TileMap.xml -o bin/Tutorial03-TileMap.v32 || abort_build

echo
echo BUILD SUCCESSFUL
