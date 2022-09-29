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
compile Test-SoundEffects.c -o obj\Test-SoundEffects.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Test-SoundEffects.asm -o obj\Test-SoundEffects.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon TexturePiano.png -o obj\TexturePiano.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon SoundPiano.wav -o obj\SoundPiano.vsnd || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Test-SoundEffects.xml -o bin\Test-SoundEffects.v32 || goto :failed
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