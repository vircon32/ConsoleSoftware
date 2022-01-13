@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Import embedded data files
echo --------------------------
tiled2vircon TileMap.tmx -o obj\ || goto :failed

echo.
echo Compile the C code
echo --------------------------
compile Test-TileMap.c -o obj\Test-TileMap.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Test-TileMap.asm -o obj\Test-TileMap.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-TileSet.png -o obj\Texture-TileSet.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Test-TileMap.xml -o bin\Test-TileMap.v32 || goto :failed
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