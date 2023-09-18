@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Test-BlendingModes.c -o obj/Test-BlendingModes.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj/Test-BlendingModes.asm -o obj/Test-BlendingModes.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-Room.png -o obj/Texture-Room.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon Sound-LampMoving.wav -o obj/Sound-LampMoving.vsnd || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Test-BlendingModes.xml -o "bin/Test - Blending modes.v32" || goto :failed
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