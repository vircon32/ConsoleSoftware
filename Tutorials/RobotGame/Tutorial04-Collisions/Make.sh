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
compile Tutorial04-Collisions.c -o obj/Tutorial04-Collisions.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
assemble obj/Tutorial04-Collisions.asm -o obj/Tutorial04-Collisions.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-RobotLab.png -o obj/Texture-RobotLab.vtex || abort_build

echo
echo Pack the ROM
echo --------------------------
packrom Tutorial04-Collisions.xml -o bin/Tutorial04-Collisions.v32 || abort_build

echo
echo BUILD SUCCESSFUL
