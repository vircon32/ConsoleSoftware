@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Test-LargeZoom.c -o obj\Test-LargeZoom.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Test-LargeZoom.asm -o obj\Test-LargeZoom.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon TextureCheckers.png -o obj\TextureCheckers.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Test-LargeZoom.xml -o bin\Test-LargeZoom.v32 || goto :failed
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