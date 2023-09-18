@echo off

REM prepare the script to be run from another directory
pushd %~dp0

REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Main.c -o obj\Main.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Main.asm -o obj\Main.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon TextureChip8.png -o obj\TextureChip8.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon SoundBeep.wav -o obj\SoundBeep.vsnd || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Chip8Emulator.xml -o "bin\Chip-8 Emulator.v32" || goto :failed
goto :succeeded

:failed
popd
echo.
echo BUILD FAILED
exit /b %errorlevel%

:succeeded
popd
echo.
echo BUILD SUCCESSFUL
exit /b

@echo on