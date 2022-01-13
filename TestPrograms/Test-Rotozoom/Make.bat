@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Test-Rotozoom.c -o obj\Test-Rotozoom.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Test-Rotozoom.asm -o obj\Test-Rotozoom.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-Background.png -o obj\Texture-Background.vtex || goto :failed
png2vircon Texture-Wheel.png -o obj\Texture-Wheel.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Test-Rotozoom.xml -o bin\Test-Rotozoom.v32 || goto :failed
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