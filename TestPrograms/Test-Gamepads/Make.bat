@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Test-Gamepads.c -o obj\Test-Gamepads.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Test-Gamepads.asm -o obj\Test-Gamepads.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-Gamepad.png -o obj\Texture-Gamepad.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Test-Gamepads.xml -o "bin\Test - Gamepads.v32" || goto :failed
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