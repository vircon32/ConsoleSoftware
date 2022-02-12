@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Tutorial-PaletteSwap.c -o obj\Tutorial-PaletteSwap.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Tutorial-PaletteSwap.asm -o obj\Tutorial-PaletteSwap.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon TexturePaletteSwap.png -o obj\TexturePaletteSwap.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Tutorial-PaletteSwap.xml -o bin\Tutorial-PaletteSwap.v32 || goto :failed
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