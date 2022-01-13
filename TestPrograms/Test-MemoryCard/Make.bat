@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Test-MemoryCard.c -o obj\Test-MemoryCard.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Test-MemoryCard.asm -o obj\Test-MemoryCard.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-GameScreen.png -o obj\Texture-GameScreen.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Test-MemoryCard.xml -o bin\Test-MemoryCard.v32 || goto :failed
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