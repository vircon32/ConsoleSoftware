@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Import tile map as an embedded data file
echo -------------------------------------------
tiled2vircon map\TileMap-RobotLab.tmx -o obj\ || goto :failed

echo.
echo Compile the C code
echo --------------------------
compile Tutorial03-TileMap.c -o obj\Tutorial03-TileMap.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Tutorial03-TileMap.asm -o obj\Tutorial03-TileMap.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-RobotLab.png -o obj\Texture-RobotLab.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Tutorial03-TileMap.xml -o bin\Tutorial03-TileMap.v32 || goto :failed
goto :succeeded

:failed
echo.
echo BUILD FAILED
exit /b %errorlevel%

:succeeded
echo.
echo BUILD SUCCESSFUL
exit /b

@echo on