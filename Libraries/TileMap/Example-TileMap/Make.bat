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
compile Example-TileMap.c -o obj\Example-TileMap.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Example-TileMap.asm -o obj\Example-TileMap.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon TextureTiles.png -o obj\TextureTiles.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Example-TileMap.xml -o bin\Example-TileMap.v32 || goto :failed
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