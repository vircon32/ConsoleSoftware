@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Test-MinimalTest.c -o obj\Test-MinimalTest.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Test-MinimalTest.asm -o obj\Test-MinimalTest.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-ColorsWithAlpha.png -o obj\Texture-ColorsWithAlpha.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon Sound-SoftSynth.wav -o obj\Sound-SoftSynth.vsnd || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Test-MinimalTest.xml -o "bin\Test - Minimal test.v32" || goto :failed
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